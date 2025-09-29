package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCH9 implements Serializable {
    int id;

    MultiHashMap<TPCH9Part> r_p;
    MultiHashMap<TPCH9Supplier> r_s;
    MultiHashMap<TPCH9SupplierJoinKey> apv_s;
    MultiHashMap<TPCH9PartSupp> r_ps;
    MultiHashMap<TPCH9Orders> r_o;
    MultiHashMap<TPCH9Lineitem> r_l;
    MultiHashMap<TPCH9Nation> r_n;
    MultiHashMap<TPCH9GLineitem> hv_gl1;
    MultiHashMap<TPCH9GLineitem> hv_gl2;

    TPCH9Part t_p;
    TPCH9Supplier t_s;
    TPCH9SupplierJoinKey k_s;
    TPCH9PartSupp t_ps;
    TPCH9Orders t_o;
    TPCH9Lineitem t_l;
    TPCH9Nation t_n;
    TPCH9GLineitem t_gl;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCH9(double epsilon, boolean print_result, boolean delta_mode) {
        r_p = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9Part.Hash(), new TPCH9Part.Equal()), List.of());

        r_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9Supplier.Hash0(), new TPCH9Supplier.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH9Supplier.Hash1(), new TPCH9Supplier.Equal1())));

        apv_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9SupplierJoinKey.Hash(), new TPCH9SupplierJoinKey.Equal()),
                List.of());

        r_ps = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9PartSupp.Hash(), new TPCH9PartSupp.Equal()), List.of());

        r_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9Orders.Hash(), new TPCH9Orders.Equal()), List.of());

        r_l = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9Lineitem.Hash(), new TPCH9Lineitem.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCH9Lineitem.Hash0(), new TPCH9Lineitem.Equal0()),
                        new SecondaryHashIndex<>(new TPCH9Lineitem.Hash1(), new TPCH9Lineitem.Equal1()),
                        new SecondaryHashIndex<>(new TPCH9Lineitem.Hash2(), new TPCH9Lineitem.Equal2()),
                        new SecondaryHashIndex<>(new TPCH9Lineitem.Hash12(), new TPCH9Lineitem.Equal12())));

        r_n = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9Nation.Hash(), new TPCH9Nation.Equal()), List.of());

        hv_gl1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9GLineitem.Hash(), new TPCH9GLineitem.Equal()),
                List.of());

        hv_gl2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH9GLineitem.Hash(), new TPCH9GLineitem.Equal()),
                List.of());

        t_p = new TPCH9Part();
        t_s = new TPCH9Supplier();
        k_s = new TPCH9SupplierJoinKey();
        t_ps = new TPCH9PartSupp();
        t_o = new TPCH9Orders();
        t_l = new TPCH9Lineitem();
        t_n = new TPCH9Nation();
        t_gl =new TPCH9GLineitem();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCH9GLineitem gk_gl1 = hv_gl1.get_keys();
            while (gk_gl1 != null) {
                String n_name = gk_gl1.n_name;
                int o_year = gk_gl1.o_year;
                double value1 = gk_gl1.get_annotation();

                t_gl.n_name = n_name;
                t_gl.o_year = o_year;
                TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);
                double value2 = g_gl2 != null ? g_gl2.get_annotation() : 0;

                print(n_name, o_year, value1, value2);
                gk_gl1 = (TPCH9GLineitem) gk_gl1.get_next();
            }
        }
    }

    public void insert_into_part(int p_partkey) {
        update_activate_r_p(p_partkey);
        t_l.l_partkey = p_partkey;
        LinkedListNode<TPCH9Lineitem> f_l = r_l.find(1, t_l);
        while (f_l != null) {
            k_s.s_suppkey = f_l.object.l_suppkey;
            TPCH9SupplierJoinKey g_s = apv_s.get(k_s);
            if (g_s != null) {
                t_ps.ps_partkey = f_l.object.l_partkey;
                t_ps.ps_suppkey = f_l.object.l_suppkey;
                TPCH9PartSupp g_ps = r_ps.get(t_ps);
                if (g_ps != null) {
                    t_o.o_orderkey = f_l.object.l_orderkey;
                    TPCH9Orders g_o = r_o.get(t_o);
                    if (g_o != null) {
                        update_add_hv_gl1(g_s.n_name, g_o.o_year, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                        update_add_hv_gl2(g_s.n_name, g_o.o_year, g_ps.ps_supplycost * f_l.object.l_quantity);

                        if (delta_mode) {
                            t_gl.n_name = g_s.n_name;
                            t_gl.o_year = g_o.o_year;
                            TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                            TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                            print(g_s.n_name, g_o.o_year,
                                    (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                    (g_gl2 != null ? g_gl2.get_annotation() : 0));
                        }
                    }
                }
            }
            f_l = f_l.next;
        }
    }

    public void insert_into_supplier(int s_suppkey, int s_nationkey) {
        update_activate_r_s(s_suppkey, s_nationkey);
        t_n.n_nationkey = s_nationkey;
        TPCH9Nation g_n = r_n.get(t_n);
        if (g_n != null) {
            update_activate_apv_s(s_suppkey, g_n.n_name);
            t_l.l_suppkey = s_suppkey;
            LinkedListNode<TPCH9Lineitem> f_l = r_l.find(2, t_l);
            while (f_l != null) {
                t_p.p_partkey = f_l.object.l_partkey;
                TPCH9Part g_p = r_p.get(t_p);
                if (g_p != null) {
                    t_ps.ps_partkey = f_l.object.l_partkey;
                    t_ps.ps_suppkey = f_l.object.l_suppkey;
                    TPCH9PartSupp g_ps = r_ps.get(t_ps);
                    if (g_ps != null) {
                        t_o.o_orderkey = f_l.object.l_orderkey;
                        TPCH9Orders g_o = r_o.get(t_o);
                        if (g_o != null) {
                            update_add_hv_gl1(g_n.n_name, g_o.o_year, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                            update_add_hv_gl2(g_n.n_name, g_o.o_year, g_ps.ps_supplycost * f_l.object.l_quantity);

                            if (delta_mode) {
                                t_gl.n_name = g_n.n_name;
                                t_gl.o_year = g_o.o_year;
                                TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                                TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                                print(g_n.n_name, g_o.o_year,
                                        (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                        (g_gl2 != null ? g_gl2.get_annotation() : 0));
                            }
                        }
                    }
                }
                f_l = f_l.next;
            }
        }
    }

    public void insert_into_partsupp(int ps_partkey, int ps_suppkey, double ps_supplycost) {
        update_activate_r_ps(ps_partkey, ps_suppkey, ps_supplycost);
        t_l.l_partkey = ps_partkey;
        t_l.l_suppkey = ps_suppkey;
        LinkedListNode<TPCH9Lineitem> f_l = r_l.find(3, t_l);
        while (f_l != null) {
            t_p.p_partkey = f_l.object.l_partkey;
            TPCH9Part g_p = r_p.get(t_p);
            if (g_p != null) {
                k_s.s_suppkey = f_l.object.l_suppkey;
                TPCH9SupplierJoinKey g_s = apv_s.get(k_s);
                if (g_s != null) {
                    t_o.o_orderkey = f_l.object.l_orderkey;
                    TPCH9Orders g_o = r_o.get(t_o);
                    if (g_o != null) {
                        update_add_hv_gl1(g_s.n_name, g_o.o_year, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                        update_add_hv_gl2(g_s.n_name, g_o.o_year, ps_supplycost * f_l.object.l_quantity);

                        if (delta_mode) {
                            t_gl.n_name = g_s.n_name;
                            t_gl.o_year = g_o.o_year;
                            TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                            TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                            print(g_s.n_name, g_o.o_year,
                                    (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                    (g_gl2 != null ? g_gl2.get_annotation() : 0));
                        }
                    }
                }
            }
            f_l = f_l.next;
        }
    }

    public void insert_into_orders(int o_orderkey, int o_year) {
        update_activate_r_o(o_orderkey, o_year);
        t_l.l_orderkey = o_orderkey;
        LinkedListNode<TPCH9Lineitem> f_l = r_l.find(0, t_l);
        while (f_l != null) {
            t_p.p_partkey = f_l.object.l_partkey;
            TPCH9Part g_p = r_p.get(t_p);
            if (g_p != null) {
                k_s.s_suppkey = f_l.object.l_suppkey;
                TPCH9SupplierJoinKey g_s = apv_s.get(k_s);
                if (g_s != null) {
                    t_ps.ps_partkey = f_l.object.l_partkey;
                    t_ps.ps_suppkey = f_l.object.l_suppkey;
                    TPCH9PartSupp g_ps = r_ps.get(t_ps);
                    if (g_ps != null) {
                        update_add_hv_gl1(g_s.n_name, o_year, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                        update_add_hv_gl2(g_s.n_name, o_year, g_ps.ps_supplycost * f_l.object.l_quantity);

                        if (delta_mode) {
                            t_gl.n_name = g_s.n_name;
                            t_gl.o_year = o_year;
                            TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                            TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                            print(g_s.n_name, o_year,
                                    (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                    (g_gl2 != null ? g_gl2.get_annotation() : 0));
                        }
                    }
                }
            }
            f_l = f_l.next;
        }
    }

    public void insert_into_lineitem(int l_orderkey, int l_partkey, int l_suppkey,
                                            int l_linenumber, double l_quantity, double l_extendedprice,
                                            double l_discount) {
        update_activate_r_l(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount);
        t_p.p_partkey = l_partkey;
        TPCH9Part g_p = r_p.get(t_p);
        if (g_p != null) {
            k_s.s_suppkey = l_suppkey;
            TPCH9SupplierJoinKey g_s = apv_s.get(k_s);
            if (g_s != null) {
                t_ps.ps_partkey = l_partkey;
                t_ps.ps_suppkey = l_suppkey;
                TPCH9PartSupp g_ps = r_ps.get(t_ps);
                if (g_ps != null) {
                    t_o.o_orderkey = l_orderkey;
                    TPCH9Orders g_o = r_o.get(t_o);
                    if (g_o != null) {
                        update_add_hv_gl1(g_s.n_name, g_o.o_year, l_extendedprice * (1 - l_discount));
                        update_add_hv_gl2(g_s.n_name, g_o.o_year, g_ps.ps_supplycost * l_quantity);

                        if (delta_mode) {
                            t_gl.n_name = g_s.n_name;
                            t_gl.o_year = g_o.o_year;
                            TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                            TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                            print(g_s.n_name, g_o.o_year,
                                    (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                    (g_gl2 != null ? g_gl2.get_annotation() : 0));
                        }
                    }
                }
            }
        }
    }

    public void insert_into_nation(int n_nationkey, String n_name) {
        update_activate_r_n(n_nationkey, n_name);
        t_s.s_nationkey = n_nationkey;
        LinkedListNode<TPCH9Supplier> f_s = r_s.find(0, t_s);
        while (f_s != null) {
            update_activate_apv_s(f_s.object.s_suppkey, n_name);
            t_l.l_suppkey = f_s.object.s_suppkey;
            LinkedListNode<TPCH9Lineitem> f_l = r_l.find(2, t_l);
            while (f_l != null) {
                t_p.p_partkey = f_l.object.l_partkey;
                TPCH9Part g_p = r_p.get(t_p);
                if (g_p != null) {
                    t_ps.ps_partkey = f_l.object.l_partkey;
                    t_ps.ps_suppkey = f_l.object.l_suppkey;
                    TPCH9PartSupp g_ps = r_ps.get(t_ps);
                    if (g_ps != null) {
                        t_o.o_orderkey = f_l.object.l_orderkey;
                        TPCH9Orders g_o = r_o.get(t_o);
                        if (g_o != null) {
                            update_add_hv_gl1(n_name, g_o.o_year, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                            update_add_hv_gl2(n_name, g_o.o_year, g_ps.ps_supplycost * f_l.object.l_quantity);

                            if (delta_mode) {
                                t_gl.n_name = n_name;
                                t_gl.o_year = g_o.o_year;
                                TPCH9GLineitem g_gl1 = hv_gl1.get(t_gl);
                                TPCH9GLineitem g_gl2 = hv_gl2.get(t_gl);

                                print(n_name, g_o.o_year,
                                        (g_gl1 != null ? g_gl1.get_annotation() : 0),
                                        (g_gl2 != null ? g_gl2.get_annotation() : 0));
                            }
                        }
                    }
                }
                f_l = f_l.next;
            }
            f_s = f_s.next;
        }
    }

    private void print(String n_name, int o_year, double value1, double value2) {
        if (print_result) {
            System.out.println(id + "> (" + n_name + "," + o_year + ") -> (" + value1 + "," + value2 + ")");
        }
    }

    private void update_activate_r_p(int p_partkey) {
        t_p.p_partkey = p_partkey;
        r_p.update_activate(t_p);
    }

    private void update_activate_r_s(int s_suppkey, int s_nationkey) {
        t_s.s_suppkey = s_suppkey;
        t_s.s_nationkey = s_nationkey;
        r_s.update_activate(t_s);
    }

    private void update_activate_apv_s(int s_suppkey, String n_name) {
        k_s.s_suppkey = s_suppkey;
        k_s.n_name = n_name;
        apv_s.update_activate(k_s);
    }

    private void update_activate_r_ps(int ps_partkey, int ps_suppkey, double ps_supplycost) {
        t_ps.ps_partkey = ps_partkey;
        t_ps.ps_suppkey = ps_suppkey;
        t_ps.ps_supplycost = ps_supplycost;
        r_ps.update_activate(t_ps);
    }

    private void update_activate_r_o(int o_orderkey, int o_year) {
        t_o.o_orderkey = o_orderkey;
        t_o.o_year = o_year;
        r_o.update_activate(t_o);
    }

    private void update_activate_r_l(int l_orderkey, int l_partkey, int l_suppkey,
                                           int l_linenumber, double l_quantity, double l_extendedprice,
                                           double l_discount) {
        t_l.l_orderkey = l_orderkey;
        t_l.l_partkey = l_partkey;
        t_l.l_suppkey = l_suppkey;
        t_l.l_linenumber = l_linenumber;
        t_l.l_quantity = l_quantity;
        t_l.l_extendedprice = l_extendedprice;
        t_l.l_discount = l_discount;
        r_l.update_activate(t_l);
    }

    private void update_activate_r_n(int n_nationkey, String n_name) {
        t_n.n_nationkey = n_nationkey;
        t_n.n_name = n_name;
        r_n.update_activate(t_n);
    }

    private void update_add_hv_gl1(String n_name, int o_year, double value) {
        t_gl.n_name = n_name;
        t_gl.o_year = o_year;
        hv_gl1.update_add(t_gl, value);
    }

    private void update_add_hv_gl2(String n_name, int o_year, double value) {
        t_gl.n_name = n_name;
        t_gl.o_year = o_year;
        hv_gl2.update_add(t_gl, value);
    }
}
