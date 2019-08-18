// package nfatree;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Hashtable;
import dir.Tree;
/*
* Exception for NFA 
*/
class NFAException extends Exception
{
    private static final long serialVersionUID = 1L;

    public NFAException(String message)
  {
    super(message);
  }
}

/*
* structure for NFATree
*/
class NFATree{
    private Integer start;
    private ArrayList<Integer> endList; // List of end node num.
    private ArrayList<NFATreePath> graphTable; // graph shown as table

    public String toString(){
        String returnString = "";
        returnString += String.format("%d [shape=Diamond]\n" , this.start);

        if (this.graphTable.length == 0){
            returnString += "\n";
        }
        else{
            for (int i=0; i < this.graphTable.size() ; i++ ){
                Integer orig = this.graphTable.get(i).getOrig();
                Integer dest = this.graphTable.get(i).getDest();

                returnString += String.format("%d -> %d\n" , orig , dest);
            }
        }

        String endString = "";

        for (int i=0; i<this.endList.size(); i++){
            endString += String.format("%d [shape=square]\n", this.endList.get(i));   
        }
        
        returnString += endString;
    }

    public ArrayList<Integer> getEndList(){
        return this.endList;
    }

    /**
     * @return the start
     */
    public Integer getStart() {
        return this.start;
    }

    public ArrayList<NFATreePath> getGraphTable(){
        return this.graphTable;
    }

    public setEndList(ArrayList<Integer> endList){
        this.endList = endList;
    }

    public setStart(Integer startId){
        this.start = startId;
    }

    public addGraphPath(NFATreePath path){
        this.graphTable.add(path);
    }


    public static void main(String[] args) {
        NFATree _a = new NFATree();
        _a.start = 1;
        NFATreePath p1 = new NFATreePath(1, 2, "a");
        NFATreePath p2 = new NFATreePath(2, 3, "g");
        NFATreePath p3 = new NFATreePath(2, 4, "c");

        Tree<String> pattern = new Tree<String>("SET");
        pattern.appendChildrenList(Arrays.asList("F", "G", "H"));
        NFATreePath p4 = new NFATreePath(2, 5, pattern);

        _a.graphTable.add(p1);
        _a.graphTable.add(p2);
        _a.graphTable.add(p3);
        _a.graphTable.add(p4);

        try{
        Hashtable h = _a.nfaTableToHashMap();
        int z = 1 + 1;

        } catch (NFAException e) {
            System.out.println(e);
        }


    }

    public NFATree(int orig, int dest, NFATreePath path){
        this.start = (Integer) orig;
        this.endList = new ArrayList<Integer>(Arrays.asList((Integer) dest));
        this.graphTable = new ArrayList<NFATreePath>(Arrays.asList(path));
    }

    public NFATree(){
        this.start = (Integer)-1 ; // impossible value
        this.endList = new ArrayList<Integer>();
        this.graphTable = new ArrayList<NFATreePath>();
    }

    public NFATree(int orig, ArrayList<Integer> destList, NFATreePath path){
        this.start = (Integer)orig;
        this.endList = destList;
        this.graphTable = new ArrayList<NFATreePath>(Arrays.asList(path));
    }

    /* return a Hashtable in the form of:
    * Hashtable< OrigId, <Pattern, DestId>>
    *
    */
    Hashtable<Integer, Hashtable<Tree<String>,Integer>> nfaTableToHashMap () throws NFAException{
        Hashtable<Integer, Hashtable<Tree<String>,Integer>> graphHashMap  = new Hashtable();
        for (int i=0;i<this.graphTable.size(); i++){
            Integer orig = this.graphTable.get(i).getOrig();
            Integer dest = this.graphTable.get(i).getDest();
            Tree<String> pattern = this.graphTable.get(i).getPatt();

            if (graphHashMap.containsKey(orig)){
                if (graphHashMap.get(orig).containsKey(pattern)){
                    throw new NFAException(
                        "Each 2 links from the same link shouldn't have the same value.");
                }
                
            }else{
                // graph_dict[orig] = dict()
                graphHashMap.put(orig, new Hashtable());
            }

            graphHashMap.get(orig).put(pattern, dest);

        }
        return graphHashMap;
    }
    /*
    * (value_match)
    * Match a char string with char pattern string (1-char) "pattern". 
    */
    boolean charPatternMatch(String patt, String c) throws NFAException{
        if (c.length()>1){
            throw NFAException("Char or pattern input too many.");
        }
        else if(patt.length()>1){
            throw NFAException("patt or pattern input too many.");
        }
        else{
            if(c == patt){
                return true;
            }
            else{
                return false;
            }
        }
    }

