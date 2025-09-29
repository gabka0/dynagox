package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCH3 implements FlatMapFunction<String, TPCHEvent> {
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
                    String c_mktsegment = fields[8];
                    if (c_mktsegment.equals("BUILDING")) {
                        int c_custkey = Integer.parseInt(fields[2]);
                        for (int key = 0; key < 16; key++) {
                            out.collect(new TPCH3CustomerEvent(key, is_insert, c_custkey));
                        }
                    }
                    break;
                case 4:
                    int o_orderdate = Integer.parseInt(fields[6].substring(0, 4)) * 10000
                            + Integer.parseInt(fields[6].substring(5, 7)) * 100
                            + Integer.parseInt(fields[6].substring(8, 10));
                    if (o_orderdate < 19950315) {
                        int o_orderkey = Integer.parseInt(fields[2]);
                        int o_custkey = Integer.parseInt(fields[3]);
                        int o_shippriority = Integer.parseInt(fields[9]);
                        out.collect(new TPCH3OrdersEvent(o_orderkey % 16, is_insert, o_orderkey, o_custkey, o_orderdate, o_shippriority));
                    }
                    break;
                case 5:
                    int l_shipdate = Integer.parseInt(fields[12].substring(0, 4)) * 10000
                            + Integer.parseInt(fields[12].substring(5, 7)) * 100
                            + Integer.parseInt(fields[12].substring(8, 10));
                    if (l_shipdate > 19950315) {
                        int l_orderkey = Integer.parseInt(fields[2]);
                        double l_extendedprice = Double.parseDouble(fields[7]);
                        double l_discount = Double.parseDouble(fields[8]);
                        int key = l_orderkey % 16;
                        out.collect(new TPCH3LineitemEvent(key, is_insert, l_orderkey, l_extendedprice, l_discount));
                    }
                    break;
            }
        }
    }
}
