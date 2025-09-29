package dynagox.experiments.hashmap;

import java.io.Serializable;

public interface HashFunction<T> extends Serializable {
    int hash(T key);
}