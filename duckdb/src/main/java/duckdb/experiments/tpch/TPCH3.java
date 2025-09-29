package duckdb.experiments.tpch;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCH3 extends TPCHExperiment {
    public TPCH3(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT\n" +
                "    o_orderkey,\n" +
                "    o_orderdate,\n" +
                "    o_shippriority,\n" +
                "    sum(l_extendedprice * (1 - l_discount)) as revenue\n" +
                "FROM CUSTOMER, ORDERS, LINEITEM\n" +
                "WHERE c_mktsegment = 'BUILDING'\n" +
                "AND c_custkey = o_custkey\n" +
                "AND l_orderkey = o_orderkey\n" +
                "AND o_orderdate < DATE '1995-03-15'\n" +
                "AND l_shipdate > DATE '1995-03-15'\n" +
                "GROUP BY o_orderkey, o_orderdate, o_shippriority;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        int o_orderkey = resultSet.getInt(1);
        String o_orderdate = resultSet.getDate(2).toString().replaceAll("-", "");
        int o_shippriority = resultSet.getInt(3);
        double value = resultSet.getDouble(4);
        System.out.println("(" + o_orderkey + "," + o_orderdate + "," + o_shippriority + ") -> " + value);
    }
}
