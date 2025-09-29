package dynagox.experiments.line5;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;

import java.util.List;

public class DynaGoxLine5gc extends DynaGoxLine5 {
    MultiHashMap<Tuple2> hv_gr3;
    boolean is_delta = false;

    public DynaGoxLine5gc(double epsilon, boolean print, boolean delta_mode) {
        apv_r1 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        apv_r2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0()),
                        new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_r3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_r5 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        hv_gr3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()), List.of());

        b = Utils.get_b(epsilon, 2);
        print_result = print;
        is_delta = delta_mode;
    }

    @Override
    public void insert_into_r1(int src, int dst) {
        k_r1.f0 = dst;
        double d_r1 = apv_r1.update_add_approx(k_r1, 1, b);
        if (d_r1 != 0) {
            t_r2.f0 = dst;
            LinkedListNode<Tuple2> f_r2 = r_r2.find(0, t_r2);
            while (f_r2 != null) {
                k_r2.f0 = f_r2.object.f1;
                apv_r2.update_add(k_r2, d_r1);
                t_r3.f0 = f_r2.object.f1;
                LinkedListNode<Tuple2> f_r3 = r_r3.find(0, t_r3);
                while (f_r3 != null) {
                    k_r4.f0 = f_r3.object.f1;
                    Tuple1 g_r4 = apv_r4.get(k_r4);
                    if (g_r4 != null) {
                        hv_gr3.update_add(f_r3.object, d_r1 * g_r4.get_annotation());
                        if (is_delta) {
                            Tuple2 g_gr3 = hv_gr3.get(f_r3.object);
                            if (print_result) {
                                print(g_gr3.f0, g_gr3.f1, g_gr3.get_annotation());
                            }
                        }
                    }
                    f_r3 = f_r3.next;
                }
                f_r2 = f_r2.next;
            }
        }
    }

    @Override
    public void insert_into_r2(int src, int dst) {
        t_r2.f0 = src;
        t_r2.f1 = dst;
        r_r2.update_add(t_r2, 1);
        k_r1.f0 = src;
        Tuple1 g_r1 = apv_r1.get(k_r1);
        if (g_r1 != null) {
            k_r2.f0 = dst;
            apv_r2.update_add(k_r2, g_r1.get_approximate());
            t_r3.f0 = dst;
            LinkedListNode<Tuple2> f_r3 = r_r3.find(0, t_r3);
            while (f_r3 != null) {
                k_r4.f0 = f_r3.object.f1;
                Tuple1 g_r4 = apv_r4.get(k_r4);
                if (g_r4 != null) {
                    hv_gr3.update_add(f_r3.object, g_r1.get_approximate() * g_r4.get_annotation());
                    if (is_delta) {
                        Tuple2 g_gr3 = hv_gr3.get(f_r3.object);
                        if (print_result) {
                            print(g_gr3.f0, g_gr3.f1, g_gr3.get_annotation());
                        }
                    }
                }
                f_r3 = f_r3.next;
            }
        }
    }

    @Override
    public void insert_into_r3(int src, int dst) {
        t_r3.f0 = src;
        t_r3.f1 = dst;
        r_r3.update_add(t_r3, 1);

        k_r2.f0 = src;
        Tuple1 g_r2 = apv_r2.get(k_r2);
        if (g_r2 != null) {
            k_r4.f0 = dst;
            Tuple1 g_r4 = apv_r4.get(k_r4);
            if (g_r4 != null) {
                hv_gr3.update_add(t_r3, g_r2.get_annotation() * g_r4.get_annotation());
                if (is_delta) {
                    Tuple2 g_gr3 = hv_gr3.get(t_r3);
                    if (print_result) {
                        print(g_gr3.f0, g_gr3.f1, g_gr3.get_annotation());
                    }
                }
            }
        }
    }

    @Override
    public void insert_into_r4(int src, int dst) {
        t_r4.f0 = src;
        t_r4.f1 = dst;
        r_r4.update_add(t_r4, 1);
        k_r5.f0 = dst;
        Tuple1 g_r5 = apv_r5.get(k_r5);
        if (g_r5 != null) {
            k_r4.f0 = src;
            apv_r4.update_add(k_r4, g_r5.get_approximate());
            t_r3.f1 = src;
            LinkedListNode<Tuple2> f_r3 = r_r3.find(1, t_r3);
            while (f_r3 != null) {
                k_r2.f0 = f_r3.object.f0;
                Tuple1 g_r2 = apv_r2.get(k_r2);
                if (g_r2 != null) {
                    hv_gr3.update_add(f_r3.object, g_r2.get_annotation() * g_r5.get_approximate());
                    if (is_delta) {
                        Tuple2 g_gr3 = hv_gr3.get(f_r3.object);
                        if (print_result) {
                            print(g_gr3.f0, g_gr3.f1, g_gr3.get_annotation());
                        }
                    }
                }
                f_r3 = f_r3.next;
            }
        }
    }

    @Override
    public void insert_into_r5(int src, int dst) {
        k_r5.f0 = src;
        double d_r5 = apv_r5.update_add_approx(k_r5, 1, b);
        if (d_r5 != 0) {
            t_r4.f1 = src;
            LinkedListNode<Tuple2> f_r4 = r_r4.find(0, t_r4);
            while (f_r4 != null) {
                k_r4.f0 = f_r4.object.f0;
                apv_r4.update_add(k_r4, d_r5);
                t_r3.f1 = f_r4.object.f0;
                LinkedListNode<Tuple2> f_r3 = r_r3.find(1, t_r3);
                while (f_r3 != null) {
                    k_r2.f0 = f_r3.object.f0;
                    Tuple1 g_r2 = apv_r2.get(k_r2);
                    if (g_r2 != null) {
                        hv_gr3.update_add(f_r3.object, g_r2.get_annotation() * d_r5);
                        if (is_delta) {
                            Tuple2 g_gr3 = hv_gr3.get(f_r3.object);
                            if (print_result) {
                                print(g_gr3.f0, g_gr3.f1, g_gr3.get_annotation());
                            }
                        }
                    }
                    f_r3 = f_r3.next;
                }
                f_r4 = f_r4.next;
            }
        }
    }

    @Override
    public void milestone() {
        if (!is_delta) {
            Tuple2 gk_gr3 = hv_gr3.get_keys();
            while (gk_gr3 != null) {
                if (print_result) {
                    print(gk_gr3.f0, gk_gr3.f1, gk_gr3.get_annotation());
                }
                gk_gr3 = (Tuple2) gk_gr3.get_next();
            }
        }
    }

    private void print(int src, int dst, double value) {
        System.out.println(id + "> " + "(" + src + "," + dst + ") -> " + value);
    }
}
