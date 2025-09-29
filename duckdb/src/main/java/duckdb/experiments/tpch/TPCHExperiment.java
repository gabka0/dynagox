package duckdb.experiments.tpch;

import duckdb.experiments.Experiment;

import java.math.BigDecimal;
import java.sql.*;

public abstract class TPCHExperiment extends Experiment {
    PreparedStatement preparedInsertIntoPartStatement;
    PreparedStatement preparedDeleteFromPartStatement;
    PreparedStatement preparedInsertIntoSupplierStatement;
    PreparedStatement preparedDeleteFromSupplierStatement;
    PreparedStatement preparedInsertIntoPartsuppStatement;
    PreparedStatement preparedDeleteFromPartsuppStatement;
    PreparedStatement preparedInsertIntoCustomerStatement;
    PreparedStatement preparedDeleteFromCustomerStatement;
    PreparedStatement preparedInsertIntoOrdersStatement;
    PreparedStatement preparedDeleteFromOrdersStatement;
    PreparedStatement preparedInsertIntoLineitemStatement;
    PreparedStatement preparedDeleteFromLineitemStatement;
    PreparedStatement preparedInsertIntoNationStatement;
    PreparedStatement preparedDeleteFromNationStatement;
    PreparedStatement preparedInsertIntoRegionStatement;
    PreparedStatement preparedDeleteFromRegionStatement;

    public TPCHExperiment(boolean print) {
        super(print);
    }

