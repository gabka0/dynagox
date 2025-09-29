package dynagox.experiments.hashmap;

import java.io.Serializable;

public class SecondaryHashIndexNode<T> implements Serializable {
    LinkedListNode<T> linkedListNode;
    int hashValue;
    SecondaryHashIndexNode<T> next;

    SecondaryHashIndexNode(T object, int hashValue) {
        this.linkedListNode = new LinkedListNode<>(object);
        this.hashValue = hashValue;
        this.next = null;
    }
}