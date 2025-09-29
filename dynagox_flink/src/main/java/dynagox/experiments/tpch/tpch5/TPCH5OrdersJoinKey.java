package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5OrdersJoinKey implements MultiHashMapKey {
    public int o_orderkey;
    public int c_nationkey;
    String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5OrdersJoinKey previous;
    private TPCH5OrdersJoinKey next;

    public TPCH5OrdersJoinKey() {
        this.o_orderkey = 0;
        this.c_nationkey = 0;
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
        this.next = (TPCH5OrdersJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5OrdersJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5OrdersJoinKey result = new TPCH5OrdersJoinKey();
        result.o_orderkey = this.o_orderkey;
        result.c_nationkey = this.c_nationkey;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH5OrdersJoinKey> {
        @Override
        public int hash(TPCH5OrdersJoinKey key) {
            return (key.o_orderkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH5OrdersJoinKey> {
        @Override
        public boolean equal(TPCH5OrdersJoinKey a, TPCH5OrdersJoinKey b) {
            return a.o_orderkey == b.o_orderkey;
        }
    }
}
