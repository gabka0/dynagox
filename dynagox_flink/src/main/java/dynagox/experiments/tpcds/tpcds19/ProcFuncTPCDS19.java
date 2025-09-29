package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEnumEvent;
import dynagox.experiments.tpcds.TPCDSEvent;
import dynagox.experiments.tpcds.tpcds15.*;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncTPCDS19 extends ProcessFunction<TPCDSEvent, String> {
    private final DynaGoxTPCDS19 tpcds19;

    public ProcFuncTPCDS19(double epsilon, boolean print_result, boolean delta_mode) {
        tpcds19 = new DynaGoxTPCDS19(epsilon, print_result, delta_mode);
    }

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        tpcds19.setId(id);
    }

    @Override
    public void processElement(TPCDSEvent e, ProcessFunction<TPCDSEvent, String>.Context context, Collector<String> collector) throws Exception {
        if (e instanceof TPCDS19StoreSalesEvent) {
            TPCDS19StoreSalesEvent event = (TPCDS19StoreSalesEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_store_sales(event.ss_sold_date_sk, event.ss_item_sk, event.ss_customer_sk, event.ss_store_sk, event.ss_ext_sales_price);
            }
        } else if (e instanceof TPCDS19StoreEvent) {
            TPCDS19StoreEvent event = (TPCDS19StoreEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_store(event.s_store_sk);
            }
        } else if (e instanceof TPCDS19CustomerEvent) {
            TPCDS19CustomerEvent event = (TPCDS19CustomerEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_customer(event.c_customer_sk, event.c_current_addr_sk);
            }
        } else if (e instanceof TPCDS19CustomerAddressEvent) {
            TPCDS19CustomerAddressEvent event = (TPCDS19CustomerAddressEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_customer_address(event.ca_address_sk);
            }
        } else if (e instanceof TPCDS19DateDimEvent) {
            TPCDS19DateDimEvent event = (TPCDS19DateDimEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_date_dim(event.d_date_sk);
            }
        } else if (e instanceof TPCDS19ItemEvent) {
            TPCDS19ItemEvent event = (TPCDS19ItemEvent) e;
            if (event.is_insert) {
                tpcds19.insert_into_item(event.i_item_sk, event.i_brand_id, event.i_brand, event.i_manufact_id, event.i_manufact);
            }
        } else if (e instanceof TPCDSEnumEvent) {
            tpcds19.milestone();
        }
    }
}
