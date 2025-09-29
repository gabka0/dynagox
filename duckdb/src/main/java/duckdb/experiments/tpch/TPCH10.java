package duckdb.experiments.tpch;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TPCH10 extends TPCHExperiment {
    public TPCH10(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT\n" +
                "    c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name,\n" +
                "    SUM(l_extendedprice * (1 - l_discount)) AS revenue\n" +
                "FROM\n" +
                "    CUSTOMER,\n" +
                "    ORDERS,\n" +
                "    LINEITEM,\n" +
                "    NATION\n" +
                "WHERE c_custkey = o_custkey\n" +
                "  AND l_orderkey = o_orderkey\n" +
                "  AND o_orderdate >= DATE '1993-10-01'\n" +
                "  AND o_orderdate < DATE '1993-10-01' + INTERVAL '3' MONTH\n" +
                "  AND l_returnflag = 'R'\n" +
                "  AND c_nationkey = n_nationkey\n" +
                "GROUP BY c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name;";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        long c_custkey = resultSet.getLong(1);
        String c_name = resultSet.getString(2);
        String c_address = resultSet.getString(3);
        String c_phone = resultSet.getString(4);
        double c_acctbal = resultSet.getDouble(5);
        String c_comment = resultSet.getString(6);
        String n_name = resultSet.getString(7);
        double revenue = resultSet.getDouble(8);

        String acctbal = String.valueOf(c_acctbal)
                .replaceAll("\\.0+$", "")
                .replaceAll("(\\.\\d*[1-9])0+$", "$1");
        System.out.println("(" + c_custkey + "," + c_name + "," + c_address + "," + c_phone + "," + acctbal
                + "," + c_comment + "," + n_name + ") -> " + revenue);
    }
}
