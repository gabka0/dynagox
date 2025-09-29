package duckdb.experiments.tpcds;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCDS15 extends TPCDSExperiment {
    public TPCDS15(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT ca_zip, SUM(cs_sales_price)\n" +
                "FROM catalog_sales,\n" +
                "     customer,\n" +
                "     customer_address,\n" +
                "     date_dim\n" +
                "WHERE cs_bill_customer_sk = c_customer_sk\n" +
                "  AND c_current_addr_sk = ca_address_sk\n" +
                "  AND cs_sold_date_sk = d_date_sk\n" +
                "GROUP BY ca_zip;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        String ca_zip = resultSet.getString(1);
        double value = resultSet.getDouble(2);
        System.out.println("(" + ca_zip + ") -> " + value);
    }
}
