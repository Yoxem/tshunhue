#!/usr/bin/env python3

# pattern pair
ptn_pair = [
                ('L_PARA', "[(]"),
                ('R_PARA', "[)]"),
                ('POS_INT', "[+]?\d+"),
                ('NEG_INT', "[-]\d+"),
                ('ZER_INT', "0+"),
                ('BOOL', "#[tf]"),
                ('DOUBLE', "[+-]?([0-9]*[.])?[0-9]+"),
                ('STR', '\\"(([^\\\\]|\\\\)+)\\"'),
                ('FUNC_INF', '->'), # operator
                ('OP', '[+\-*/]'), # operator
                ('SYMBOL', "[a-zA-Z_%][a-zA-Z0-9_%+*/\-]*"), #SYMBOL
                ('CMT', ";.*"), #COMMENT
                ('WS', "([ ]|\t)+"), # Whitespace
                ('NL', "\n"), # Newline
                ('ELSE', ".+"), # elsewhere
                ]