package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH3GOrders implements MultiHashMapKey {
    public int o_orderkey;
    public int o_orderdate;
    public int o_shippriority;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH3GOrders previous;
    private TPCH3GOrders next;

    public TPCH3GOrders() {
        this.o_orderkey = 0;
        this.o_orderdate = 0;
        this.o_shippriority = 0;
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
        this.next = (TPCH3GOrders) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH3GOrders) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH3GOrders result = new TPCH3GOrders();
        result.o_orderkey = this.o_orderkey;
        result.o_orderdate = this.o_orderdate;
        result.o_shippriority = this.o_shippriority;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH3GOrders> {
        @Override
        public int hash(TPCH3GOrders key) {
            return (key.o_orderkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH3GOrders> {
        @Override
        public boolean equal(TPCH3GOrders a, TPCH3GOrders b) {
            return a.o_orderkey == b.o_orderkey;
        }
    }
}
