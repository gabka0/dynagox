package dynagox.experiments.tpch.tpch10;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCH10 extends ProcessFunction<TPCHEvent, String> {
    private final DynaGoxTPCH10 tpch10;

    public ProcFuncTPCH10(double epsilon, boolean print_result, boolean delta_mode) {
        tpch10 = new DynaGoxTPCH10(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpch10.setId(id);
    }

    @Override
    public void processElement(TPCHEvent e, ProcessFunction<TPCHEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCH10CustomerEvent) {
            TPCH10CustomerEvent event = (TPCH10CustomerEvent) e;
            if (event.is_insert) {
                tpch10.insert_into_customer(event.c_custkey, event.c_name, event.c_address, event.c_nationkey,
                        event.c_phone, event.c_acctbal, event.c_comment);
            } else {
                tpch10.delete_from_customer(event.c_custkey, event.c_name, event.c_address, event.c_nationkey,
                        event.c_phone, event.c_acctbal, event.c_comment);
            }
        } else if (e instanceof TPCH10OrdersEvent) {
            TPCH10OrdersEvent event = (TPCH10OrdersEvent) e;
            if (event.is_insert) {
                tpch10.insert_into_orders(event.o_orderkey, event.o_custkey);
            } else {
                tpch10.delete_from_orders(event.o_orderkey, event.o_custkey);
            }
        } else if (e instanceof TPCH10LineitemEvent) {
            TPCH10LineitemEvent event = (TPCH10LineitemEvent) e;
            if (event.is_insert) {
                tpch10.insert_into_lineitem(event.l_orderkey, event.l_extendedprice, event.l_discount);
            } else {
                tpch10.delete_from_lineitem(event.l_orderkey, event.l_extendedprice, event.l_discount);
            }
        } else if (e instanceof TPCH10NationEvent) {
            TPCH10NationEvent event = (TPCH10NationEvent) e;
            if (event.is_insert) {
                tpch10.insert_into_nation(event.n_nationkey, event.n_name);
            }
        } else if (e instanceof TPCHEnumEvent) {
            tpch10.milestone();
        }
    }
}
