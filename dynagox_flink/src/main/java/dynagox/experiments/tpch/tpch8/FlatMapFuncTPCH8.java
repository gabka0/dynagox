package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCH8 implements FlatMapFunction<String, TPCHEvent> {
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
                    String p_type = fields[6];
                    if (p_type.equals("ECONOMY ANODIZED STEEL")) {
                        int p_partkey = Integer.parseInt(fields[2]);
                        for (int key = 0; key < 16; key++) {
                            out.collect(new TPCH8PartEvent(key, is_insert, p_partkey));
                        }
                    }
                    break;
                case 1:
                    int s_suppkey = Integer.parseInt(fields[2]);
                    int s_nationkey = Integer.parseInt(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH8SupplierEvent(key, is_insert, s_suppkey, s_nationkey));
                    }
                    break;
                case 3:
                    int c_custkey = Integer.parseInt(fields[2]);
                    int c_nationkey = Integer.parseInt(fields[5]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH8CustomerEvent(key, is_insert, c_custkey, c_nationkey));
                    }
                    break;
                case 4:
                    int o_orderdate = Integer.parseInt(fields[6].substring(0, 4)) * 10000
                            + Integer.parseInt(fields[6].substring(5, 7)) * 100
                            + Integer.parseInt(fields[6].substring(8, 10));
                    if (19950101 <= o_orderdate && o_orderdate <= 19961231) {
                        int o_orderkey = Integer.parseInt(fields[2]);
                        int o_custkey = Integer.parseInt(fields[3]);
                        int o_year = Integer.parseInt(fields[6].substring(0, 4));
                        out.collect(new TPCH8OrdersEvent(o_orderkey % 16, is_insert, o_orderkey, o_custkey, o_year));
                    }
                    break;
                case 5:
                    int l_orderkey = Integer.parseInt(fields[2]);
                    int l_partkey = Integer.parseInt(fields[3]);
                    int l_suppkey = Integer.parseInt(fields[4]);
                    int l_linenumber = Integer.parseInt(fields[5]);
                    double l_extendedprice = Double.parseDouble(fields[7]);
                    double l_discount = Double.parseDouble(fields[8]);
                    out.collect(new TPCH8LineitemEvent(l_orderkey % 16, is_insert, l_orderkey, l_partkey, l_suppkey,
                            l_linenumber, l_extendedprice, l_discount));
                    break;
                case 6:
                    int n_nationkey = Integer.parseInt(fields[2]);
                    String n_name = fields[3];
                    int n_regionkey = Integer.parseInt(fields[4]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH8NationEvent(key, is_insert, n_nationkey, n_name, n_regionkey));
                    }
                    break;
                case 7:
                    String r_name = fields[3];
                    if (r_name.equals("AMERICA")) {
                        int r_regionkey = Integer.parseInt(fields[2]);
                        for (int key = 0; key < 16; key++) {
                            out.collect(new TPCH8RegionEvent(key, is_insert, r_regionkey));
                        }
                    }
                    break;
            }
        }
    }
}
