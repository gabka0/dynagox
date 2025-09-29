package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH3CustomerEvent extends TPCHEvent {
    public int c_custkey;

    public TPCH3CustomerEvent(int key, boolean is_insert, int c_custkey) {
        super(key, is_insert);
        this.c_custkey = c_custkey;
    }
}
