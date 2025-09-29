package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH10LineitemJoinKey implements MultiHashMapKey {
    public int l_orderkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH10LineitemJoinKey previous;
    private TPCH10LineitemJoinKey next;

    public TPCH10LineitemJoinKey() {
        this.l_orderkey = 0;
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
        this.next = (TPCH10LineitemJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH10LineitemJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH10LineitemJoinKey result = new TPCH10LineitemJoinKey();
        result.l_orderkey = this.l_orderkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH10LineitemJoinKey> {
        @Override
        public int hash(TPCH10LineitemJoinKey key) {
            return (key.l_orderkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH10LineitemJoinKey> {
        @Override
        public boolean equal(TPCH10LineitemJoinKey a, TPCH10LineitemJoinKey b) {
            return a.l_orderkey == b.l_orderkey;
        }
    }
}
