package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15GCustomer implements MultiHashMapKey {
    public String ca_zip;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15GCustomer previous;
    private TPCDS15GCustomer next;

    public TPCDS15GCustomer() {
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
        this.next = (TPCDS15GCustomer) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15GCustomer) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15GCustomer result = new TPCDS15GCustomer();
        result.ca_zip = this.ca_zip;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15GCustomer> {
        @Override
        public int hash(TPCDS15GCustomer key) {
            long hash_value = key.ca_zip.hashCode() + 0x9e3779b9L;
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS15GCustomer> {
        @Override
        public boolean equal(TPCDS15GCustomer a, TPCDS15GCustomer b) {
            return a.ca_zip.equals(b.ca_zip);
        }
    }
}
