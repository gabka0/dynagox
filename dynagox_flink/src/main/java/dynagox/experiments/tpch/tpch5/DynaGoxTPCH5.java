package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;
import dynagox.experiments.tpch.tpch3.*;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCH5 implements Serializable {
    int id;

    MultiHashMap<TPCH5Supplier> r_s;
    MultiHashMap<TPCH5SupplierJoinKey> apv_s;
    MultiHashMap<TPCH5Customer> r_c;
    MultiHashMap<TPCH5CustomerJoinKey> apv_c;
    MultiHashMap<TPCH5Orders> r_o;
    MultiHashMap<TPCH5OrdersJoinKey> apv_o;
    MultiHashMap<TPCH5Lineitem> r_l;
    MultiHashMap<TPCH5Nation> r_n;
    MultiHashMap<TPCH5NationJoinKey> apv_n;
    MultiHashMap<TPCH5Region> r_r;
    MultiHashMap<TPCH5GLineitem> hv_gl;

    TPCH5Supplier t_s;
    TPCH5SupplierJoinKey k_s;
    TPCH5Customer t_c;
    TPCH5CustomerJoinKey k_c;
    TPCH5Orders t_o;
    TPCH5OrdersJoinKey k_o;
    TPCH5Lineitem t_l;
    TPCH5Nation t_n;
    TPCH5NationJoinKey k_n;
    TPCH5Region t_r;
    TPCH5GLineitem t_gl;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCH5(double epsilon, boolean print_result, boolean delta_mode) {
        r_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Supplier.Hash0(), new TPCH5Supplier.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH5Supplier.Hash1(), new TPCH5Supplier.Equal1())));

        apv_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5SupplierJoinKey.Hash(), new TPCH5SupplierJoinKey.Equal()), List.of());

        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Customer.Hash0(), new TPCH5Customer.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH5Customer.Hash1(), new TPCH5Customer.Equal1())));

        apv_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5CustomerJoinKey.Hash(), new TPCH5CustomerJoinKey.Equal()), List.of());

        r_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Orders.Hash0(), new TPCH5Orders.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH5Orders.Hash1(), new TPCH5Orders.Equal1())));

        apv_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5OrdersJoinKey.Hash(), new TPCH5OrdersJoinKey.Equal()), List.of());

        r_l = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Lineitem.Hash(), new TPCH5Lineitem.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCH5Lineitem.Hash0(), new TPCH5Lineitem.Equal0()),
                        new SecondaryHashIndex<>(new TPCH5Lineitem.Hash1(), new TPCH5Lineitem.Equal1())));

        r_n = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Nation.Hash0(), new TPCH5Nation.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH5Nation.Hash2(), new TPCH5Nation.Equal2())));

        apv_n = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5NationJoinKey.Hash(), new TPCH5NationJoinKey.Equal()), List.of());

        r_r = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5Region.Hash(), new TPCH5Region.Equal()), List.of());

        hv_gl = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH5GLineitem.Hash(), new TPCH5GLineitem.Equal()), List.of());

        t_s = new TPCH5Supplier();
        k_s = new TPCH5SupplierJoinKey();
        t_c = new TPCH5Customer();
        k_c = new TPCH5CustomerJoinKey();
        t_o = new TPCH5Orders();
        k_o = new TPCH5OrdersJoinKey();
        t_l = new TPCH5Lineitem();
        t_n = new TPCH5Nation();
        k_n = new TPCH5NationJoinKey();
        t_r = new TPCH5Region();
        t_gl = new TPCH5GLineitem();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCH5GLineitem gk_gl = hv_gl.get_keys();
            while (gk_gl != null) {
                print(gk_gl.n_name, gk_gl.get_annotation());
                gk_gl = (TPCH5GLineitem) gk_gl.get_next();
            }
        }
    }

    public void insert_into_supplier(int s_suppkey, int s_nationkey) {
        update_activate_r_s(s_suppkey, s_nationkey);
        k_n.n_nationkey = s_nationkey;
        TPCH5NationJoinKey g_n = apv_n.get(k_n);
        if (g_n != null) {
            update_activate_apv_s(s_suppkey, s_nationkey);
            t_l.l_suppkey = s_suppkey;
            LinkedListNode<TPCH5Lineitem> f_l = r_l.find(1, t_l);
            while (f_l != null) {
                k_o.o_orderkey = f_l.object.l_orderkey;
                TPCH5OrdersJoinKey g_o = apv_o.get(k_o);
                if (g_o != null && g_o.c_nationkey == s_nationkey) {
                    update_add_hv_gl(g_o.n_name, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                    if (delta_mode) {
                        t_gl.n_name = g_o.n_name;
                        TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                        if (g_gl != null) {
                            print(g_gl.n_name, g_gl.get_annotation());
                        }
                    }
                }
                f_l = f_l.next;
            }
        }
    }

    public void insert_into_customer(int c_custkey, int c_nationkey) {
        update_activate_r_c(c_custkey, c_nationkey);
        k_n.n_nationkey = c_nationkey;
        TPCH5NationJoinKey g_n = apv_n.get(k_n);
        if (g_n != null) {
            update_activate_apv_c(c_custkey, c_nationkey, g_n.n_name);
            t_o.o_custkey = c_custkey;
            LinkedListNode<TPCH5Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_activate_apv_o(f_o.object.o_orderkey, c_nationkey, g_n.n_name);
                t_l.l_orderkey = f_o.object.o_orderkey;
                LinkedListNode<TPCH5Lineitem> f_l = r_l.find(0, t_l);
                while (f_l != null) {
                    k_s.s_suppkey = f_l.object.l_suppkey;
                    TPCH5SupplierJoinKey g_s = apv_s.get(k_s);
                    if (g_s != null && g_s.s_nationkey == c_nationkey) {
                        update_add_hv_gl(g_n.n_name, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                        if (delta_mode) {
                            t_gl.n_name = g_n.n_name;
                            TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                            if (g_gl != null) {
                                print(g_gl.n_name, g_gl.get_annotation());
                            }
                        }
                    }
                    f_l = f_l.next;
                }
                f_o = f_o.next;
            }
        }
    }

    public void insert_into_orders(int o_orderkey, int o_custkey) {
        update_activate_r_o(o_orderkey, o_custkey);
        k_c.c_custkey = o_custkey;
        TPCH5CustomerJoinKey g_c = apv_c.get(k_c);
        if (g_c != null) {
            update_activate_apv_o(o_orderkey, g_c.c_nationkey, g_c.n_name);
            t_l.l_orderkey = o_orderkey;
            LinkedListNode<TPCH5Lineitem> f_l = r_l.find(0, t_l);
            while (f_l != null) {
                k_s.s_suppkey = f_l.object.l_suppkey;
                TPCH5SupplierJoinKey g_s = apv_s.get(k_s);
                if (g_s != null && g_s.s_nationkey == g_c.c_nationkey) {
                    update_add_hv_gl(g_c.n_name, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                    if (delta_mode) {
                        t_gl.n_name = g_c.n_name;
                        TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                        if (g_gl != null) {
                            print(g_gl.n_name, g_gl.get_annotation());
                        }
                    }
                }
                f_l = f_l.next;
            }
        }
    }

    public void insert_into_lineitem(int l_orderkey, int l_suppkey, int l_linenumber,
                                     double l_extendedprice, double l_discount) {
        update_activate_r_l(l_orderkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
        k_o.o_orderkey = l_orderkey;
        TPCH5OrdersJoinKey g_o = apv_o.get(k_o);
        if (g_o != null) {
            k_s.s_suppkey = l_suppkey;
            TPCH5SupplierJoinKey g_s = apv_s.get(k_s);
            if (g_s != null && g_o.c_nationkey == g_s.s_nationkey) {
                update_add_hv_gl(g_o.n_name, l_extendedprice * (1 - l_discount));
                if (delta_mode) {
                    t_gl.n_name = g_o.n_name;
                    TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                    if (g_gl != null) {
                        print(g_gl.n_name, g_gl.get_annotation());
                    }
                }
            }
        }
    }

    public void insert_into_nation(int n_nationkey, String n_name, int n_regionkey) {
        update_activate_r_n(n_nationkey, n_name, n_regionkey);
        t_r.r_regionkey = n_regionkey;
        TPCH5Region g_r = r_r.get(t_r);
        if (g_r != null) {
            update_activate_apv_n(n_nationkey, n_name);
            t_s.s_nationkey = n_nationkey;
            LinkedListNode<TPCH5Supplier> f_s = r_s.find(0, t_s);
            while (f_s != null) {
                update_activate_apv_s(f_s.object.s_suppkey, f_s.object.s_nationkey);
                f_s = f_s.next;
            }

            t_c.c_nationkey = n_nationkey;
            LinkedListNode<TPCH5Customer> f_c = r_c.find(0, t_c);
            while (f_c != null) {
                update_activate_apv_c(f_c.object.c_custkey, f_c.object.c_nationkey, n_name);
                t_o.o_custkey = f_c.object.c_custkey;
                LinkedListNode<TPCH5Orders> f_o = r_o.find(0, t_o);
                while (f_o != null) {
                    update_activate_apv_o(f_o.object.o_orderkey, n_nationkey, n_name);
                    t_l.l_orderkey = f_o.object.o_orderkey;
                    LinkedListNode<TPCH5Lineitem> f_l = r_l.find(0, t_l);
                    while (f_l != null) {
                        k_s.s_suppkey = f_l.object.l_suppkey;
                        TPCH5SupplierJoinKey g_s = apv_s.get(k_s);
                        if (g_s != null && g_s.s_nationkey == n_nationkey) {
                            update_add_hv_gl(n_name, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                            if (delta_mode) {
                                t_gl.n_name = n_name;
                                TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                                if (g_gl != null) {
                                    print(g_gl.n_name, g_gl.get_annotation());
                                }
                            }
                        }
                        f_l = f_l.next;
                    }
                    f_o = f_o.next;
                }
                f_c = f_c.next;
            }
        }
    }

    public void insert_into_region(int r_regionkey) {
        update_activate_r_r(r_regionkey);
        t_n.n_regionkey = r_regionkey;
        LinkedListNode<TPCH5Nation> f_n = r_n.find(0, t_n);
        while (f_n != null) {
            update_activate_apv_n(f_n.object.n_nationkey, f_n.object.n_name);
            t_s.s_nationkey = f_n.object.n_nationkey;
            LinkedListNode<TPCH5Supplier> f_s = r_s.find(0, t_s);
            while (f_s != null) {
                update_activate_apv_s(f_s.object.s_suppkey, f_s.object.s_nationkey);
                f_s = f_s.next;
            }

            t_c.c_nationkey = f_n.object.n_nationkey;
            LinkedListNode<TPCH5Customer> f_c = r_c.find(0, t_c);
            while (f_c != null) {
                update_activate_apv_c(f_c.object.c_custkey, f_c.object.c_nationkey, f_n.object.n_name);
                t_o.o_custkey = f_c.object.c_custkey;
                LinkedListNode<TPCH5Orders> f_o = r_o.find(0, t_o);
                while (f_o != null) {
                    update_activate_apv_o(f_o.object.o_orderkey, f_n.object.n_nationkey, f_n.object.n_name);
                    t_l.l_orderkey = f_o.object.o_orderkey;
                    LinkedListNode<TPCH5Lineitem> f_l = r_l.find(0, t_l);
                    while (f_l != null) {
                        k_s.s_suppkey = f_l.object.l_suppkey;
                        TPCH5SupplierJoinKey g_s = apv_s.get(k_s);
                        if (g_s != null && g_s.s_nationkey == f_n.object.n_nationkey) {
                            update_add_hv_gl(f_n.object.n_name, f_l.object.l_extendedprice * (1 - f_l.object.l_discount));
                            if (delta_mode) {
                                t_gl.n_name = f_n.object.n_name;
                                TPCH5GLineitem g_gl = hv_gl.get(t_gl);
                                if (g_gl != null) {
                                    print(g_gl.n_name, g_gl.get_annotation());
                                }
                            }
                        }
                        f_l = f_l.next;
                    }
                    f_o = f_o.next;
                }
                f_c = f_c.next;
            }
            f_n = f_n.next;
        }
    }

    private void print(String n_name, double value) {
        if (print_result) {
            System.out.println(id + "> (" + n_name + ") -> " + value);
        }
    }

    private void update_activate_r_s(int s_suppkey, int s_nationkey) {
        t_s.s_suppkey = s_suppkey;
        t_s.s_nationkey = s_nationkey;
        r_s.update_activate(t_s);
    }

    private void update_activate_apv_s(int s_suppkey, int s_nationkey) {
        k_s.s_suppkey = s_suppkey;
        k_s.s_nationkey = s_nationkey;
        apv_s.update_activate(k_s);
    }

    private void update_activate_r_c(int c_custkey, int c_nationkey) {
        t_c.c_custkey = c_custkey;
        t_c.c_nationkey = c_nationkey;
        r_c.update_activate(t_c);
    }

    private void update_activate_apv_c(int c_custkey, int c_nationkey, String n_name) {
        k_c.c_custkey = c_custkey;
        k_c.c_nationkey = c_nationkey;
        k_c.n_name = n_name;
        apv_c.update_activate(k_c);
    }

    private void update_activate_r_o(int o_orderkey, int o_custkey) {
        t_o.o_orderkey = o_orderkey;
        t_o.o_custkey = o_custkey;
        r_o.update_activate(t_o);
    }

    private void update_activate_apv_o(int o_orderkey, int c_nationkey, String n_name) {
        k_o.o_orderkey = o_orderkey;
        k_o.c_nationkey = c_nationkey;
        k_o.n_name = n_name;
        apv_o.update_activate(k_o);
    }

    private void update_activate_r_l(int l_orderkey, int l_suppkey, int l_linenumber,
                                     double l_extendedprice, double l_discount) {
        t_l.l_orderkey = l_orderkey;
        t_l.l_suppkey = l_suppkey;
        t_l.l_linenumber = l_linenumber;
        t_l.l_extendedprice = l_extendedprice;
        t_l.l_discount = l_discount;
        r_l.update_activate(t_l);
    }

    private void update_activate_r_n(int n_nationkey, String n_name, int n_regionkey) {
        t_n.n_nationkey = n_nationkey;
        t_n.n_name = n_name;
        t_n.n_regionkey = n_regionkey;
        r_n.update_activate(t_n);
    }

    private void update_activate_apv_n(int n_nationkey, String n_name) {
        k_n.n_nationkey = n_nationkey;
        k_n.n_name = n_name;
        apv_n.update_activate(k_n);
    }

    private void update_activate_r_r(int r_regionkey) {
        t_r.r_regionkey = r_regionkey;
        r_r.update_activate(t_r);
    }

    private void update_add_hv_gl(String n_name, double value) {
        t_gl.n_name = n_name;
        hv_gl.update_add(t_gl, value);
    }
}
