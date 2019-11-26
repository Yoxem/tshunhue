type
  ASTNode*[T] = ref object
    obj*: T
    children*: seq[ASTNode[T]]


proc add_child*(parent :var ASTNode[string], child : ASTNode[string]): bool = 
    add parent.children, child

proc set_children_array(parent : ASTNode[string], children_array : seq[ASTNode[string]]): bool =
    for i in 0..<len(children_array):
        parent.children.add(children_array[i])


proc to_string*(a: ASTNode[string]): string =
    var result = ""
    result &= $a.obj

    if len(a.children) > 0:
        result &= "("

        for i in 0..<len(a.children):
            result &= to_string(a.children[i])

            if i < len(a.children)-1:
                result &= " "
        
        result &= ")"
    
    return result
    
var ast_example : ASTNode[string]
ast_example = ASTNode[string](obj: "", children: @[])

let ast_example_line1_seq = @[ASTNode[string](obj: "define", children: @[]), AstNode[string](obj: "x", children: @[]), AstNode[string](obj: "int", children: @[]), AstNode[string](obj: "16", children: @[])]
let ast_example_line2_seq = @[ASTNode[string](obj: "define", children: @[]), AstNode[string](obj: "y", children: @[]), AstNode[string](obj: "double", children: @[]), AstNode[string](obj: "16.2", children: @[])]

var ast_example_line1 : ASTNode[string]
ast_example_line1 = ASTNode[string](obj: "", children: @[])
var ast_example_line2 : ASTNode[string]
ast_example_line2 = ASTNode[string](obj: "", children: @[])

discard ast_example_line1.set_children_array(ast_example_line1_seq)
discard ast_example_line2.set_children_array(ast_example_line2_seq)

discard ast_example.add_child(ast_example_line1)
discard ast_example.add_child(ast_example_line2)



echo to_string(ast_example)