package dynagox.experiments.hashmap;

import java.io.Serializable;
import java.util.List;

public class MultiHashMap<T extends MultiHashMapKey> implements Serializable {

    private final PrimaryHashIndex<T> primaryHashIndex;
    private final List<SecondaryHashIndex<T>> secondaryHashIndexes;
    private T keys;

    public MultiHashMap(PrimaryHashIndex<T> primaryHashIndex, List<SecondaryHashIndex<T>> secondaryHashIndexes) {
        this.primaryHashIndex = primaryHashIndex;
        this.secondaryHashIndexes = secondaryHashIndexes;
        this.keys = null;
    }

    private boolean is_zero(double annotation) {
        return (-0.000001 < annotation) && (annotation < 0.000001);
    }

    private T put(T key, int hashValue) {
        T object = (T) key.copy();
        primaryHashIndex.put(object, hashValue);

        for (SecondaryHashIndex<T> index : secondaryHashIndexes) {
            index.put(object);
        }

        if (keys != null) {
            keys.set_previous(object);
        }
        object.set_previous(null);
        object.set_next(keys);
        keys = object;

        return object;
    }

    private T get(T key, int hashValue) {
        return primaryHashIndex.get(key, hashValue);
    }

    private void remove(T key, int hashValue) {
        if (key.get_previous() != null) {
            key.get_previous().set_next(key.get_next());
        }

        if (key.get_next() != null) {
            key.get_next().set_previous(key.get_previous());
        }

        if (keys == key) {
            keys = (T) key.get_next();
        }
        key.set_previous(null);
        key.set_next(null);

        primaryHashIndex.remove(key, hashValue);
        for (SecondaryHashIndex<T> index : secondaryHashIndexes) {
            index.remove(key);
        }
    }

    public T get(T key) {
        int hashValue = primaryHashIndex.hash(key);
        return primaryHashIndex.get(key, hashValue);
    }

    public LinkedListNode<T> find(int n, T key) {
        return secondaryHashIndexes.get(n).find(key);
    }

    public T get_keys() {
        return keys;
    }

    public void update_add(T key, double annotation) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            p.set_annotation(p.get_annotation() + annotation);
        } else {
            key.set_annotation(annotation);
            put(key, hashValue);
        }
    }

    public double update_add_approx(T key, double annotation, double b) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            p.set_annotation(p.get_annotation() + annotation);
            if (p.get_annotation() > p.get_approximate()) {
                double d = p.get_approximate();
                p.set_approximate(p.get_annotation() * b);
                return p.get_approximate() - d;
            } else {
                return 0;
            }
        } else {
            key.set_annotation(annotation);
            double approx = annotation * b;
            key.set_approximate(approx);
            put(key, hashValue);
            return approx;
        }
    }

    public boolean update_activate(T key) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            if (!p.is_active()) {
                p.set_active(true);
                return true;
            } else {
                return false;
            }
        } else {
            T object = put(key, hashValue);
            object.set_active(true);
            return true;
        }
    }

    public boolean update_add_and_activate(T key, double annotation) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            p.set_annotation(p.get_annotation() + annotation);
            if (!p.is_active()) {
                p.set_active(true);
                return true;
            } else {
                return false;
            }
        } else {
            key.set_annotation(annotation);
            T object = put(key, hashValue);
            object.set_active(true);
            return true;
        }
    }

    public boolean update_subtract(T key, double annotation) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p == null) throw new IllegalStateException("Key not found");
        p.set_annotation(p.get_annotation() - annotation);
        if (is_zero(p.get_annotation())) {
            remove(p, hashValue);
            return true;
        }
        return false;
    }

    public double update_subtract_approx(T key, double annotation, double b) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p == null) throw new IllegalStateException("Key not found");
        p.set_annotation(p.get_annotation() - annotation);
        if (p.get_annotation() * b < p.get_approximate()) {
            double d = p.get_approximate();
            p.set_approximate(p.get_annotation() * b);
            if (is_zero(p.get_approximate())) {
                remove(p, hashValue);
                return d;
            } else {
                double r = d - p.get_approximate();
                return is_zero(r) ? 0 : r;
            }
        }
        return 0;
    }

    public boolean update_deactivate(T key) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p == null) throw new IllegalStateException("Key not found");
        if (p.is_active()) {
            p.set_active(false);
            return true;
        }
        return false;
    }

    public void update_remove_if_exists(T key) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            remove(p, hashValue);
        }
    }

    public boolean update_max(T key, double annotation) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            if (annotation > p.get_annotation()) {
                p.set_annotation(annotation);
                return true;
            }
            return false;
        } else {
            key.set_annotation(annotation);
            put(key, hashValue);
            return true;
        }
    }

    public double update_max_approx(T key, double annotation, double b) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            p.set_annotation(Math.max(p.get_annotation(), annotation));
            if (p.get_annotation() > p.get_approximate()) {
                p.set_approximate(p.get_annotation() * b);
                return p.get_approximate();
            }
            return 0;
        } else {
            key.set_annotation(annotation);
            double approx = annotation * b;
            key.set_approximate(approx);
            put(key, hashValue);
            return approx;
        }
    }

    public double update_max2_approx(T key, double annotation, double e) {
        int hashValue = primaryHashIndex.hash(key);
        T p = get(key, hashValue);
        if (p != null) {
            p.set_annotation(Math.max(p.get_annotation(), annotation));
            if (p.get_annotation() > p.get_approximate()) {
                p.set_approximate(p.get_annotation() + e);
                return p.get_approximate();
            }
            return 0;
        } else {
            key.set_annotation(annotation);
            double approx = annotation + e;
            key.set_approximate(approx);
            put(key, hashValue);
            return approx;
        }
    }
}