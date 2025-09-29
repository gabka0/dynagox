package dynagox.experiments.hashmap;

import java.io.Serializable;

public interface MultiHashMapKey extends Serializable {
    double get_annotation();

    void set_annotation(double annotation);

    double get_approximate();

    void set_approximate(double approximate);

    boolean is_active();

    void set_active(boolean active);

    MultiHashMapKey get_next();

    void set_next(MultiHashMapKey next);

    MultiHashMapKey get_previous();

    void set_previous(MultiHashMapKey previous);

    MultiHashMapKey copy();
}