    @Override
    public void init() throws SQLException {
        connection = DriverManager.getConnection("jdbc:duckdb:");

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("CREATE TABLE IF NOT EXISTS PART (" +
                    "    p_partkey           INTEGER NOT NULL," +
                    "    p_name              VARCHAR(55) NOT NULL," +
                    "    p_mfgr              CHAR(25) NOT NULL," +
                    "    p_brand             CHAR(10) NOT NULL," +
                    "    p_type              VARCHAR(25) NOT NULL," +
                    "    p_size              INTEGER NOT NULL," +
                    "    p_container         CHAR(10) NOT NULL," +
                    "    p_retailprice       DECIMAL(15,2) NOT NULL," +
                    "    p_comment           VARCHAR(23) NOT NULL," +
                    "    PRIMARY KEY (p_partkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS SUPPLIER (" +
                    "    s_suppkey           INTEGER NOT NULL," +
                    "    s_name              CHAR(25) NOT NULL," +
                    "    s_address           VARCHAR(40) NOT NULL," +
                    "    s_nationkey         INTEGER NOT NULL," +
                    "    s_phone             CHAR(15) NOT NULL," +
                    "    s_acctbal           DECIMAL(15,2) NOT NULL," +
                    "    s_comment           VARCHAR(101) NOT NULL," +
                    "    PRIMARY KEY (s_suppkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS PARTSUPP (" +
                    "    ps_partkey          INTEGER NOT NULL," +
                    "    ps_suppkey          INTEGER NOT NULL," +
                    "    ps_availqty         INTEGER NOT NULL," +
                    "    ps_supplycost       DECIMAL(15,2)  NOT NULL," +
                    "    ps_comment          VARCHAR(199) NOT NULL," +
                    "    PRIMARY KEY (ps_partkey, ps_suppkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS CUSTOMER (" +
                    "    c_custkey           INTEGER NOT NULL," +
                    "    c_name              VARCHAR(25) NOT NULL," +
                    "    c_address           VARCHAR(40) NOT NULL," +
                    "    c_nationkey         INTEGER NOT NULL," +
                    "    c_phone             CHAR(15) NOT NULL," +
                    "    c_acctbal           DECIMAL(15,2)   NOT NULL," +
                    "    c_mktsegment        CHAR(10) NOT NULL," +
                    "    c_comment           VARCHAR(117) NOT NULL," +
                    "    PRIMARY KEY (c_custkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS ORDERS (" +
                    "    o_orderkey          INTEGER NOT NULL," +
                    "    o_custkey           INTEGER NOT NULL," +
                    "    o_orderstatus       CHAR(1) NOT NULL," +
                    "    o_totalprice        DECIMAL(15,2) NOT NULL," +
                    "    o_orderdate         DATE NOT NULL," +
                    "    o_orderpriority     CHAR(15) NOT NULL," +
                    "    o_clerk             CHAR(15) NOT NULL," +
                    "    o_shippriority      INTEGER NOT NULL," +
                    "    o_comment           VARCHAR(79) NOT NULL," +
                    "    PRIMARY KEY (o_orderkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS LINEITEM (" +
                    "    l_orderkey          INTEGER NOT NULL," +
                    "    l_partkey           INTEGER NOT NULL," +
                    "    l_suppkey           INTEGER NOT NULL," +
                    "    l_linenumber        INTEGER NOT NULL," +
                    "    l_quantity          DECIMAL(15,2) NOT NULL," +
                    "    l_extendedprice     DECIMAL(15,2) NOT NULL," +
                    "    l_discount          DECIMAL(15,2) NOT NULL," +
                    "    l_tax               DECIMAL(15,2) NOT NULL," +
                    "    l_returnflag        CHAR(1) NOT NULL," +
                    "    l_linestatus        CHAR(1) NOT NULL," +
                    "    l_shipdate          DATE NOT NULL," +
                    "    l_commitdate        DATE NOT NULL," +
                    "    l_receiptdate       DATE NOT NULL," +
                    "    l_shipinstruct      CHAR(25) NOT NULL," +
                    "    l_shipmode          CHAR(10) NOT NULL," +
                    "    l_comment           VARCHAR(44) NOT NULL," +
                    "    PRIMARY KEY (l_orderkey, l_linenumber))");

            stmt.execute("CREATE TABLE IF NOT EXISTS NATION (" +
                    "    n_nationkey         INTEGER NOT NULL," +
                    "    n_name              CHAR(25) NOT NULL," +
                    "    n_regionkey         INTEGER NOT NULL," +
                    "    n_comment           VARCHAR(152)," +
                    "    PRIMARY KEY (n_nationkey))");

            stmt.execute("CREATE TABLE IF NOT EXISTS REGION (" +
                    "    r_regionkey         INTEGER NOT NULL," +
                    "    r_name              CHAR(25) NOT NULL," +
                    "    r_comment           VARCHAR(152)," +
                    "    PRIMARY KEY (r_regionkey))");
        }

        preparedInsertIntoPartStatement = connection.prepareStatement(
                "INSERT INTO part (p_partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
        );
        preparedDeleteFromPartStatement = connection.prepareStatement(
                "DELETE FROM part WHERE p_partkey = ?"
        );

        preparedInsertIntoSupplierStatement = connection.prepareStatement(
                "INSERT INTO supplier (s_suppkey, s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?)"
        );
        preparedDeleteFromSupplierStatement = connection.prepareStatement(
                "DELETE FROM supplier WHERE s_suppkey = ?"
        );

        preparedInsertIntoPartsuppStatement = connection.prepareStatement(
                "INSERT INTO partsupp (ps_partkey, ps_suppkey, ps_availqty, ps_supplycost, ps_comment) " +
                        "VALUES (?, ?, ?, ?, ?)"
        );
        preparedDeleteFromPartsuppStatement = connection.prepareStatement(
                "DELETE FROM partsupp WHERE ps_partkey = ? AND ps_suppkey = ?"
        );

        preparedInsertIntoCustomerStatement = connection.prepareStatement(
                "INSERT INTO customer (c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
        );
        preparedDeleteFromCustomerStatement = connection.prepareStatement(
                "DELETE FROM customer WHERE c_custkey = ?"
        );

