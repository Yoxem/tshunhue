import collections
import re
import pattern_pair

Token = collections.namedtuple('Token', ['val', 'type', 'line', 'column'])

class Token(Token):
    def __repr__(self):
        return "%s{%s}" % (self.val, self.type)    
    
def tokenize(code):
    token_list = []

    tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in pattern_pair.ptn_pair)
    line_num = 1
    line_start = 0

    for item in re.finditer(tok_regex, code):
        kind = item.lastgroup
        value = item.group()
        column = item.start() - line_start
        if kind in ['POS_INT', 'NEG_INT', 'ZER_INT']:
            value = int(value)
        elif kind == 'BOOL':
            if value == '#t':
                value = True
            else:
                value = False
        elif kind == 'NL':
            line_start = item.end()
            line_num += 1
        elif kind == 'ELSE':
            raise Exception('unexpected item \'%s\' on line %d, column %d.' % (value, line, column))
        token_list.append(Token(value, kind, line_num, column))
    
    return token_list