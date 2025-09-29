package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH3Customer implements MultiHashMapKey {
    public int c_custkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH3Customer previous;
    private TPCH3Customer next;

    public TPCH3Customer() {
        this.c_custkey = 0;
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
        this.next = (TPCH3Customer) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH3Customer) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH3Customer result = new TPCH3Customer();
        result.c_custkey = this.c_custkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH3Customer> {
        @Override
        public int hash(TPCH3Customer key) {
            return (key.c_custkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH3Customer> {
        @Override
        public boolean equal(TPCH3Customer a, TPCH3Customer b) {
            return a.c_custkey == b.c_custkey;
        }
    }
}
