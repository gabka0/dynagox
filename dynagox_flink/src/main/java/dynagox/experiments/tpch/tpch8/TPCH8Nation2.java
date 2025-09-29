package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8Nation2 implements MultiHashMapKey {
    public int n_nationkey;
    public String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8Nation2 previous;
    private TPCH8Nation2 next;

    public TPCH8Nation2() {
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
        this.next = (TPCH8Nation2) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8Nation2) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8Nation2 result = new TPCH8Nation2();
        result.n_nationkey = this.n_nationkey;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8Nation2> {
        @Override
        public int hash(TPCH8Nation2 key) {
            return (key.n_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH8Nation2> {
        @Override
        public boolean equal(TPCH8Nation2 a, TPCH8Nation2 b) {
            return a.n_nationkey == b.n_nationkey;
        }
    }
}
