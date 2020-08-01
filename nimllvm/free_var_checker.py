#!/usr/bin/env python
import re
from collections import namedtuple
ASTNode = namedtuple('ASTNode', 'obj type')

ast_orig = [
["def","x","int","15"],
["def","y","int","26"],
["def","f","function",["l",["c","d"], [["l",["x"],[["def","a","int","12"],["+", "a", "x"], ["+", "a", "c"], ["+", "y", "y"]]]]]]]

def ast_to_named_tuple(ast_orig):
    obj = []

    if isinstance(ast_orig, list):
        for i in ast_orig:
            obj.append(ast_to_named_tuple(i))
    else:
        obj=ast_orig

    result = ASTNode(obj, None)

    return result


ast = ast_to_named_tuple(ast_orig)

def is_var(item):
    return re.match("[a-zA-Z]+", item)

def variable_check_line(ast,var_dict_box):
    if isinstance(ast,str):
        if is_var(ast):
            if ast in var_dict_box[0][0]:
                pass # OK
            else:
                var_found_frame_index = None
                for i in range(len(var_dict_box)):
                    if ast in var_dict_box[i][0] or ast in var_dict_box[i][1]:
                        var_found_frame_index = i
                    
                        for j in range(var_found_frame_index):
                            var_dict_box[j][1].add(ast)
                if var_found_frame_index == None:
                    print("找不到變數 %s" % ast)
        else:
            pass



    elif ast[0] == "def":
        if ast[1] in var_dict_box[0][0]:
            raise Exception("The variable is existed")
        elif ast[1] in var_dict_box[0][1]:
            raise Exception("The variable is existed")
        else:
            var_dict_box[0][0].add(ast[1])
            variable_check_line(ast[3],var_dict_box)

    elif ast[0] == "l":
        new_variable_frame = [[set(),set()]]
        for i in ast[1]:
            new_variable_frame[0][0].add(i)

        lambda_var_dict_box = variable_check(ast[2], new_variable_frame + var_dict_box)
        print("lambda 自由變數：", lambda_var_dict_box[0][1].__repr__())
    else:
        for i in ast:
            variable_check_line(i, var_dict_box)
        
    return var_dict_box

def variable_check(ast,var_dict_box):
    for i in ast:
        var_dict_box = variable_check_line(i, var_dict_box)
    return var_dict_box

variable_check(ast,[[set(),set()]])