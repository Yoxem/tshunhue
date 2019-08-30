#!/usr/bin/env python3

class Parser:
    def __init__(self):
        self.token = None
        self.remained_token = None

    '''from regex string to NFA class'''
    def parse(self, regex_str):
        if len(regex_str) == 0:
            print("The compiled content is empty. Stop parsing.")
            return None
        regex_str_list = [i for i in regex_str] + [None]

        self.token = regex_str_list[0]
        self.remained_token = regex_str_list[1:]
        result = self.re()
        
        return result

    def advance(self):
        self.token = self.remained_token[0]
        self.remained_token = self.remained_token[1:]

    '''<RE> 	::= (<simple-RE> "|" RE>) | <simple-RE>'''
    def re(self):
        result = self.simple_re()
        while self.token == "|":
            result_head = self.token
            self.advance()
            rhs = self.simple_re()
            result = [result_head, result, rhs]
            rhs = result
        return result

    '''<simple-RE> 	::= <basic-RE>+'''
    def simple_re(self):
        result = self.basic_re()
        # self.token != ")" and self.token != "|" is used to fix the bug
        while self.token != None and self.token != ")" and self.token != "|":
            result_head = "CONC" # concat
            rhs = self.basic_re()
            result = [result_head, result, rhs]
            rhs = result

        return result
        
    '''<basic-RE> 	::=	<elementary-RE> "*" | <elementary-RE> "?" | elementary-RE> "+" | <elementary-RE>'''    
    def basic_re(self):
        result = self.elementary_re()

        if self.token == "*":
            self.advance()
            result = ["STAR", result]

        elif self.token == "?":
            self.advance()
            result = ["QUESTION", result]
        
        elif self.token == "+":
            self.advance()
            result = ["PLUS", result]
        
        return result

    '''<elementary-RE> 	::=	<group> | <any> | <char> | <char-set>'''
    '''<group> 	::= 	"(" <RE> ")"'''

    def elementary_re(self):

        # <group> ::= "(" <RE> ")"
        if self.token == "(":
            self.advance()

            result = self.re()
            
            if self.token != ")":
                raise("Except \")\", found %s", self.token)
            self.advance()
            result = [result]
        elif self.token == ".":
            self.advance()
            result = "ANY"
        elif self.token == "[":
            result = self.char_set()
        else:
            result = self.char()
        
        return result

    
    '''<char> 	::= 	any non metacharacter | "\" metacharacter'''
    '''metacharacter = set{ \ * + . \ [ ] ( ) | }'''
    def char(self):
        meta_char_list = ["*", "+", ".", "\\", "[", "]", "(", ")", "|", "?"]

        # \ + metacharacter
        if self.token == "\\":
            self.advance()
            
            if self.token in meta_char_list:
                result = self.token
                self.advance()
                return result
            else:
                # exception description
                exception_desc = "Expect" + ", ".join(meta_char_list) + " , found \"%s\"" % self.token
                raise(exception_desc)
        else:
            if self.token in meta_char_list:
                print("Caution: \"%s\" is not recommended to be singly used." % self.token)
            result = self.token
            self.advance()

            return result
    
    '''<char-set> 	::= 	<positive-set> | <negative-set>'''
    def char_set(self):
        # token [ is checked in elementary_re. so advance()
        self.advance()
        
        '''<negative-set> 	::= 	"[^" <set-items> "]"'''
        if self.token == "^":
            result_rhs = self.set_items()
            result = ["NOT", result_rhs]

        else:
            result = self.set_items()
        
        return result

    def set_items(self):
        result = ["SET"]
        if self.token != "]":
            result.append(self.set_item())
        return result
    
    '''<set-item> 	::= 	<range> | <char>
    <range> 	::= 	<char> "-" <char>'''
    def set_item(self):

        result = self.char()
        
        # range = <char> "-" <char>
        if self.token == "-":
            result_lhs = result
            self.advance()
            result_rhs = self.char()
            result = ["RANGE", result_lhs, result_rhs]

        return result

regex_parser = Parser()
print(regex_parser.parse("(abc)"))
print(regex_parser.parse("(a|bc)"))
print(regex_parser.parse("a*bc"))
print(regex_parser.parse("(a*|b)c"))
print(regex_parser.parse("a+bc"))
print(regex_parser.parse("(a+|b)c"))
print(regex_parser.parse("a*+bc"))
print(regex_parser.parse("a*+bc"))
