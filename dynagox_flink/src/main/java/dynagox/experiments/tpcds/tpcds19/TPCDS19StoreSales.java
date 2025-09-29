package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS19StoreSales implements MultiHashMapKey {
    public int ss_sold_date_sk;
    public int ss_item_sk;
    public int ss_customer_sk;
    public int ss_store_sk;
    public double ss_ext_sales_price;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS19StoreSales previous;
    private TPCDS19StoreSales next;

    public TPCDS19StoreSales() {
        this.ss_sold_date_sk = 0;
        this.ss_item_sk = 0;
        this.ss_customer_sk = 0;
        this.ss_store_sk = 0;
        this.ss_ext_sales_price = 0.0;
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
        this.next = (TPCDS19StoreSales) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS19StoreSales) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS19StoreSales result = new TPCDS19StoreSales();
        result.ss_sold_date_sk = this.ss_sold_date_sk;
        result.ss_item_sk = this.ss_item_sk;
        result.ss_customer_sk = this.ss_customer_sk;
        result.ss_store_sk = this.ss_store_sk;
        result.ss_ext_sales_price = this.ss_ext_sales_price;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS19StoreSales> {
        @Override
        public int hash(TPCDS19StoreSales key) {
            long hash_value = Double.hashCode(key.ss_ext_sales_price);
            hash_value = key.ss_sold_date_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            hash_value = key.ss_item_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            hash_value = key.ss_customer_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            hash_value = key.ss_store_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS19StoreSales> {
        @Override
        public boolean equal(TPCDS19StoreSales a, TPCDS19StoreSales b) {
            return a.ss_sold_date_sk == b.ss_sold_date_sk
                    && a.ss_item_sk == b.ss_item_sk
                    && a.ss_customer_sk == b.ss_customer_sk
                    && a.ss_store_sk == b.ss_store_sk
                    && a.ss_ext_sales_price == b.ss_ext_sales_price;
        }
    }

    public static class Hash0 implements HashFunction<TPCDS19StoreSales> {
        @Override
        public int hash(TPCDS19StoreSales key) {
            return (key.ss_sold_date_sk + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCDS19StoreSales> {
        @Override
        public boolean equal(TPCDS19StoreSales a, TPCDS19StoreSales b) {
            return a.ss_sold_date_sk == b.ss_sold_date_sk;
        }
    }

    public static class Hash1 implements HashFunction<TPCDS19StoreSales> {
        @Override
        public int hash(TPCDS19StoreSales key) {
            return (key.ss_item_sk + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCDS19StoreSales> {
        @Override
        public boolean equal(TPCDS19StoreSales a, TPCDS19StoreSales b) {
            return a.ss_item_sk == b.ss_item_sk;
        }
    }

    public static class Hash2 implements HashFunction<TPCDS19StoreSales> {
        @Override
        public int hash(TPCDS19StoreSales key) {
            return (key.ss_customer_sk + 0x9e3779b9);
        }
    }

    public static class Equal2 implements EqualFunction<TPCDS19StoreSales> {
        @Override
        public boolean equal(TPCDS19StoreSales a, TPCDS19StoreSales b) {
            return a.ss_customer_sk == b.ss_customer_sk;
        }
    }

    public static class Hash3 implements HashFunction<TPCDS19StoreSales> {
        @Override
        public int hash(TPCDS19StoreSales key) {
            return (key.ss_store_sk + 0x9e3779b9);
        }
    }

    public static class Equal3 implements EqualFunction<TPCDS19StoreSales> {
        @Override
        public boolean equal(TPCDS19StoreSales a, TPCDS19StoreSales b) {
            return a.ss_store_sk == b.ss_store_sk;
        }
    }
}
