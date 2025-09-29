package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH10CustomerJoinKey implements MultiHashMapKey {
    public int c_custkey;
    public String c_name;
    public String c_address;
    public String c_phone;
    public double c_acctbal;
    public String c_comment;
    public String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH10CustomerJoinKey previous;
    private TPCH10CustomerJoinKey next;

    public TPCH10CustomerJoinKey() {
        this.c_custkey = 0;
        this.c_name = "";
        this.c_address = "";
        this.c_phone = "";
        this.c_acctbal = 0.0;
        this.c_comment = "";
        this.n_name = "";
        this.annotation = 0.0;
        this.approximate = 0.0;
        this.active = false;
        this.previous = null;
        this.next = null;
    }

    @Override
    public double get_annotation() {
        return annotation;
    }

    @Override
    public void set_annotation(double annotation) {
        this.annotation = annotation;
    }

    @Override
    public double get_approximate() {
        return approximate;
    }

    @Override
    public void set_approximate(double approximate) {
        this.approximate = approximate;
    }

    @Override
    public boolean is_active() {
        return active;
    }

    @Override
    public void set_active(boolean active) {
        this.active = active;
    }

    @Override
    public MultiHashMapKey get_next() {
        return next;
    }

    @Override
    public void set_next(MultiHashMapKey next) {
        this.next = (TPCH10CustomerJoinKey) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH10CustomerJoinKey) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH10CustomerJoinKey result = new TPCH10CustomerJoinKey();
        result.c_custkey = this.c_custkey;
        result.c_name = this.c_name;
        result.c_address = this.c_address;
        result.c_phone = this.c_phone;
        result.c_acctbal = this.c_acctbal;
        result.c_comment = this.c_comment;
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH10CustomerJoinKey> {
        @Override
        public int hash(TPCH10CustomerJoinKey key) {
            return (key.c_custkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH10CustomerJoinKey> {
        @Override
        public boolean equal(TPCH10CustomerJoinKey a, TPCH10CustomerJoinKey b) {
            return a.c_custkey == b.c_custkey;
        }
    }
}
