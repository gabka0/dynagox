package duckdb.experiments.tpcds;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCDS19 extends TPCDSExperiment {
    public TPCDS19(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT i_brand_id,\n" +
                "       i_brand,\n" +
                "       i_manufact_id,\n" +
                "       i_manufact,\n" +
                "       SUM(ss_ext_sales_price)\n" +
                "FROM store_sales,\n" +
                "     store,\n" +
                "     customer,\n" +
                "     customer_address,\n" +
                "     date_dim,\n" +
                "     item\n" +
                "WHERE d_date_sk = ss_sold_date_sk\n" +
                "  AND ss_item_sk = i_item_sk\n" +
                "  AND ss_customer_sk = c_customer_sk\n" +
                "  AND c_current_addr_sk = ca_address_sk\n" +
                "  AND ss_store_sk = s_store_sk\n" +
                "GROUP BY i_brand_id, i_brand, i_manufact_id, i_manufact;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        int i_brand_id = resultSet.getInt(1);
        String i_brand = resultSet.getString(2);
        int i_manufact_id = resultSet.getInt(3);
        String i_manufact = resultSet.getString(4);
        double value = resultSet.getDouble(5);
        System.out.println("(" + i_brand_id + "," + i_brand + "," + i_manufact_id + "," + i_manufact + ") -> " + value);
    }
}
