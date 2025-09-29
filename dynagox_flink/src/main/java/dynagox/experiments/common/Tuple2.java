package dynagox.experiments.common;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class Tuple2 implements MultiHashMapKey {
    public int f0;
    public int f1;
    private double annotation;
    private double approximate;
    private boolean active;
    private Tuple2 previous;
    private Tuple2 next;


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
        this.next = (Tuple2) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (Tuple2) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        Tuple2 result = new Tuple2();
        result.f0 = f0;
        result.f1 = f1;
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }

    public static class Hash implements HashFunction<Tuple2> {
        @Override
        public int hash(Tuple2 key) {
            return (key.f0 << 16) + key.f1;
        }
    }

    public static class Equal implements EqualFunction<Tuple2> {
        @Override
        public boolean equal(Tuple2 a, Tuple2 b) {
            return a.f0 == b.f0 && a.f1 == b.f1;
        }
    }

    public static class Hash0 implements HashFunction<Tuple2> {
        @Override
        public int hash(Tuple2 key) {
            return key.f0;
        }
    }

    public static class Equal0 implements EqualFunction<Tuple2> {
        @Override
        public boolean equal(Tuple2 a, Tuple2 b) {
            return a.f0 == b.f0;
        }
    }

    public static class Hash1 implements HashFunction<Tuple2> {
        @Override
        public int hash(Tuple2 key) {
            return key.f1;
        }
    }

    public static class Equal1 implements EqualFunction<Tuple2> {
        @Override
        public boolean equal(Tuple2 a, Tuple2 b) {
            return a.f1 == b.f1;
        }
    }
}
