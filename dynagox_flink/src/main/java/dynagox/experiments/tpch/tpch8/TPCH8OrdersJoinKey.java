package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8OrdersJoinKey implements MultiHashMapKey {
    public int o_orderkey;
    public int o_year;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8OrdersJoinKey previous;
    private TPCH8OrdersJoinKey next;

    public TPCH8OrdersJoinKey() {
        this.o_orderkey = 0;
        this.o_year = 0;
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
        this.next = (TPCH8OrdersJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8OrdersJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8OrdersJoinKey result = new TPCH8OrdersJoinKey();
        result.o_orderkey = this.o_orderkey;
        result.o_year = this.o_year;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8OrdersJoinKey> {
        @Override
        public int hash(TPCH8OrdersJoinKey key) {
            return (key.o_orderkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH8OrdersJoinKey> {
        @Override
        public boolean equal(TPCH8OrdersJoinKey a, TPCH8OrdersJoinKey b) {
            return a.o_orderkey == b.o_orderkey;
        }
    }
}
