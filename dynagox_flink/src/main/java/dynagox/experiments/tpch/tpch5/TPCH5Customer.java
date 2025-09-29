package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5Customer implements MultiHashMapKey {
    public int c_custkey;
    public int c_nationkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5Customer previous;
    private TPCH5Customer next;

    public TPCH5Customer() {
        this.c_custkey = 0;
        this.c_nationkey = 0;
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
        this.next = (TPCH5Customer) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5Customer) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5Customer result = new TPCH5Customer();
        result.c_custkey = this.c_custkey;
        result.c_nationkey = this.c_nationkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash0 implements HashFunction<TPCH5Customer> {
        @Override
        public int hash(TPCH5Customer key) {
            return (key.c_custkey + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCH5Customer> {
        @Override
        public boolean equal(TPCH5Customer a, TPCH5Customer b) {
            return a.c_custkey == b.c_custkey;
        }
    }

    public static class Hash1 implements HashFunction<TPCH5Customer> {
        @Override
        public int hash(TPCH5Customer key) {
            return (key.c_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCH5Customer> {
        @Override
        public boolean equal(TPCH5Customer a, TPCH5Customer b) {
            return a.c_nationkey == b.c_nationkey;
        }
    }
}
