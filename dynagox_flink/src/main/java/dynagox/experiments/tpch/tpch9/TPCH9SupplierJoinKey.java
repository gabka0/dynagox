package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH9SupplierJoinKey implements MultiHashMapKey {
    public int s_suppkey;
    public String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH9SupplierJoinKey previous;
    private TPCH9SupplierJoinKey next;

    public TPCH9SupplierJoinKey() {
        this.s_suppkey = 0;
        this.n_name = "";
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
        this.next = (TPCH9SupplierJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH9SupplierJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH9SupplierJoinKey result = new TPCH9SupplierJoinKey();
        result.s_suppkey = this.s_suppkey;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH9SupplierJoinKey> {
        @Override
        public int hash(TPCH9SupplierJoinKey key) {
            return (key.s_suppkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH9SupplierJoinKey> {
        @Override
        public boolean equal(TPCH9SupplierJoinKey a, TPCH9SupplierJoinKey b) {
            return a.s_suppkey == b.s_suppkey;
        }
    }
}
