package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS15DateDimEvent extends TPCDSEvent {
    public int d_date_sk;

    public TPCDS15DateDimEvent(int key, boolean is_insert, int d_date_sk) {
        super(key, is_insert);
        this.d_date_sk = d_date_sk;
    }
}
