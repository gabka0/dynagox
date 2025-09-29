package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19DateDimEvent extends TPCDSEvent {
    public int d_date_sk;

    public TPCDS19DateDimEvent(int key, boolean is_insert, int d_date_sk) {
        super(key, is_insert);
        this.d_date_sk = d_date_sk;
    }
}
