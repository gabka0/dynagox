package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH5OrdersEvent extends TPCHEvent {
    int o_orderkey;
    int o_custkey;

    public TPCH5OrdersEvent(int key, boolean is_insert, int o_orderkey, int o_custkey) {
        super(key, is_insert);
        this.o_orderkey = o_orderkey;
        this.o_custkey = o_custkey;
    }
}
