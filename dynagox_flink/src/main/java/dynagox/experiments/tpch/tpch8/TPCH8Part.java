package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8Part implements MultiHashMapKey {
    public int p_partkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8Part previous;
    private TPCH8Part next;

    public TPCH8Part() {
        this.p_partkey = 0;
        this.annotation = 0.0;
        this.approximate = 0.0;
        this.active = false;
        this.previous = null;
        this.next = null;
    }

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
        this.next = (TPCH8Part) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8Part) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8Part result = new TPCH8Part();
        result.p_partkey = this.p_partkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8Part> {
        @Override
        public int hash(TPCH8Part key) {
            return (key.p_partkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH8Part> {
        @Override
        public boolean equal(TPCH8Part a, TPCH8Part b) {
            return a.p_partkey == b.p_partkey;
        }
    }
}
