package dynagox.experiments.tpch.tpch5;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import dynagox.experiments.tpch.tpch3.DynaGoxTPCH3;
import dynagox.experiments.tpch.tpch3.TPCH3CustomerEvent;
import dynagox.experiments.tpch.tpch3.TPCH3LineitemEvent;
import dynagox.experiments.tpch.tpch3.TPCH3OrdersEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCH5 extends ProcessFunction<TPCHEvent, String> {
    private final DynaGoxTPCH5 tpch5;

    public ProcFuncTPCH5(double epsilon, boolean print_result, boolean delta_mode) {
        tpch5 = new DynaGoxTPCH5(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpch5.setId(id);
    }

    @Override
    public void processElement(TPCHEvent e, ProcessFunction<TPCHEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCH5SupplierEvent) {
            TPCH5SupplierEvent event = (TPCH5SupplierEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_supplier(event.s_suppkey, event.s_nationkey);
            }
        } else if (e instanceof TPCH5CustomerEvent) {
            TPCH5CustomerEvent event = (TPCH5CustomerEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_customer(event.c_custkey, event.c_nationkey);
            }
        } else if (e instanceof TPCH5OrdersEvent) {
            TPCH5OrdersEvent event = (TPCH5OrdersEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_orders(event.o_orderkey, event.o_custkey);
            }
        } else if (e instanceof TPCH5LineitemEvent) {
            TPCH5LineitemEvent event = (TPCH5LineitemEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_lineitem(event.l_orderkey, event.l_suppkey, event.l_linenumber,
                        event.l_extendedprice, event.l_discount);
            }
        } else if (e instanceof TPCH5NationEvent) {
            TPCH5NationEvent event = (TPCH5NationEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_nation(event.n_nationkey, event.n_name, event.n_regionkey);
            }
        } else if (e instanceof TPCH5RegionEvent) {
            TPCH5RegionEvent event = (TPCH5RegionEvent) e;
            if (event.is_insert) {
                tpch5.insert_into_region(event.r_regionkey);
            }
        } else if (e instanceof TPCHEnumEvent) {
            tpch5.milestone();
        }
    }
}
