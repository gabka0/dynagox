package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15CustomerAddress implements MultiHashMapKey {
    public int ca_address_sk;
    public String ca_zip;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15CustomerAddress previous;
    private TPCDS15CustomerAddress next;

    public TPCDS15CustomerAddress() {
        this.ca_address_sk = 0;
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
        this.next = (TPCDS15CustomerAddress) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15CustomerAddress) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15CustomerAddress result = new TPCDS15CustomerAddress();
        result.ca_address_sk = this.ca_address_sk;
        result.ca_zip = this.ca_zip;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15CustomerAddress> {
        @Override
        public int hash(TPCDS15CustomerAddress key) {
            return (key.ca_address_sk + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCDS15CustomerAddress> {
        @Override
        public boolean equal(TPCDS15CustomerAddress a, TPCDS15CustomerAddress b) {
            return a.ca_address_sk == b.ca_address_sk;
        }
    }
}
