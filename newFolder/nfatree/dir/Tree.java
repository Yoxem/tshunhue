package dir;
import java.util.ArrayList;
import java.util.List;

public class Tree<T> {
    private T node;
    private List<Tree<T>> children;

    public Tree(T data){
        this.node = data;
        this.children = new ArrayList<Tree<T>>();
    }

    public void addChild(T child) {
        Tree<T> childNode = new Tree<T>(child);
        this.children.add(childNode);
    }

    public void appendChildrenList(List<T> childList) {
        for(int i=0;i<childList.size();i++){
            Tree<T> childNode = new Tree<T>(childList.get(i));
            this.children.add(childNode);
        }
    }

    public T getNode(){
        return this.node;
    }

    public List<Tree<T>> getChildrenList(){
        return this.children;
    }
}