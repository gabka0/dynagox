package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS19CustomerAddress implements MultiHashMapKey {
    public int ca_address_sk;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS19CustomerAddress previous;
    private TPCDS19CustomerAddress next;

    public TPCDS19CustomerAddress() {
        this.ca_address_sk = 0;
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
        this.next = (TPCDS19CustomerAddress) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS19CustomerAddress) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS19CustomerAddress result = new TPCDS19CustomerAddress();
        result.ca_address_sk = this.ca_address_sk;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS19CustomerAddress> {
        @Override
        public int hash(TPCDS19CustomerAddress key) {
            return (key.ca_address_sk + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCDS19CustomerAddress> {
        @Override
        public boolean equal(TPCDS19CustomerAddress a, TPCDS19CustomerAddress b) {
            return a.ca_address_sk == b.ca_address_sk;
        }
    }
}
