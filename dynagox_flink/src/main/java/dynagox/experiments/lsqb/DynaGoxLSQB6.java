package dynagox.experiments.lsqb;

import dynagox.experiments.Utils;
import dynagox.experiments.common.TupleL1;
import dynagox.experiments.common.TupleL2;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxLSQB6 implements Serializable {
    int id;

    MultiHashMap<TupleL1> apv_pkp1;
    MultiHashMap<TupleL1> apv_pkp2;
    MultiHashMap<TupleL2> pkp2;
    MultiHashMap<TupleL1> apv_phit;

    TupleL1 pkp1k;
    TupleL1 pkp2k;
    TupleL1 phitk;
    TupleL2 pkp2t;

    double b;
    double result = 0;
    boolean print_result = false;

    public DynaGoxLSQB6(double epsilon, boolean print) {
        apv_pkp1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TupleL1.Hash(), new TupleL1.Equal()), List.of());

        apv_pkp2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TupleL1.Hash(), new TupleL1.Equal()), List.of());

        pkp2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TupleL2.Hash(), new TupleL2.Equal()),
                List.of(new SecondaryHashIndex<>(new TupleL2.Hash1(), new TupleL2.Equal1())));

        apv_phit = new MultiHashMap<>(new PrimaryHashIndex<>(new TupleL1.Hash(), new TupleL1.Equal()), List.of());

        pkp1k = new TupleL1();
        pkp2k = new TupleL1();
        phitk = new TupleL1();
        pkp2t = new TupleL2();

        b = Utils.get_b(epsilon, 1);
        print_result = print;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() { print(); }

    public void insert_into_pkp1(long person1_id, long person2_id) {
        pkp1k.f0 = person2_id;
        apv_pkp1.update_add(pkp1k, 1);

        pkp2k.f0 = person2_id;
        TupleL1 kpkp2 = apv_pkp2.get(pkp2k);
        if (kpkp2 != null) {
            result += kpkp2.get_annotation();
        }
    }

    public void insert_into_pkp2(long person1_id, long person2_id) {
        pkp2t.f0 = person1_id;
        pkp2t.f1 = person2_id;
        pkp2.update_add(pkp2t, 1);

        phitk.f0 = person2_id;
        TupleL1 kphit = apv_phit.get(phitk);
        if (kphit != null) {
            pkp2k.f0 = person1_id;
            apv_pkp2.update_add(pkp2k, kphit.get_approximate());

            pkp1k.f0 = person1_id;
            TupleL1 kpkp1 = apv_pkp1.get(pkp1k);
            if (kpkp1 != null) {
                result += kpkp1.get_annotation() * kphit.get_approximate();
            }
        }
    }

    public void insert_into_phit(long person_id, long tag_id) {
        phitk.f0 = person_id;
        double delta_pv_phit = apv_phit.update_add_approx(phitk, 1, b);
        if (delta_pv_phit != 0) {
            pkp2t.f1 = person_id;
            LinkedListNode<TupleL2> tuple_pkp2 = pkp2.find(0, pkp2t);
            while (tuple_pkp2 != null) {
                pkp2k.f0 = tuple_pkp2.object.f0;
                apv_pkp2.update_add(pkp2k, delta_pv_phit);

                pkp1k.f0 = tuple_pkp2.object.f0;
                TupleL1 kpkp1 = apv_pkp1.get(pkp1k);
                if (kpkp1 != null) {
                    result += kpkp1.get_annotation() * delta_pv_phit;
                }

                tuple_pkp2 = tuple_pkp2.next;
            }
        }
    }

    private void print() {
        if (print_result) {
            System.out.println(id + "> " + result);
        }
    }
}
