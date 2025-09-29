package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH3OrdersEvent extends TPCHEvent {
    int o_orderkey;
    int o_custkey;
    int o_orderdate;
    int o_shippriority;

    public TPCH3OrdersEvent(int key, boolean is_insert, int o_orderkey, int o_custkey, int o_orderdate,
                            int o_shippriority) {
        super(key, is_insert);
        this.o_orderkey = o_orderkey;
        this.o_custkey = o_custkey;
        this.o_orderdate = o_orderdate;
        this.o_shippriority = o_shippriority;
    }
}
