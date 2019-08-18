import ASTTree;

/* *
* descent recursive parser, convert regex to nfa-tree.
* the BNF of the regex is from:
* https://www2.cs.sfu.ca/~cameron/Teaching/384/99-3/regexp-plg.html
*
*/
public class Parser{
    private String characterList;
    private int index;
    private int nextIndex;

    public Parser(){
        this.characterList = null;
        this.index = 0;
        this.nextIndex = 1;
    }

    private moveForward(){
        this.index += 1;
        
        if (this.index < this.characterList.length() - 1){
            this.nextIndex += 1;
        }
        else{
            this.nextIndex = null;
        }
    }

    private parse(String regexStr){
        return this.expr();
    }

}
    /*
    """expr = term,{“+",term}"""
def expr():
    global token_list, current_token_index
    result = term() # go to lower grammar rule

    """using loops to save the recursivily-defined grammar.
    If the next token can't be gotten, don't enter the loop"""
    while next_token_index != None and token_list[next_token_index] == "+":
        # if the result is a list, set the left term (result) as the left item(sub-tree).
        left = result

        move_forward()
        middle = token_list[current_token_index]
        move_forward()
        right = term()

        """arrange the syntax tree (in S-exp like form)"""
        result = [middle, left, right]
    
    return result

"""term = factor,{“∗",factor}"""
def term():
    global token_list, current_token_index
    result = factor()

    while next_token_index != None and token_list[next_token_index] == "*":
        left = result 
        move_forward()
        middle = token_list[current_token_index]
        move_forward()
        right = term()

        """arrange the syntax tree (in S-exp like form)"""
        result = [middle, left, right]
    
    return result

'''factor = “0"|“1"|“2"|“3"|“4"|“5"|“6"|“7"|“8"|“9"'''
def factor():
    global token_list, current_token_index
    item = int(token_list[current_token_index])

    # if it matches the pattern, return the number in integer type, or raise a error.
    if item in range(0,10): # pattern = 0, 1, 2, ..., 9
        return item
    else:
        raise Exception("Except integer from 0 to 9, given %d" % item)

/*
<RE> 	::= 	<union> | <simple-RE>
<union> 	::=	<RE> "|" <simple-RE>
<simple-RE> 	::= 	<concatenation> | <basic-RE>
<concatenation> 	::=	<simple-RE> <basic-RE>
<basic-RE> 	::=	<star> | <plus> | <elementary-RE>
<star> 	::=	<elementary-RE> "*"
<plus> 	::=	<elementary-RE> "+"
<elementary-RE> 	::=	<group> | <any> | <eos> | <char> | <set>
<group> 	::= 	"(" <RE> ")"
<any> 	::= 	"."
<eos> 	::= 	"$"
<char> 	::= 	any non metacharacter | "\" metacharacter
<set> 	::= 	<positive-set> | <negative-set>
<positive-set> 	::= 	"[" <set-items> "]"
<negative-set> 	::= 	"[^" <set-items> "]"
<set-items> 	::= 	<set-item> | <set-item> <set-items>
<set-items> 	::= 	<range> | <char>
<range> 	::= 	<char> "-" <char> */