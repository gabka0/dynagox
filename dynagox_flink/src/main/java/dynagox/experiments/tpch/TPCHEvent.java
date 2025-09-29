package dynagox.experiments.tpch;

import java.io.Serializable;

public class TPCHEvent implements Serializable {
    public int key;
    public boolean is_insert;

    public TPCHEvent(int key, boolean is_insert) {
        this.key = key;
        this.is_insert = is_insert;
    }
}
