package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCDS19 implements Serializable {
    int id;

    MultiHashMap<TPCDS19StoreSales> r_ss;
    MultiHashMap<TPCDS19Store> r_s;
    MultiHashMap<TPCDS19Customer> r_c;
    MultiHashMap<TPCDS19CustomerJoinKey> apv_c;
    MultiHashMap<TPCDS19CustomerAddress> r_ca;
    MultiHashMap<TPCDS19DateDim> r_d;
    MultiHashMap<TPCDS19Item> r_i;
    MultiHashMap<TPCDS19GStoreSales> hv_gss;

    TPCDS19StoreSales t_ss;
    TPCDS19Store t_s;
    TPCDS19Customer t_c;
    TPCDS19CustomerJoinKey k_c;
    TPCDS19CustomerAddress t_ca;
    TPCDS19DateDim t_d;
    TPCDS19Item t_i;
    TPCDS19GStoreSales t_gss;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCDS19(double epsilon, boolean print_result, boolean delta_mode) {
        r_ss = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19StoreSales.Hash(), new TPCDS19StoreSales.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCDS19StoreSales.Hash0(), new TPCDS19StoreSales.Equal0()),
                        new SecondaryHashIndex<>(new TPCDS19StoreSales.Hash1(), new TPCDS19StoreSales.Equal1()),
                        new SecondaryHashIndex<>(new TPCDS19StoreSales.Hash2(), new TPCDS19StoreSales.Equal2()),
                        new SecondaryHashIndex<>(new TPCDS19StoreSales.Hash3(), new TPCDS19StoreSales.Equal3())));

        r_s = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19Store.Hash(), new TPCDS19Store.Equal()), List.of());

        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19Customer.Hash(), new TPCDS19Customer.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCDS19Customer.Hash1(), new TPCDS19Customer.Equal1())));
        apv_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19CustomerJoinKey.Hash(), new TPCDS19CustomerJoinKey.Equal()),
                List.of());

        r_ca = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19CustomerAddress.Hash(), new TPCDS19CustomerAddress.Equal()),
                List.of());

        r_d = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19DateDim.Hash(), new TPCDS19DateDim.Equal()), List.of());

        r_i = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19Item.Hash(), new TPCDS19Item.Equal()), List.of());

        hv_gss = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS19GStoreSales.Hash(), new TPCDS19GStoreSales.Equal()),
                List.of());

        t_ss = new TPCDS19StoreSales();
        t_s = new TPCDS19Store();
        t_c = new TPCDS19Customer();
        k_c = new TPCDS19CustomerJoinKey();
        t_ca = new TPCDS19CustomerAddress();
        t_d = new TPCDS19DateDim();
        t_i = new TPCDS19Item();
        t_gss = new TPCDS19GStoreSales();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCDS19GStoreSales gk_gss = hv_gss.get_keys();
            while (gk_gss != null) {
                print(gk_gss.i_brand_id, gk_gss.i_brand, gk_gss.i_manufact_id, gk_gss.i_manufact, gk_gss.get_annotation());
                gk_gss = (TPCDS19GStoreSales) gk_gss.get_next();
            }
        }
    }

    public void insert_into_store_sales(int ss_sold_date_sk, int ss_item_sk,
                                                 int ss_customer_sk, int ss_store_sk,
                                                 double ss_ext_sales_price) {
        t_ss.ss_sold_date_sk = ss_sold_date_sk;
        t_ss.ss_item_sk = ss_item_sk;
        t_ss.ss_customer_sk = ss_customer_sk;
        t_ss.ss_store_sk = ss_store_sk;
        t_ss.ss_ext_sales_price = ss_ext_sales_price;
        r_ss.update_add(t_ss, 1);

        t_s.s_store_sk = ss_store_sk;
        TPCDS19Store g_s = r_s.get(t_s);
        if (g_s != null) {
            k_c.c_customer_sk = ss_customer_sk;
            TPCDS19CustomerJoinKey g_c = apv_c.get(k_c);
            if (g_c != null) {
                t_d.d_date_sk = ss_sold_date_sk;
                TPCDS19DateDim g_d = r_d.get(t_d);
                if (g_d != null) {
                    t_i.i_item_sk = ss_item_sk;
                    TPCDS19Item g_i = r_i.get(t_i);
                    if (g_i != null) {
                        t_gss.i_brand_id = g_i.i_brand_id;
                        t_gss.i_brand = g_i.i_brand;
                        t_gss.i_manufact_id = g_i.i_manufact_id;
                        t_gss.i_manufact = g_i.i_manufact;
                        hv_gss.update_add(t_gss, ss_ext_sales_price);
                        if (delta_mode) {
                            TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                            print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                        }
                    }
                }
            }
        }
    }

    public void insert_into_store(int s_store_sk) {
        t_s.s_store_sk = s_store_sk;
        r_s.update_add(t_s, 1);

        t_ss.ss_store_sk = s_store_sk;
        LinkedListNode<TPCDS19StoreSales> f_ss = r_ss.find(3, t_ss);
        while (f_ss != null) {
            k_c.c_customer_sk = f_ss.object.ss_customer_sk;
            TPCDS19CustomerJoinKey g_c = apv_c.get(k_c);
            if (g_c != null) {
                t_d.d_date_sk = f_ss.object.ss_sold_date_sk;
                TPCDS19DateDim g_d = r_d.get(t_d);
                if (g_d != null) {
                    t_i.i_item_sk = f_ss.object.ss_item_sk;
                    TPCDS19Item g_i = r_i.get(t_i);
                    if (g_i != null) {
                        t_gss.i_brand_id = g_i.i_brand_id;
                        t_gss.i_brand = g_i.i_brand;
                        t_gss.i_manufact_id = g_i.i_manufact_id;
                        t_gss.i_manufact = g_i.i_manufact;
                        hv_gss.update_add(t_gss, f_ss.object.ss_ext_sales_price);
                        if (delta_mode) {
                            TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                            print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                        }
                    }
                }
            }
            f_ss = f_ss.next;
        }
    }

    public void insert_into_customer(int c_customer_sk, int c_current_addr_sk) {
        t_c.c_customer_sk = c_customer_sk;
        t_c.c_current_addr_sk = c_current_addr_sk;
        r_c.update_add(t_c, 1);

        t_ca.ca_address_sk = c_current_addr_sk;
        TPCDS19CustomerAddress g_ca = r_ca.get(t_ca);
        if (g_ca != null) {
            k_c.c_customer_sk = c_customer_sk;
            apv_c.update_add(k_c, 1);

            t_ss.ss_customer_sk = c_customer_sk;
            LinkedListNode<TPCDS19StoreSales> f_ss = r_ss.find(2, t_ss);
            while (f_ss != null) {
                t_s.s_store_sk = f_ss.object.ss_store_sk;
                TPCDS19Store g_s = r_s.get(t_s);
                if (g_s != null) {
                    t_d.d_date_sk = f_ss.object.ss_sold_date_sk;
                    TPCDS19DateDim g_d = r_d.get(t_d);
                    if (g_d != null) {
                        t_i.i_item_sk = f_ss.object.ss_item_sk;
                        TPCDS19Item g_i = r_i.get(t_i);
                        if (g_i != null) {
                            t_gss.i_brand_id = g_i.i_brand_id;
                            t_gss.i_brand = g_i.i_brand;
                            t_gss.i_manufact_id = g_i.i_manufact_id;
                            t_gss.i_manufact = g_i.i_manufact;
                            hv_gss.update_add(t_gss, f_ss.object.ss_ext_sales_price);
                            if (delta_mode) {
                                TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                                print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                            }
                        }
                    }
                }
                f_ss = f_ss.next;
            }
        }
    }

    public void insert_into_customer_address(int ca_address_sk) {
        t_ca.ca_address_sk = ca_address_sk;
        r_ca.update_add(t_ca, 1);

        t_c.c_current_addr_sk = ca_address_sk;
        LinkedListNode<TPCDS19Customer> f_c = r_c.find(0, t_c);
        while (f_c != null) {
            k_c.c_customer_sk = f_c.object.c_customer_sk;
            apv_c.update_add(k_c, 1);

            t_ss.ss_customer_sk = f_c.object.c_customer_sk;
            LinkedListNode<TPCDS19StoreSales> f_ss = r_ss.find(2, t_ss);
            while (f_ss != null) {
                t_s.s_store_sk = f_ss.object.ss_store_sk;
                TPCDS19Store g_s = r_s.get(t_s);
                if (g_s != null) {
                    t_d.d_date_sk = f_ss.object.ss_sold_date_sk;
                    TPCDS19DateDim g_d = r_d.get(t_d);
                    if (g_d != null) {
                        t_i.i_item_sk = f_ss.object.ss_item_sk;
                        TPCDS19Item g_i = r_i.get(t_i);
                        if (g_i != null) {
                            t_gss.i_brand_id = g_i.i_brand_id;
                            t_gss.i_brand = g_i.i_brand;
                            t_gss.i_manufact_id = g_i.i_manufact_id;
                            t_gss.i_manufact = g_i.i_manufact;
                            hv_gss.update_add(t_gss, f_ss.object.ss_ext_sales_price);
                            if (delta_mode) {
                                TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                                print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                            }
                        }
                    }
                }
                f_ss = f_ss.next;
            }
            f_c = f_c.next;
        }
    }

    public void insert_into_date_dim(int d_date_sk) {
        t_d.d_date_sk = d_date_sk;
        r_d.update_add(t_d, 1);

        t_ss.ss_sold_date_sk = d_date_sk;
        LinkedListNode<TPCDS19StoreSales> f_ss = r_ss.find(0, t_ss);
        while (f_ss != null) {
            t_s.s_store_sk = f_ss.object.ss_store_sk;
            TPCDS19Store g_s = r_s.get(t_s);
            if (g_s != null) {
                k_c.c_customer_sk = f_ss.object.ss_customer_sk;
                TPCDS19CustomerJoinKey g_c = apv_c.get(k_c);
                if (g_c != null) {
                    t_i.i_item_sk = f_ss.object.ss_item_sk;
                    TPCDS19Item g_i = r_i.get(t_i);
                    if (g_i != null) {
                        t_gss.i_brand_id = g_i.i_brand_id;
                        t_gss.i_brand = g_i.i_brand;
                        t_gss.i_manufact_id = g_i.i_manufact_id;
                        t_gss.i_manufact = g_i.i_manufact;
                        hv_gss.update_add(t_gss, f_ss.object.ss_ext_sales_price);
                        if (delta_mode) {
                            TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                            print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                        }
                    }
                }
            }
            f_ss = f_ss.next;
        }
    }

    public void insert_into_item(int i_item_sk, int i_brand_id, String i_brand,
                                          int i_manufact_id, String i_manufact) {
        t_i.i_item_sk = i_item_sk;
        t_i.i_brand_id = i_brand_id;
        t_i.i_brand = i_brand;
        t_i.i_manufact_id = i_manufact_id;
        t_i.i_manufact = i_manufact;
        r_i.update_add(t_i, 1);

        t_ss.ss_item_sk = i_item_sk;
        LinkedListNode<TPCDS19StoreSales> f_ss = r_ss.find(1, t_ss);
        while (f_ss != null) {
            t_s.s_store_sk = f_ss.object.ss_store_sk;
            TPCDS19Store g_s = r_s.get(t_s);
            if (g_s != null) {
                k_c.c_customer_sk = f_ss.object.ss_customer_sk;
                TPCDS19CustomerJoinKey g_c = apv_c.get(k_c);
                if (g_c != null) {
                    t_d.d_date_sk = f_ss.object.ss_sold_date_sk;
                    TPCDS19DateDim g_d = r_d.get(t_d);
                    if (g_d != null) {
                        t_gss.i_brand_id = i_brand_id;
                        t_gss.i_brand = i_brand;
                        t_gss.i_manufact_id = i_manufact_id;
                        t_gss.i_manufact = i_manufact;
                        hv_gss.update_add(t_gss, f_ss.object.ss_ext_sales_price);
                        if (delta_mode) {
                            TPCDS19GStoreSales g_gss = hv_gss.get(t_gss);
                            print(g_gss.i_brand_id, g_gss.i_brand, g_gss.i_manufact_id, g_gss.i_manufact, g_gss.get_annotation());
                        }
                    }
                }
            }
            f_ss = f_ss.next;
        }
    }

    public void print(int i_brand_id, String i_brand, int i_manufact_id,
                         String i_manufact, double value) {
        if (print_result) {
            System.out.println(id + "> (" + i_brand_id + "," + i_brand + "," + i_manufact_id + "," + i_manufact + ") -> " + value);
        }
    }
}
