package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5NationJoinKey implements MultiHashMapKey {
    public int n_nationkey;
    public String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5NationJoinKey previous;
    private TPCH5NationJoinKey next;

    public TPCH5NationJoinKey() {
        this.n_nationkey = 0;
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
        this.next = (TPCH5NationJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5NationJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5NationJoinKey result = new TPCH5NationJoinKey();
        result.n_nationkey = this.n_nationkey;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH5NationJoinKey> {
        @Override
        public int hash(TPCH5NationJoinKey key) {
            return (key.n_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH5NationJoinKey> {
        @Override
        public boolean equal(TPCH5NationJoinKey a, TPCH5NationJoinKey b) {
            return a.n_nationkey == b.n_nationkey;
        }
    }
}
