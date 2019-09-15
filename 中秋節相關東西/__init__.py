#!/usr/bin/env python3

import tokenizer
import parser
import sys
import collections

# path = sys.argv[1]
path = 'a.scm'

f = open(path, 'r')
file_content = "".join(f.readlines())

tokenized_content = tokenizer.tokenize(file_content)

parser = parser.Parser()

raw_ast = parser.parse(tokenized_content)

'''
Type inferencing
'''
ASTItemToken = collections.namedtuple('Node', ['content', 'type','free_var'])

def generate_empty_ast_token(ls):
    kind = ""
    free_var = []
    if isinstance(ls,list):
        content =  [generate_empty_ast_token(i) for i in ls]
        return ASTItemToken(content=content, type=kind, free_var=free_var)
    else:
        return ASTItemToken(content=ls, type=kind, free_var=free_var)

to_check_if_double_defined_ast = generate_empty_ast_token(raw_ast)

'''
data type list (Non-Closure or void):
[
    (Type_name, Parent_Type, Detail)
    Parent_type: if it's not subtype, the value is None.
    Detail: Type detail of type [record or union] ["record", {"a": "int", "b": "dou"}] or None

]
'''
type_ls = [
    ("p_int","int",None),
    ("n_int", "int", None),
    ("z_int", "int", None),
    ("int", None, None),
    ("dou", None, None),
    ("str", None, None),
    ]

'''
var_and_type_list
'''
var_and_type_list = [
    ("%+", ("case", [
                    ("->", ("p_int", "p_int"), "p_int"),
                    ("->", ("n_int", "n_int"), "n_int"),
                    ("->", ("p_int", "n_int"), "int"),
                    ("->", ("n_int", "p_int"), "int"),
                    ("->", ("int", "int"), "int"),
                    ("->", ("dou", "dou"), "dou")
                    ]) 
    )
]

vars_ls = [{'defined' : {'%+', '%p'}, 'used' : {}}]

'''
check if there are any double defined variable
'''
def find_if_double_defined(ast, vars_ls):
    if isinstance(ast.content,list):
        for item in ast:
            vars_ls = find_if_double_defined_aux(item, vars_ls)
        return vars_ls
    else:
        return find_if_double_defined_aux(ast, vars_ls)

def find_if_double_defined_aux(ast, vars_ls):
    if isinstance(ast.content,list):
        ast_head = ast.content[0]

        # define variable
        if ast_head.val == 'define':
            var_name = ast.content[1].val
            current_variable_frame = vars_ls[-1]
            if var_name in current_variable_frame['defined'] or var_name in current_variable_frame['used']:
                raise Exception("variable double defined at ln %d, col %d" % (ast_head.line, ast_head.column))
            else:
                vars_ls[-1]['defined'].add(var_name) # add the new-defined to the list
                rhs_var_ls = find_if_double_defined_aux(ast.content[3], vars_ls)
                return vars_ls
        
        # lambda
        if ast_head.val == 'lambda':
            orig_vars_ls = vars_ls
            extended_vars_ls = vars_ls + [{'defined' : {}, 'used' : {}}]
            # (lambda ((y int) ...)
            if isinstance(ast.content[1].content, list):
                arg_list = map(lambda x : x.content[0], ast.content[1].content)
            # lambda (y ...)
            else:
                arg_list = map(lambda x : x.content, ast.content[1].content)
                
            for arg in arg_list:
                arg_name = arg.val
                line = arg.line
                column = arg.column

                if arg_name in extended_vars_ls[-1]['defined']:
                    raise Exception("variable double defined at ln %d, col %d" % (line, column))
                else:
                    extended_vars_ls[-1]['defined'].add(arg_name)

            # after adding arguments, visit lambda body
            for item in ast.content[2:]:
                extended_vars_ls = find_if_double_defined_aux(item, extended_vars_ls)
            
            # return orig_vars_ls, don't pollute it.
            return orig_vars_ls

        # function applying or basic operator, visiting all items
        else:
            for item in ast.content:
                vars_ls = find_if_double_defined_aux(item, vars_ls)
            return vars_ls
    
    # variable or constant
    else:
        if ast.type in ['SYMBOL', 'OP']:
            vars_ls[-1]['used'].add(arg_name)
        else:
            pass

        return vars_ls

to_be_typeinferenced_ast = find_if_double_defined(to_check_if_double_defined_ast, vars_ls)


'''
TODO: 型別推測

def typeinfer(ast, type_ls, var_type_ls):
    if isinstance(ast.content,list):
        return [typeinfer_aux(ast, type_ls, var_type_ls) for i in ast]
    else:
        return typeinfer_aux(ast, type_ls, var_type_ls)

def typeinfer_aux(ast, type_ls, var):
    if isinstance(ast.content, list):
        list_head = ast.content[0]
        if list_head.content.val == 'define':
            '''