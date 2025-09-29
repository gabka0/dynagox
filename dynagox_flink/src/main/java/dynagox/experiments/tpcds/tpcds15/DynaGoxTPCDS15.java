package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.Utils;
import dynagox.experiments.hashmap.LinkedListNode;
import dynagox.experiments.hashmap.MultiHashMap;
import dynagox.experiments.hashmap.PrimaryHashIndex;
import dynagox.experiments.hashmap.SecondaryHashIndex;

import java.io.Serializable;
import java.util.List;

public class DynaGoxTPCDS15 implements Serializable {
    int id;

    MultiHashMap<TPCDS15CatalogSales> r_cs;
    MultiHashMap<TPCDS15CatalogSalesJoinKey> apv_cs;
    MultiHashMap<TPCDS15Customer> r_c;
    MultiHashMap<TPCDS15CustomerJoinKey> apv_c;
    MultiHashMap<TPCDS15CustomerAddress> r_ca;
    MultiHashMap<TPCDS15DateDim> r_d;
    MultiHashMap<TPCDS15GCustomer> hv_gc;

    TPCDS15CatalogSales t_cs;
    TPCDS15CatalogSalesJoinKey k_cs;
    TPCDS15Customer t_c;
    TPCDS15CustomerJoinKey k_c;
    TPCDS15CustomerAddress t_ca;
    TPCDS15DateDim t_d;
    TPCDS15GCustomer t_gc;

    double b;
    boolean print_result = false;
    boolean delta_mode = false;

