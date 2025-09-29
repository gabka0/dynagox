package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15CatalogSales implements MultiHashMapKey {
    public int cs_sold_date_sk;
    public int cs_bill_customer_sk;
    public double cs_sales_price;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15CatalogSales previous;
    private TPCDS15CatalogSales next;

    public TPCDS15CatalogSales() {
        this.cs_sold_date_sk = 0;
        this.cs_bill_customer_sk = 0;
        this.cs_sales_price = 0.0;
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
        this.next = (TPCDS15CatalogSales) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15CatalogSales) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15CatalogSales result = new TPCDS15CatalogSales();
        result.cs_sold_date_sk = this.cs_sold_date_sk;
        result.cs_bill_customer_sk = this.cs_bill_customer_sk;
        result.cs_sales_price = this.cs_sales_price;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15CatalogSales> {
        @Override
        public int hash(TPCDS15CatalogSales key) {
            long hash_value = Double.hashCode(key.cs_sales_price);
            hash_value = key.cs_sold_date_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            hash_value = key.cs_bill_customer_sk + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS15CatalogSales> {
        @Override
        public boolean equal(TPCDS15CatalogSales a, TPCDS15CatalogSales b) {
            return a.cs_sold_date_sk == b.cs_sold_date_sk
                    && a.cs_bill_customer_sk == b.cs_bill_customer_sk
                    && a.cs_sales_price == b.cs_sales_price;
        }
    }

    public static class Hash0 implements HashFunction<TPCDS15CatalogSales> {
        @Override
        public int hash(TPCDS15CatalogSales key) {
            return (key.cs_sold_date_sk + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCDS15CatalogSales> {
        @Override
        public boolean equal(TPCDS15CatalogSales a, TPCDS15CatalogSales b) {
            return a.cs_sold_date_sk == b.cs_sold_date_sk;
        }
    }
}
