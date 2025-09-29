package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8Orders implements MultiHashMapKey {
    public int o_orderkey;
    public int o_custkey;
    public int o_year;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8Orders previous;
    private TPCH8Orders next;

    public TPCH8Orders() {
        this.o_orderkey = 0;
        this.o_custkey = 0;
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
        this.next = (TPCH8Orders) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8Orders) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8Orders result = new TPCH8Orders();
        result.o_orderkey = this.o_orderkey;
        result.o_custkey = this.o_custkey;
        result.o_year = this.o_year;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash0 implements HashFunction<TPCH8Orders> {
        @Override
        public int hash(TPCH8Orders key) {
            return (key.o_orderkey + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCH8Orders> {
        @Override
        public boolean equal(TPCH8Orders a, TPCH8Orders b) {
            return a.o_orderkey == b.o_orderkey;
        }
    }

    public static class Hash1 implements HashFunction<TPCH8Orders> {
        @Override
        public int hash(TPCH8Orders key) {
            return (key.o_custkey + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCH8Orders> {
        @Override
        public boolean equal(TPCH8Orders a, TPCH8Orders b) {
            return a.o_custkey == b.o_custkey;
        }
    }
}
