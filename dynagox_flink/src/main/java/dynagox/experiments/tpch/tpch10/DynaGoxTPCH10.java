package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCH10 implements Serializable {
    int id;

    MultiHashMap<TPCH10Customer> r_c;
    MultiHashMap<TPCH10CustomerJoinKey> apv_c;
    MultiHashMap<TPCH10Orders> r_o;
    MultiHashMap<TPCH10OrdersJoinKey> apv_o;
    MultiHashMap<TPCH10LineitemJoinKey> apv_l;
    MultiHashMap<TPCH10Nation> r_n;
    MultiHashMap<TPCH10GOrders> hv_go;

    TPCH10Customer t_c;
    TPCH10CustomerJoinKey k_c;
    TPCH10Orders t_o;
    TPCH10OrdersJoinKey k_o;
    TPCH10LineitemJoinKey k_l;
    TPCH10Nation t_n;
    TPCH10GOrders t_go;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCH10(double epsilon, boolean print_result, boolean delta_mode) {
        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10Customer.Hash0(), new TPCH10Customer.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH10Customer.Hash1(), new TPCH10Customer.Equal1())));

        apv_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10CustomerJoinKey.Hash(), new TPCH10CustomerJoinKey.Equal()),
                List.of());

        r_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10Orders.Hash0(), new TPCH10Orders.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH10Orders.Hash1(), new TPCH10Orders.Equal1())));

        apv_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10OrdersJoinKey.Hash(), new TPCH10OrdersJoinKey.Equal()),
                List.of());

        apv_l = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10LineitemJoinKey.Hash(), new TPCH10LineitemJoinKey.Equal()),
                List.of());

        r_n = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10Nation.Hash(), new TPCH10Nation.Equal()), List.of());

        hv_go = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH10GOrders.Hash(), new TPCH10GOrders.Equal()), List.of());

        t_c = new TPCH10Customer();
        k_c = new TPCH10CustomerJoinKey();
        t_o = new TPCH10Orders();
        k_o = new TPCH10OrdersJoinKey();
        k_l = new TPCH10LineitemJoinKey();
        t_n = new TPCH10Nation();
        t_go = new TPCH10GOrders();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCH10GOrders gk_go = hv_go.get_keys();
            while (gk_go != null) {
                print(gk_go.c_custkey, gk_go.c_name, gk_go.c_address, gk_go.c_phone, gk_go.c_acctbal, gk_go.c_comment,
                        gk_go.n_name, gk_go.get_annotation());
                gk_go = (TPCH10GOrders) gk_go.get_next();
            }
        }
    }

    public void insert_into_customer(int c_custkey, String c_name, String c_address,
                                     int c_nationkey, String c_phone, double c_acctbal,
                                     String c_comment) {
        update_activate_r_c(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_comment);
        t_n.n_nationkey = c_nationkey;
        TPCH10Nation g_n = r_n.get(t_n);
        if (g_n != null) {
            update_activate_apv_c(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n.n_name);
            t_o.o_custkey = c_custkey;
            LinkedListNode<TPCH10Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_activate_apv_o(f_o.object.o_orderkey, c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment,
                        g_n.n_name);
                k_l.l_orderkey = f_o.object.o_orderkey;
                TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
                if (g_l != null) {
                    update_add_hv_go(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n.n_name, g_l.get_approximate());
                    if (delta_mode) {
                        TPCH10GOrders g_go = hv_go.get(t_go);
                        if (g_go != null) {
                            print(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n.n_name, g_go.get_annotation());
                        }
                    }
                }
                f_o = f_o.next;
            }
        }
    }

    public void delete_from_customer(int c_custkey, String c_name, String c_address,
                                     int c_nationkey, String c_phone, double c_acctbal,
                                     String c_comment) {
        update_remove_r_c(c_custkey);
        t_n.n_nationkey = c_nationkey;
        TPCH10Nation g_n = r_n.get(t_n);
        if (g_n != null) {
            update_remove_apv_c(c_custkey);
            t_o.o_custkey = c_custkey;
            LinkedListNode<TPCH10Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_remove_apv_o(f_o.object.o_orderkey);
                k_l.l_orderkey = f_o.object.o_orderkey;
                TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
                if (g_l != null) {
                    update_subtract_hv_go(c_custkey, g_l.get_approximate());
                    if (delta_mode) {
                        print(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n.n_name, 0);
                    }
                }
                f_o = f_o.next;
            }
        }
    }

    public void insert_into_orders(int o_orderkey, int o_custkey) {
        update_activate_r_o(o_orderkey, o_custkey);
        k_c.c_custkey = o_custkey;
        TPCH10CustomerJoinKey g_c = apv_c.get(k_c);
        if (g_c != null) {
            update_activate_apv_o(o_orderkey, o_custkey, g_c.c_name, g_c.c_address, g_c.c_phone, g_c.c_acctbal,
                    g_c.c_comment, g_c.n_name);
            k_l.l_orderkey = o_orderkey;
            TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_add_hv_go(o_custkey, g_c.c_name, g_c.c_address, g_c.c_phone, g_c.c_acctbal,
                        g_c.c_comment, g_c.n_name, g_l.get_approximate());
                if (delta_mode) {
                    TPCH10GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_c.c_custkey, g_c.c_name, g_c.c_address, g_c.c_phone, g_c.c_acctbal, g_c.c_comment,
                                g_c.n_name, g_go.get_annotation());
                    }
                }
            }
        }
    }

    public void delete_from_orders(int o_orderkey, int o_custkey) {
        update_remove_r_o(o_orderkey);
        k_c.c_custkey = o_custkey;
        TPCH10CustomerJoinKey g_c = apv_c.get(k_c);
        if (g_c != null) {
            update_remove_apv_o(o_orderkey);
            k_l.l_orderkey = o_orderkey;
            TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_subtract_hv_go(g_c.c_custkey, g_l.get_approximate());
                if (delta_mode) {
                    t_go.c_custkey = g_c.c_custkey;
                    TPCH10GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_c.c_custkey, g_c.c_name, g_c.c_address, g_c.c_phone, g_c.c_acctbal, g_c.c_comment,
                                g_c.n_name, g_go.get_annotation());
                    } else {
                        print(g_c.c_custkey, g_c.c_name, g_c.c_address, g_c.c_phone, g_c.c_acctbal, g_c.c_comment,
                                g_c.n_name, 0);
                    }
                }
            }
        }
    }

    public void insert_into_lineitem(int l_orderkey, double l_extendedprice, double l_discount) {
        double delta_pv_l = update_add_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
        if (delta_pv_l != 0) {
            k_o.o_orderkey = l_orderkey;
            TPCH10OrdersJoinKey g_o = apv_o.get(k_o);
            if (g_o != null) {
                update_add_hv_go(g_o.c_custkey, g_o.c_name, g_o.c_address, g_o.c_phone, g_o.c_acctbal, g_o.c_comment,
                        g_o.n_name, delta_pv_l);
                if (delta_mode) {
                    TPCH10GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_o.c_custkey, g_o.c_name, g_o.c_address, g_o.c_phone, g_o.c_acctbal, g_o.c_comment, g_o.n_name,
                                g_go.get_annotation());
                    }
                }
            }
        }
    }

    public void delete_from_lineitem(int l_orderkey, double l_extendedprice, double l_discount) {
        double delta_pv_l = update_subtract_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
        if (delta_pv_l != 0) {
            k_o.o_orderkey = l_orderkey;
            TPCH10OrdersJoinKey g_o = apv_o.get(k_o);
            if (g_o != null) {
                update_subtract_hv_go(g_o.c_custkey, delta_pv_l);
                if (delta_mode) {
                    t_go.c_custkey = g_o.c_custkey;
                    TPCH10GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_o.c_custkey, g_o.c_name, g_o.c_address, g_o.c_phone, g_o.c_acctbal, g_o.c_comment,
                                g_o.n_name, g_go.get_annotation());
                    } else {
                        print(g_o.c_custkey, g_o.c_name, g_o.c_address, g_o.c_phone, g_o.c_acctbal, g_o.c_comment,
                                g_o.n_name, 0);
                    }
                }
            }
        }
    }

    public void insert_into_nation(int n_nationkey, String n_name) {
        update_activate_r_n(n_nationkey, n_name);
        t_c.c_nationkey = n_nationkey;
        LinkedListNode<TPCH10Customer> f_c = r_c.find(0, t_c);
        while (f_c != null) {
            update_activate_apv_c(f_c.object.c_custkey, f_c.object.c_name, f_c.object.c_address, f_c.object.c_phone,
                    f_c.object.c_acctbal, f_c.object.c_comment, n_name);
            t_o.o_custkey = f_c.object.c_custkey;
            LinkedListNode<TPCH10Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_activate_apv_o(f_o.object.o_orderkey, f_c.object.c_custkey, f_c.object.c_name, f_c.object.c_address,
                        f_c.object.c_phone, f_c.object.c_acctbal, f_c.object.c_comment, n_name);
                k_l.l_orderkey = f_o.object.o_orderkey;
                TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
                if (g_l != null) {
                    update_add_hv_go(f_c.object.c_custkey, f_c.object.c_name, f_c.object.c_address,
                            f_c.object.c_phone, f_c.object.c_acctbal, f_c.object.c_comment, n_name, g_l.get_approximate());
                    if (delta_mode) {
                        TPCH10GOrders g_go = hv_go.get(t_go);
                        if (g_go != null) {
                            print(f_c.object.c_custkey, f_c.object.c_name, f_c.object.c_address, f_c.object.c_phone,
                                    f_c.object.c_acctbal, f_c.object.c_comment, n_name, g_go.get_annotation());
                        }
                    }
                }
                f_o = f_o.next;
            }
            f_c = f_c.next;
        }
    }

    public void delete_from_nation(int n_nationkey, String n_name) {
        update_remove_r_n(n_nationkey);
        t_c.c_nationkey = n_nationkey;
        LinkedListNode<TPCH10Customer> f_c = r_c.find(0, t_c);
        while (f_c != null) {
            update_remove_apv_c(f_c.object.c_custkey);
            t_o.o_custkey = f_c.object.c_custkey;
            LinkedListNode<TPCH10Orders> f_o = r_o.find(0, t_o);
            while (f_o != null) {
                update_remove_apv_o(f_o.object.o_orderkey);
                k_l.l_orderkey = f_o.object.o_orderkey;
                TPCH10LineitemJoinKey g_l = apv_l.get(k_l);
                if (g_l != null) {
                    update_subtract_hv_go(f_c.object.c_custkey, g_l.get_approximate());
                    if (delta_mode) {
                        print(f_c.object.c_custkey, f_c.object.c_name, f_c.object.c_address, f_c.object.c_phone,
                                f_c.object.c_acctbal, f_c.object.c_comment, n_name, 0);
                    }
                }
                f_o = f_o.next;
            }
            f_c = f_c.next;
        }
    }

    public void print(int c_custkey, String c_name, String c_address,
                      String c_phone, double c_acctbal, String c_comment,
                      String n_name, double value) {
        if (print_result) {
            String acctbal = String.valueOf(c_acctbal)
                    .replaceAll("\\.0+$", "")
                    .replaceAll("(\\.\\d*[1-9])0+$", "$1");
            System.out.println(id + "> (" + c_custkey + "," + c_name + "," + c_address + "," + c_phone + "," + acctbal
                    + "," + c_comment + "," + n_name + ") -> " + value);
        }
    }

    private void update_activate_r_c(int c_custkey, String c_name,
                                     String c_address, int c_nationkey,
                                     String c_phone, double c_acctbal, String c_comment) {
        t_c.c_custkey = c_custkey;
        t_c.c_name = c_name;
        t_c.c_address = c_address;
        t_c.c_nationkey = c_nationkey;
        t_c.c_phone = c_phone;
        t_c.c_acctbal = c_acctbal;
        t_c.c_comment = c_comment;
        r_c.update_activate(t_c);
    }

    private void update_remove_r_c(int c_custkey) {
        t_c.c_custkey = c_custkey;
        r_c.update_remove_if_exists(t_c);
    }

    private void update_activate_apv_c(int c_custkey, String c_name,
                                       String c_address, String c_phone, double c_acctbal,
                                       String c_comment, String n_name) {
        k_c.c_custkey = c_custkey;
        k_c.c_name = c_name;
        k_c.c_address = c_address;
        k_c.c_phone = c_phone;
        k_c.c_acctbal = c_acctbal;
        k_c.c_comment = c_comment;
        k_c.n_name = n_name;
        apv_c.update_activate(k_c);
    }

    private void update_remove_apv_c(int c_custkey) {
        k_c.c_custkey = c_custkey;
        apv_c.update_remove_if_exists(k_c);
    }

    private void update_activate_r_o(int o_orderkey, int o_custkey) {
        t_o.o_orderkey = o_orderkey;
        t_o.o_custkey = o_custkey;
        r_o.update_activate(t_o);
    }

    private void update_remove_r_o(int o_orderkey) {
        t_o.o_orderkey = o_orderkey;
        r_o.update_remove_if_exists(t_o);
    }

    private void update_activate_apv_o(int o_orderkey, int c_custkey, String c_name,
                                       String c_address, String c_phone, double c_acctbal,
                                       String c_comment, String n_name) {
        k_o.o_orderkey = o_orderkey;
        k_o.c_custkey = c_custkey;
        k_o.c_name = c_name;
        k_o.c_address = c_address;
        k_o.c_phone = c_phone;
        k_o.c_acctbal = c_acctbal;
        k_o.c_comment = c_comment;
        k_o.n_name = n_name;
        apv_o.update_activate(k_o);
    }

    private void update_remove_apv_o(int o_orderkey) {
        k_o.o_orderkey = o_orderkey;
        apv_o.update_remove_if_exists(k_o);
    }

    private double update_add_approx_apv_l(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l.update_add_approx(k_l, value, b);
    }

    private double update_subtract_approx_apv_l(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l.update_subtract_approx(k_l, value, b);
    }

    private void update_activate_r_n(int n_nationkey, String n_name) {
        t_n.n_nationkey = n_nationkey;
        t_n.n_name = n_name;
        r_n.update_activate(t_n);
    }

    private void update_remove_r_n(int n_nationkey) {
        t_n.n_nationkey = n_nationkey;
        r_n.update_remove_if_exists(t_n);
    }

    private void update_add_hv_go(int c_custkey, String c_name, String c_address,
                                  String c_phone, double c_acctbal, String c_comment,
                                  String n_name, double value) {
        t_go.c_custkey = c_custkey;
        t_go.c_name = c_name;
        t_go.c_address = c_address;
        t_go.c_phone = c_phone;
        t_go.c_acctbal = c_acctbal;
        t_go.c_comment = c_comment;
        t_go.n_name = n_name;
        hv_go.update_add(t_go, value);
    }

    private void update_subtract_hv_go(int c_custkey, double value) {
        t_go.c_custkey = c_custkey;
        hv_go.update_subtract(t_go, value);
    }
}
