package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH5SupplierEvent extends TPCHEvent {
    public int s_suppkey;
    public int s_nationkey;

    public TPCH5SupplierEvent(int key, boolean is_insert, int s_suppkey, int s_nationkey) {
        super(key, is_insert);
        this.s_suppkey = s_suppkey;
        this.s_nationkey = s_nationkey;
    }
}
