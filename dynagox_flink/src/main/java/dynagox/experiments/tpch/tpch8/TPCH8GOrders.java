package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8GOrders implements MultiHashMapKey {
    public int o_year;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8GOrders previous;
    private TPCH8GOrders next;

    public TPCH8GOrders() {
        this.o_year = 0;
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
        this.next = (TPCH8GOrders) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8GOrders) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8GOrders result = new TPCH8GOrders();
        result.o_year = this.o_year;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8GOrders> {
        @Override
        public int hash(TPCH8GOrders key) {
            return (key.o_year + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH8GOrders> {
        @Override
        public boolean equal(TPCH8GOrders a, TPCH8GOrders b) {
            return a.o_year == b.o_year;
        }
    }
}
