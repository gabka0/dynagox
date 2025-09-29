package duckdb.experiments.tpch;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCH5 extends TPCHExperiment {
    public TPCH5(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT\n" +
                "    n_name,\n" +
                "    SUM(l_extendedprice * (1 - l_discount)) AS revenue\n" +
                "FROM\n" +
                "    CUSTOMER,\n" +
                "    ORDERS,\n" +
                "    LINEITEM,\n" +
                "    SUPPLIER,\n" +
                "    NATION,\n" +
                "    REGION\n" +
                "WHERE c_custkey = o_custkey\n" +
                "AND l_orderkey = o_orderkey\n" +
                "  AND l_suppkey = s_suppkey\n" +
                "  AND c_nationkey = s_nationkey\n" +
                "  AND s_nationkey = n_nationkey\n" +
                "  AND n_regionkey = r_regionkey\n" +
                "  AND r_name = 'ASIA'\n" +
                "  AND o_orderdate >= DATE '1994-01-01'\n" +
                "  AND o_orderdate < DATE '1994-01-01' + INTERVAL '1' YEAR\n" +
                "GROUP BY n_name;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        String n_name = resultSet.getString(1);
        double value = resultSet.getDouble(2);
        System.out.println("(" + n_name + ") -> " + value);
    }
}
