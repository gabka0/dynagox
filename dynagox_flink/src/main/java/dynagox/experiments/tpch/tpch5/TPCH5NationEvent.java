package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH5NationEvent extends TPCHEvent {
    int n_nationkey;
    String n_name;
    int n_regionkey;

    public TPCH5NationEvent(int key, boolean is_insert, int n_nationkey, String n_name, int n_regionkey) {
        super(key, is_insert);
        this.n_nationkey = n_nationkey;
        this.n_name = n_name;
        this.n_regionkey = n_regionkey;
    }
}
