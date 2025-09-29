package dynagox.experiments.tpch.tpch3;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCH3 extends ProcessFunction<TPCHEvent, String> {
    private final DynaGoxTPCH3 tpch3;

    public ProcFuncTPCH3(double epsilon, boolean print_result, boolean delta_mode) {
        tpch3 = new DynaGoxTPCH3(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpch3.setId(id);
    }

    @Override
    public void processElement(TPCHEvent e, ProcessFunction<TPCHEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCH3CustomerEvent) {
            TPCH3CustomerEvent event = (TPCH3CustomerEvent) e;
            if (event.is_insert) {
                tpch3.insert_into_customer(event.c_custkey);
            } else {
                tpch3.delete_from_customer(event.c_custkey);
            }
        } else if (e instanceof TPCH3OrdersEvent) {
            TPCH3OrdersEvent event = (TPCH3OrdersEvent) e;
            if (event.is_insert) {
                tpch3.insert_into_orders(event.o_orderkey, event.o_custkey, event.o_orderdate,
                        event.o_shippriority);
            } else {
                tpch3.delete_from_orders(event.o_orderkey, event.o_custkey, event.o_orderdate,
                        event.o_shippriority);
            }
        } else if (e instanceof TPCH3LineitemEvent) {
            TPCH3LineitemEvent event = (TPCH3LineitemEvent) e;
            if (event.is_insert) {
                tpch3.insert_into_lineitem(event.l_orderkey, event.l_extendedprice, event.l_discount);
            } else {
                tpch3.delete_from_lineitem(event.l_orderkey, event.l_extendedprice, event.l_discount);
            }
        } else if (e instanceof TPCHEnumEvent) {
            tpch3.milestone();
        }
    }
}
