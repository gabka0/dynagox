package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH9PartEvent extends TPCHEvent {
    int p_partkey;

    public TPCH9PartEvent(int key, boolean is_insert, int p_partkey) {
        super(key, is_insert);
        this.p_partkey = p_partkey;
    }
}
