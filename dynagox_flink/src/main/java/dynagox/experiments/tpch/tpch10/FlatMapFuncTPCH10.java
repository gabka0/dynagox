package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCH10 implements FlatMapFunction<String, TPCHEvent> {
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
                case 3:
                    int c_custkey = Integer.parseInt(fields[2]);
                    String c_name = fields[3];
                    String c_address = fields[4];
                    int c_nationkey = Integer.parseInt(fields[5]);
                    String c_phone = fields[6];
                    double c_acctbal = Double.parseDouble(fields[7]);
                    String c_comment = fields[9];
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH10CustomerEvent(key, is_insert, c_custkey, c_name, c_address, c_nationkey,
                                c_phone, c_acctbal, c_comment));
                    }
                    break;
                case 4:
                    int o_orderdate = Integer.parseInt(fields[6].substring(0, 4)) * 10000
                            + Integer.parseInt(fields[6].substring(5, 7)) * 100
                            + Integer.parseInt(fields[6].substring(8, 10));
                    if (o_orderdate >= 19931001 && o_orderdate < 19940101) {
                        int o_orderkey = Integer.parseInt(fields[2]);
                        int o_custkey = Integer.parseInt(fields[3]);
                        out.collect(new TPCH10OrdersEvent(o_orderkey % 16, is_insert, o_orderkey, o_custkey));
                    }
                    break;
                case 5:
                    String l_returnflag = fields[10];
                    if (l_returnflag.equals("R")) {
                        int l_orderkey = Integer.parseInt(fields[2]);
                        double l_extendedprice = Double.parseDouble(fields[7]);
                        double l_discount = Double.parseDouble(fields[8]);
                        out.collect(new TPCH10LineitemEvent(l_orderkey % 16, is_insert, l_orderkey,
                                l_extendedprice, l_discount));
                    }
                    break;
                case 6:
                    int n_nationkey = Integer.parseInt(fields[2]);
                    String n_name = fields[3];
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCH10NationEvent(key, is_insert, n_nationkey, n_name));
                    }
                    break;
            }
        }
    }
}
