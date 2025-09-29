package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCH3 implements Serializable {
    int id;

    MultiHashMap<TPCH3Customer> r_c;
    MultiHashMap<TPCH3Orders> r_o;
    MultiHashMap<TPCH3OrdersJoinKey> apv_o;
    MultiHashMap<TPCH3LineitemJoinKey> apv_l;
    MultiHashMap<TPCH3GOrders> hv_go;

    TPCH3Customer t_c;
    TPCH3LineitemJoinKey k_l;
    TPCH3Orders t_o;
    TPCH3OrdersJoinKey k_o;
    TPCH3GOrders t_go;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCH3(double epsilon, boolean print_result, boolean delta_mode) {
        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH3Customer.Hash(), new TPCH3Customer.Equal()), List.of());

        r_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH3Orders.Hash0(), new TPCH3Orders.Equal0()),
                List.of(new SecondaryHashIndex<>(new TPCH3Orders.Hash1(), new TPCH3Orders.Equal1())));

        apv_o = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH3OrdersJoinKey.Hash(), new TPCH3OrdersJoinKey.Equal()), List.of());

        apv_l = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH3LineitemJoinKey.Hash(), new TPCH3LineitemJoinKey.Equal()), List.of());

        hv_go = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCH3GOrders.Hash(), new TPCH3GOrders.Equal()), List.of());

        t_c = new TPCH3Customer();
        k_l = new TPCH3LineitemJoinKey();
        t_o = new TPCH3Orders();
        k_o = new TPCH3OrdersJoinKey();
        t_go = new TPCH3GOrders();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCH3GOrders gk_go = hv_go.get_keys();
            while (gk_go != null) {
                print(gk_go.o_orderkey, gk_go.o_orderdate, gk_go.o_shippriority, gk_go.get_annotation());
                gk_go = (TPCH3GOrders) gk_go.get_next();
            }
        }
    }

    public void insert_into_customer(int c_custkey) {
        update_activate_r_c(c_custkey);
        t_o.o_custkey = c_custkey;
        LinkedListNode<TPCH3Orders> f_o = r_o.find(0, t_o);
        while (f_o != null) {
            update_activate_apv_o(f_o.object.o_orderkey, f_o.object.o_orderdate, f_o.object.o_shippriority);
            k_l.l_orderkey = f_o.object.o_orderkey;
            TPCH3LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_add_hv_go(f_o.object.o_orderkey, f_o.object.o_orderdate, f_o.object.o_shippriority, g_l.get_approximate());
                if (delta_mode) {
                    print(f_o.object.o_orderkey, f_o.object.o_orderdate, f_o.object.o_shippriority, g_l.get_approximate());
                }
            }
            f_o = f_o.next;
        }
    }

    public void delete_from_customer(int c_custkey) {
        update_remove_r_c(c_custkey);
        t_o.o_custkey = c_custkey;
        LinkedListNode<TPCH3Orders> f_o = r_o.find(0, t_o);
        while (f_o != null) {
            update_remove_apv_o(f_o.object.o_orderkey);
            k_l.l_orderkey = f_o.object.o_orderkey;
            TPCH3LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_subtract_hv_go(f_o.object.o_orderkey, g_l.get_approximate());
                if (delta_mode) {
                    print(f_o.object.o_orderkey, f_o.object.o_orderdate, f_o.object.o_shippriority, 0);
                }
            }
            f_o = f_o.next;
        }
    }

    public void insert_into_orders(int o_orderkey, int o_custkey, int o_orderdate,
                                          int o_shippriority) {
        update_activate_r_o(o_orderkey, o_custkey, o_orderdate, o_shippriority);
        t_c.c_custkey = o_custkey;
        TPCH3Customer g_c = r_c.get(t_c);
        if (g_c != null) {
            update_activate_apv_o(o_orderkey, o_orderdate, o_shippriority);
            k_l.l_orderkey = o_orderkey;
            TPCH3LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_add_hv_go(o_orderkey, o_orderdate, o_shippriority, g_l.get_approximate());
                if (delta_mode) {
                    print(o_orderkey, o_orderdate, o_shippriority, g_l.get_approximate());
                }
            }
        }
    }

    public void delete_from_orders(int o_orderkey, int o_custkey, int o_orderdate, int o_shippriority) {
        update_remove_r_o(o_orderkey);
        t_c.c_custkey = o_custkey;
        TPCH3Customer g_c = r_c.get(t_c);
        if (g_c != null) {
            update_remove_apv_o(o_orderkey);
            k_l.l_orderkey = o_orderkey;
            TPCH3LineitemJoinKey g_l = apv_l.get(k_l);
            if (g_l != null) {
                update_subtract_hv_go(o_orderkey, g_l.get_approximate());
                if (delta_mode) {
                    print(o_orderkey, o_orderdate, o_shippriority, 0);
                }
            }
        }
    }

    public void insert_into_lineitem(int l_orderkey, double l_extendedprice, double l_discount) {
        double delta_pv_l = update_add_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
        if (delta_pv_l != 0) {
            k_o.o_orderkey = l_orderkey;
            TPCH3OrdersJoinKey g_o = apv_o.get(k_o);
            if (g_o != null) {
                update_add_hv_go(g_o.o_orderkey, g_o.o_orderdate, g_o.o_shippriority, delta_pv_l);
                if (delta_mode) {
                    t_go.o_orderkey = l_orderkey;
                    TPCH3GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_o.o_orderkey, g_o.o_orderdate, g_o.o_shippriority, g_go.get_annotation());
                    }
                }
            }
        }
    }

    public void delete_from_lineitem(int l_orderkey, double l_extendedprice, double l_discount) {
        double delta_pv_l = update_subtract_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
        if (delta_pv_l != 0) {
            k_o.o_orderkey = l_orderkey;
            TPCH3OrdersJoinKey g_o = apv_o.get(k_o);
            if (g_o != null) {
                update_subtract_hv_go(g_o.o_orderkey, delta_pv_l);
                if (delta_mode) {
                    t_go.o_orderkey = l_orderkey;
                    TPCH3GOrders g_go = hv_go.get(t_go);
                    if (g_go != null) {
                        print(g_o.o_orderkey, g_o.o_orderdate, g_o.o_shippriority, g_go.get_annotation());
                    } else {
                        print(g_o.o_orderkey, g_o.o_orderdate, g_o.o_shippriority, 0);
                    }
                }
            }
        }
    }

    private void print(int o_orderkey, int o_orderdate, int o_shippriority, double value) {
        if (print_result) {
            System.out.println(id + "> (" + o_orderkey + "," + o_orderdate + "," + o_shippriority + ") -> " + value);
        }
    }

    private void update_activate_r_c(int c_custkey) {
        t_c.c_custkey = c_custkey;
        r_c.update_activate(t_c);
    }

    private void update_remove_r_c(int c_custkey) {
        t_c.c_custkey = c_custkey;
        r_c.update_remove_if_exists(t_c);
    }

    private void update_activate_apv_o(int o_orderkey, int o_orderdate, int o_shippriority) {
        k_o.o_orderkey = o_orderkey;
        k_o.o_orderdate = o_orderdate;
        k_o.o_shippriority = o_shippriority;
        apv_o.update_activate(k_o);
    }

    private void update_remove_apv_o(int o_orderkey) {
        k_o.o_orderkey = o_orderkey;
        apv_o.update_remove_if_exists(k_o);
    }

    private void update_activate_r_o(int o_orderkey, int o_custkey, int o_orderdate,
                                           int o_shippriority) {
        t_o.o_orderkey = o_orderkey;
        t_o.o_custkey = o_custkey;
        t_o.o_orderdate = o_orderdate;
        t_o.o_shippriority = o_shippriority;
        r_o.update_activate(t_o);
    }

    private void update_remove_r_o(int o_orderkey) {
        t_o.o_orderkey = o_orderkey;
        r_o.update_remove_if_exists(t_o);
    }

    private double update_add_approx_apv_l(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l.update_add_approx(k_l, value, b);
    }

    private double update_subtract_approx_apv_l(int l_orderkey, double value) {
        k_l.l_orderkey = l_orderkey;
        return apv_l.update_subtract_approx(k_l, value, b);
    }

    private void update_add_hv_go(int o_orderkey, int o_orderdate, int o_shippriority, double value) {
        t_go.o_orderkey = o_orderkey;
        t_go.o_orderdate = o_orderdate;
        t_go.o_shippriority = o_shippriority;
        hv_go.update_add(t_go, value);
    }

    private void update_subtract_hv_go(int o_orderkey, double value) {
        t_go.o_orderkey = o_orderkey;
        hv_go.update_subtract(t_go, value);
    }
}
