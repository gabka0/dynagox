package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS19Customer implements MultiHashMapKey {
    public int c_customer_sk;
    public int c_current_addr_sk;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS19Customer previous;
    private TPCDS19Customer next;

    public TPCDS19Customer() {
        this.c_customer_sk = 0;
        this.c_current_addr_sk = 0;
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
        this.next = (TPCDS19Customer) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS19Customer) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS19Customer result = new TPCDS19Customer();
        result.c_customer_sk = this.c_customer_sk;
        result.c_current_addr_sk = this.c_current_addr_sk;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS19Customer> {
        @Override
        public int hash(TPCDS19Customer key) {
            return (key.c_customer_sk + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCDS19Customer> {
        @Override
        public boolean equal(TPCDS19Customer a, TPCDS19Customer b) {
            return a.c_customer_sk == b.c_customer_sk;
        }
    }

    public static class Hash1 implements HashFunction<TPCDS19Customer> {
        @Override
        public int hash(TPCDS19Customer key) {
            return (key.c_current_addr_sk + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCDS19Customer> {
        @Override
        public boolean equal(TPCDS19Customer a, TPCDS19Customer b) {
            return a.c_current_addr_sk == b.c_current_addr_sk;
        }
    }
}
