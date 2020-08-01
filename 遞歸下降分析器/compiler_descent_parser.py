import sys
import re

# command description
cmd_desc = """
simple compiler example
compiler.py filename
"""

if len(sys.argv) == 1:
    print(cmd_desc)
if len(sys.argv) > 2:
    raise Exception("you input too more arguments.")

#input_filename = sys.argv[1]
#file_content = open(input_filename,'r')

# test code
# test = "1 + 2 * (23 / 12)"
test = "1 + 2 * 3 * 4"

# TOKEN
INT = r'(?P<INT>\d+)'

ADD = r'(?P<ADD>\+)'
SUB = r'(?P<SUB>-)'
MUL = r'(?P<MUL>\*)'
DIV = r'(?P<DIV>/)'
LPARE = r'(?P<LPARE>\()'
RPARE = r'(?P<RPARE>\))'
ASSIGN = r'(?P<ASSIGN>=)'

L_INT = r'(?P<L_INT>int)' # let int

ID = r'(?P<ID>[_a-zA-Z][_0-9a-zA-Z]+)' #indicator
WSNL = r'(?P<WS>(\s|\n|\r)+)'# whitespace & newline

token_pattern_list = [INT, ADD, SUB, MUL, DIV , LPARE, RPARE, ASSIGN, L_INT , ID, WSNL]

token_pattern = re.compile("|".join(token_pattern_list))

raw_token_result_list = []

for match in re.finditer(token_pattern, test):
    raw_token_result_list.append((match.lastgroup, match.group(0)))

# [(None, None)] 必加，以避免出錯
token_result_list = [i for i in raw_token_result_list if i[0] != "WS"] + [(None, None)]
print(token_result_list)


"""
expr = term {+ term }*
term = factor {* factor}*
factor = number
"""
class Parser:
    def __init__(self):
        self.token = None
        self.remained_token = None
    
    def parse(self, token_list):
        if token_list[0] == (None, None):
            print("The compiled content is empty. Stop parsing.")
            return None

        self.token = token_list[0]
        self.remained_token = token_list[1:]
        result = self.expr()
        print(result)

        return result

    def advance(self):
        self.token = self.remained_token[0]
        self.remained_token = self.remained_token[1:]

    def expr(self):
        result = self.term()
        while self.token[0] == "ADD":
            result_head = self.token[1]
            self.advance()
            rhs = self.term()
            result = [result_head, result, rhs]
            rhs = result
        return result

    
    def term(self):
        result = self.factor()
        while self.token[0] == "MUL":
            result_head = self.token[1]
            self.advance()
            rhs = self.factor()
            result = [result_head, result, rhs]
            rhs = result

        return result
     
    def factor(self):
        if self.token[0] != "INT":
            raise Exception("Expect Number, find %s" % self.token[1])
        else:
            result = int(self.token[1])
            self.advance()
        
        return result

"""
    def statements(self):
        if token
"""

parser = Parser()
parser.parse(token_result_list)

'''
日後的文法：

statements ::= {statement}*
statement ::= declare | exp
declare ::= decl_lfs = exp
decl_lfs ::= type var
type ::= int # 記得加上或集 return False
var ::= id
exp ::= term {+/- term}*
term ::= factor {*// factor}*
factor ::= ( exp ) | const
const ::= int

    """或集時使用：expr = term1 (= +) | term2 (= int) """
    def expr(self):
        result = self.term1()
        if result == False:
            result = self.term2()
            print("term2")
        return result

    def term1(self):
        if self.token[0] == "ADD":
            result = self.token[1]
            self.advance()
            return result
        else:
            return False # 或集時必加
    
    def term2(self):
        if self.token[0] == "INT":
            result = self.token[1]
            self.advance()
            return result
        else:
            raise Exception("錯誤")
'''