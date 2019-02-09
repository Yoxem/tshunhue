玩具語言的 BNF 文法
===================

```
 lines = line | line ; lines
 line = expression | value_assignment | function_assignment | type_definition | jump_statment
 ```
 expression
 ------------
 ```
 expression = term | term {+ -} expression
 term = factor | factor {* /} term
 # paren_exp =  parenthesis_expression
 
 factor = list_unit  | function(factor_list)
 factor_list = factor | factor , factor_list
 function = factor | lambda (lambda_inputs){lines}
 lambda_inputs = "" | lambda_input_pair | lambda_input_pair, lambda_inputs
 lambda_input_pair = lambda_input_type identifier
 lambda_input_type = non_function_type | simple_function_type
 list_unit = list_ref | { list_items }
 list_items = list_ref | list_ref , list_items
 list_ref = paren_exp | list_unit [ paren_exp ]
 paren_exp = compound_identifier | const | ( expression )
 compound_identifier = identifier | identifier . identifier
 const = number | bool | string
 number = float_num | integer
 string = " string_content "
```

type definition
--------------------
```
type_definition = typedef identifier = type
type = non_function_type | function_type
non_function_type = struct_type | array_type | union_type | simple_type
simple_type = int | float | bool | char | void

# struct {foo: int, bar: bool}
struct_type = struct {struct_items}
struct_items = struct_item | struct_item , struct_items
struct_item = type identifier

list_type = list ( type )
union_type = union ( types )
types = type | type , types
function_type = simple_function_type | poly_function_type
simple_function_type = paren_types -> paren_type
paren_types = paren_type | paren_type , paren_types
paren_type = type | ( paren_types )
poly_function_type = forall identifier , poly_function_content
poly_function_content = poly_paren_types -> poly_paren_type
poly_paren_types = poly_paren_type | poly_paren_type , poly_paren_types
poly_paren_type = type | identifier | ( poly_paren_type )
```

function assignment
------------------
```
function_assignment
function_type identifier = lambda (function_inputs){lines}
function_inputs = identifier | identidier , identifiers

```

value assignment
------------------
```
value_assignment = non_function_type identifier = expression

```

jump statement
--------------------
```
jump_statement = return expression
```