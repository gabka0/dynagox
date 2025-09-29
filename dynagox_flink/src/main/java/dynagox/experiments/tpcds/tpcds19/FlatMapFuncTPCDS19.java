package dynagox.experiments.tpcds.tpcds19;

import dynagox.experiments.tpcds.TPCDSEnumEvent;
import dynagox.experiments.tpcds.TPCDSEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCDS19 implements FlatMapFunction<String, TPCDSEvent> {
    @Override
    public void flatMap(String value, Collector<TPCDSEvent> out) throws Exception {
        if (value.equals("*")) {
            for (int key = 0; key < 16; key++) {
                out.collect(new TPCDSEnumEvent(key, true));
            }
        } else {
            String[] fields = value.split("\\|");
            int rid = Integer.parseInt(fields[0]);
            switch (rid) {
                case 0:
                    int ss_sold_date_sk = Integer.parseInt(fields[1]);
                    int ss_item_sk = Integer.parseInt(fields[2]);
                    int ss_customer_sk = Integer.parseInt(fields[3]);
                    int ss_store_sk = Integer.parseInt(fields[4]);
                    double ss_ext_sales_price = Double.parseDouble(fields[5]);
                    out.collect(new TPCDS19StoreSalesEvent(ss_sold_date_sk % 16, true,
                            ss_sold_date_sk, ss_item_sk, ss_customer_sk, ss_store_sk, ss_ext_sales_price));
                    break;
                case 2:
                    int s_store_sk = Integer.parseInt(fields[1]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS19StoreEvent(key, true, s_store_sk));
                    }
                    break;
                case 3:
                    int c_customer_sk = Integer.parseInt(fields[1]);
                    int c_current_addr_sk = Integer.parseInt(fields[2]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS19CustomerEvent(key, true, c_customer_sk, c_current_addr_sk));
                    }
                    break;
                case 4:
                    int ca_address_sk = Integer.parseInt(fields[1]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS19CustomerAddressEvent(key, true, ca_address_sk));
                    }
                    break;
                case 5:
                    int d_date_sk = Integer.parseInt(fields[1]);
                    out.collect(new TPCDS19DateDimEvent(d_date_sk % 16, true, d_date_sk));
                    break;
                case 6:
                    int i_item_sk = Integer.parseInt(fields[1]);
                    int i_brand_id = Integer.parseInt(fields[2]);
                    String i_brand = fields[3];
                    int i_manufact_id = Integer.parseInt(fields[4]);
                    String i_manufact = fields[5];
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS19ItemEvent(key, true, i_item_sk, i_brand_id, i_brand,
                                i_manufact_id, i_manufact));
                    }
                    break;
            }
        }
    }
}
