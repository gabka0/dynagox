package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH8LineitemEvent extends TPCHEvent {
    int l_orderkey;
    int l_partkey;
    int l_suppkey;
    int l_linenumber;
    double l_extendedprice;
    double l_discount;

    public TPCH8LineitemEvent(int key, boolean is_insert, int l_orderkey, int l_partkey, int l_suppkey,
                              int l_linenumber, double l_extendedprice, double l_discount) {
        super(key, is_insert);
        this.l_orderkey = l_orderkey;
        this.l_partkey = l_partkey;
        this.l_suppkey = l_suppkey;
        this.l_linenumber = l_linenumber;
        this.l_extendedprice = l_extendedprice;
        this.l_discount = l_discount;
    }
}
