package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.tpcds.TPCDSEnumEvent;
import dynagox.experiments.tpcds.TPCDSEvent;
import dynagox.experiments.tpch.TPCHEnumEvent;
import dynagox.experiments.tpch.tpch3.DynaGoxTPCH3;
import dynagox.experiments.tpch.tpch3.TPCH3CustomerEvent;
import dynagox.experiments.tpch.tpch3.TPCH3LineitemEvent;
import dynagox.experiments.tpch.tpch3.TPCH3OrdersEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCDS15 extends ProcessFunction<TPCDSEvent, String> {
    private final DynaGoxTPCDS15 tpcds15;

    public ProcFuncTPCDS15(double epsilon, boolean print_result, boolean delta_mode) {
        tpcds15 = new DynaGoxTPCDS15(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpcds15.setId(id);
    }

    @Override
    public void processElement(TPCDSEvent e, ProcessFunction<TPCDSEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCDS15CatalogSalesEvent) {
            TPCDS15CatalogSalesEvent event = (TPCDS15CatalogSalesEvent) e;
            if (event.is_insert) {
                tpcds15.insert_into_catalog_sales(event.cs_sold_date_sk, event.cs_bill_customer_sk, event.cs_sales_price);
            }
        } else if (e instanceof TPCDS15CustomerEvent) {
            TPCDS15CustomerEvent event = (TPCDS15CustomerEvent) e;
            if (event.is_insert) {
                tpcds15.insert_into_customer(event.c_customer_sk, event.c_current_addr_sk);
            }
        } else if (e instanceof TPCDS15CustomerAddressEvent) {
            TPCDS15CustomerAddressEvent event = (TPCDS15CustomerAddressEvent) e;
            if (event.is_insert) {
                tpcds15.insert_into_customer_address(event.ca_address_sk, event.ca_zip);
            }
        } else if (e instanceof TPCDS15DateDimEvent) {
            TPCDS15DateDimEvent event = (TPCDS15DateDimEvent) e;
            if (event.is_insert) {
                tpcds15.insert_into_date_dim(event.d_date_sk);
            }
        } else if (e instanceof TPCDSEnumEvent) {
            tpcds15.milestone();
        }
    }
}
