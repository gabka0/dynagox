package dynagox.experiments.ceb;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;

import java.util.List;

public class DynaGoxCEB3 extends DynaGoxCEB {
    public DynaGoxCEB3(double epsilon, boolean print) {
        apv_an = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_ci = new MultiHashMap<>(
                new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_ci = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_mi = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_mii1 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_mii2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_pi = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        k_an = new Tuple1();
        t_ci = new Tuple2();
        k_ci = new Tuple1();
        k_mi = new Tuple1();
        k_mii1 = new Tuple1();
        k_mii2 = new Tuple1();
        k_pi = new Tuple1();

        b = Utils.get_b(epsilon, 1);
        print_result = print;
    }

    @Override
    public void insert_into_aka_name(int id, int personId) {
        k_an.f0 = personId;
        apv_an.update_add(k_an, 1);
        k_pi.f0 = personId;
        Tuple1 g_pi = apv_pi.get(k_pi);
        if (g_pi != null) {
            k_ci.f0 = personId;
            Tuple1 g_ci = apv_ci.get(k_ci);
            if (g_ci != null) {
                result += g_ci.get_annotation() * g_pi.get_annotation();
            }
        }
    }

    @Override
    public void insert_into_cast_info(int personId, int movieId) {
        t_ci.f0 = personId;
        t_ci.f1 = movieId;
        r_ci.update_add(t_ci, 1);
        k_mi.f0 = movieId;
        Tuple1 g_mi = apv_mi.get(k_mi);
        if (g_mi != null) {
            k_mii1.f0 = movieId;
            Tuple1 g_mii1 = apv_mii1.get(k_mii1);
            if (g_mii1 != null) {
                k_mii2.f0 = movieId;
                Tuple1 g_mii2 = apv_mii2.get(k_mii2);
                if (g_mii2 != null) {
                    k_ci.f0 = personId;
                    apv_ci.update_add(k_ci, g_mi.get_approximate() * g_mii1.get_annotation() * g_mii2.get_annotation());
                    k_an.f0 = personId;
                    Tuple1 g_an = apv_an.get(k_an);
                    if (g_an != null) {
                        k_pi.f0 = personId;
                        Tuple1 g_pi = apv_pi.get(k_pi);
                        if (g_pi != null) {
                            result += g_an.get_annotation() * g_mi.get_approximate() * g_mii1.get_annotation() * g_mii2.get_annotation() * g_pi.get_annotation();
                        }
                    }
                }
            }
        }
    }

    @Override
    public void insert_into_movie_info(int id, int movieId) {
        k_mi.f0 = movieId;
        double d_mi = apv_mi.update_add_approx(k_mi, 1, b);
        if (d_mi != 0) {
            k_mii1.f0 = movieId;
            Tuple1 g_mii1 = apv_mii1.get(k_mii1);
            if (g_mii1 != null) {
                k_mii2.f0 = movieId;
                Tuple1 g_mii2 = apv_mii2.get(k_mii2);
                if (g_mii2 != null) {
                    t_ci.f1 = movieId;
                    LinkedListNode<Tuple2> f_ci = r_ci.find(0, t_ci);
                    while (f_ci != null) {
                        k_ci.f0 = f_ci.object.f0;
                        apv_ci.update_add(k_ci, f_ci.object.get_annotation() * d_mi * g_mii1.get_annotation() * g_mii2.get_annotation());
                        k_an.f0 = f_ci.object.f0;
                        Tuple1 g_an = apv_an.get(k_an);
                        if (g_an != null) {
                            k_pi.f0 = f_ci.object.f0;
                            Tuple1 g_pi = apv_pi.get(k_pi);
                            if (g_pi != null) {
                                result += g_an.get_annotation() * f_ci.object.get_annotation() * d_mi * g_mii1.get_annotation() * g_mii2.get_annotation() * g_pi.get_annotation();
                            }
                        }
                        f_ci = f_ci.next;
                    }
                }
            }
        }
    }

    @Override
    public void insert_into_movie_info_idx1(int id, int movieId) {
        k_mii1.f0 = movieId;
        apv_mii1.update_add(k_mii1, 1);
        k_mi.f0 = movieId;
        Tuple1 g_mi = apv_mi.get(k_mi);
        if (g_mi != null) {
            k_mii2.f0 = movieId;
            Tuple1 g_mii2 = apv_mii2.get(k_mii2);
            if (g_mii2 != null) {
                t_ci.f1 = movieId;
                LinkedListNode<Tuple2> f_ci = r_ci.find(0, t_ci);
                while (f_ci != null) {
                    k_ci.f0 = f_ci.object.f0;
                    apv_ci.update_add(k_ci, f_ci.object.get_annotation() * g_mi.get_approximate() * g_mii2.get_annotation());
                    k_an.f0 = f_ci.object.f0;
                    Tuple1 g_an = apv_an.get(k_an);
                    if (g_an != null) {
                        k_pi.f0 = f_ci.object.f0;
                        Tuple1 g_pi = apv_pi.get(k_pi);
                        if (g_pi != null) {
                            result += g_an.get_annotation() * f_ci.object.get_annotation() * g_mi.get_approximate() * g_mii2.get_annotation() * g_pi.get_annotation();
                        }
                    }
                    f_ci = f_ci.next;
                }
            }
        }
    }

    @Override
    public void insert_into_movie_info_idx2(int id, int movieId) {
        k_mii2.f0 = movieId;
        apv_mii2.update_add(k_mii2, 1);
        k_mi.f0 = movieId;
        Tuple1 g_mi = apv_mi.get(k_mi);
        if (g_mi != null) {
            k_mii1.f0 = movieId;
            Tuple1 g_mii1 = apv_mii1.get(k_mii1);
            if (g_mii1 != null) {
                t_ci.f1 = movieId;
                LinkedListNode<Tuple2> f_ci = r_ci.find(0, t_ci);
                while (f_ci != null) {
                    k_ci.f0 = f_ci.object.f0;
                    apv_ci.update_add(k_ci, f_ci.object.get_annotation() * g_mi.get_approximate() * g_mii1.get_annotation());
                    k_an.f0 = f_ci.object.f0;
                    Tuple1 g_an = apv_an.get(k_an);
                    if (g_an != null) {
                        k_pi.f0 = f_ci.object.f0;
                        Tuple1 g_pi = apv_pi.get(k_pi);
                        if (g_pi != null) {
                            result += g_an.get_annotation() * f_ci.object.get_annotation() * g_mi.get_approximate() * g_mii1.get_annotation() * g_pi.get_annotation();
                        }
                    }
                    f_ci = f_ci.next;
                }
            }
        }
    }

    @Override
    public void insert_into_person_info(int id, int personId) {
        k_pi.f0 = personId;
        apv_pi.update_add(k_pi, 1);
        k_an.f0 = personId;
        Tuple1 g_an = apv_an.get(k_an);
        if (g_an != null) {
            k_ci.f0 = personId;
            Tuple1 g_ci = apv_ci.get(k_ci);
            if (g_ci != null) {
                result += g_an.get_annotation() * g_ci.get_annotation();
            }
        }
    }
}
