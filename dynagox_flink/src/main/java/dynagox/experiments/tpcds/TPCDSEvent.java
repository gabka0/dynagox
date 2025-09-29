package dynagox.experiments.tpcds;

import java.io.Serializable;

public class TPCDSEvent implements Serializable {
    public int key;
    public boolean is_insert;

    public TPCDSEvent(int key, boolean is_insert) {
        this.key = key;
        this.is_insert = is_insert;
    }
}
