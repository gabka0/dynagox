package duckdb.experiments.tpch;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCH9 extends TPCHExperiment {
    public TPCH9(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT\n" +
                "    nation,\n" +
                "    o_year,\n" +
                "    SUM(v1) AS value1,\n" +
                "    SUM(v2) AS value2\n" +
                "FROM (\n" +
                "    SELECT\n" +
                "        n_name AS nation,\n" +
                "        EXTRACT(YEAR FROM o_orderdate) AS o_year,\n" +
                "        l_extendedprice * (1 - l_discount) AS v1,\n" +
                "        ps_supplycost * l_quantity AS v2\n" +
                "    FROM\n" +
                "        PART,\n" +
                "        SUPPLIER,\n" +
                "        LINEITEM,\n" +
                "        PARTSUPP,\n" +
                "        ORDERS,\n" +
                "        NATION\n" +
                "    WHERE s_suppkey = l_suppkey\n" +
                "      AND ps_suppkey = l_suppkey\n" +
                "      AND ps_partkey = l_partkey\n" +
                "      AND p_partkey = l_partkey\n" +
                "      AND o_orderkey = l_orderkey\n" +
                "      AND s_nationkey = n_nationkey\n" +
                "      AND p_name LIKE '%green%'\n" +
                ") AS PROFIT\n" +
                "GROUP BY nation, o_year;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        String nation = resultSet.getString(1);
        long o_year = resultSet.getLong(2);
        double value1 = resultSet.getDouble(3);
        double value2 = resultSet.getDouble(4);
        System.out.println("(" + nation + "," + o_year + ") -> (" + value1 + "," + value2 + ")");
    }
}
