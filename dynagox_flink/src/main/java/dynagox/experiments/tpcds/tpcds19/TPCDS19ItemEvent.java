package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEvent;

public class TPCDS19ItemEvent extends TPCDSEvent {
    public int i_item_sk;
    public int i_brand_id;
    public String i_brand;
    public int i_manufact_id;
    public String i_manufact;

    public TPCDS19ItemEvent(int key, boolean is_insert, int i_item_sk, int i_brand_id, String i_brand, int i_manufact_id, String i_manufact) {
        super(key, is_insert);
        this.i_item_sk = i_item_sk;
        this.i_brand_id = i_brand_id;
        this.i_brand = i_brand;
        this.i_manufact_id = i_manufact_id;
        this.i_manufact = i_manufact;
    }
}
