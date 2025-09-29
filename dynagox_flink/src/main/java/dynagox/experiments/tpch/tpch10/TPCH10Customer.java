package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH10Customer implements MultiHashMapKey {
    public int c_custkey;
    public String c_name;
    public String c_address;
    public int c_nationkey;
    public String c_phone;
    public double c_acctbal;
    public String c_comment;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH10Customer previous;
    private TPCH10Customer next;

    public TPCH10Customer() {
        this.c_custkey = 0;
        this.c_name = "";
        this.c_address = "";
        this.c_nationkey = 0;
        this.c_phone = "";
        this.c_acctbal = 0.0;
        this.c_comment = "";
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
        this.next = (TPCH10Customer) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH10Customer) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH10Customer result = new TPCH10Customer();
        result.c_custkey = this.c_custkey;
        result.c_name = this.c_name;
        result.c_address = this.c_address;
        result.c_nationkey = this.c_nationkey;
        result.c_phone = this.c_phone;
        result.c_acctbal = this.c_acctbal;
        result.c_comment = this.c_comment;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash0 implements HashFunction<TPCH10Customer> {
        @Override
        public int hash(TPCH10Customer key) {
            return (key.c_custkey + 0x9e3779b9);
        }
    }

    public static class Equal0 implements EqualFunction<TPCH10Customer> {
        @Override
        public boolean equal(TPCH10Customer a, TPCH10Customer b) {
            return a.c_custkey == b.c_custkey;
        }
    }

    public static class Hash1 implements HashFunction<TPCH10Customer> {
        @Override
        public int hash(TPCH10Customer key) {
            return (key.c_nationkey + 0x9e3779b9);
        }
    }

    public static class Equal1 implements EqualFunction<TPCH10Customer> {
        @Override
        public boolean equal(TPCH10Customer a, TPCH10Customer b) {
            return a.c_nationkey == b.c_nationkey;
        }
    }
}
