package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.tpch.TPCHEvent;

public class TPCH10CustomerEvent extends TPCHEvent {
    public int c_custkey;
    public String c_name;
    public String c_address;
    public int c_nationkey;
    public String c_phone;
    public double c_acctbal;
    public String c_comment;

    public TPCH10CustomerEvent(int key, boolean is_insert, int c_custkey, String c_name, String c_address,
                               int c_nationkey, String c_phone, double c_acctbal, String c_comment) {
        super(key, is_insert);
        this.c_custkey = c_custkey;
        this.c_name = c_name;
        this.c_address = c_address;
        this.c_nationkey = c_nationkey;
        this.c_phone = c_phone;
        this.c_acctbal = c_acctbal;
        this.c_comment = c_comment;
    }
}
