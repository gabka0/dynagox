package duckdb.experiments.graph;

import java.sql.ResultSet;
import java.sql.SQLException;

public class Line5gc extends GraphExperiment {
    public Line5gc(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT g3.src, g3.dst, COUNT(*) FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, " +
                "GRAPH AS g5 WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src " +
                "GROUP BY g3.src, g3.dst";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        int src = resultSet.getInt(1);
        int dst = resultSet.getInt(2);
        long value = resultSet.getLong(3);
        System.out.println("(" + src + "," + dst + ") -> " + value);
    }
}
