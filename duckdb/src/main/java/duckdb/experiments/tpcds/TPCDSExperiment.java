package duckdb.experiments.tpcds;

import duckdb.experiments.Experiment;

import java.sql.*;

public abstract class TPCDSExperiment extends Experiment {
    PreparedStatement preparedInsertIntoStoreSalesStatement;
    PreparedStatement preparedInsertIntoCatalogSalesStatement;
    PreparedStatement preparedInsertIntoStoreStatement;
    PreparedStatement preparedInsertIntoCustomerStatement;
    PreparedStatement preparedInsertIntoCustomerAddressStatement;
    PreparedStatement preparedInsertIntoDateDimStatement;
    PreparedStatement preparedInsertIntoItemStatement;

    public TPCDSExperiment(boolean print) {
        super(print);
    }

    @Override
    public void init() throws SQLException {
        connection = DriverManager.getConnection("jdbc:duckdb:");

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("CREATE TABLE IF NOT EXISTS store_sales (" +
                    "    ss_sold_date_sk     INTEGER NOT NULL," +
                    "    ss_item_sk          INTEGER NOT NULL," +
                    "    ss_customer_sk      INTEGER NOT NULL," +
                    "    ss_store_sk         INTEGER NOT NULL," +
                    "    ss_ext_sales_price  DECIMAL(7,2) NOT NULL)");

            stmt.execute("CREATE TABLE IF NOT EXISTS catalog_sales (" +
                    "    cs_sold_date_sk     INTEGER NOT NULL," +
                    "    cs_bill_customer_sk INTEGER NOT NULL," +
                    "    cs_sales_price      DECIMAL(7,2) NOT NULL)");

            stmt.execute("CREATE TABLE IF NOT EXISTS store (" +
                    "    s_store_sk          INTEGER NOT NULL," +
                    "    PRIMARY KEY (s_store_sk))");

            stmt.execute("CREATE TABLE IF NOT EXISTS customer (" +
                    "    c_customer_sk       INTEGER NOT NULL," +
                    "    c_current_addr_sk   INTEGER NOT NULL," +
                    "    PRIMARY KEY (c_customer_sk))");

            stmt.execute("CREATE TABLE IF NOT EXISTS customer_address (" +
                    "    ca_address_sk       INTEGER NOT NULL," +
                    "    ca_zip              CHAR(10) NOT NULL," +
                    "    PRIMARY KEY (ca_address_sk))");

            stmt.execute("CREATE TABLE IF NOT EXISTS date_dim (" +
                    "    d_date_sk           INTEGER NOT NULL," +
                    "    PRIMARY KEY (d_date_sk))");

            stmt.execute("CREATE TABLE IF NOT EXISTS item (" +
                    "    i_item_sk           INTEGER NOT NULL," +
                    "    i_brand_id          INTEGER NOT NULL," +
                    "    i_brand             CHAR(50) NOT NULL," +
                    "    i_manufact_id       INTEGER NOT NULL," +
                    "    i_manufact          CHAR(50) NOT NULL," +
                    "    PRIMARY KEY (i_item_sk))");
        }

        preparedInsertIntoStoreSalesStatement = connection.prepareStatement(
                "INSERT INTO store_sales (ss_sold_date_sk, ss_item_sk, ss_customer_sk, ss_store_sk, ss_ext_sales_price) " +
                        "VALUES (?, ?, ?, ?, ?)"
        );

        preparedInsertIntoCatalogSalesStatement = connection.prepareStatement(
                "INSERT INTO catalog_sales (cs_sold_date_sk, cs_bill_customer_sk, cs_sales_price) " +
                        "VALUES (?, ?, ?)"
        );

        preparedInsertIntoStoreStatement = connection.prepareStatement(
                "INSERT INTO store (s_store_sk) " +
                        "VALUES (?)"
        );

        preparedInsertIntoCustomerStatement = connection.prepareStatement(
                "INSERT INTO customer (c_customer_sk, c_current_addr_sk) " +
                        "VALUES (?, ?)"
        );

        preparedInsertIntoCustomerAddressStatement = connection.prepareStatement(
                "INSERT INTO customer_address (ca_address_sk, ca_zip) " +
                        "VALUES (?, ?)"
        );

