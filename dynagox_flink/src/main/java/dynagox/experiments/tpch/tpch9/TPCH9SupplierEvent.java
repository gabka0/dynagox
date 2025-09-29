package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH9SupplierEvent extends TPCHEvent {
    public int s_suppkey;
    public int s_nationkey;

    public TPCH9SupplierEvent(int key, boolean is_insert, int s_suppkey, int s_nationkey) {
        super(key, is_insert);
        this.s_suppkey = s_suppkey;
        this.s_nationkey = s_nationkey;
    }
}
