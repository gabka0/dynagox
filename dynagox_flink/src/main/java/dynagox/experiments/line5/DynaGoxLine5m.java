package dynagox.experiments.line5;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;

import java.util.List;

public class DynaGoxLine5m extends DynaGoxLine5 {
    double result = 0;

    public DynaGoxLine5m(double epsilon, boolean print) {
        apv_r1 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        apv_r2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_r3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        r_r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        apv_r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_r5 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        b = Utils.get_b(epsilon, 3);
        print_result = print;
    }

    @Override
    public void insert_into_r1(int src, int dst) {
        k_r1.f0 = dst;
        double d_r1 = apv_r1.update_max_approx(k_r1, src, b);
        if (d_r1 != 0) {
            t_r2.f0 = dst;
            LinkedListNode<Tuple2> f_r2 = r_r2.find(0, t_r2);
            while (f_r2 != null) {
                k_r2.f0 = f_r2.object.f1;
                apv_r2.update_max(k_r2, d_r1);
                k_r3.f0 = f_r2.object.f1;
                Tuple1 g_r3 = apv_r3.get(k_r3);
                if (g_r3 != null) {
                    result = Math.max(result, d_r1 * g_r3.get_annotation());
                }
                f_r2 = f_r2.next;
            }
        }
    }

    @Override
    public void insert_into_r2(int src, int dst) {
        t_r2.f0 = src;
        t_r2.f1 = dst;
        r_r2.update_max(t_r2, 1);
        k_r1.f0 = src;
        Tuple1 g_r1 = apv_r1.get(k_r1);
        if (g_r1 != null) {
            k_r2.f0 = dst;
            apv_r2.update_max(k_r2, g_r1.get_approximate());
            k_r3.f0 = dst;
            Tuple1 g_r3 = apv_r3.get(k_r3);
            if (g_r3 != null) {
                result = Math.max(result, g_r1.get_approximate() * g_r3.get_annotation());
            }
        }
    }

    @Override
    public void insert_into_r3(int src, int dst) {
        t_r3.f0 = src;
        t_r3.f1 = dst;
        r_r3.update_max(t_r3, 1);
        k_r4.f0 = dst;
        Tuple1 g_r4 = apv_r4.get(k_r4);
        if (g_r4 != null) {
            k_r3.f0 = src;
            apv_r3.update_max(k_r3, g_r4.get_approximate());
            k_r2.f0 = src;
            Tuple1 g_r2 = apv_r2.get(k_r2);
            if (g_r2 != null) {
                result = Math.max(result, g_r2.get_annotation() * g_r4.get_approximate());
            }
        }
    }

    @Override
    public void insert_into_r4(int src, int dst) {
        t_r4.f0 = src;
        t_r4.f1 = dst;
        r_r4.update_max(t_r4, 1);
        k_r5.f0 = dst;
        Tuple1 g_r5 = apv_r5.get(k_r5);
        if (g_r5 != null) {
            k_r4.f0 = src;
            double d_r4 = apv_r4.update_max_approx(k_r4, g_r5.get_approximate(), b);
            if (d_r4 != 0) {
                t_r3.f1 = src;
                LinkedListNode<Tuple2> f_r3 = r_r3.find(0, t_r3);
                while (f_r3 != null) {
                    k_r3.f0 = f_r3.object.f0;
                    apv_r3.update_max(k_r3, d_r4);
                    k_r2.f0 = f_r3.object.f0;
                    Tuple1 g_r2 = apv_r2.get(k_r2);
                    if (g_r2 != null) {
                        result = Math.max(result, g_r2.get_annotation() * d_r4);
                    }
                    f_r3 = f_r3.next;
                }
            }
        }
    }

    @Override
    public void insert_into_r5(int src, int dst) {
        k_r5.f0 = src;
        double d_r5 = apv_r5.update_max_approx(k_r5, dst, b);
        if (d_r5 != 0) {
            MultiHashMap<Tuple1> d_r4s = new MultiHashMap<>(
                    new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());
            t_r4.f1 = src;
            LinkedListNode<Tuple2> f_r4 = r_r4.find(0, t_r4);
            while (f_r4 != null) {
                k_r4.f0 = f_r4.object.f0;
                double d_r4 = apv_r4.update_max_approx(k_r4, d_r5, b);
                if (d_r4 != 0) {
                    d_r4s.update_max(k_r4, d_r4);
                }
                f_r4 = f_r4.next;
            }
            Tuple1 gk_r4 = d_r4s.get_keys();
            while (gk_r4 != null) {
                t_r3.f1 = gk_r4.f0;
                LinkedListNode<Tuple2> f_r3 = r_r3.find(0, t_r3);
                while (f_r3 != null) {
                    k_r3.f0 = f_r3.object.f0;
                    apv_r3.update_max(k_r3, gk_r4.get_annotation());
                    k_r2.f0 = f_r3.object.f0;
                    Tuple1 g_r2 = apv_r2.get(k_r2);
                    if (g_r2 != null) {
                        result = Math.max(result, g_r2.get_annotation() * gk_r4.get_annotation());
                    }
                    f_r3 = f_r3.next;
                }
                gk_r4 = (Tuple1) gk_r4.get_next();
            }
        }
    }

    @Override
    public void milestone() {
        if (print_result) {
            System.out.println(id + "> " + result);
        }
    }
}
