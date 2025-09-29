package dynagox.experiments.dumbbell;

import dynagox.experiments.Utils;
import dynagox.experiments.common.Tuple1;
import dynagox.experiments.common.Tuple2;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxDumbbell implements Serializable {
    int id;

    MultiHashMap<Tuple2> r1;
    MultiHashMap<Tuple2> r2;
    MultiHashMap<Tuple2> r3;

    MultiHashMap<Tuple1> pv_r4;
    MultiHashMap<Tuple2> r4;

    MultiHashMap<Tuple2> r5;
    MultiHashMap<Tuple2> r6;
    MultiHashMap<Tuple2> r7;

    MultiHashMap<Tuple1> apv_b1;
    MultiHashMap<Tuple1> apv_b2;

    Tuple1 b1k;
    Tuple1 r4k;
    Tuple1 b2k;
    Tuple2 r1t;
    Tuple2 r2t;
    Tuple2 r3t;
    Tuple2 r4t;
    Tuple2 r5t;
    Tuple2 r6t;
    Tuple2 r7t;

    double b;
    double result = 0;
    boolean print_result = false;

    public DynaGoxDumbbell(double epsilon, boolean print) {
        r1 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        r2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        r3 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        pv_r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());
        r4 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash1(), new Tuple2.Equal1())));

        r5 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        r6 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        r7 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple2.Hash(), new Tuple2.Equal()),
                List.of(new SecondaryHashIndex<>(new Tuple2.Hash0(), new Tuple2.Equal0())));

        apv_b1 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        apv_b2 = new MultiHashMap<>(new PrimaryHashIndex<>(new Tuple1.Hash(), new Tuple1.Equal()), List.of());

        b1k = new Tuple1();
        r4k = new Tuple1();
        b2k = new Tuple1();
        r1t = new Tuple2();
        r2t = new Tuple2();
        r3t = new Tuple2();
        r4t = new Tuple2();
        r5t = new Tuple2();
        r6t = new Tuple2();
        r7t = new Tuple2();

        b = Utils.get_b(epsilon, 1);
        print_result = print;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() { print(); }

    public void insert_into_r1(int src, int dst) {
        r1t.f0 = src;
        r1t.f1 = dst;
        r1.update_add(r1t, 1);

        r2t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r2 = r2.find(0, r2t);
        while (tuple_r2 != null) {
            r3t.f0 = tuple_r2.object.f1;
            r3t.f1 = src;
            if (r3.get(r3t) != null) {
                insert_into_b1(src, dst, tuple_r2.object.f1);
            }
            tuple_r2 = tuple_r2.next;
        }
    }

    public void insert_into_r2(int src, int dst) {
        r2t.f0 = src;
        r2t.f1 = dst;
        r2.update_add(r2t, 1);

        r3t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r3 = r3.find(0, r3t);
        while (tuple_r3 != null) {
            r1t.f0 = tuple_r3.object.f1;
            r1t.f1 = src;
            if (r1.get(r1t) != null) {
                insert_into_b1(tuple_r3.object.f1, src, dst);
            }
            tuple_r3 = tuple_r3.next;
        }
    }

    public void insert_into_r3(int src, int dst) {
        r3t.f0 = src;
        r3t.f1 = dst;
        r3.update_add(r3t, 1);

        r1t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r1 = r1.find(0, r1t);
        while (tuple_r1 != null) {
            r2t.f0 = tuple_r1.object.f1;
            r2t.f1 = src;
            if (r2.get(r2t) != null) {
                insert_into_b1(dst, tuple_r1.object.f1, src);
            }
            tuple_r1 = tuple_r1.next;
        }
    }

    public void insert_into_r4(int src, int dst) {
        r4t.f0 = src;
        r4t.f1 = dst;
        r4.update_add(r4t, 1);

        b2k.f0 = dst;
        Tuple1 kb2 = apv_b2.get(b2k);
        if (kb2 != null) {
            r4k.f0 = src;
            pv_r4.update_add(r4k, kb2.get_approximate());

            b1k.f0 = src;
            Tuple1 kb1 = apv_b1.get(b1k);
            if (kb1 != null) {
                result += (kb1.get_annotation() * kb2.get_approximate());
            }
        }
    }

    public void insert_into_r5(int src, int dst) {
        r5t.f0 = src;
        r5t.f1 = dst;
        r5.update_add(r5t, 1);

        r6t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r6 = r6.find(0, r6t);
        while (tuple_r6 != null) {
            r7t.f0 = tuple_r6.object.f1;
            r7t.f1 = src;
            if (r7.get(r7t) != null) {
                insert_into_b2(src, dst, tuple_r6.object.f1);
            }
            tuple_r6 = tuple_r6.next;
        }
    }

    public void insert_into_r6(int src, int dst) {
        r6t.f0 = src;
        r6t.f1 = dst;
        r6.update_add(r6t, 1);

        r7t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r7 = r7.find(0, r7t);
        while (tuple_r7 != null) {
            r5t.f0 = tuple_r7.object.f1;
            r5t.f1 = src;
            if (r5.get(r5t) != null) {
                insert_into_b2(tuple_r7.object.f1, src, dst);
            }
            tuple_r7 = tuple_r7.next;
        }
    }

    public void insert_into_r7(int src, int dst) {
        r7t.f0 = src;
        r7t.f1 = dst;
        r7.update_add(r7t, 1);

        r5t.f0 = dst;
        LinkedListNode<Tuple2> tuple_r5 = r5.find(0, r5t);
        while (tuple_r5 != null) {
            r6t.f0 = tuple_r5.object.f1;
            r6t.f1 = src;
            if (r6.get(r6t) != null) {
                insert_into_b2(dst, tuple_r5.object.f1, src);
            }
            tuple_r5 = tuple_r5.next;
        }
    }

    public void insert_into_b1(int f0, int f1, int f2) {
        b1k.f0 = f2;
        apv_b1.update_add(b1k, 1);

        r4k.f0 = f2;
        Tuple1 k4 = pv_r4.get(r4k);
        if (k4 != null) {
            result += k4.get_annotation();
        }
    }

    public void insert_into_b2(int f0, int f1, int f2) {
        b2k.f0 = f0;
        double delta_pv_b2 = apv_b2.update_add_approx(b2k, 1, b);
        if (delta_pv_b2 != 0) {
            r4t.f1 = f0;
            LinkedListNode<Tuple2> tuple_r4 = r4.find(0, r4t);
            while (tuple_r4 != null) {
                r4k.f0 = tuple_r4.object.f0;
                pv_r4.update_add(r4k, delta_pv_b2);

                b1k.f0 = tuple_r4.object.f0;
                Tuple1 kb1 = apv_b1.get(b1k);
                if (kb1 != null) {
                    result += (kb1.get_annotation() * delta_pv_b2);
                }
                tuple_r4 = tuple_r4.next;
            }
        }
    }

    public void print() {
        if (print_result) {
            System.out.println(id + "> " + result);
        }
    }
}
