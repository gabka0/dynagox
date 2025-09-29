package duckdb.experiments.lsqb;

import duckdb.experiments.Experiment;

import java.sql.*;

public class LSQB6 extends Experiment {
    PreparedStatement preparedInsertIntoPersonKnowsPersonStatement;
    PreparedStatement preparedInsertIntoPersonHasInterestTagStatement;

    public LSQB6(boolean print) {
        super(print);
    }

    @Override
    public void init() throws SQLException {
        connection = DriverManager.getConnection("jdbc:duckdb:");

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("CREATE TABLE IF NOT EXISTS PERSON_KNOWS_PERSON (person1_id BIGINT, person2_id BIGINT)");
            stmt.execute("CREATE TABLE IF NOT EXISTS PERSON_HAS_INTEREST_TAG (person_id BIGINT, tag_id BIGINT)");
        }

        preparedInsertIntoPersonKnowsPersonStatement =
                connection.prepareStatement("INSERT INTO PERSON_KNOWS_PERSON (person1_id, person2_id) VALUES (?, ?)");
        preparedInsertIntoPersonHasInterestTagStatement =
                connection.prepareStatement("INSERT INTO PERSON_HAS_INTEREST_TAG (person_id, tag_id) VALUES (?, ?)");
    }

    @Override
    public void process(String line) throws SQLException {
        String[] fields = line.split("\\|");
        int rid = Integer.parseInt(fields[0]);
        long f0 = Long.parseLong(fields[1]);
        long f1 = Long.parseLong(fields[2]);

        if (rid == 0) {
            preparedInsertIntoPersonKnowsPersonStatement.setLong(1, f0);
            preparedInsertIntoPersonKnowsPersonStatement.setLong(2, f1);
            preparedInsertIntoPersonKnowsPersonStatement.executeUpdate();
        } else if (rid == 1) {
            preparedInsertIntoPersonHasInterestTagStatement.setLong(1, f0);
            preparedInsertIntoPersonHasInterestTagStatement.setLong(2, f1);
            preparedInsertIntoPersonHasInterestTagStatement.executeUpdate();
        }
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM PERSON_KNOWS_PERSON AS pkp1, PERSON_KNOWS_PERSON AS pkp2, PERSON_HAS_INTEREST_TAG AS phit " +
                "WHERE pkp1.person2_id = pkp2.person1_id AND pkp2.person2_id = phit.person_id";
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        System.out.println(resultSet.getLong(1));
    }

    @Override
    public void close() throws SQLException {
        preparedInsertIntoPersonKnowsPersonStatement.close();
        preparedInsertIntoPersonHasInterestTagStatement.close();
        connection.close();
    }
}
