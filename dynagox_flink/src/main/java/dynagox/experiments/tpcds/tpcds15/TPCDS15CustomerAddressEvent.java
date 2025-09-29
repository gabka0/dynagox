package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS15CustomerAddressEvent extends TPCDSEvent {
    public int ca_address_sk;
    public String ca_zip;

    public TPCDS15CustomerAddressEvent(int key, boolean is_insert, int ca_address_sk, String ca_zip) {
        super(key, is_insert);
        this.ca_address_sk = ca_address_sk;
        this.ca_zip = ca_zip;
    }
}