    public DynaGoxTPCDS15(double epsilon, boolean print_result, boolean delta_mode) {
        r_cs = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15CatalogSales.Hash(), new TPCDS15CatalogSales.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCDS15CatalogSales.Hash0(), new TPCDS15CatalogSales.Equal0())));
        apv_cs = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15CatalogSalesJoinKey.Hash(),
                new TPCDS15CatalogSalesJoinKey.Equal()), List.of());

        r_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15Customer.Hash(), new TPCDS15Customer.Equal()),
                List.of(new SecondaryHashIndex<>(new TPCDS15Customer.Hash1(), new TPCDS15Customer.Equal1())));
        apv_c = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15CustomerJoinKey.Hash(), new TPCDS15CustomerJoinKey.Equal()),
                List.of());

        r_ca = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15CustomerAddress.Hash(), new TPCDS15CustomerAddress.Equal()),
                List.of());

        r_d = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15DateDim.Hash(), new TPCDS15DateDim.Equal()), List.of());

        hv_gc = new MultiHashMap<>(new PrimaryHashIndex<>(new TPCDS15GCustomer.Hash(), new TPCDS15GCustomer.Equal()), List.of());

        t_cs = new TPCDS15CatalogSales();
        k_cs = new TPCDS15CatalogSalesJoinKey();
        t_c = new TPCDS15Customer();
        k_c = new TPCDS15CustomerJoinKey();
        t_ca = new TPCDS15CustomerAddress();
        t_d = new TPCDS15DateDim();
        t_gc = new TPCDS15GCustomer();

        b = Utils.get_b(epsilon, 1);
        this.print_result = print_result;
        this.delta_mode = delta_mode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void milestone() {
        if (!delta_mode) {
            TPCDS15GCustomer gk_gc = hv_gc.get_keys();
            while (gk_gc != null) {
                print(gk_gc.ca_zip, gk_gc.get_annotation());
                gk_gc = (TPCDS15GCustomer) gk_gc.get_next();
            }
        }
    }

    public void insert_into_catalog_sales(int cs_sold_date_sk, int cs_bill_customer_sk,
                                                   double cs_sales_price) {
        t_cs.cs_sold_date_sk = cs_sold_date_sk;
        t_cs.cs_bill_customer_sk = cs_bill_customer_sk;
        t_cs.cs_sales_price = cs_sales_price;
        r_cs.update_add(t_cs, 1);

        t_d.d_date_sk = cs_sold_date_sk;
        TPCDS15DateDim g_d = r_d.get(t_d);
        if (g_d != null) {
            k_cs.cs_bill_customer_sk = cs_bill_customer_sk;
            apv_cs.update_add(k_cs, cs_sales_price);

            k_c.c_customer_sk = cs_bill_customer_sk;
            TPCDS15CustomerJoinKey g_c = apv_c.get(k_c);
            if (g_c != null) {
                t_gc.ca_zip = g_c.ca_zip;
                hv_gc.update_add(t_gc, cs_sales_price);
                if (delta_mode) {
                    TPCDS15GCustomer g_gc = hv_gc.get(t_gc);
                    print(g_gc.ca_zip, g_gc.get_annotation());
                }
            }
        }
    }

    public void insert_into_customer(int c_customer_sk, int c_current_addr_sk) {
        t_c.c_customer_sk = c_customer_sk;
        t_c.c_current_addr_sk = c_current_addr_sk;
        r_c.update_add(t_c, 1);

        t_ca.ca_address_sk = c_current_addr_sk;
        TPCDS15CustomerAddress g_ca = r_ca.get(t_ca);
        if (g_ca != null) {
            k_c.c_customer_sk = c_customer_sk;
            k_c.ca_zip = g_ca.ca_zip;
            apv_c.update_add(k_c, 1);

            k_cs.cs_bill_customer_sk = c_customer_sk;
            TPCDS15CatalogSalesJoinKey g_cs = apv_cs.get(k_cs);
            if (g_cs != null) {
                t_gc.ca_zip = g_ca.ca_zip;
                hv_gc.update_add(t_gc, g_cs.get_annotation());
                if (delta_mode) {
                    TPCDS15GCustomer g_gc = hv_gc.get(t_gc);
                    print(g_gc.ca_zip, g_gc.get_annotation());
                }
            }
        }
    }

    public void insert_into_customer_address(int ca_address_sk, String ca_zip) {
        t_ca.ca_address_sk = ca_address_sk;
        t_ca.ca_zip = ca_zip;
        r_ca.update_add(t_ca, 1);

        t_c.c_current_addr_sk = ca_address_sk;
        LinkedListNode<TPCDS15Customer> f_c = r_c.find(0, t_c);
        while (f_c != null) {
            k_c.c_customer_sk = f_c.object.c_customer_sk;
            k_c.ca_zip = ca_zip;
            apv_c.update_add(k_c, 1);

            k_cs.cs_bill_customer_sk = f_c.object.c_customer_sk;
            TPCDS15CatalogSalesJoinKey g_cs = apv_cs.get(k_cs);
            if (g_cs != null) {
                t_gc.ca_zip = ca_zip;
                hv_gc.update_add(t_gc, g_cs.get_annotation());
                if (delta_mode) {
                    TPCDS15GCustomer g_gc = hv_gc.get(t_gc);
                    print(g_gc.ca_zip, g_gc.get_annotation());
                }
            }
            f_c = f_c.next;
        }
    }

    public void insert_into_date_dim(int d_date_sk) {
        t_d.d_date_sk = d_date_sk;
        r_d.update_add(t_d, 1);

        t_cs.cs_sold_date_sk = d_date_sk;
        LinkedListNode<TPCDS15CatalogSales> f_cs = r_cs.find(0, t_cs);
        while (f_cs != null) {
            k_cs.cs_bill_customer_sk = f_cs.object.cs_bill_customer_sk;
            apv_cs.update_add(k_cs, f_cs.object.cs_sales_price * f_cs.object.get_annotation());

            k_c.c_customer_sk = f_cs.object.cs_bill_customer_sk;
            TPCDS15CustomerJoinKey g_c = apv_c.get(k_c);
            if (g_c != null) {
                t_gc.ca_zip = g_c.ca_zip;
                hv_gc.update_add(t_gc, f_cs.object.cs_sales_price * f_cs.object.get_annotation());
                if (delta_mode) {
                    TPCDS15GCustomer g_gc = hv_gc.get(t_gc);
                    print(g_gc.ca_zip, g_gc.get_annotation());
                }
            }

            f_cs = f_cs.next;
        }
    }

    public void print(String ca_zip, double value) {
        if (print_result) {
            System.out.println(id + "> (" + ca_zip + ") -> " + value);
        }
    }
}
