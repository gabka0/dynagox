package duckdb.experiments;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;

public abstract class Experiment {
    public int K = 100;
    public Connection connection;
    private final boolean print;
    private int progress = 0;
    private long t0 = 0;

    public Experiment(boolean print) {
        this.print = print;
    }

    public abstract void init() throws SQLException;

    public abstract void process(String line) throws SQLException;

    public abstract String getQuery();

    public abstract void printResultSet(ResultSet resultSet) throws SQLException;

    public void milestone() throws SQLException {
        progress += 1;
        System.out.println("Progress: " + progress + "/" + K);

        long t1 = System.currentTimeMillis();
        System.out.println("Running Time: " + (t1 - t0) + " ms");
        System.out.println("Memory Usage: " + Utils.getMemoryUsage() + " kB");
        System.out.println("");

        Statement statement = connection.createStatement();
        try (ResultSet resultSet = statement.executeQuery(getQuery())) {
            while (resultSet.next()) {
                if (print) {
                    printResultSet(resultSet);
                }
            }
        }
        statement.close();
    };

    public void run(String file) throws IOException, SQLException {
        List<String> lines = Files.readAllLines(Paths.get(file));
        int size = lines.size();
        int step = size / K + 1;
        int next = step;
        int current = 0;
        t0 = System.currentTimeMillis();

        while (current < size) {
            String line = lines.get(current);
            if (!line.equals("*")) {
                process(line);
            }
            if (current >= next) {
                milestone();
                next += step;
            }
            current += 1;

            if (current % 100000 == 0) {
                System.out.println("Processed " + current + " lines of input.");
            }
        }
        milestone();
    }

    public abstract void close() throws SQLException;
}
