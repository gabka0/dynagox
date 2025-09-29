package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.hashmap.EqualFunction;
import dynagox.experiments.hashmap.HashFunction;
import dynagox.experiments.hashmap.MultiHashMapKey;

public class TPCDS15DateDim implements MultiHashMapKey {
    public int d_date_sk;
    private double annotation;
    private double approximate;
    private boolean active;
    private TPCDS15DateDim previous;
    private TPCDS15DateDim next;

    public TPCDS15DateDim() {
        this.d_date_sk = 0;
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
        this.next = (TPCDS15DateDim) next;
    }

    @Override
    public MultiHashMapKey get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (TPCDS15DateDim) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        TPCDS15DateDim result = new TPCDS15DateDim();
        result.d_date_sk = this.d_date_sk;
        result.annotation = this.annotation;
        result.approximate = this.approximate;
        return result;
    }

    public static class Hash implements HashFunction<TPCDS15DateDim> {
        @Override
        public int hash(TPCDS15DateDim key) {
            long hash_value = key.d_date_sk + 0x9e3779b9L;
            return (int) hash_value;
        }
    }

    public static class Equal implements EqualFunction<TPCDS15DateDim> {
        @Override
        public boolean equal(TPCDS15DateDim a, TPCDS15DateDim b) {
            return a.d_date_sk == b.d_date_sk;
        }
    }
}
