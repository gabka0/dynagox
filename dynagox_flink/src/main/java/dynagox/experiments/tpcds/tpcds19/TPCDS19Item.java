package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS19Item implements MultiHashMapKey {
    public int i_item_sk;
    public int i_brand_id;
    String i_brand;
    public int i_manufact_id;
    String i_manufact;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS19Item previous;
    private TPCDS19Item next;

    public TPCDS19Item() {
        this.i_item_sk = 0;
        this.i_brand_id = 0;
        this.i_brand = "";
        this.i_manufact_id = 0;
        this.i_manufact = "";
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
        this.next = (TPCDS19Item) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS19Item) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS19Item result = new TPCDS19Item();
        result.i_item_sk = this.i_item_sk;
        result.i_brand_id = this.i_brand_id;
        result.i_brand = this.i_brand;
        result.i_manufact_id = this.i_manufact_id;
        result.i_manufact = this.i_manufact;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS19Item> {
        @Override
        public int hash(TPCDS19Item key) {
            long hash_value = key.i_item_sk + 0x9e3779b9L;
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS19Item> {
        @Override
        public boolean equal(TPCDS19Item a, TPCDS19Item b) {
            return a.i_item_sk == b.i_item_sk;
        }
    }
}
