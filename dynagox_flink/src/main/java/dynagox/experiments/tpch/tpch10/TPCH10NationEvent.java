package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH10NationEvent extends TPCHEvent {
    int n_nationkey;
    String n_name;

    public TPCH10NationEvent(int key, boolean is_insert, int n_nationkey, String n_name) {
        super(key, is_insert);
        this.n_nationkey = n_nationkey;
        this.n_name = n_name;
    }
}
