package dynagox.experiments.job;

import dynagox.experiments.Utils;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.util.List;

public class DynaGoxJOB8m2 extends DynaGoxJOB {
    public DynaGoxJOB8m2(double epsilon, boolean print) {
        apv_an = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_ci = new MultiHashMap<>(
                new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_ci = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_mc = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        k_an = new Tuple1();
        t_ci = new Tuple2();
        k_ci = new Tuple1();
        k_mc = new Tuple1();

        b = epsilon;
        print_result = print;
    }

    @Override
    public void insert_into_aka_name(int id, int personId) {
        k_an.f0 = personId;
        boolean d_an = apv_an.update_max(k_an, id);
        if (d_an) {
            k_ci.f0 = personId;
            Tuple1 g_ci = apv_ci.get(k_ci);
            if (g_ci != null) {
                result = Math.max(result, id + g_ci.get_annotation());
            }
        }
    }

    @Override
    public void insert_into_cast_info(int personId, int movieId) {
        t_ci.f0 = personId;
        t_ci.f1 = movieId;
        r_ci.update_add(t_ci, 1);
        k_mc.f0 = movieId;
        Tuple1 g_mc = apv_mc.get(k_mc);
        if (g_mc != null) {
            k_ci.f0 = personId;
            boolean d_ci = apv_ci.update_max(k_ci, g_mc.get_approximate());
            if (d_ci) {
                k_an.f0 = personId;
                Tuple1 g_an = apv_an.get(k_an);
                if (g_an != null) {
                    result = Math.max(result, g_an.get_annotation() + g_mc.get_approximate());
                }
            }
        }
    }

    @Override
    public void insert_into_movie_companies(int id, int movieId) {
        k_mc.f0 = movieId;
        double d_mc = apv_mc.update_max2_approx(k_mc, id, b);
        if (d_mc != 0) {
            t_ci.f1 = movieId;
            LinkedListNode<Tuple2> f_ci = r_ci.find(0, t_ci);
            while (f_ci != null) {
                k_ci.f0 = f_ci.object.f0;
                boolean d_ci = apv_ci.update_max(k_ci, d_mc);
                if (d_ci) {
                    k_an.f0 = f_ci.object.f0;
                    Tuple1 g_an = apv_an.get(k_an);
                    if (g_an != null) {
                        result = Math.max(result, g_an.get_annotation() + d_mc);
                    }
                }

                f_ci = f_ci.next;
            }
        }
    }
}
