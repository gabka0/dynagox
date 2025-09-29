package dynagox.experiments.hashmap;

import java.io.Serializable;

public class PrimaryHashIndexNode<T> implements Serializable {
    T object;
    int hashValue;
    PrimaryHashIndexNode<T> next;

    PrimaryHashIndexNode(T object, int hashValue) {
        this.object = object;
        this.hashValue = hashValue;
        this.next = null;
    }
}