        preparedInsertIntoDateDimStatement = connection.prepareStatement(
                "INSERT INTO date_dim (d_date_sk) " +
                        "VALUES (?)"
        );

        preparedInsertIntoItemStatement = connection.prepareStatement(
                "INSERT INTO item (i_item_sk, i_brand_id, i_brand, i_manufact_id, i_manufact) " +
                        "VALUES (?, ?, ?, ?, ?)"
        );
    }

    @Override
    public void process(String line) throws SQLException {
        String[] fields = line.split("\\|");
        int rid = Integer.parseInt(fields[0]);

        if (rid == 0) { // store_sales
            preparedInsertIntoStoreSalesStatement.setInt(1, Integer.parseInt(fields[1]));                   // ss_sold_date_sk
            preparedInsertIntoStoreSalesStatement.setInt(2, Integer.parseInt(fields[2]));                   // ss_item_sk
            preparedInsertIntoStoreSalesStatement.setInt(3, Integer.parseInt(fields[3]));                   // ss_customer_sk
            preparedInsertIntoStoreSalesStatement.setInt(4, Integer.parseInt(fields[4]));                   // ss_store_sk
            preparedInsertIntoStoreSalesStatement.setBigDecimal(5, new java.math.BigDecimal(fields[5]));    // ss_ext_sales_price
            preparedInsertIntoStoreSalesStatement.executeUpdate();
        } else if (rid == 1) { // catalog_sales
            preparedInsertIntoCatalogSalesStatement.setInt(1, Integer.parseInt(fields[1]));                 // cs_sold_date_sk
            preparedInsertIntoCatalogSalesStatement.setInt(2, Integer.parseInt(fields[2]));                 // cs_bill_customer_sk
            preparedInsertIntoCatalogSalesStatement.setBigDecimal(3, new java.math.BigDecimal(fields[3]));  // cs_sales_price
            preparedInsertIntoCatalogSalesStatement.executeUpdate();
        } else if (rid == 2) { // store
            preparedInsertIntoStoreStatement.setInt(1, Integer.parseInt(fields[1]));                        // s_store_sk
            preparedInsertIntoStoreStatement.executeUpdate();
        } else if (rid == 3) { // customer
            preparedInsertIntoCustomerStatement.setInt(1, Integer.parseInt(fields[1]));                     // c_customer_sk
            preparedInsertIntoCustomerStatement.setInt(2, Integer.parseInt(fields[2]));                     // c_current_addr_sk
            preparedInsertIntoCustomerStatement.executeUpdate();
        } else if (rid == 4) { // customer_address
            preparedInsertIntoCustomerAddressStatement.setInt(1, Integer.parseInt(fields[1]));              // ca_address_sk
            preparedInsertIntoCustomerAddressStatement.setString(2, fields[2]);                             // ca_zip
            preparedInsertIntoCustomerAddressStatement.executeUpdate();
        } else if (rid == 5) { // date_dim
            preparedInsertIntoDateDimStatement.setInt(1, Integer.parseInt(fields[1]));                      // d_date_sk
            preparedInsertIntoDateDimStatement.executeUpdate();
        } else if (rid == 6) { // item
            preparedInsertIntoItemStatement.setInt(1, Integer.parseInt(fields[1]));                         // i_item_sk
            preparedInsertIntoItemStatement.setInt(2, Integer.parseInt(fields[2]));                         // i_brand_id
            preparedInsertIntoItemStatement.setString(3, fields[3]);                                        // i_brand
            preparedInsertIntoItemStatement.setInt(4, Integer.parseInt(fields[4]));                         // i_manufact_id
            preparedInsertIntoItemStatement.setString(5, fields[5]);                                        // i_manufact
            preparedInsertIntoItemStatement.executeUpdate();
        }
    }

    @Override
    public void close() throws SQLException {
        preparedInsertIntoStoreSalesStatement.close();
        preparedInsertIntoCatalogSalesStatement.close();
        preparedInsertIntoStoreStatement.close();
        preparedInsertIntoCustomerStatement.close();
        preparedInsertIntoCustomerAddressStatement.close();
        preparedInsertIntoDateDimStatement.close();
        preparedInsertIntoItemStatement.close();
        connection.close();
    }
}
