package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCH9GLineitem implements MultiHashMapKey {
    public String n_name;
    public int o_year;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCH9GLineitem previous;
    private TPCH9GLineitem next;

    public TPCH9GLineitem() {
        this.n_name = "";
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
        this.next = (TPCH9GLineitem) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCH9GLineitem) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCH9GLineitem result = new TPCH9GLineitem();
        result.n_name = this.n_name;
        result.o_year = this.o_year;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCH9GLineitem> {
        @Override
        public int hash(TPCH9GLineitem key) {
            long hash_value = key.n_name.hashCode();
            hash_value = key.o_year + 0x9e3779b9L + (hash_value << 6) + (hash_value >> 2);
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCH9GLineitem> {
        @Override
        public boolean equal(TPCH9GLineitem a, TPCH9GLineitem b) {
            return a.n_name.equals(b.n_name) && a.o_year == b.o_year;
        }
    }
}
