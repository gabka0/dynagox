package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH8PartEvent extends TPCHEvent {
    int p_partkey;

    public TPCH8PartEvent(int key, boolean is_insert, int p_partkey) {
        super(key, is_insert);
        this.p_partkey = p_partkey;
    }
}
