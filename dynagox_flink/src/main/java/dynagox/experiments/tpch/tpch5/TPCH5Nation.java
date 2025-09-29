package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5Nation implements MultiHashMapKey {
    public int n_nationkey;
    public String n_name;
    public int n_regionkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5Nation previous;
    private TPCH5Nation next;

    public TPCH5Nation() {
        this.n_nationkey = 0;
        this.n_name = "";
        this.n_regionkey = 0;
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
        this.next = (TPCH5Nation) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5Nation) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5Nation result = new TPCH5Nation();
        result.n_nationkey = this.n_nationkey;
        result.n_name = this.n_name;
        result.n_regionkey = this.n_regionkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash0 implements HashFunction<TPCH5Nation> {
        @Override
        public int hash(TPCH5Nation key) {
            return (key.n_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCH5Nation> {
        @Override
        public boolean equal(TPCH5Nation a, TPCH5Nation b) {
            return a.n_nationkey == b.n_nationkey;
        }
    }

    public static class Hash2 implements HashFunction<TPCH5Nation> {
        @Override
        public int hash(TPCH5Nation key) {
            return (key.n_regionkey + 0x9e3779b9);
        }
    }

    public static class Equal2 implements EqualFunction<TPCH5Nation> {
        @Override
        public boolean equal(TPCH5Nation a, TPCH5Nation b) {
            return a.n_regionkey == b.n_regionkey;
        }
    }
}
