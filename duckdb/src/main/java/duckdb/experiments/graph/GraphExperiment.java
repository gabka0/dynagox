package duckdb.experiments.graph;

import duckdb.experiments.Experiment;

import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

public abstract class GraphExperiment extends Experiment {
    PreparedStatement preparedInsertStatement;
    PreparedStatement preparedDeleteStatement;

    public GraphExperiment(boolean print) {
        super(print);
    }

    @Override
    public void init() throws SQLException {
        connection = DriverManager.getConnection("jdbc:duckdb:");

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("CREATE TABLE IF NOT EXISTS GRAPH (src BIGINT, dst BIGINT)");
        }

        preparedInsertStatement = connection.prepareStatement("INSERT INTO GRAPH (src, dst) VALUES (?, ?)");
        preparedDeleteStatement = connection.prepareStatement("DELETE FROM GRAPH WHERE src = ? AND dst = ?");
    }

    @Override
    public void process(String line) throws SQLException {
        String[] fields = line.split("\\|");
        boolean isInsert = fields[0].equals("+");
        long src = Long.parseLong(fields[1]);
        long dst = Long.parseLong(fields[2]);

        if (isInsert) {
            preparedInsertStatement.setLong(1, src);
            preparedInsertStatement.setLong(2, dst);
            preparedInsertStatement.executeUpdate();
        } else {
            preparedDeleteStatement.setLong(1, src);
            preparedDeleteStatement.setLong(2, dst);
            preparedDeleteStatement.executeUpdate();
        }
    }

    @Override
    public void close() throws SQLException {
        preparedInsertStatement.close();
        preparedDeleteStatement.close();
        connection.close();
    }
}
