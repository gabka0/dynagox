package dynagox.experiments.tpch.tpch9;

import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.TPCHEvent;
import dynagox.experiments.tpch.tpch8.*;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCH9 extends ProcessFunction<TPCHEvent, String> {
    private final DynaGoxTPCH9 tpch9;

    public ProcFuncTPCH9(double epsilon, boolean print_result, boolean delta_mode) {
        tpch9 = new DynaGoxTPCH9(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpch9.setId(id);
    }

    @Override
    public void processElement(TPCHEvent e, ProcessFunction<TPCHEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCH9PartEvent) {
            TPCH9PartEvent event = (TPCH9PartEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_part(event.p_partkey);
            }
        } else if (e instanceof TPCH9SupplierEvent) {
            TPCH9SupplierEvent event = (TPCH9SupplierEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_supplier(event.s_suppkey, event.s_nationkey);
            }
        } else if (e instanceof TPCH9PartSuppEvent) {
            TPCH9PartSuppEvent event = (TPCH9PartSuppEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_partsupp(event.ps_partkey, event.ps_suppkey, event.ps_supplycost);
            }
        } else if (e instanceof TPCH9OrdersEvent) {
            TPCH9OrdersEvent event = (TPCH9OrdersEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_orders(event.o_orderkey, event.o_year);
            }
        } else if (e instanceof TPCH9LineitemEvent) {
            TPCH9LineitemEvent event = (TPCH9LineitemEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_lineitem(event.l_orderkey, event.l_partkey, event.l_suppkey, event.l_linenumber,
                        event.l_quantity, event.l_extendedprice, event.l_discount);
            }
        } else if (e instanceof TPCH9NationEvent) {
            TPCH9NationEvent event = (TPCH9NationEvent) e;
            if (event.is_insert) {
                tpch9.insert_into_nation(event.n_nationkey, event.n_name);
            }
        } else if (e instanceof TPCHEnumEvent) {
            tpch9.milestone();
        }
    }
}
