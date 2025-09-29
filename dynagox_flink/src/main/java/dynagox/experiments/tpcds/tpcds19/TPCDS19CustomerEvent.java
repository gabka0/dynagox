package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19CustomerEvent extends TPCDSEvent {
    public int c_customer_sk;
    public int c_current_addr_sk;

    public TPCDS19CustomerEvent(int key, boolean is_insert, int c_customer_sk, int c_current_addr_sk) {
        super(key, is_insert);
        this.c_customer_sk = c_customer_sk;
        this.c_current_addr_sk = c_current_addr_sk;
    }
}
