package dynagox.experiments.ceb;

import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;

import java.io.Serializable;

public class DynaGoxCEB implements Serializable {
    int id;

    MultiHashMap<Tuple1> apv_an = null;
    MultiHashMap<Tuple2> r_ci = null;
    MultiHashMap<Tuple1> apv_ci = null;
    MultiHashMap<Tuple1> apv_mc = null;
    MultiHashMap<Tuple1> apv_mi = null;
    MultiHashMap<Tuple1> apv_mii1 = null;
    MultiHashMap<Tuple1> apv_mii2 = null;
    MultiHashMap<Tuple1> apv_mk = null;
    MultiHashMap<Tuple1> apv_pi = null;

    Tuple1 k_an = null;
    Tuple2 t_ci = null;
    Tuple1 k_ci = null;
    Tuple1 k_mc = null;
    Tuple1 k_mi = null;
    Tuple1 k_mii1 = null;
    Tuple1 k_mii2 = null;
    Tuple1 k_mk = null;
    Tuple1 k_pi = null;

    double b;
    double result = 0;
    boolean print_result = false;

    public void insert_into_aka_name(int id, int personId) {}

    public void insert_into_cast_info(int personId, int movieId) {}

    public void insert_into_movie_companies(int id, int movieId) {}

    public void insert_into_movie_info(int id, int movieId) {}

    public void insert_into_movie_info_idx1(int id, int movieId) {}

    public void insert_into_movie_info_idx2(int id, int movieId) {}

    public void insert_into_movie_keyword(int id, int movieId) {}

    public void insert_into_person_info(int id, int personId) {}

    final public void milestone() {
        if (print_result) {
            System.out.println(id + "> " + result);
        }
    }

    public void setId(int id) {
        this.id = id;
    }
}
