package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCH8 implements Serializable {
    int id;

    MultiHashMap<TPCH8Part> r_p;
    MultiHashMap<TPCH8Supplier> r_s;
    MultiHashMap<TPCH8SupplierJoinKey> apv_s;
    MultiHashMap<TPCH8Customer> r_c;
    MultiHashMap<TPCH8CustomerJoinKey> apv_c;
    MultiHashMap<TPCH8Orders> r_o;
    MultiHashMap<TPCH8OrdersJoinKey> apv_o;
    MultiHashMap<TPCH8Lineitem> r_l;
    MultiHashMap<TPCH8LineitemJoinKey> apv_l1;
    MultiHashMap<TPCH8LineitemJoinKey> apv_l2;
    MultiHashMap<TPCH8Nation1> r_n1;
    MultiHashMap<TPCH8Nation1JoinKey> apv_n1;
    MultiHashMap<TPCH8Nation2> r_n2;
    MultiHashMap<TPCH8Region> r_r;
    MultiHashMap<TPCH8GOrders> hv_go1;
    MultiHashMap<TPCH8GOrders> hv_go2;

    TPCH8Part t_p;
    TPCH8Supplier t_s;
    TPCH8SupplierJoinKey k_s;
    TPCH8Customer t_c;
    TPCH8CustomerJoinKey k_c;
    TPCH8Orders t_o;
    TPCH8OrdersJoinKey k_o;
    TPCH8Lineitem t_l;
    TPCH8LineitemJoinKey k_l;
    TPCH8Nation1 t_n1;
    TPCH8Nation1JoinKey k_n1;
    TPCH8Nation2 t_n2;
    TPCH8Region t_r;
    TPCH8GOrders t_go;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCH8(double epsilon, boolean print_result, boolean delta_mode) {
        r_p = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Part.Hash(), new TPCH8Part.Equal()), List.of());

        r_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Supplier.Hash0(), new TPCH8Supplier.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH8Supplier.Hash1(), new TPCH8Supplier.Equal1())));

        apv_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8SupplierJoinKey.Hash(), new TPCH8SupplierJoinKey.Equal()),
                List.of());

        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Customer.Hash0(), new TPCH8Customer.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH8Customer.Hash1(), new TPCH8Customer.Equal1())));

        apv_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8CustomerJoinKey.Hash(), new TPCH8CustomerJoinKey.Equal()),
                List.of());

        r_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Orders.Hash0(), new TPCH8Orders.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH8Orders.Hash1(), new TPCH8Orders.Equal1())));

        apv_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8OrdersJoinKey.Hash(), new TPCH8OrdersJoinKey.Equal()),
                List.of());

        r_l = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Lineitem.Hash(), new TPCH8Lineitem.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCH8Lineitem.Hash1(), new TPCH8Lineitem.Equal1()),
                        new SecondaryHashIndex<>(new TPCH8Lineitem.Hash2(), new TPCH8Lineitem.Equal2())));

        apv_l1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8LineitemJoinKey.Hash(), new TPCH8LineitemJoinKey.Equal()),
                List.of());

        apv_l2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8LineitemJoinKey.Hash(), new TPCH8LineitemJoinKey.Equal()),
                List.of());

        r_n1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Nation1.Hash0(), new TPCH8Nation1.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH8Nation1.Hash1(), new TPCH8Nation1.Equal1())));

        apv_n1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Nation1JoinKey.Hash(), new TPCH8Nation1JoinKey.Equal()),
                List.of());

        r_n2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Nation2.Hash(), new TPCH8Nation2.Equal()), List.of());

        r_r = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8Region.Hash(), new TPCH8Region.Equal()), List.of());

        hv_go1 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8GOrders.Hash(), new TPCH8GOrders.Equal()), List.of());

        hv_go2 = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH8GOrders.Hash(), new TPCH8GOrders.Equal()), List.of());

        t_p = new TPCH8Part();
        t_s = new TPCH8Supplier();
        k_s = new TPCH8SupplierJoinKey();
        t_c = new TPCH8Customer();
        k_c = new TPCH8CustomerJoinKey();
        t_o = new TPCH8Orders();
        k_o = new TPCH8OrdersJoinKey();
        t_l = new TPCH8Lineitem();
        k_l = new TPCH8LineitemJoinKey();
        t_n1 = new TPCH8Nation1();
        k_n1 = new TPCH8Nation1JoinKey();
        t_n2 = new TPCH8Nation2();
        t_r = new TPCH8Region();
        t_go = new TPCH8GOrders();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCH8GOrders gk_go2 = hv_go2.get_keys();
            while (gk_go2 != null) {
                int o_year = gk_go2.o_year;
                double value2 = gk_go2.get_annotation();

                t_go.o_year = o_year;
                TPCH8GOrders g_go1 = hv_go1.get(t_go);
                double value1 = g_go1 != null ? g_go1.get_annotation() : 0;

                print(gk_go2.o_year, value1, value2);
                gk_go2 = (TPCH8GOrders) gk_go2.get_next();
            }
        }
    }

    public void insert_into_part(int p_partkey) {
        update_activate_r_p(p_partkey);
        t_l.l_partkey = p_partkey;
        LinkedListNode<TPCH8Lineitem> f_l = r_l.find(0, t_l);
        while (f_l != null) {
            k_s.s_suppkey = f_l.object.l_suppkey;
            TPCH8SupplierJoinKey g_s = apv_s.get(k_s);
            if (g_s != null) {
                double volume = f_l.object.l_extendedprice * (1 - f_l.object.l_discount);
                double delta_pv_l1 = 0;
                double delta_pv_l2 = 0;
                if (g_s.n_name.equals("BRAZIL")) {
                    delta_pv_l1 = update_add_approx_apv_l1(f_l.object.l_orderkey, volume);
                }
                delta_pv_l2 = update_add_approx_apv_l2(f_l.object.l_orderkey, volume);

                if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
                    k_o.o_orderkey = f_l.object.l_orderkey;
                    TPCH8OrdersJoinKey g_o = apv_o.get(k_o);
                    if (g_o != null) {
                        if (g_s.n_name.equals("BRAZIL")) {
                            update_add_hv_go1(g_o.o_year, delta_pv_l1);
                        }
                        update_add_hv_go2(g_o.o_year, delta_pv_l2);

                        if (delta_mode) {
                            t_go.o_year = g_o.o_year;
                            TPCH8GOrders g_go1 = hv_go1.get(t_go);
                            double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                            TPCH8GOrders g_go2 = hv_go2.get(t_go);
                            double value2 = g_go2.get_annotation();
                            print(g_o.o_year, value1, value2);
                        }
                    }
                }
            }
            f_l = f_l.next;
        }
    }

    public void insert_into_supplier(int s_suppkey, int s_nationkey) {
        update_activate_r_s(s_suppkey, s_nationkey);
        t_n2.n_nationkey = s_nationkey;
        TPCH8Nation2 g_n2 = r_n2.get(t_n2);
        if (g_n2 != null) {
            update_activate_apv_s(s_suppkey, g_n2.n_name);
            t_l.l_suppkey = s_suppkey;
            LinkedListNode<TPCH8Lineitem> f_l = r_l.find(1, t_l);
            while (f_l != null) {
                t_p.p_partkey = f_l.object.l_partkey;
                TPCH8Part g_p = r_p.get(t_p);
                if (g_p != null) {
                    double volume = f_l.object.l_extendedprice * (1 - f_l.object.l_discount);
                    double delta_pv_l1 = 0;
                    double delta_pv_l2 = 0;
                    if (g_n2.n_name.equals("BRAZIL")) {
                        delta_pv_l1 = update_add_approx_apv_l1(f_l.object.l_orderkey, volume);
                    }
                    delta_pv_l2 = update_add_approx_apv_l2(f_l.object.l_orderkey, volume);

                    if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
                        k_o.o_orderkey = f_l.object.l_orderkey;
                        TPCH8OrdersJoinKey g_o = apv_o.get(k_o);
                        if (g_o != null) {
                            if (g_n2.n_name.equals("BRAZIL")) {
                                update_add_hv_go1(g_o.o_year, delta_pv_l1);
                            }
                            update_add_hv_go2(g_o.o_year, delta_pv_l2);

                            if (delta_mode) {
                                t_go.o_year = g_o.o_year;
                                TPCH8GOrders g_go1 = hv_go1.get(t_go);
                                double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                                TPCH8GOrders g_go2 = hv_go2.get(t_go);
                                double value2 = g_go2.get_annotation();
                                print(g_o.o_year, value1, value2);
                            }
                        }
                    }
                }
                f_l = f_l.next;
            }
        }
    }

    public void insert_into_customer(int c_custkey, int c_nationkey) {
        update_activate_r_c(c_custkey, c_nationkey);
        k_n1.n_nationkey = c_nationkey;
        TPCH8Nation1JoinKey g_n1 = apv_n1.get(k_n1);
        if (g_n1 != null) {
            update_activate_apv_c(c_custkey);
            t_o.o_custkey = c_custkey;
            LinkedListNode<TPCH8Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_activate_apv_o(f_o.object.o_orderkey, f_o.object.o_year);
                k_l.l_orderkey = f_o.object.o_orderkey;
                TPCH8LineitemJoinKey g_l1 = apv_l1.get(k_l);
                if (g_l1 != null) {
                    update_add_hv_go1(f_o.object.o_year, g_l1.get_approximate());
                }

                TPCH8LineitemJoinKey g_l2 = apv_l2.get(k_l);
                if (g_l2 != null) {
                    update_add_hv_go2(f_o.object.o_year, g_l2.get_approximate());

                    if (delta_mode) {
                        t_go.o_year = f_o.object.o_year;
                        TPCH8GOrders g_go1 = hv_go1.get(t_go);
                        double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                        TPCH8GOrders g_go2 = hv_go2.get(t_go);
                        double value2 = g_go2.get_annotation();
                        print(f_o.object.o_year, value1, value2);
                    }
                }
                f_o = f_o.next;
            }
        }
    }

    public void insert_into_orders(int o_orderkey, int o_custkey, int o_year) {
        update_activate_r_o(o_orderkey, o_custkey, o_year);
        k_c.c_custkey = o_custkey;
        TPCH8CustomerJoinKey g_c = apv_c.get(k_c);
        if (g_c != null) {
            update_activate_apv_o(o_orderkey, o_year);
            k_l.l_orderkey = o_orderkey;
            TPCH8LineitemJoinKey g_l1 = apv_l1.get(k_l);
            if (g_l1 != null) {
                update_add_hv_go1(o_year, g_l1.get_approximate());
            }

            TPCH8LineitemJoinKey g_l2 = apv_l2.get(k_l);
            if (g_l2 != null) {
                update_add_hv_go2(o_year, g_l2.get_approximate());

                if (delta_mode) {
                    t_go.o_year = o_year;
                    TPCH8GOrders g_go1 = hv_go1.get(t_go);
                    double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                    TPCH8GOrders g_go2 = hv_go2.get(t_go);
                    double value2 = g_go2.get_annotation();
                    print(o_year, value1, value2);
                }
            }
        }
    }

    public void insert_into_lineitem(int l_orderkey, int l_partkey, int l_suppkey,
                                            int l_linenumber, double l_extendedprice, double l_discount) {
        update_activate_r_l(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
        t_p.p_partkey = l_partkey;
        TPCH8Part g_p = r_p.get(t_p);
        if (g_p != null) {
            k_s.s_suppkey = l_suppkey;
            TPCH8SupplierJoinKey g_s = apv_s.get(k_s);
            if (g_s != null) {
                double volume = l_extendedprice * (1 - l_discount);
                double delta_pv_l1 = 0;
                double delta_pv_l2 = 0;
                if (g_s.n_name.equals("BRAZIL")) {
                    delta_pv_l1 = update_add_approx_apv_l1(l_orderkey, volume);
                }
                delta_pv_l2 = update_add_approx_apv_l2(l_orderkey, volume);

                if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
                    k_o.o_orderkey = l_orderkey;
                    TPCH8OrdersJoinKey g_o = apv_o.get(k_o);
                    if (g_o != null) {
                        if (g_s.n_name.equals("BRAZIL")) {
                            update_add_hv_go1(g_o.o_year, delta_pv_l1);
                        }
                        update_add_hv_go2(g_o.o_year, delta_pv_l2);

                        if (delta_mode) {
                            t_go.o_year = g_o.o_year;
                            TPCH8GOrders g_go1 = hv_go1.get(t_go);
                            double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                            TPCH8GOrders g_go2 = hv_go2.get(t_go);
                            double value2 = g_go2.get_annotation();
                            print(g_o.o_year, value1, value2);
                        }
                    }
                }
            }
        }
    }

    public void insert_into_nation1(int n_nationkey, int n_regionkey) {
        update_activate_r_n1(n_nationkey, n_regionkey);
        t_r.r_regionkey = n_regionkey;
        TPCH8Region g_r = r_r.get(t_r);
        if (g_r != null) {
            update_activate_apv_n1(n_nationkey);
            t_c.c_nationkey = n_nationkey;
            LinkedListNode<TPCH8Customer> f_c = r_c.find(0, t_c);
            while (f_c != null) {
                update_activate_apv_c(f_c.object.c_custkey);
                t_o.o_custkey = f_c.object.c_custkey;
                LinkedListNode<TPCH8Orders> f_o = r_o.find(0, t_o);
                while (f_o != null) {
                    update_activate_apv_o(f_o.object.o_orderkey, f_o.object.o_year);
                    k_l.l_orderkey = f_o.object.o_orderkey;
                    TPCH8LineitemJoinKey g_l1 = apv_l1.get(k_l);
                    if (g_l1 != null) {
                        update_add_hv_go1(f_o.object.o_year, g_l1.get_approximate());
                    }

                    TPCH8LineitemJoinKey g_l2 = apv_l2.get(k_l);
                    if (g_l2 != null) {
                        update_add_hv_go2(f_o.object.o_year, g_l2.get_approximate());

                        if (delta_mode) {
                            t_go.o_year = f_o.object.o_year;
                            TPCH8GOrders g_go1 = hv_go1.get(t_go);
                            double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                            TPCH8GOrders g_go2 = hv_go2.get(t_go);
                            double value2 = g_go2.get_annotation();
                            print(f_o.object.o_year, value1, value2);
                        }
                    }
                    f_o = f_o.next;
                }
                f_c = f_c.next;
            }
        }
    }

    public void insert_into_nation2(int n_nationkey, String n_name) {
        update_activate_r_n2(n_nationkey, n_name);
        t_s.s_nationkey = n_nationkey;
        LinkedListNode<TPCH8Supplier> f_s = r_s.find(0, t_s);
        while (f_s != null) {
            update_activate_apv_s(f_s.object.s_suppkey, n_name);
            t_l.l_suppkey = f_s.object.s_suppkey;
            LinkedListNode<TPCH8Lineitem> f_l = r_l.find(1, t_l);
            while (f_l != null) {
                t_p.p_partkey = f_l.object.l_partkey;
                TPCH8Part g_p = r_p.get(t_p);
                if (g_p != null) {
                    double volume = f_l.object.l_extendedprice * (1 - f_l.object.l_discount);
                    double delta_pv_l1 = 0;
                    double delta_pv_l2 = 0;
                    if (n_name.equals("BRAZIL")) {
                        delta_pv_l1 = update_add_approx_apv_l1(f_l.object.l_orderkey, volume);
                    }
                    delta_pv_l2 = update_add_approx_apv_l2(f_l.object.l_orderkey, volume);

                    if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
                        k_o.o_orderkey = f_l.object.l_orderkey;
                        TPCH8OrdersJoinKey g_o = apv_o.get(k_o);
                        if (g_o != null) {
                            if (n_name.equals("BRAZIL")) {
                                update_add_hv_go1(g_o.o_year, delta_pv_l1);
                            }
                            update_add_hv_go2(g_o.o_year, delta_pv_l2);

                            if (delta_mode) {
                                t_go.o_year = g_o.o_year;
                                TPCH8GOrders g_go1 = hv_go1.get(t_go);
                                double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                                TPCH8GOrders g_go2 = hv_go2.get(t_go);
                                double value2 = g_go2.get_annotation();
                                print(g_o.o_year, value1, value2);
                            }
                        }
                    }
                }
                f_l = f_l.next;
            }
            f_s = f_s.next;
        }
    }

    public void insert_into_region(int r_regionkey) {
        update_activate_r_r(r_regionkey);
        t_n1.n_regionkey = r_regionkey;
        LinkedListNode<TPCH8Nation1> f_n1 = r_n1.find(0, t_n1);
        while (f_n1 != null) {
            update_activate_apv_n1(f_n1.object.n_nationkey);
            t_c.c_nationkey = f_n1.object.n_nationkey;
            LinkedListNode<TPCH8Customer> f_c = r_c.find(0, t_c);
            while (f_c != null) {
                update_activate_apv_c(f_c.object.c_custkey);
                t_o.o_custkey = f_c.object.c_custkey;
                LinkedListNode<TPCH8Orders> f_o = r_o.find(0, t_o);
                while (f_o != null) {
                    update_activate_apv_o(f_o.object.o_orderkey, f_o.object.o_year);
                    k_l.l_orderkey = f_o.object.o_orderkey;
                    TPCH8LineitemJoinKey g_l1 = apv_l1.get(k_l);
                    if (g_l1 != null) {
                        update_add_hv_go1(f_o.object.o_year, g_l1.get_approximate());
                    }

                    TPCH8LineitemJoinKey g_l2 = apv_l2.get(k_l);
                    if (g_l2 != null) {
                        update_add_hv_go2(f_o.object.o_year, g_l2.get_approximate());

                        if (delta_mode) {
                            t_go.o_year = f_o.object.o_year;
                            TPCH8GOrders g_go1 = hv_go1.get(t_go);
                            double value1 = g_go1 != null ? g_go1.get_annotation() : 0;
                            TPCH8GOrders g_go2 = hv_go2.get(t_go);
                            double value2 = g_go2.get_annotation();
                            print(f_o.object.o_year, value1, value2);
                        }
                    }
                    f_o = f_o.next;
                }
                f_c = f_c.next;
            }
            f_n1 = f_n1.next;
        }
    }

    private void print(int o_year, double value1, double value2) {
        if (print_result) {
            System.out.println(id + "> (" + o_year + ") -> (" + value1 + "," + value2 + ")");
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

    private void update_activate_r_c(int c_custkey, int c_nationkey) {
        t_c.c_custkey = c_custkey;
        t_c.c_nationkey = c_nationkey;
        r_c.update_activate(t_c);
    }

    private void update_activate_apv_c(int c_custkey) {
        k_c.c_custkey = c_custkey;
        apv_c.update_activate(k_c);
    }

    private void update_activate_r_o(int o_orderkey, int o_custkey, int o_year) {
        t_o.o_orderkey = o_orderkey;
        t_o.o_custkey = o_custkey;
        t_o.o_year = o_year;
        r_o.update_activate(t_o);
    }

    private void update_activate_apv_o(int o_orderkey, int o_year) {
        k_o.o_orderkey = o_orderkey;
        k_o.o_year = o_year;
        apv_o.update_activate(k_o);
    }

    private void update_activate_r_l(int l_orderkey, int l_partkey, int l_suppkey,
                                           int l_linenumber, double l_extendedprice, double l_discount) {
        t_l.l_orderkey = l_orderkey;
        t_l.l_partkey = l_partkey;
        t_l.l_suppkey = l_suppkey;
        t_l.l_linenumber = l_linenumber;
        t_l.l_extendedprice = l_extendedprice;
        t_l.l_discount = l_discount;
        r_l.update_activate(t_l);
    }

    private double update_add_approx_apv_l1(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l1.update_add_approx(k_l, value, b);
    }

    private double update_add_approx_apv_l2(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l2.update_add_approx(k_l, value, b);
    }

    private void update_activate_r_n1(int n_nationkey, int n_regionkey) {
        t_n1.n_nationkey = n_nationkey;
        t_n1.n_regionkey = n_regionkey;
        r_n1.update_activate(t_n1);
    }

    private void update_activate_apv_n1(int n_nationkey) {
        k_n1.n_nationkey = n_nationkey;
        apv_n1.update_activate(k_n1);
    }

    private void update_activate_r_n2(int n_nationkey, String n_name) {
        t_n2.n_nationkey = n_nationkey;
        t_n2.n_name = n_name;
        r_n2.update_activate(t_n2);
    }

    private void update_activate_r_r(int r_regionkey) {
        t_r.r_regionkey = r_regionkey;
        r_r.update_activate(t_r);
    }

    private void update_add_hv_go1(int o_year, double value) {
        t_go.o_year = o_year;
        hv_go1.update_add(t_go, value);
    }

    private void update_add_hv_go2(int o_year, double value) {
        t_go.o_year = o_year;
        hv_go2.update_add(t_go, value);
    }
}
