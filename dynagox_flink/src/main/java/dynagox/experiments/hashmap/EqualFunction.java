package dynagox.experiments.hashmap;

import java.io.Serializable;

public interface EqualFunction<T> extends Serializable {
    boolean equal(T a, T b);
}