        preparedInsertIntoOrdersStatement = connection.prepareStatement(
                "INSERT INTO orders (o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
        );
        preparedDeleteFromOrdersStatement = connection.prepareStatement(
                "DELETE FROM orders WHERE o_orderkey = ?"
        );

        preparedInsertIntoLineitemStatement = connection.prepareStatement(
                "INSERT INTO lineitem (l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) " +
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
        );
        preparedDeleteFromLineitemStatement = connection.prepareStatement(
                "DELETE FROM lineitem WHERE l_orderkey = ? AND l_linenumber = ?"
        );

        preparedInsertIntoNationStatement = connection.prepareStatement(
                "INSERT INTO nation (n_nationkey, n_name, n_regionkey, n_comment) " +
                        "VALUES (?, ?, ?, ?)"
        );
        preparedDeleteFromNationStatement = connection.prepareStatement(
                "DELETE FROM nation WHERE n_nationkey = ?"
        );

        preparedInsertIntoRegionStatement = connection.prepareStatement(
                "INSERT INTO region (r_regionkey, r_name, r_comment) " +
                        "VALUES (?, ?, ?)"
        );
        preparedDeleteFromRegionStatement = connection.prepareStatement(
                "DELETE FROM region WHERE r_regionkey = ?"
        );
    }

    @Override
    public void process(String line) throws SQLException {
        String[] fields = line.split("\\|");
        boolean isInsert = fields[0].equals("+");
        int rid = Integer.parseInt(fields[1]);

        if (rid == 0) {
            if (isInsert) {
                preparedInsertIntoPartStatement.setLong(1, Long.parseLong(fields[2]));          // p_partkey
                preparedInsertIntoPartStatement.setString(2, fields[3]);                        // p_name
                preparedInsertIntoPartStatement.setString(3, fields[4]);                        // p_mfgr
                preparedInsertIntoPartStatement.setString(4, fields[5]);                        // p_brand
                preparedInsertIntoPartStatement.setString(5, fields[6]);                        // p_type
                preparedInsertIntoPartStatement.setInt(6, Integer.parseInt(fields[7]));         // p_size
                preparedInsertIntoPartStatement.setString(7, fields[8]);                        // p_container
                preparedInsertIntoPartStatement.setBigDecimal(8, new BigDecimal(fields[9]));    // p_retailprice
                preparedInsertIntoPartStatement.setString(9, fields[10]);                       // p_comment
                preparedInsertIntoPartStatement.executeUpdate();
            } else {
                preparedDeleteFromPartStatement.setLong(1, Long.parseLong(fields[2]));          // p_partkey
                preparedDeleteFromPartStatement.executeUpdate();
            }
        } else if (rid == 1) {
            if (isInsert) {
                preparedInsertIntoSupplierStatement.setLong(1, Long.parseLong(fields[2]));      // s_suppkey
                preparedInsertIntoSupplierStatement.setString(2, fields[3]);                    // s_name
                preparedInsertIntoSupplierStatement.setString(3, fields[4]);                    // s_address
                preparedInsertIntoSupplierStatement.setLong(4, Long.parseLong(fields[5]));      // s_nationkey
                preparedInsertIntoSupplierStatement.setString(5, fields[6]);                    // s_phone
                preparedInsertIntoSupplierStatement.setBigDecimal(6, new BigDecimal(fields[7]));// s_acctbal
                preparedInsertIntoSupplierStatement.setString(7, fields[8]);                    // s_comment
                preparedInsertIntoSupplierStatement.executeUpdate();
            } else {
                preparedDeleteFromSupplierStatement.setLong(1, Long.parseLong(fields[2]));      // s_suppkey
                preparedDeleteFromSupplierStatement.executeUpdate();
            }
        } else if (rid == 2) {
            if (isInsert) {
                preparedInsertIntoPartsuppStatement.setLong(1, Long.parseLong(fields[2]));      // ps_partkey
                preparedInsertIntoPartsuppStatement.setLong(2, Long.parseLong(fields[3]));      // ps_suppkey
                preparedInsertIntoPartsuppStatement.setInt(3, Integer.parseInt(fields[4]));     // ps_availqty
                preparedInsertIntoPartsuppStatement.setBigDecimal(4, new BigDecimal(fields[5]));// ps_supplycost
                preparedInsertIntoPartsuppStatement.setString(5, fields[6]);                    // ps_comment
                preparedInsertIntoPartsuppStatement.executeUpdate();
            } else {
                preparedDeleteFromPartsuppStatement.setLong(1, Long.parseLong(fields[2]));      // ps_partkey
                preparedDeleteFromPartsuppStatement.setLong(2, Long.parseLong(fields[3]));      // ps_suppkey
                preparedDeleteFromPartsuppStatement.executeUpdate();
            }
        } else if (rid == 3) {
            if (isInsert) {
                preparedInsertIntoCustomerStatement.setLong(1, Long.parseLong(fields[2]));      // c_custkey
                preparedInsertIntoCustomerStatement.setString(2, fields[3]);                    // c_name
                preparedInsertIntoCustomerStatement.setString(3, fields[4]);                    // c_address
                preparedInsertIntoCustomerStatement.setLong(4, Long.parseLong(fields[5]));      // c_nationkey
                preparedInsertIntoCustomerStatement.setString(5, fields[6]);                    // c_phone
                preparedInsertIntoCustomerStatement.setBigDecimal(6, new BigDecimal(fields[7]));// c_acctbal
                preparedInsertIntoCustomerStatement.setString(7, fields[8]);                    // c_mktsegment
                preparedInsertIntoCustomerStatement.setString(8, fields[9]);                    // c_comment
                preparedInsertIntoCustomerStatement.executeUpdate();
            } else {
                preparedDeleteFromCustomerStatement.setLong(1, Long.parseLong(fields[2]));      // c_custkey
                preparedDeleteFromCustomerStatement.executeUpdate();
            }
        } else if (rid == 4) {
            if (isInsert) {
                preparedInsertIntoOrdersStatement.setLong(1, Long.parseLong(fields[2]));        // o_orderkey
                preparedInsertIntoOrdersStatement.setLong(2, Long.parseLong(fields[3]));        // o_custkey
                preparedInsertIntoOrdersStatement.setString(3, fields[4]);                      // o_orderstatus
                preparedInsertIntoOrdersStatement.setBigDecimal(4, new BigDecimal(fields[5]));  // o_totalprice
                preparedInsertIntoOrdersStatement.setDate(5, Date.valueOf(fields[6]));          // o_orderdate
                preparedInsertIntoOrdersStatement.setString(6, fields[7]);                      // o_orderpriority
                preparedInsertIntoOrdersStatement.setString(7, fields[8]);                      // o_clerk
                preparedInsertIntoOrdersStatement.setInt(8, Integer.parseInt(fields[9]));       // o_shippriority
                preparedInsertIntoOrdersStatement.setString(9, fields[10]);                     // o_comment
                preparedInsertIntoOrdersStatement.executeUpdate();
            } else {
                preparedDeleteFromOrdersStatement.setLong(1, Long.parseLong(fields[2]));        // o_orderkey
                preparedDeleteFromOrdersStatement.executeUpdate();
            }
        } else if (rid == 5) {
            if (isInsert) {
                preparedInsertIntoLineitemStatement.setLong(1, Long.parseLong(fields[2]));      // l_orderkey
                preparedInsertIntoLineitemStatement.setLong(2, Long.parseLong(fields[3]));      // l_partkey
                preparedInsertIntoLineitemStatement.setLong(3, Long.parseLong(fields[4]));      // l_suppkey
                preparedInsertIntoLineitemStatement.setInt(4, Integer.parseInt(fields[5]));     // l_linenumber
                preparedInsertIntoLineitemStatement.setBigDecimal(5, new BigDecimal(fields[6]));// l_quantity
                preparedInsertIntoLineitemStatement.setBigDecimal(6, new BigDecimal(fields[7]));// l_extendedprice
                preparedInsertIntoLineitemStatement.setBigDecimal(7, new BigDecimal(fields[8]));// l_discount
                preparedInsertIntoLineitemStatement.setBigDecimal(8, new BigDecimal(fields[9]));// l_tax
                preparedInsertIntoLineitemStatement.setString(9, fields[10]);                   // l_returnflag
                preparedInsertIntoLineitemStatement.setString(10, fields[11]);                  // l_linestatus
                preparedInsertIntoLineitemStatement.setDate(11, Date.valueOf(fields[12]));      // l_shipdate
                preparedInsertIntoLineitemStatement.setDate(12, Date.valueOf(fields[13]));      // l_commitdate
                preparedInsertIntoLineitemStatement.setDate(13, Date.valueOf(fields[14]));      // l_receiptdate
                preparedInsertIntoLineitemStatement.setString(14, fields[15]);                  // l_shipinstruct
                preparedInsertIntoLineitemStatement.setString(15, fields[16]);                  // l_shipmode
                preparedInsertIntoLineitemStatement.setString(16, fields[17]);                  // l_comment
                preparedInsertIntoLineitemStatement.executeUpdate();
            } else {
                preparedDeleteFromLineitemStatement.setLong(1, Long.parseLong(fields[2]));      // l_orderkey
                preparedDeleteFromLineitemStatement.setInt(2, Integer.parseInt(fields[5]));     // l_linenumber
                preparedDeleteFromLineitemStatement.executeUpdate();
            }
        } else if (rid == 6) {
            if (isInsert) {
                preparedInsertIntoNationStatement.setLong(1, Long.parseLong(fields[2]));        // n_nationkey
                preparedInsertIntoNationStatement.setString(2, fields[3]);                      // n_name
                preparedInsertIntoNationStatement.setLong(3, Long.parseLong(fields[4]));        // n_regionkey
                preparedInsertIntoNationStatement.setString(4, fields[5]);                      // n_comment
                preparedInsertIntoNationStatement.executeUpdate();
            } else {
                preparedDeleteFromNationStatement.setLong(1, Long.parseLong(fields[2]));        // n_nationkey
                preparedDeleteFromNationStatement.executeUpdate();
            }
        } else if (rid == 7) {
            if (isInsert) {
                preparedInsertIntoRegionStatement.setLong(1, Long.parseLong(fields[2]));        // r_regionkey
                preparedInsertIntoRegionStatement.setString(2, fields[3]);                      // r_name
                preparedInsertIntoRegionStatement.setString(3, fields[4]);                      // r_comment
                preparedInsertIntoRegionStatement.executeUpdate();
            } else {
                preparedDeleteFromRegionStatement.setLong(1, Long.parseLong(fields[2]));        // r_regionkey
                preparedDeleteFromRegionStatement.executeUpdate();
            }
        }
    }

    @Override
    public void close() throws SQLException {
        preparedInsertIntoPartStatement.close();
        preparedDeleteFromPartStatement.close();
        preparedInsertIntoSupplierStatement.close();
        preparedDeleteFromSupplierStatement.close();
        preparedInsertIntoPartsuppStatement.close();
        preparedDeleteFromPartsuppStatement.close();
        preparedInsertIntoCustomerStatement.close();
        preparedDeleteFromCustomerStatement.close();
        preparedInsertIntoOrdersStatement.close();
        preparedDeleteFromOrdersStatement.close();
        preparedInsertIntoLineitemStatement.close();
        preparedDeleteFromLineitemStatement.close();
        preparedInsertIntoNationStatement.close();
        preparedDeleteFromNationStatement.close();
        preparedInsertIntoRegionStatement.close();
        preparedDeleteFromRegionStatement.close();
        connection.close();
    }
}
