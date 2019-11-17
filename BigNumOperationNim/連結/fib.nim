

type HugeNum {.exportc: "HugeNum", bycopy.} = tuple
      isNotNeg : bool
      num_seq : seq[cuint]

proc fib(a: cint): cint {.exportc.} =
    if a <= 2:
      result = 1
    else:
      result = fib(a - 1) + fib(a - 2)
    return result

proc get_seq2() : seq[cuint] {.exportc.} = 
    var s : seq[cuint] = @[9'u32]
    return s

proc example() : HugeNum {.exportc, cdecl.} = 
  var s : HugeNum
  s = (isNotNeg : true
       , num_seq : @[32'u32 , 12'u32]
       )
  return s
