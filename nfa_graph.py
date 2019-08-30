#!/usr/bin/env python3
from copy import copy

start_point = 1
'''graph_list = list of links represented by (orig, dest, value)'''
graph_list = [(1, 2, "a"), 
        (2, 3, "g"), 
        (2, 4, "c"), 
        (2, 5, ("SET", "F", "G", "H")), 
        (4, 2, ("NOT", ""))]

def graph_list2dict(graph_list):
    graph_dict = dict()

    for item in graph_list:
        orig = item[0]
        dest = item[1]
        value = item[2]

        if orig in graph_dict:
            if value in graph_dict[orig]:
                raise Exception("Each 2 links from the same link shouldn't have the same value.")
            '''
			else:
                for link_value in graph_dict[orig]:
                    if dest == graph_dict[orig][link_value]:
                        raise Exception("The orig-dest pair are duplicated.")'''

        else:
            graph_dict[orig] = dict()
        
        graph_dict[orig][value] = dest
    
    return graph_dict
        
graph_dict = graph_list2dict(graph_list)
print(graph_dict)

string1 = "ag"
string2 = "acxcxcxc"


def char_pattern_list_match(value, c):
    '''Match a char with char pattern list "value". Called with pattern match'''
    head = value[0]
    # char pattern ["NOT", "a"] = [^a]
    if head == "NOT":
        if len(value) > 2:
            raise Exception("in char pattern NOT contains only 1 sub-pattern")
        return not (value_match(value[1], c))
    
    # char pattern ["SET", "a", "b", ...] = [ab...]
    elif head == "SET":
        if len(value) == 1:
            raise Exception("in char pattern SET contains at least 1 sub-pattern")
        for i in value[1:]:
            if value_match(i, c):
                return True
        return False
    
    elif head == "RANGE":
        if len(value) != 3 or not (isinstance(value[1], str) and isinstance(value[2], str)):
            raise Exception("in char pattern RANGE contains only 2 chars")
        elif len(value[1]) > 1 or len(value[2]) > 1:
            raise Exception("in char pattern RANGE contains no string of which length > 1")
        else:
            lower_bound = ord(value[1])
            upper_bound = ord(value[2])
            if lower_bound > upper_bound:
                raise Exception(
                    "in char pattern RANGE the lower_bound must not bigger than upper bound.")

            if ord(c) >= lower_bound and ord(c) <= upper_bound:
                return True
            else:
                return False
    else:
        raise Exception("The head of a char pattern list is not acceptable.")

def value_match(value, c):
    if isinstance(value, str):
        is_meta_character = (len(value) == 2 and value[1] == "\\")
        if len(value) > 1 and not is_meta_character:
            raise Exception("Can't compare a char with 2 or more chars.")
        elif value == c:
            return True
        else:
            return False

    elif isinstance(value, tuple):
        return char_pattern_list_match(value, c)

    else:
        raise Exception("The argument called with match_value is not available.")    

print(value_match(("SET", ("RANGE", "a", "g")), "k"))

def transit_string_in_graph(graph_dict, start_point, string):
    status = start_point
    for c in string:
        for value in graph_dict[status]:
            if value_match(value, c):
                status = graph_dict[status][value]
    
    return status

print(transit_string_in_graph(graph_dict, start_point, string2))


def id_gen():
    id_num = 0
    while True:
        new_value = (yield id_num)
        if new_value != None:
            id_num = new_value
        else:
            id_num += 1

real_id_generator = id_gen()

class NFA():
    '''Definition of NFA graph'''
    def __init__(self):
        self.start = None
        self.end = []
        self.graph = [] # list of triary tuple
    
    def __repr__(self):
        return_string = ""
        return_string += "Start: %d\n" % self.start
        
        if self.graph == []:
            return_string += "EMPTY PATH"

        else:
            for path in self.graph:
                path_string = "%s -> %s : %s\n" % (path[0], path[1], path[2])
                return_string += path_string
        
        end_string = "End: " + str(self.end)
        return_string += end_string
        
        return return_string
            
def make_simple_nfa(pattern):
    nfa = NFA()
    nfa.start = next(real_id_generator)
    nfa.end.append(next(real_id_generator))

    nfa.graph.append(tuple([nfa.start, nfa.end[0], pattern]))

    return nfa

def nfa_concat(nfa1, nfa2):
    """xy"""
    new_nfa = NFA()
    new_nfa.start = copy(nfa1.start)

    connecting_point_pair = [nfa1.end, nfa2.start]
    
    new_graph = copy(nfa1.graph)
    new_end = []
    for path in nfa2.graph:
        if path[0] == connecting_point_pair[1]:
            for n in connecting_point_pair[0]:
                new_graph.append(tuple([n] + list(path[1:3])))
        else:
            new_graph.append(path)
    
    for e in nfa2.end:
        if e == connecting_point_pair[1]:
            new_end += nfa1.end
        else:
            new_end.append(e)
    
    new_nfa.graph = new_graph
    new_nfa.end = new_end

    return new_nfa

def nfa_or(nfa1, nfa2):
    """x|y"""
    new_nfa = NFA()
    new_nfa.start = copy(nfa1.start)

    new_end = copy(nfa1.end)
    
    new_graph = copy(nfa1.graph)

    for path in nfa2.graph:
        if path[0] == nfa2.start:
            appended_link = tuple([nfa1.start] + list(path[1:3]))
            new_graph.append(appended_link)
        else:
            new_graph.append(path)
    
    for e in nfa2.end:
        if e == nfa2.start:
            new_end.append(nfa1.start)
        else:
            new_end.append(e)
    
    new_nfa.graph = new_graph
    new_nfa.end = new_end

    return new_nfa

def nfa_once_or_none(nfa1, nfa2):
    """"xy?"""
    new_nfa = NFA()
    new_nfa.start = copy(nfa1.start)
    new_nfa.end = nfa1.end + filter(lambda x : x != nfa2.start, nfa2.end)

    new_graph = copy(nfa1.graph)
    for path in nfa2.graph:
        generated_links = []

        if path[0] == nfa2.start:
            for new_link_orig in nfa1.end:
                appended_link = tuple([new_link_orig] + list(path[1:]))
                generated_links.append(appended_link)
        else:
            generated_links = [path]

    new_graph += generated_links
    new_nfa.graph = new_graph

    return new_nfa


def nfa_repeat_or_none(nfa1, nfa2):
    """xy*"""
    new_nfa = NFA()
    new_nfa.start = copy(nfa1.start)
    new_nfa.end = copy(nfa1.end)

    new_graph = copy(nfa1.graph)

    for path in nfa2.graph:
        temp_links = []

        if path[0] == nfa2.start:
            for new_link_orig in nfa1.end:
                appended_link = tuple([new_link_orig] + list(path[1:]))
                temp_links.append(appended_link)
        else:
            temp_links = [path]
    
        generated_new_links = []
        for link in temp_links:
            if link[1] in nfa2.end:
                for new_link_end in nfa1.end:
                    appended_link = tuple([link[0]]+[new_link_end]+[link[2]])
                    generated_new_links.append(appended_link)
            else:
                generated_new_links.append(link)

    new_graph += generated_new_links
    new_nfa.graph = new_graph
    
    return new_nfa

nfa1 = make_simple_nfa("a")
nfa2 = make_simple_nfa(("NOT", "b"))
nfa3 = nfa_or(nfa1, nfa2)
print(nfa3)
print(nfa_repeat_or_none(nfa1, nfa2).graph)
print(nfa_once_or_none(nfa1, nfa2).graph)
