package duckdb.experiments.graph;

import java.sql.ResultSet;
import java.sql.SQLException;

public class Line5c extends GraphExperiment {
    public Line5c(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, " +
                "GRAPH AS g5 WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        System.out.println(resultSet.getLong(1));
    }
}
