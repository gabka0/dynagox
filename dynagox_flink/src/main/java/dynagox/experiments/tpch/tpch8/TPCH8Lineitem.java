package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8Lineitem implements MultiHashMapKey {
    public int l_orderkey;
    public int l_partkey;
    public int l_suppkey;
    public int l_linenumber;
    public double l_extendedprice;
    public double l_discount;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8Lineitem previous;
    private TPCH8Lineitem next;

    public TPCH8Lineitem() {
        this.l_orderkey = 0;
        this.l_partkey = 0;
        this.l_suppkey = 0;
        this.l_linenumber = 0;
        this.l_extendedprice = 0.0;
        this.l_discount = 0.0;
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
        this.next = (TPCH8Lineitem) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8Lineitem) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8Lineitem result = new TPCH8Lineitem();
        result.l_orderkey = this.l_orderkey;
        result.l_partkey = this.l_partkey;
        result.l_suppkey = this.l_suppkey;
        result.l_linenumber = this.l_linenumber;
        result.l_extendedprice = this.l_extendedprice;
        result.l_discount = this.l_discount;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8Lineitem> {
        @Override
        public int hash(TPCH8Lineitem key) {
            long hash_value = key.l_orderkey + 0x9e3779b9L;
            hash_value = key.l_linenumber + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCH8Lineitem> {
        @Override
        public boolean equal(TPCH8Lineitem a, TPCH8Lineitem b) {
            return a.l_orderkey == b.l_orderkey && a.l_linenumber == b.l_linenumber;
        }
    }

    public static class Hash1 implements HashFunction<TPCH8Lineitem> {
        @Override
        public int hash(TPCH8Lineitem key) {
            return (key.l_partkey + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCH8Lineitem> {
        @Override
        public boolean equal(TPCH8Lineitem a, TPCH8Lineitem b) {
            return a.l_partkey == b.l_partkey;
        }
    }

    public static class Hash2 implements HashFunction<TPCH8Lineitem> {
        @Override
        public int hash(TPCH8Lineitem key) {
            return (key.l_suppkey + 0x9e3779b9);
        }
    }

    public static class Equal2 implements EqualFunction<TPCH8Lineitem> {
        @Override
        public boolean equal(TPCH8Lineitem a, TPCH8Lineitem b) {
            return a.l_suppkey == b.l_suppkey;
        }
    }
}
