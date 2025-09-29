package dynagox.experiments.common;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TupleL2 implements MultiHashMapKey {
    public long f0;
    public long f1;
    private double annotation;
    private double approximate;
    private boolean active;
    private TupleL2 previous;
    private TupleL2 next;


    @Override
    public double get_annotation() {
        return annotation;
    }

    @Override
    public void set_annotation(double annotation) {
        this.annotation = annotation;
    }

    @Override
    public double get_approximate() {
        return approximate;
    }

    @Override
    public void set_approximate(double approximate) {
        this.approximate = approximate;
    }

    @Override
    public boolean is_active() {
        return active;
    }

    @Override
    public void set_active(boolean active) {
        this.active = active;
    }

    @Override
    public MultiHashMapKey get_next() {
        return next;
    }

    @Override
    public void set_next(MultiHashMapKey next) {
        this.next = (TupleL2) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TupleL2) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TupleL2 result = new TupleL2();
        result.f0 = f0;
        result.f1 = f1;
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }

    public static class Hash implements HashFunction<TupleL2> {
        @Override
        public int hash(TupleL2 key) {
            return (int) ((key.f0 << 16) + key.f1);
        }
    }

    public static class Equal implements EqualFunction<TupleL2> {
        @Override
        public boolean equal(TupleL2 a, TupleL2 b) {
            return a.f0 == b.f0 && a.f1 == b.f1;
        }
    }

    public static class Hash0 implements HashFunction<TupleL2> {
        @Override
        public int hash(TupleL2 key) {
            return (int) key.f0;
        }
    }

    public static class Equal0 implements EqualFunction<TupleL2> {
        @Override
        public boolean equal(TupleL2 a, TupleL2 b) {
            return a.f0 == b.f0;
        }
    }

    public static class Hash1 implements HashFunction<TupleL2> {
        @Override
        public int hash(TupleL2 key) {
            return (int) key.f1;
        }
    }

    public static class Equal1 implements EqualFunction<TupleL2> {
        @Override
        public boolean equal(TupleL2 a, TupleL2 b) {
            return a.f1 == b.f1;
        }
    }
}
