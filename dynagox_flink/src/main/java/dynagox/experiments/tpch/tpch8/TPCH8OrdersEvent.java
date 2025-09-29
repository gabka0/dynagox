package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH8OrdersEvent extends TPCHEvent {
    int o_orderkey;
    int o_custkey;
    int o_year;

    public TPCH8OrdersEvent(int key, boolean is_insert, int o_orderkey, int o_custkey, int o_year) {
        super(key, is_insert);
        this.o_orderkey = o_orderkey;
        this.o_custkey = o_custkey;
        this.o_year = o_year;
    }
}
