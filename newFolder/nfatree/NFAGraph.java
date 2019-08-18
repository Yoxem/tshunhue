// package NFAGraph;
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
* structure for NFAGraph
*/
class NFAGraph{
    private Integer start;
    private ArrayList<Integer> endList; // List of end node num.
    private ArrayList<NFAGraphPath> graphTable; // graph shown as table

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

    public ArrayList<NFAGraphPath> getGraphTable(){
        return this.graphTable;
    }

    public setEndList(ArrayList<Integer> endList){
        this.endList = endList;
    }

    public setStart(Integer startId){
        this.start = startId;
    }

    public addGraphPath(NFAGraphPath path){
        this.graphTable.add(path);
    }


    public static void main(String[] args) {
        NFAGraph _a = new NFAGraph();
        _a.start = 1;
        NFAGraphPath p1 = new NFAGraphPath(1, 2, "a");
        NFAGraphPath p2 = new NFAGraphPath(2, 3, "g");
        NFAGraphPath p3 = new NFAGraphPath(2, 4, "c");

        Tree<String> pattern = new Tree<String>("SET");
        pattern.appendChildrenList(Arrays.asList("F", "G", "H"));
        NFAGraphPath p4 = new NFAGraphPath(2, 5, pattern);

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

    public NFAGraph(int orig, int dest, NFAGraphPath path){
        this.start = (Integer) orig;
        this.endList = new ArrayList<Integer>(Arrays.asList((Integer) dest));
        this.graphTable = new ArrayList<NFAGraphPath>(Arrays.asList(path));
    }

    public NFAGraph(){
        this.start = (Integer)-1 ; // impossible value
        this.endList = new ArrayList<Integer>();
        this.graphTable = new ArrayList<NFAGraphPath>();
    }

    public NFAGraph(int orig, ArrayList<Integer> destList, NFAGraphPath path){
        this.start = (Integer)orig;
        this.endList = destList;
        this.graphTable = new ArrayList<NFAGraphPath>(Arrays.asList(path));
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
* Path for NFAGraph.graph
* [2]-- a ->[3]
* orig = 2;
* dest = 3;
* patt = ['a'];
*/
class NFAGraphPath {
    private Integer orig;
    private Integer dest;
    private Tree<String> patt;

    public NFAGraphPath(int ori, int des, String ptn) {
        this.orig = (Integer) ori;
        this.dest = (Integer) des;
        this.patt = new Tree<String>(ptn);

    }

    public NFAGraphPath(int ori, int des, Tree<String> ptnTree) {
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

    public NFAGraph makeSimpleNFA(String pattern){
        Integer start = this.nextId();
        Integer end = this.nextId();
        NFAGraphPath path = NFAGraphPath(start, end, pattern);
        NFAGraph nfa = NFAGraph(start, end, path);
        return nfa;
    }

    public NFAGraph makeSimpleNFA(Tree<String> pattern){
        Integer start = this.nextId();
        Integer end = this.nextId();
        NFAGraphPath path = NFAGraphPath(start, end, pattern);
        NFAGraph nfa = NFAGraph(start, end, path);
        return nfa;
    }

    /**
    * xy
    */
    public nfaConcat(NFAGraph nfa1, NFAGraph nfa2){
        Integer nfa1Start = nfa1.getStart();
        ArrayList<Integer> nfa1EndList = nfa1.getEndList();
        
        ArrayList<NFAGraphPath> nfa1NFAGraphList = nfa1.getGraphTable();
        ArrayList<NFAGraphPath> nfa2NFAGraphList = nfa2.getGraphTable();

        NFAGraph resultNFA = new NFAGraph();

        resultNFA.setStart(nfa1Start);

        concatNFAGraph(nfa2, nfa1EndList, nfa1NFAGraphList, nfa2NFAGraphList, resultNFA);

        resultNFA.setEndList(nfa2.getEndList());
        return resultNFA;
    }

    private void concatNFAGraph(NFAGraph nfa2, ArrayList<Integer> nfa1EndList, ArrayList<NFAGraphPath> nfa1NFAGraphList,
            ArrayList<NFAGraphPath> nfa2NFAGraphList, NFAGraph resultNFA) {
        for(int i=0; i<nfa1NFAGraphList.size();i++){
            resultNFA.addGraphPath(nfa1NFAGraphList.get(i));
        }

        for(int i=0; i<nfa1EndList.size();i++){
            for (int j=0; i<nfa2NFAGraphList.size();j++){
                NFAGraphPath path = nfa2NFAGraphList.get(j);

                if(path.getOrig() == (Integer) nfa1EndList.get(i)){
                    NFAGraphPath newPath =
                        new NFAGraphPath(nfa2.getStart(), path.getDest(), path.getPatt());
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
    public nfaOr(NFAGraph nfa1, NFAGraph nfa2){
        Integer nfa1Start = nfa1.getStart();
        Integer nfa2Start = nfa2.getStart();

        ArrayList<Integer> nfa1EndList = nfa1.getEndList();
        ArrayList<Integer> nfa2EndList = nfa2.getEndList();

        ArrayList<NFAGraphPath> nfa1NFAGraphList = nfa1.getGraphTable();
        ArrayList<NFAGraphPath> nfa2NFAGraphList = nfa2.getGraphTable();

        NFAGraph resultNFA = new NFAGraph();
        resultNFA.setStart(nfa1Start);
        ArrayList<Integer> NewEndList = nfa1EndList;

        for(int i=0; i<nfa1NFAGraphList.size();i++){
            resultNFA.addGraphPath(nfa1NFAGraphList.get(i));
        }

        for(int i=0;i<nfa2NFAGraphList.size();i++){
            NFAGraphPath path = nfa2NFAGraphList.get(i);

            if(path.getOrig() == nfa2Start){
                NFAGraphPath newPath = new NFAGraphPath(nfa1Start, path.getDest(), path.getPatt());
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
    public NFAGraph nfaOnceOrNone(NFAGraph nfa1, NFAGraph nfa2){
        NFAGraph resultNFA = new NFAGraph();
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
    public NFAGraph nfaRepeatOrNone(NFAGraph nfa1, NFAGraph nfa2){
        NFAGraph resultNFA = new NFAGraph();

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
                NFAGraphPath path = nfa2.getGraphTable().get(j);
                
                Integer pathOrig = path.getOrig();
                Integer pathDest = path.getDest();

                if(pathOrig == nfa2.getStart()){
                    pathOrig = currentNfa1End;
                }
                
                if(nfa2EndList.contains(pathDest)){
                    pathDest = currentNfa1End;
                }

                path = new NFAGraphPath((int)pathOrig,(int) pathDest, path.getPatt());

                if (!resultNFA.getGraphTable().contains(path)){
                    resultNFA.addGraphPath(path);
                }
            
            }
        }

        return resultNFA;
    
    }
}
    
/*

*/