    /*
    * Match a char string with char pattern (tree) "pattern".
    */
    boolean charPatternMatch(Tree<String> patt, String c) throws NFAException{
        if (c.length()>1){
            throw NFAException("Char input too many.");
        }
        else{
            head = patt.getNode();
            if (head == "NOT"){
                if(patt.getChildrenList().size()>1){
                    throw NFAException("in char pattern NOT contains only 1 sub-pattern");
                    return not(this.charPatternMatch(patt.getChildrenList().get(0),c));
                }
            }
            else if(patt.getNode() == "SET"){
                this.charPatternMatchSet(patt, c);
            }
            else if (patt.getNode() == "RANGE"){
                this.charPatternMatchRange(patt, c);
            }
            else if (patt.getNode().length() == 1){
                thischarPatternMatch(patt.getNode(),c); // char-to-char matching
            }
            else{
                throw NFAException("The form of a char pattern tree is not acceptable.");
            }
        }
    }



    /*
    * check part for patt of which head node is "SET". 
    */
    boolean charPatternMatchSet(Tree<String> patt, String c) throws NFAException{
        if (patt.getChildrenList().size() == 0){
            throw NFAException("in char pattern SET contains at least 1 sub-pattern");
        }else{
            for (int i=0;i<patt.getChildrenList().size();i++){
                if (this.charPatternMatch(patt, c)){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
    }

    /*
    * check part for patt of which head node is "RANGE". 
    */  
    boolean charPatternMatchRange(Tree<String> patt, String c) throws NFAException{
        if (patt.getChildrenList().size() > 2){
            throw NFAException("in char pattern RANGE contains only 2 chars");
        }
        else if (patt.getChildrenList().get(0).length()>1 || patt.getChildrenList().get(1).length()>1){
            throw NFAException("In char pattern RANGE contains no string of which length > 1.");
        }
        else {
            char lowerBound = patt.getChildrenList().get(0).charAt(0);
            char upperBound = patt.getChildrenList().get(1).charAt(0);
            if (lowerBound > upperBound){
                throw NFAException("In char pattern RANGE the lower_bound must not bigger than upper bound.");
            }
            else{
                if (c.charAt(0) >= lowerBound && c.charAt(0) <= upperBound){
                    return true;
                }
                else{
                    return false;
                }
            }
        
        }
    }

    public Integer transitStrInGraph(int startPoint, String str){
        Integer status = (Integer) startPoint;
        Hashtable<Integer, Hashtable<Tree<String>,Integer>> nfaHashMap = this.nfaTableToHashMap();
        for (int i=0;i<str.length();i++){
            String c = str.substring(i,i+1);
            // treeKey : the key is a tree
            for(Tree<String> treeKey : nfaHashMap.get(status).keySet()){
                boolean treeKeyMatched = this.charPatternMatch(treeKey, c);
                if (treeKeyMatched){
                    status = nfaHashMap.get(status).get(treeKey);
                }
            }
        }

        return status;
    }
}

/*
* Path for NFATree.graph
* [2]-- a ->[3]
* orig = 2;
* dest = 3;
* patt = ['a'];
*/
class NFATreePath {
    private Integer orig;
    private Integer dest;
    private Tree<String> patt;

    public NFATreePath(int ori, int des, String ptn) {
        this.orig = (Integer) ori;
        this.dest = (Integer) des;
        this.patt = new Tree<String>(ptn);

    }

    public NFATreePath(int ori, int des, Tree<String> ptnTree) {
        this.orig = (Integer) ori;
        this.dest = (Integer) des;
        this.patt = ptnTree;
    }

    Integer getOrig() {
        return this.orig;
    }

    Integer getDest() {
        return this.dest;
    }
    Tree<String> getPatt() {
        return this.patt;
    }
}

/**
 * containing mainaulations for the nfa graph genarating.
 */
class NfaManipulation{
    Integer id; // the id number that will be returned by nextId

    public NNfaManipulation(){
        this.id = (Integer) 0;
    }

    Integer nextId (){
        Integer retId = this.id;
        this.id += 1;
        return retId;
    }

    public NFATree makeSimpleNFA(String pattern){
        Integer start = this.nextId();
        Integer end = this.nextId();
        NFATreePath path = NFATreePath(start, end, pattern);
        NFATree nfa = NFATree(start, end, path);
        return nfa;
    }

    public NFATree makeSimpleNFA(Tree<String> pattern){
        Integer start = this.nextId();
        Integer end = this.nextId();
        NFATreePath path = NFATreePath(start, end, pattern);
        NFATree nfa = NFATree(start, end, path);
        return nfa;
    }

    /**
    * xy
    */
    public nfaConcat(NFATree nfa1, NFATree nfa2){
        Integer nfa1Start = nfa1.getStart();
        ArrayList<Integer> nfa1EndList = nfa1.getEndList();
        
        ArrayList<NFATreePath> nfa1NFATreeList = nfa1.getGraphTable();
        ArrayList<NFATreePath> nfa2NFATreeList = nfa2.getGraphTable();

        NFATree resultNFA = new NFATree();

        resultNFA.setStart(nfa1Start);

        concatNFAGraph(nfa2, nfa1EndList, nfa1NFATreeList, nfa2NFATreeList, resultNFA);

        resultNFA.setEndList(nfa2.getEndList());
        return resultNFA;
    }

    private void concatNFAGraph(NFATree nfa2, ArrayList<Integer> nfa1EndList, ArrayList<NFATreePath> nfa1NFATreeList,
            ArrayList<NFATreePath> nfa2NFATreeList, NFATree resultNFA) {
        for(int i=0; i<nfa1NFATreeList.size();i++){
            resultNFA.addGraphPath(nfa1NFATreeList.get(i));
        }

        for(int i=0; i<nfa1EndList.size();i++){
            for (int j=0; i<nfa2NFATreeList.size();j++){
                NFATreePath path = nfa2NFATreeList.get(j);

                if(path.getOrig() == (Integer) nfa1EndList.get(i)){
                    NFATreePath newPath =
                        new NFATreePath(nfa2.getStart(), path.getDest(), path.getPatt());
                    resultNFA.addGraphPath(newPath);
                }
                else{
                    resultNFA.addGraphPath(path);
                }
            }
        }
    }

    /**
    * x | y
    */
    public nfaOr(NFATree nfa1, NFATree nfa2){
        Integer nfa1Start = nfa1.getStart();
        Integer nfa2Start = nfa2.getStart();

        ArrayList<Integer> nfa1EndList = nfa1.getEndList();
        ArrayList<Integer> nfa2EndList = nfa2.getEndList();

        ArrayList<NFATreePath> nfa1NFATreeList = nfa1.getGraphTable();
        ArrayList<NFATreePath> nfa2NFATreeList = nfa2.getGraphTable();

        NFATree resultNFA = new NFATree();
        resultNFA.setStart(nfa1Start);
        ArrayList<Integer> NewEndList = nfa1EndList;

        for(int i=0; i<nfa1NFATreeList.size();i++){
            resultNFA.addGraphPath(nfa1NFATreeList.get(i));
        }

        for(int i=0;i<nfa2NFATreeList.size();i++){
            NFATreePath path = nfa2NFATreeList.get(i);

            if(path.getOrig() == nfa2Start){
                NFATreePath newPath = new NFATreePath(nfa1Start, path.getDest(), path.getPatt());
                resultNFA.addGraphPath(newPath);
            }
            else{
                resultNFA.addGraphPath(path);
            }
        }

        for (int i=0;i<nfa2EndList.size();i++){
            if (nfa2EndList.get(i) == nfa2Start){
                NewEndList.add(nfa1Start);
            }
            else{
                NewEndList.add(nfa2EndList.get(i));
            }
        }

        resultNFA.setEndList(NewEndList);
    
    return resultNFA;
    }

    /**
     * xy?
     */
    public NFATree nfaOnceOrNone(NFATree nfa1, NFATree nfa2){
        NFATree resultNFA = new NFATree();
        resultNFA.setStart(nfa1.getStart());

        ArrayList<Integer> resultNFAEndList = nfa1.getEndList();

        for (int i=0; i<nfa1.getGraphTable().size();i++){
            resultNFA.addGraphPath(nfa1.getGraphTable.get(i));
        }
        
        for(int i=0; i<nfa2.getEndList().size();i++){
            Integer item = nfa2.getEndList().get(i);
            if(item != nfa2.getStart()){
                resultNFAEndList.add(item);
            }
        }

        resultNFA.setEndList(resultNFAEndList);
        
        concatNFAGraph(nfa2, nfa1.getEndList(), nfa1.getGraphTable(), nfa2.getGraphTable(), resultNFA);

        return resultNFA;
    }

    /**
     * xy*
     */
    public NFATree nfaRepeatOrNone(NFATree nfa1, NFATree nfa2){
        NFATree resultNFA = new NFATree();

        resultNFA.setEndList(nfa1.getEndList());
        resultNFA.setStart(nfa1.getStart());

        Integer nfa2Start = nfa2.getStart();

        ArrayList<Integer> nfa1EndList = nfa1.getEndList();
        ArrayList<Integer> nfa2EndList = nfa2.getEndList();

        for(int i=0;i<nfa1.getGraphTable().size();i++){
            resultNFA.addGraphPath(nfa1.getGraphTable().get(i));
        }

        for(int i=0;i<nfa1EndList.size();i++){
            Integer currentNfa1End = nfa1EndList.get(i);
            
            for(int j=0;j<nfa2.getGraphTable().size();j++){
                NFATreePath path = nfa2.getGraphTable().get(j);
                
                Integer pathOrig = path.getOrig();
                Integer pathDest = path.getDest();

                if(pathOrig == nfa2.getStart()){
                    pathOrig = currentNfa1End;
                }
                
                if(nfa2EndList.contains(pathDest)){
                    pathDest = currentNfa1End;
                }

                path = new NFATreePath((int)pathOrig,(int) pathDest, path.getPatt());

                if (!resultNFA.getGraphTable().contains(path)){
                    resultNFA.addGraphPath(path);
                }
            
            }
        }

        return resultNFA;
    
    }
}
    
/*
    '''       

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

*/