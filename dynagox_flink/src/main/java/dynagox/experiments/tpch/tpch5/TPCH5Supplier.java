package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5Supplier implements MultiHashMapKey {
    public int s_suppkey;
    public int s_nationkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5Supplier previous;
    private TPCH5Supplier next;

    public TPCH5Supplier() {
        this.s_suppkey = 0;
        this.s_nationkey = 0;
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
        this.next = (TPCH5Supplier) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5Supplier) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5Supplier result = new TPCH5Supplier();
        result.s_suppkey = this.s_suppkey;
        result.s_nationkey = this.s_nationkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash0 implements HashFunction<TPCH5Supplier> {
        @Override
        public int hash(TPCH5Supplier key) {
            return (key.s_suppkey + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCH5Supplier> {
        @Override
        public boolean equal(TPCH5Supplier a, TPCH5Supplier b) {
            return a.s_suppkey == b.s_suppkey;
        }
    }

    public static class Hash1 implements HashFunction<TPCH5Supplier> {
        @Override
        public int hash(TPCH5Supplier key) {
            return (key.s_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCH5Supplier> {
        @Override
        public boolean equal(TPCH5Supplier a, TPCH5Supplier b) {
            return a.s_nationkey == b.s_nationkey;
        }
    }
}
