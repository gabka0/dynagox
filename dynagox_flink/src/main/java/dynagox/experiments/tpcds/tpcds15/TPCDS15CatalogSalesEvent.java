package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS15CatalogSalesEvent extends TPCDSEvent {
    public int cs_sold_date_sk;
    public int cs_bill_customer_sk;
    public double cs_sales_price;

    public TPCDS15CatalogSalesEvent(int key, boolean is_insert, int cs_sold_date_sk, int cs_bill_customer_sk, double cs_sales_price) {
        super(key, is_insert);
        this.cs_sold_date_sk = cs_sold_date_sk;
        this.cs_bill_customer_sk = cs_bill_customer_sk;
        this.cs_sales_price = cs_sales_price;
    }
}
