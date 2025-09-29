package dynagox.experiments.line5;

import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;

import java.io.Serializable;

public abstract class DynaGoxLine5 implements Serializable {
    int id;

    MultiHashMap<Tuple1> apv_r1;

    MultiHashMap<Tuple2> r_r2;

    MultiHashMap<Tuple1> apv_r2;

    MultiHashMap<Tuple2> r_r3;

    MultiHashMap<Tuple1> apv_r3;

    MultiHashMap<Tuple2> r_r4;

    MultiHashMap<Tuple1> apv_r4;

    MultiHashMap<Tuple1> apv_r5;

    Tuple1 k_r1 = new Tuple1();
    Tuple2 t_r2 = new Tuple2();
    Tuple1 k_r2 = new Tuple1();
    Tuple2 t_r3 = new Tuple2();
    Tuple1 k_r3 = new Tuple1();
    Tuple2 t_r4 = new Tuple2();
    Tuple1 k_r4 = new Tuple1();
    Tuple1 k_r5 = new Tuple1();

    double b;
    boolean print_result = false;

    public abstract void insert_into_r1(int src, int dst);

    public abstract void insert_into_r2(int src, int dst);

    public abstract void insert_into_r3(int src, int dst);

    public abstract void insert_into_r4(int src, int dst);

    public abstract void insert_into_r5(int src, int dst);

    public void delete_from_r1(int src, int dst) {}

    public void delete_from_r2(int src, int dst) {}

    public void delete_from_r3(int src, int dst) {}

    public void delete_from_r4(int src, int dst) {}

    public void delete_from_r5(int src, int dst) {}

    public abstract void milestone();

    public void setId(int id) {
        this.id = id;
    }
}
