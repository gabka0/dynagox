package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5Region implements MultiHashMapKey {
    public int r_regionkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5Region previous;
    private TPCH5Region next;

    public TPCH5Region() {
        this.r_regionkey = 0;
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
        this.next = (TPCH5Region) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5Region) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5Region result = new TPCH5Region();
        result.r_regionkey = this.r_regionkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH5Region> {
        @Override
        public int hash(TPCH5Region key) {
            return (key.r_regionkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH5Region> {
        @Override
        public boolean equal(TPCH5Region a, TPCH5Region b) {
            return a.r_regionkey == b.r_regionkey;
        }
    }
}
