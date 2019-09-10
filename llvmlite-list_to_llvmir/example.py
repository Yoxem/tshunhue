from ctypes import CFUNCTYPE, c_int
import sys

import llvmlite.ir as ll
import llvmlite.binding as llvm

'''
define i64 @"main"(i8** %"arg") 
{
entry:
  %"x" = alloca i64
  %".3" = add i64 12, 20
  %".4" = sub i64 %".3", 89
  %".5" = add i64 %".4", 70
  store i64 %".5", i64* %"x"
  ret void %".6"
}
'''

ast = ["def", "x", ["+", ["-", ["+", 12, 20], 89], 70]]

llvm.initialize()

llvm.initialize_native_target()

def transverse(tree, irbuilder):
    if isinstance(tree, list):
        operator = tree[0]
        if operator in ["+", "-", "*", "/"]:

            lhs = transverse(tree[1], irbuilder)
            rhs = transverse(tree[2], irbuilder)

            if operator == "+":
                tmp = irbuilder.add(lhs, rhs)
            elif operator == "-":
                tmp = irbuilder.sub(lhs, rhs)
        
            return tmp

        elif operator == "def":
            var_name = tree[1]

            if not isinstance(var_name, str):
                raise(Exception("var_name expected, found %s"))
            else:
                var = irbuilder.alloca(ll.IntType(64), name=var_name)
                

                value = transverse(tree[2], irbuilder)
            
                

                ret = irbuilder.store(value, var)
                return ret
        else:
            pass

    else:
        return ll.IntType(64)(tree)


module = ll.Module()
module.triple = llvm.get_default_triple()

main_func_type = ll.FunctionType(ll.IntType(64), [ll.IntType(8).as_pointer().as_pointer()])
main_func = ll.Function(module, main_func_type, name='main')

main_func.args[0].name = 'arg'

bb_entry = main_func.append_basic_block('entry')
irbuilder = ll.IRBuilder(bb_entry)
ret_val = transverse(ast, irbuilder)


ret = irbuilder.ret(ret_val)


print(module)

ir_file = open("./a.ll", "w")
ir_file.write(module.__repr__())