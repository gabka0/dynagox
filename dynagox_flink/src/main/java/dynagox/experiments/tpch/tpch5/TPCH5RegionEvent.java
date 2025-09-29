package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH5RegionEvent extends TPCHEvent {
    public int r_regionkey;

    public TPCH5RegionEvent(int key, boolean is_insert, int r_regionkey) {
        super(key, is_insert);
        this.r_regionkey = r_regionkey;
    }
}
