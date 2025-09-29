package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH8CustomerEvent extends TPCHEvent {
    public int c_custkey;
    public int c_nationkey;

    public TPCH8CustomerEvent(int key, boolean is_insert, int c_custkey, int c_nationkey) {
        super(key, is_insert);
        this.c_custkey = c_custkey;
        this.c_nationkey = c_nationkey;
    }
}
