package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import dynagox.experiments.tpch.tpch8.*;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCH9 implements FlatMapFunction<String, TPCHEvent> {
    @Override
    public void flatMap(String value, Collector<TPCHEvent> out) throws Exception {
        if (value.equals("*")) {
            for (int key = 0; key < 16; key++) {
                out.collect(new TPCHEnumEvent(key, true));
            }
        } else {
            String[] fields = value.split("\\|");
            boolean is_insert = (fields[0].equals("+"));
            int rid = Integer.parseInt(fields[1]);
            switch (rid) {
                case 0:
                    String p_name = fields[3];
                    if (p_name.contains("green")) {
                        int p_partkey = Integer.parseInt(fields[2]);
                        for (int key = 0; key < 16; key++) {
                            out.collect(new TPCH9PartEvent(key, is_insert, p_partkey));
                        }
                    }
                    break;
                case 1:
                    int s_suppkey = Integer.parseInt(fields[2]);
                    int s_nationkey = Integer.parseInt(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH9SupplierEvent(key, is_insert, s_suppkey, s_nationkey));
                    }
                    break;
                case 2:
                    int ps_partkey = Integer.parseInt(fields[2]);
                    int ps_suppkey = Integer.parseInt(fields[3]);
                    double ps_supplycost = Double.parseDouble(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH9PartSuppEvent(key, is_insert, ps_partkey, ps_suppkey, ps_supplycost));
                    }
                    break;
                case 4:
                    int o_orderkey = Integer.parseInt(fields[2]);
                    int o_year = Integer.parseInt(fields[6].substring(0, 4));
                    out.collect(new TPCH9OrdersEvent(o_orderkey % 16, is_insert, o_orderkey, o_year));
                    break;
                case 5:
                    int l_orderkey = Integer.parseInt(fields[2]);
                    int l_partkey = Integer.parseInt(fields[3]);
                    int l_suppkey = Integer.parseInt(fields[4]);
                    int l_linenumber = Integer.parseInt(fields[5]);
                    double l_quantity = Double.parseDouble(fields[6]);
                    double l_extendedprice = Double.parseDouble(fields[7]);
                    double l_discount = Double.parseDouble(fields[8]);
                    out.collect(new TPCH9LineitemEvent(l_orderkey % 16, is_insert, l_orderkey, l_partkey, l_suppkey,
                            l_linenumber, l_quantity, l_extendedprice, l_discount));
                    break;
                case 6:
                    int n_nationkey = Integer.parseInt(fields[2]);
                    String n_name = fields[3];
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH9NationEvent(key, is_insert, n_nationkey, n_name));
                    }
                    break;
            }
        }
    }
}
