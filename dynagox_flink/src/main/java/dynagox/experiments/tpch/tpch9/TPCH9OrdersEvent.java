package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH9OrdersEvent extends TPCHEvent {
    int o_orderkey;
    int o_year;

    public TPCH9OrdersEvent(int key, boolean is_insert, int o_orderkey, int o_year) {
        super(key, is_insert);
        this.o_orderkey = o_orderkey;
        this.o_year = o_year;
    }
}
