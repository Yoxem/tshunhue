#/usr/bin/env python3

import llvmlite.ir as ll
import llvmlite.binding as llvm

llvm.initialize()
llvm.initialize_native_target()

context = ll.Context()

module = ll.Module(context=context)
module.triple = llvm.get_default_triple()



my_type2 = context.get_identified_type("MyType2")
a_type = ll.LiteralStructType((ll.FloatType(), ll.IntType(1)), packed=True)

my_type2.set_body(ll.FloatType(),ll.IntType(32)) # setting %"MyType2" = type {float, i32}

# module.add_global(my_type2)


print(module)