#[
    A simpler S-expression recursive descending parser in Nim
]#
import nre, strutils
import tables
import strutils, typetraits, unicode
import test

type
    ParserException* = object of Exception

doAssert "abc".match(re"def").isNone

let pattern_list = @[r"(?<L_PAREN>\()",
                    r"(?<R_PAREN>\))", 
                    r"(?<INT>[-]?\d+)", 
                    r"(?<DOU>[-]?\d+\.\d+)",
                    r"(?<STR>""([^""]|\\"")*"")"]



var splitted_result_with_group : seq[array[2, string]]

proc tokenize(input_string : string, pattern_list : seq[string]) : seq[array[2, string]] =
    let pattern_list_joined = pattern_list.join("|")

    for token in input_string.findIter(nre.re(pattern_list_joined)):
        let token_captured_table = token.captures.toTable
        var real_token_kind : string
    
        for key in token_captured_table.keys:
            real_token_kind = key
    
        splitted_result_with_group.add([token.match, real_token_kind])
    
    return splitted_result_with_group



type
    Parser* = object
        token* : array[2,string]
        remained_tokens* : seq[array[2, string]]


proc advance(p :var Parser) : bool =
    p.token = p.remained_tokens[0]
    p.remained_tokens.delete(0)

proc atom(p :var Parser): ASTNode[string] =
    echo "ATOM"
    echo p.token
    let atom_token_array = ["INT", "DOU", "STR"] 
    if atom_token_array.contains(p.token[1]):
        let result = ASTNode[string](obj : p.token[0] , children : @[])
        discard p.advance()
            
        return result
    else:
        raise newException(ParserException, "Token: " & (p.token[0]) & " is illegal.")

## expr = "(" expr* ")" | atom
proc expr(p :var Parser): ASTNode[string] =
    if p.token[1] == "L_PAREN":
        discard p.advance()
        var result : ASTNode[string]
        result = ASTNode[string](obj : "", children : @[])
        while p.token[1] != "R_PAREN" and p.token[1] != "":
            var item = p.expr()
            discard result.add_child(item)
        
        if p.token[1] == "R_PAREN":
            discard p.advance()
            return result
        # p.token[1] == ""
        else:
            raise newException(ParserException, "\")\" is expected.")
    else:
        let result = p.atom()
        return result

## main = expr*
proc main(p : var Parser): ASTNode[string] =
    var result : ASTNode[string]
    result = ASTNode[string](obj : "", children : @[])
    while p.token[1] != "":
        var item = p.expr()
        discard result.add_child(item)
    if p.token[1] == "":
        return result

proc parse(p :var Parser, token_seq: var seq[array[2,string]]) : ASTNode[string] = 
    var empty_ast_node : ASTNode[string]
    if token_seq == @[]:
        echo "The compiled content is empty. Stop parsing."
        return empty_ast_node

    else:
        token_seq.add(["", ""])
        p.token = token_seq[0]
        p.remained_tokens = token_seq
        p.remained_tokens.delete(0)

        result = p.main()

        echo result.to_string()
        
        return result
        
let a = @["123","112"]

var parser : Parser

parser = Parser(token : ["", ""], remained_tokens : @[["123","123"]])

var tokenized_list = tokenize("(1 2 3 4( 5 6( 7) 8)  9)",pattern_list)

let ast_result = parser.parse(tokenized_list)

echo ast_result.to_string()