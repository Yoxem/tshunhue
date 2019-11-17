import sequtils
import strformat

let numTwoPower32 = 4294967296'u64 # 2 ** 32

type
    HugeNum {.exportc: "HugeNum".} = tuple
        isNotNeg : bool
        num_seq : seq[uint32]

type
    HugeNumError* = object of Exception

proc Int32ToHugeNum(i : int32) : HugeNum =

    var isNotNeg = true
    var result_val : seq[uint32] = @[0'u32]
    if i >= 0:
        result_val = @[(uint32)i]
    else:
        isNotNeg = false
        result_val = @[(uint32)(-i)]
    
    let result : HugeNum = (isNotNeg: isNotNeg, num_seq: result_val)
    return result




# compare two sequence of which the abs. value is smaller.
proc InnerAbsLessThan(lhs_seq : seq[uint32], rhs_seq : seq[uint32]) : bool =
    if (len(lhs_seq) > len(rhs_seq)):
        return false
    elif (len(lhs_seq) < len(rhs_seq)):
        return true
    else:
        # for countdown using it.
        for i in countdown(len(lhs_seq)-1, 0):
            if lhs_seq[i] < rhs_seq[i]:
                return true
            
        return false





# compare two sequence of which the abs. value is equal to each other.
proc InnerAbsEqual(lhs_seq : seq[uint32], rhs_seq : seq[uint32]) : bool =
    if (len(lhs_seq) != len(rhs_seq)):
        return false
    else:
        for i in countdown(len(lhs_seq)-1,0):
            if lhs_seq[i] != rhs_seq[i]:
                return false
        return true


# compare two sequence of which the abs. value is greater.
proc InnerAbsGreaterThan(lhs_seq : seq[uint32], rhs_seq : seq[uint32]) : bool =
    return not (InnerAbsLessThan(lhs_seq, rhs_seq) or InnerAbsEqual(lhs_seq, rhs_seq))


# |great_num| + |smaller_num|. assumed great_num_seq is greater than the smaller_num_seq
proc InnerAbsPlus(great_seq: seq[uint32], small_seq: seq[uint32]) : seq[uint32] =

    var result = newSeq[uint32](len(great_seq)+1)
    var carry : uint64

    for i in 0..<len(great_seq):
        var great_i : uint64 = (uint64) great_seq[i]
        var small_i : uint64 = 0'u64
        var result_i : uint64
        
        if i < len(small_seq):
            small_i = (uint64) small_seq[i]
        
        result_i = great_i + carry

        if result_i + small_i >= numTwoPower32:
            carry = 1
            # upper bound of int64 is 4294967295, upper bound of (great_i, small_i)
            # is (4294967294, 4294967295)
            # modifying result_i = great_i + small_i - 4294967296 to the following line
            # can avoid overflow  
            result_i = result_i + small_i - numTwoPower32
        else:
            carry = 0
            result_i = result_i + small_i
        
        result[i] = (uint32) result_i;

    if carry > 0'u64:
        result[len(result)-1] = (uint32) carry
    else:
        result.delete(len(result)-1)
    
    return result

# |great_num| - |smaller_num|. assumed abs great_num_seq is greater than the abs. of
# smaller_num_seq
proc InnerAbsMinus(great_seq: seq[uint32], small_seq: seq[uint32]) : seq[uint32] =

    var great_seq_var = great_seq

    
    var result = newSeq[uint32](len(great_seq_var))
    var borrow : int64

    for i in 0..<len(great_seq):
        var great_i : int64 = (int64) great_seq_var[i]
        var small_i : int64 = 0'i64
        var result_i : int64;
        
        if i < len(small_seq):
            small_i = (int64) small_seq[i]
        
        great_i = great_i - borrow

        if great_i < small_i:
            borrow = 1
            # upper bound of int64 is 4294967295, upper bound of (great_i, small_i)
            # is (4294967294, 4294967295)
            # modifying result_i = great_i + small_i - 4294967296 to the following line
            # can avoid overflow  
            result_i = (great_i + 1)  + (4294967295 - small_i)
        else:
            borrow = 0
            result_i = great_i - small_i
        
        result[i] = (uint32) result_i;
    
    while result[len(result)-1] == 0:
        result.delete(len(result)-1)
    
    return result

#UNCHK
proc bigNumAdd(lhs : HugeNum, rhs : HugeNum) : HugeNum {.exportc.} =
    var result : HugeNum
    if (lhs.isNotNeg == true and rhs.isNotNeg == true):
        result.isNotNeg = true
        # 12 + 7, 9 + 0
        if InnerAbsGreaterThan(lhs.num_seq, rhs.num_seq):
            result.num_seq = InnerAbsPlus(lhs.num_seq, rhs.num_seq)
        # 7 + 12, 0 + 12
        else:
            result.num_seq = InnerAbsPlus(rhs.num_seq, lhs.num_seq)
        
        return result
    elif (lhs.isNotNeg == true and rhs.isNotNeg == false):
        # 12 + (-19) < 0 => lhs - rhs i.e. |lhs| < |rhs| => -(|rhs| - |lhs|)
        if (InnerAbsGreaterThan(rhs.num_seq, lhs.num_seq)):
            result.isNotNeg = false
            result.num_seq = InnerAbsMinus(rhs.num_seq, lhs.num_seq) # 12 + (-19) =  - (19 - 12)

        # 12 + (-9) or 12 + (-12) >= 0 => |lhs| >= |rhs| => (|lhs| - |rhs|)
        else:
            result.isNotNeg = true
            result.num_seq = InnerAbsMinus(lhs.num_seq, rhs.num_seq) # 12 + (-9) = + (12 - 9)
    
    elif (lhs.isNotNeg == false and rhs.isNotNeg == true):
        # (-12) + (19) or (-19) + (19) >= 0 i.e. |lhs| <= |rhs| => |rhs| - |lhs|
        if not InnerAbsGreaterThan(lhs.num_seq, rhs.num_seq):
            result.isNotNeg = true
            result.num_seq = InnerAbsMinus(rhs.num_seq, lhs.num_seq)
        # (-22) + (19) < 0 i.e. |lhs| > |rhs| => -(|lhs| - |rhs|)
        else:
            result.isNotNeg = false
            result.num_seq = InnerAbsMinus(lhs.num_seq, rhs.num_seq)

    # lhs < 0 and rhs < 0
    else:
        result.isNotNeg = false
        # (-5) + (-3) = - (5 + 3)
        if InnerAbsGreaterThan(lhs.num_seq, rhs.num_seq):
            result.num_seq = InnerAbsPlus(lhs.num_seq, rhs.num_seq)

        # (-5) + (-19) = -(19 + 5) => lhs + rhs = - (|rhs| + |lhs|)
        else:
            result.num_seq = InnerAbsPlus(rhs.num_seq, lhs.num_seq)
    
    return result


# UNCHK
### a - b:
#    a - 0 = a
#    a - b = a + (-b)
###
proc bigNumSubtract(lhs : HugeNum, rhs : HugeNum) : HugeNum {.exportc.} =
    let bigNumZero = Int32ToHugeNum(0)    
    if (InnerAbsEqual(rhs.num_seq,bigNumZero.num_seq)):
        return lhs
    else:
        var new_rhs = rhs
        new_rhs.isNotNeg = not (new_rhs.isNotNeg)
        return bigNumAdd(lhs, rhs)

# UNCHK
proc bigNumMultiply(lhs: HugeNum, rhs : HugeNum) : HugeNum {.exportc.} =
    var result = Int32ToHugeNum(0)

    for i in 0..(len(lhs.num_seq)-1):
        for j in 0..(len(rhs.num_seq)-1):
            let l_i : uint64 = (uint64) lhs.num_seq[i]
            let r_j : uint64 = (uint64) rhs.num_seq[j]
            let l_i_r_j : uint64 = l_i * r_j
            let base : uint64 = l_i_r_j mod numTwoPower32
            let carry : uint64 = l_i_r_j div numTwoPower32
            var temp_result : HugeNum
            temp_result.isNotNeg = true

            var result_num_seq : seq[uint32] = repeat(0'u32, i+j+2)

            result_num_seq[len(result_num_seq)-2] = (uint32)base
            
            if carry > 0'u64:
                result_num_seq[len(result_num_seq)-1] = (uint32)carry
            else:
                result_num_seq.delete(len(result_num_seq)-1)
            
            temp_result.num_seq = result_num_seq
            result = bigNumAdd(result, temp_result)

    return result 
 

#[ let test_suite = [[@[4109920726'u32, 1346167807'u32, 9424'u32], @[137586114'u32, 545115'u32],
                    @[4247506840'u32, 1346712922'u32, 9424'u32]],
                  [@[4086213910'u32, 2929037583'u32, 2244370610'u32, 2948093978'u32, 25680081'u32],
                  @[993921090'u32, 1587512372'u32, 2060251786'u32, 11882492'u32, 6880319'u32],
                  @[785167704'u32, 221582660'u32, 9655101'u32, 2959976471'u32, 32560400'u32]],
                  [@[4086213915'u32, 3195818255'u32, 1049426845'u32, 4172853648'u32, 2935028599'u32],
                  @[4086213909'u32, 3195818255'u32, 1049426845'u32, 4172853648'u32, 2935028599'u32],
                  @[3877460528'u32, 2096669215'u32, 2098853691'u32, 4050740000'u32, 1575089903'u32, 1'u32]],
                  [@[0'u32, 0'u32, 0'u32, 0'u32, 1'u32], @[1'u32], @[1'u32, 0'u32, 0'u32, 0'u32, 1'u32]],
                  [@[4294967295'u32, 4294967295'u32, 4294967295'u32, 4294967295'u32],
                  @[4294967294'u32, 4294967295'u32, 4294967295'u32, 4294967295'u32],
                  @[4294967293'u32, 4294967295'u32, 4294967295'u32, 4294967295'u32, 1'u32]],
                  [@[8'u32], @[7'u32], @[15'u32]], [@[1'u32], @[1'u32], @[2'u32]],
                  [@[4294967295'u32], @[1'u32], @[0'u32, 1'u32]],
                  [@[4294967294'u32, 4294967295'u32], @[3'u32], @[1'u32, 0'u32, 1'u32]]]



for i, c in test_suite:
    echo InnerAbsPlus(c[0], c[1])
    assert(InnerAbsPlus(c[0], c[1]) == c[2],$(InnerAbsPlus(c[0], c[1]))) ]#

var a = Int32ToHugeNum(-8)
echo a.num_seq


proc decStrToHugeNum(str : cstring) : HugeNum {.exportc.} =
    let hugeNum10 = Int32ToHugeNum(10)
    var digitHead : int = 0
    var negSigned : bool = false 
    var result : HugeNum  = Int32ToHugeNum(0)
    if(str[0] == '-'):
        digitHead = 1
        negSigned = true
    
    for i in digitHead..<len(str):
        let char_val : int32 = (int32)(int(str[i]) - int('0'))
        if char_val < 0 or char_val > 9:
            raise newException(HugeNumError,
                fmt"the string ""{str}"" is not formatted well.\n")
        else:
            let charValHugeNum : HugeNum = Int32ToHugeNum(char_val)
            result = bigNumAdd(bigNumMultiply(result, hugeNum10),charValHugeNum)

    if InnerAbsGreaterThan(result.num_seq, Int32ToHugeNum(0).num_seq) and negSigned == true:
        result.isNotNeg = false
    
    return result
