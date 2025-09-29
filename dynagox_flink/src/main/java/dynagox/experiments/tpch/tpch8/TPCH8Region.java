package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH8Region implements MultiHashMapKey {
    public int r_regionkey;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH8Region previous;
    private TPCH8Region next;

    public TPCH8Region() {
        this.r_regionkey = 0;
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
        this.next = (TPCH8Region) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH8Region) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH8Region result = new TPCH8Region();
        result.r_regionkey = this.r_regionkey;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH8Region> {
        @Override
        public int hash(TPCH8Region key) {
            return (key.r_regionkey + 0x9e3779b9);
        }
    }

    public static class Equal implements EqualFunction<TPCH8Region> {
        @Override
        public boolean equal(TPCH8Region a, TPCH8Region b) {
            return a.r_regionkey == b.r_regionkey;
        }
    }
}
