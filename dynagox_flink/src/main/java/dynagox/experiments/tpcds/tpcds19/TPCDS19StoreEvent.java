package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19StoreEvent extends TPCDSEvent {
    public int s_store_sk;

    public TPCDS19StoreEvent(int key, boolean is_insert, int s_store_sk) {
        super(key, is_insert);
        this.s_store_sk = s_store_sk;
    }
}
