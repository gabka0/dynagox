package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19CustomerAddressEvent extends TPCDSEvent {
    public int ca_address_sk;

    public TPCDS19CustomerAddressEvent(int key, boolean is_insert, int ca_address_sk) {
        super(key, is_insert);
        this.ca_address_sk = ca_address_sk;
    }
}
