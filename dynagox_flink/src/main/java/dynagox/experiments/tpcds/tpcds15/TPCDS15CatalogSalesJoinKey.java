package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15CatalogSalesJoinKey implements MultiHashMapKey {
    public int cs_bill_customer_sk;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15CatalogSalesJoinKey previous;
    private TPCDS15CatalogSalesJoinKey next;

    public TPCDS15CatalogSalesJoinKey() {
        this.cs_bill_customer_sk = 0;
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
        this.next = (TPCDS15CatalogSalesJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15CatalogSalesJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15CatalogSalesJoinKey result = new TPCDS15CatalogSalesJoinKey();
        result.cs_bill_customer_sk = this.cs_bill_customer_sk;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15CatalogSalesJoinKey> {
        @Override
        public int hash(TPCDS15CatalogSalesJoinKey key) {
            long hash_value = key.cs_bill_customer_sk + 0x9e3779b9L;
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS15CatalogSalesJoinKey> {
        @Override
        public boolean equal(TPCDS15CatalogSalesJoinKey a, TPCDS15CatalogSalesJoinKey b) {
            return a.cs_bill_customer_sk == b.cs_bill_customer_sk;
        }
    }
}
