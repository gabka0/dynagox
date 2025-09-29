package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCH5 implements FlatMapFunction<String, TPCHEvent> {
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
                case 1:
                    int s_suppkey = Integer.parseInt(fields[2]);
                    int s_nationkey = Integer.parseInt(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH5SupplierEvent(key, is_insert, s_suppkey, s_nationkey));
                    }
                    break;
                case 3:
                    int c_custkey = Integer.parseInt(fields[2]);
                    int c_nationkey = Integer.parseInt(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH5CustomerEvent(key, is_insert, c_custkey, c_nationkey));
                    }
                    break;
                case 4:
                    int o_orderdate = Integer.parseInt(fields[6].substring(0, 4)) * 10000
                            + Integer.parseInt(fields[6].substring(5, 7)) * 100
                            + Integer.parseInt(fields[6].substring(8, 10));
                    if (o_orderdate >= 19940101 && o_orderdate < 19950101) {
                        int o_orderkey = Integer.parseInt(fields[2]);
                        int o_custkey = Integer.parseInt(fields[3]);
                        out.collect(new TPCH5OrdersEvent(o_orderkey % 16, is_insert, o_orderkey, o_custkey));
                    }
                    break;
                case 5:
                    int l_orderkey = Integer.parseInt(fields[2]);
                    int l_suppkey = Integer.parseInt(fields[4]);
                    int l_linenumber = Integer.parseInt(fields[5]);
                    double l_extendedprice = Double.parseDouble(fields[7]);
                    double l_discount = Double.parseDouble(fields[8]);
                    out.collect(new TPCH5LineitemEvent(l_orderkey % 16, is_insert, l_orderkey, l_suppkey,
                            l_linenumber, l_extendedprice, l_discount));
                    break;
                case 6:
                    int n_nationkey = Integer.parseInt(fields[2]);
                    String n_name = fields[3];
                    int n_regionkey = Integer.parseInt(fields[4]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH5NationEvent(key, is_insert, n_nationkey, n_name, n_regionkey));
                    }
                    break;
                case 7:
                    String r_name = fields[3];
                    if (r_name.equals("ASIA")) {
                        int r_regionkey = Integer.parseInt(fields[2]);
                        for (int key = 0; key < 16; key++) {
                            out.collect(new TPCH5RegionEvent(key, is_insert, r_regionkey));
                        }
                    }
                    break;
            }
        }
    }
}
