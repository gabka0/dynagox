package duckdb.experiments.tpch;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCH8 extends TPCHExperiment {
    public TPCH8(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT\n" +
                "    o_year,\n" +
                "    SUM(CASE\n" +
                "            WHEN nation = 'BRAZIL' THEN volume\n" +
                "            ELSE 0\n" +
                "        END) AS value1,\n" +
                "    SUM(volume) AS value2\n" +
                "FROM (\n" +
                "    SELECT\n" +
                "        EXTRACT(YEAR FROM o_orderdate) AS o_year,\n" +
                "        l_extendedprice * (1 - l_discount) AS volume,\n" +
                "        n2.n_name AS nation\n" +
                "    FROM\n" +
                "        PART,\n" +
                "        SUPPLIER,\n" +
                "        LINEITEM,\n" +
                "        ORDERS,\n" +
                "        CUSTOMER,\n" +
                "        NATION n1,\n" +
                "        NATION n2,\n" +
                "        REGION\n" +
                "    WHERE p_partkey = l_partkey\n" +
                "      AND s_suppkey = l_suppkey\n" +
                "      AND l_orderkey = o_orderkey\n" +
                "      AND o_custkey = c_custkey\n" +
                "      AND c_nationkey = n1.n_nationkey\n" +
                "      AND n1.n_regionkey = r_regionkey\n" +
                "      AND r_name = 'AMERICA'\n" +
                "      AND s_nationkey = n2.n_nationkey\n" +
                "      AND o_orderdate between date '1995-01-01' and date '1996-12-31'\n" +
                "      AND p_type = 'ECONOMY ANODIZED STEEL'\n" +
                ") AS ALL_NATIONS\n" +
                "GROUP BY o_year;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        long o_year = resultSet.getLong(1);
        double value1 = resultSet.getDouble(2);
        double value2 = resultSet.getDouble(3);
        System.out.println("(" + o_year + ") -> (" + value1 + "," + value2 + ")");
    }
}
