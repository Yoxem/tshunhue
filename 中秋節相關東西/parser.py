#!/usr/bin/env python3
import tokenizer


class Parser:
    def __init__(self):
        self.token = None
        self.remained_token = None
    
    def parse(self, token_list):
        none_token = tokenizer.Token(val=None, type=None, line=None,column=None)
        
        # remove comment, whitespace and newline tokens
        token_list = list(filter(lambda x: not x.type in ['CMT', 'WS', 'NL'], token_list))
        
        token_list += [none_token]

        if token_list[0] == None:
            print("The compiled content is empty. Stop parsing.")
            return None

        self.token = token_list[0]
        self.remained_token = token_list[1:]
        result = self.expr_star()
        
        if self.token.val != None:
            raise Exception("The S-expression is not well-formed.")
        
        return result

    def advance(self):
        self.token = self.remained_token[0]
        self.remained_token = self.remained_token[1:]

    '''
    Grammar:

    expr_star = [None] | expr+
    expr = atom | ( expr_star )
    '''

    def expr(self):
        if self.token.val == "(":
            self.advance()
            result = self.expr_star()
            if self.token.val != ")":
                raise Exception("The S-expression is not well-formed.")
            else:
                self.advance()
                return result
        else:
            result = self.token
            self.advance()
            return result
    
    def expr_star(self):
        result = []

        while not self.token.val in [")", None]:
            result_item = self.expr()
            result.append(result_item)
        
        return result