package dynagox.experiments.hashmap;

import java.io.Serializable;

public class LinkedListNode<T> implements Serializable {
    public T object;
    public LinkedListNode<T> next;

    LinkedListNode(T object) {
        this.object = object;
        this.next = null;
    }
}