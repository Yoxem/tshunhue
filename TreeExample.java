package app;
import java.util.ArrayList;
import java.util.List;

public class TreeExample {
    public static void main(String[] args) throws Exception {
        Tree<String> aSyntaxTree = new Tree<String>("123");

        List<Tree<String>> syntaxChildrenList = new ArrayList<Tree<String>>();
        syntaxChildrenList.add(new Tree<String>("456"));
        syntaxChildrenList.add(new Tree<String>("789"));

        Tree<String> aBiggerSyntaxTree = new Tree<String>(syntaxChildrenList);
        
        
        List<Tree<String>> aMuchLargerSyntaxList = new ArrayList<Tree<String>>();
        aMuchLargerSyntaxList.add(aBiggerSyntaxTree);
        aMuchLargerSyntaxList.add(aSyntaxTree);
        aMuchLargerSyntaxList.add(new Tree<String>("Happy new Asian year"));

        Tree<String> aMuchLargerSyntaxTree = new Tree<String>(aMuchLargerSyntaxList);

        System.out.println(aSyntaxTree);
        System.out.println(aBiggerSyntaxTree);
        System.out.println(aMuchLargerSyntaxTree);
    }
}

enum TreeContain {NODE, TREE};

class Tree<T>{
    
    
    public TreeContain treeContain;
    private T nodeValue;
    private List<Tree<T>> children;

    public Tree(T unitItem){
        this.treeContain = TreeContain.NODE;
        this.nodeValue = unitItem;
    }

    public Tree(List<Tree<T>> itemList){
        this.treeContain = TreeContain.TREE;
        this.children = itemList;
    }

    public String toString(){
        if (this.treeContain == TreeContain.NODE){
            String resultString = "\"" ;
            resultString += nodeValue.toString();
            resultString += "\"";
            return resultString;
        }
        /* for tree*/
        else{
            String resultString = "(";
            for (int i = 0; i < children.size(); i++){
                resultString += children.get(i).toString();

                if (i < children.size() - 1){
                    resultString += ", ";
                }
            }

            resultString += ")";

            return resultString;
        }
    }
}
