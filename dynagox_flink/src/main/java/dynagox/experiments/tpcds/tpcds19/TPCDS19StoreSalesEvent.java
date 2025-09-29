package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19StoreSalesEvent extends TPCDSEvent {
    public int ss_sold_date_sk;
    public int ss_item_sk;
    public int ss_customer_sk;
    public int ss_store_sk;
    public double ss_ext_sales_price;

    public TPCDS19StoreSalesEvent(int key, boolean is_insert, int ss_sold_date_sk, int ss_item_sk, int ss_customer_sk,
                                  int ss_store_sk, double ss_ext_sales_price) {
        super(key, is_insert);
        this.ss_sold_date_sk = ss_sold_date_sk;
        this.ss_item_sk = ss_item_sk;
        this.ss_customer_sk = ss_customer_sk;
        this.ss_store_sk = ss_store_sk;
        this.ss_ext_sales_price = ss_ext_sales_price;
    }
}
