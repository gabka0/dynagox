package duckdb.experiments.graph;

import java.sql.ResultSet;
import java.sql.SQLException;

public class Dumbbell extends GraphExperiment {
    public Dumbbell(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5, GRAPH AS g6, GRAPH AS g7 " +
                "WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g1.src AND g3.src = g4.src " +
                "AND g4.dst = g5.src AND g5.dst = g6.src AND g6.dst = g7.src AND g7.dst = g5.src";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        System.out.println(resultSet.getLong(1));
    }
}
