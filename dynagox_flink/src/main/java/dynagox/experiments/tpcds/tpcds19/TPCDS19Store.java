package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS19Store implements MultiHashMapKey {
    public int s_store_sk;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS19Store previous;
    private TPCDS19Store next;

    public TPCDS19Store() {
        this.s_store_sk = 0;
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
        this.next = (TPCDS19Store) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS19Store) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS19Store result = new TPCDS19Store();
        result.s_store_sk = this.s_store_sk;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS19Store> {
        @Override
        public int hash(TPCDS19Store key) {
            long hash_value = key.s_store_sk + 0x9e3779b9L;
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS19Store> {
        @Override
        public boolean equal(TPCDS19Store a, TPCDS19Store b) {
            return a.s_store_sk == b.s_store_sk;
        }
    }
}
