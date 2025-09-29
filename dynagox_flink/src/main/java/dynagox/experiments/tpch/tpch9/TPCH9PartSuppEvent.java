package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH9PartSuppEvent extends TPCHEvent {
    public int ps_partkey;
    public int ps_suppkey;
    public double ps_supplycost;

    public TPCH9PartSuppEvent(int key, boolean is_insert, int ps_partkey, int ps_suppkey, double ps_supplycost) {
        super(key, is_insert);
        this.ps_partkey = ps_partkey;
        this.ps_suppkey = ps_suppkey;
        this.ps_supplycost = ps_supplycost;
    }
}
