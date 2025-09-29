package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5CustomerJoinKey implements MultiHashMapKey {
    public int c_custkey;
    public int c_nationkey;
    String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5CustomerJoinKey previous;
    private TPCH5CustomerJoinKey next;

    public TPCH5CustomerJoinKey() {
        this.c_custkey = 0;
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
        this.next = (TPCH5CustomerJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5CustomerJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5CustomerJoinKey result = new TPCH5CustomerJoinKey();
        result.c_custkey = this.c_custkey;
        result.c_nationkey = this.c_nationkey;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH5CustomerJoinKey> {
        @Override
        public int hash(TPCH5CustomerJoinKey key) {
            return (key.c_custkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH5CustomerJoinKey> {
        @Override
        public boolean equal(TPCH5CustomerJoinKey a, TPCH5CustomerJoinKey b) {
            return a.c_custkey == b.c_custkey;
        }
    }
}
