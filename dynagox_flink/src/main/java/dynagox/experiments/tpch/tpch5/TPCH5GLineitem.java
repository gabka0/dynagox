package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH5GLineitem implements MultiHashMapKey {
    public String n_name;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH5GLineitem previous;
    private TPCH5GLineitem next;

    public TPCH5GLineitem() {
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
        this.next = (TPCH5GLineitem) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH5GLineitem) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH5GLineitem result = new TPCH5GLineitem();
        result.n_name = this.n_name;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH5GLineitem> {
        @Override
        public int hash(TPCH5GLineitem key) {
            return key.n_name.hashCode();
        }
    }

    public static class Equal implements EqualFunction<TPCH5GLineitem> {
        @Override
        public boolean equal(TPCH5GLineitem a, TPCH5GLineitem b) {
            return a.n_name.equals(b.n_name);
        }
    }
}
