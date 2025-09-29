package dynagox.experiments.common;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class Tuple1 implements MultiHashMapKey {
    public int f0;
    private double annotation;
    private double approximate;
    private boolean active;
    private Tuple1 previous;
    private Tuple1 next;


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
        this.next = (Tuple1) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (Tuple1) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        Tuple1 result = new Tuple1();
        result.f0 = f0;
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }

    public static class Hash implements HashFunction<Tuple1> {
        @Override
        public int hash(Tuple1 key) {
            return key.f0;
        }
    }

    public static class Equal implements EqualFunction<Tuple1> {
        @Override
        public boolean equal(Tuple1 a, Tuple1 b) {
            return a.f0 == b.f0;
        }
    }
}