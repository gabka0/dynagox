package dynagox.experiments.hashmap;

import java.io.Serializable;

public interface AbstractSecondaryHashIndex<T> extends Serializable {
    void put(T object);

    LinkedListNode<T> find(T key);

    void remove(T object);
}
