package dynagox.experiments.tpch.tpch8;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import dynagox.experiments.tpch.tpch5.*;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCH8 extends ProcessFunction<TPCHEvent, String> {
    private final DynaGoxTPCH8 tpch8;

    public ProcFuncTPCH8(double epsilon, boolean print_result, boolean delta_mode) {
        tpch8 = new DynaGoxTPCH8(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpch8.setId(id);
    }

    @Override
    public void processElement(TPCHEvent e, ProcessFunction<TPCHEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCH8PartEvent) {
            TPCH8PartEvent event = (TPCH8PartEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_part(event.p_partkey);
            }
        } else if (e instanceof TPCH8SupplierEvent) {
            TPCH8SupplierEvent event = (TPCH8SupplierEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_supplier(event.s_suppkey, event.s_nationkey);
            }
        } else if (e instanceof TPCH8CustomerEvent) {
            TPCH8CustomerEvent event = (TPCH8CustomerEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_customer(event.c_custkey, event.c_nationkey);
            }
        } else if (e instanceof TPCH8OrdersEvent) {
            TPCH8OrdersEvent event = (TPCH8OrdersEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_orders(event.o_orderkey, event.o_custkey, event.o_year);
            }
        } else if (e instanceof TPCH8LineitemEvent) {
            TPCH8LineitemEvent event = (TPCH8LineitemEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_lineitem(event.l_orderkey, event.l_partkey, event.l_suppkey, event.l_linenumber,
                        event.l_extendedprice, event.l_discount);
            }
        } else if (e instanceof TPCH8NationEvent) {
            TPCH8NationEvent event = (TPCH8NationEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_nation1(event.n_nationkey, event.n_regionkey);
                tpch8.insert_into_nation2(event.n_nationkey, event.n_name);
            }
        } else if (e instanceof TPCH8RegionEvent) {
            TPCH8RegionEvent event = (TPCH8RegionEvent) e;
            if (event.is_insert) {
                tpch8.insert_into_region(event.r_regionkey);
            }
        } else if (e instanceof TPCHEnumEvent) {
            tpch8.milestone();
        }
    }
}
