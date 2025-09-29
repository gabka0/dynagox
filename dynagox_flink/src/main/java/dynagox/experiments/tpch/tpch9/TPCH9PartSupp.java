package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH9PartSupp implements MultiHashMapKey {
    public int ps_partkey;
    public int ps_suppkey;
    double ps_supplycost;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH9PartSupp previous;
    private TPCH9PartSupp next;

    public TPCH9PartSupp() {
        this.ps_partkey = 0;
        this.ps_suppkey = 0;
        this.ps_supplycost = 0.0;
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
        this.next = (TPCH9PartSupp) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH9PartSupp) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH9PartSupp result = new TPCH9PartSupp();
        result.ps_partkey = this.ps_partkey;
        result.ps_suppkey = this.ps_suppkey;
        result.ps_supplycost = this.ps_supplycost;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH9PartSupp> {
        @Override
        public int hash(TPCH9PartSupp key) {
            long hash_value = key.ps_partkey + 0x9e3779b9L;
            hash_value = key.ps_suppkey + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCH9PartSupp> {
        @Override
        public boolean equal(TPCH9PartSupp a, TPCH9PartSupp b) {
            return a.ps_partkey == b.ps_partkey && a.ps_suppkey == b.ps_suppkey;
        }
    }
}
