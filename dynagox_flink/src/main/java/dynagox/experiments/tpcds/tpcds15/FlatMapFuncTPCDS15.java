package dynagox.experiments.tpcds.tpcds15;

import dynagox.experiments.tpcds.TPCDSEnumEvent;
import dynagox.experiments.tpcds.TPCDSEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncTPCDS15 implements FlatMapFunction<String, TPCDSEvent> {
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
                case 1:
                    int cs_sold_date_sk = Integer.parseInt(fields[1]);
                    int cs_bill_customer_sk = Integer.parseInt(fields[2]);
                    double cs_sales_price = Double.parseDouble(fields[3]);
                    out.collect(new TPCDS15CatalogSalesEvent(cs_bill_customer_sk % 16, true,
                            cs_sold_date_sk, cs_bill_customer_sk, cs_sales_price));
                    break;
                case 3:
                    int c_customer_sk = Integer.parseInt(fields[1]);
                    int c_current_addr_sk = Integer.parseInt(fields[2]);
                    out.collect(new TPCDS15CustomerEvent(c_customer_sk % 16, true, c_customer_sk, c_current_addr_sk));
                    break;
                case 4:
                    int ca_address_sk = Integer.parseInt(fields[1]);
                    String ca_zip = fields[2];
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS15CustomerAddressEvent(key, true, ca_address_sk, ca_zip));
                    }
                    break;
                case 5:
                    int d_date_sk = Integer.parseInt(fields[1]);
                    for (int key = 0; key < 16; key++) {
                        out.collect(new TPCDS15DateDimEvent(key, true, d_date_sk));
                    }
                    break;
            }
        }
    }
}
