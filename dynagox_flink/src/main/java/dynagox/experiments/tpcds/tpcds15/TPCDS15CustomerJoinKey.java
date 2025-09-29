package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15CustomerJoinKey implements MultiHashMapKey {
    public int c_customer_sk;
    public String ca_zip;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15CustomerJoinKey previous;
    private TPCDS15CustomerJoinKey next;

    public TPCDS15CustomerJoinKey() {
        this.c_customer_sk = 0;
        this.ca_zip = "";
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
        this.next = (TPCDS15CustomerJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15CustomerJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15CustomerJoinKey result = new TPCDS15CustomerJoinKey();
        result.c_customer_sk = this.c_customer_sk;
        result.ca_zip = this.ca_zip;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15CustomerJoinKey> {
        @Override
        public int hash(TPCDS15CustomerJoinKey key) {
            return (key.c_customer_sk + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCDS15CustomerJoinKey> {
        @Override
        public boolean equal(TPCDS15CustomerJoinKey a, TPCDS15CustomerJoinKey b) {
            return a.c_customer_sk == b.c_customer_sk;
        }
    }
}
