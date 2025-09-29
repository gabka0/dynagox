package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH10LineitemEvent extends TPCHEvent {
    int l_orderkey;
    double l_extendedprice;
    double l_discount;

    public TPCH10LineitemEvent(int key, boolean is_insert, int l_orderkey, double l_extendedprice, double l_discount) {
        super(key, is_insert);
        this.l_orderkey = l_orderkey;
        this.l_extendedprice = l_extendedprice;
        this.l_discount = l_discount;
    }
}
