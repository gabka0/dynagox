package duckdb.experiments.ceb;

import duckdb.experiments.Experiment;

import java.sql.*;

public abstract class CEBExperiment extends Experiment {
    PreparedStatement preparedInsertIntoAkaNameStatement;
    PreparedStatement preparedInsertIntoCastInfoStatement;
    PreparedStatement preparedInsertIntoMovieCompaniesStatement;
    PreparedStatement preparedInsertIntoMovieInfoStatement;
    PreparedStatement preparedInsertIntoMovieInfoIdxStatement;
    PreparedStatement preparedInsertIntoMovieKeywordStatement;
    PreparedStatement preparedInsertIntoPersonInfoStatement;


    public CEBExperiment(boolean print) {
        super(print);
    }

    @Override
    public void init() throws SQLException {
        connection = DriverManager.getConnection("jdbc:duckdb:");

        try (Statement stmt = connection.createStatement()) {
            stmt.execute("CREATE TABLE IF NOT EXISTS AKA_NAME (id BIGINT NOT NULL, person_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS CAST_INFO (id BIGINT NOT NULL, person_id BIGINT NOT NULL, movie_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS MOVIE_COMPANIES (id BIGINT NOT NULL, movie_id BIGINT NOT NULL, company_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS MOVIE_INFO (id BIGINT NOT NULL, movie_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS MOVIE_INFO_IDX (id BIGINT NOT NULL, movie_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS MOVIE_KEYWORD (id BIGINT NOT NULL, movie_id BIGINT NOT NULL, keyword_id BIGINT NOT NULL)");
            stmt.execute("CREATE TABLE IF NOT EXISTS PERSON_INFO (id BIGINT NOT NULL, person_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL)");
        }

        preparedInsertIntoAkaNameStatement =
                connection.prepareStatement("INSERT INTO AKA_NAME (id, person_id) VALUES (?, ?)");
        preparedInsertIntoCastInfoStatement =
                connection.prepareStatement("INSERT INTO CAST_INFO (id, person_id, movie_id) VALUES (?, ?, ?)");
        preparedInsertIntoMovieCompaniesStatement =
                connection.prepareStatement("INSERT INTO MOVIE_COMPANIES (id, movie_id, company_id) VALUES (?, ?, ?)");
        preparedInsertIntoMovieInfoStatement =
                connection.prepareStatement("INSERT INTO MOVIE_INFO (id, movie_id, info_type_id) VALUES (?, ?, ?)");
        preparedInsertIntoMovieInfoIdxStatement =
                connection.prepareStatement("INSERT INTO MOVIE_INFO_IDX (id, movie_id) VALUES (?, ?)");
        preparedInsertIntoMovieKeywordStatement =
                connection.prepareStatement("INSERT INTO MOVIE_KEYWORD (id, movie_id, keyword_id) VALUES (?, ?, ?)");
        preparedInsertIntoPersonInfoStatement =
                connection.prepareStatement("INSERT INTO PERSON_INFO (id, person_id, info_type_id) VALUES (?, ?, ?)");
    }

    @Override
    public void process(String line) throws SQLException {
        String[] fields = line.split("\\|");
        int rid = Integer.parseInt(fields[0]);

        if (rid == 0) {
            preparedInsertIntoAkaNameStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoAkaNameStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoAkaNameStatement.executeUpdate();
        } else if (rid == 1) {
            preparedInsertIntoCastInfoStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoCastInfoStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoCastInfoStatement.setLong(3, Long.parseLong(fields[3]));
            preparedInsertIntoCastInfoStatement.executeUpdate();
        } else if (rid == 2) {
            preparedInsertIntoMovieCompaniesStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoMovieCompaniesStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoMovieCompaniesStatement.setLong(3, Long.parseLong(fields[3]));
            preparedInsertIntoMovieCompaniesStatement.executeUpdate();
        } else if (rid == 3) {
            preparedInsertIntoMovieInfoStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoMovieInfoStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoMovieInfoStatement.setLong(3, Long.parseLong(fields[3]));
            preparedInsertIntoMovieInfoStatement.executeUpdate();
        } else if (rid == 4) {
            preparedInsertIntoMovieInfoIdxStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoMovieInfoIdxStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoMovieInfoIdxStatement.executeUpdate();
        } else if (rid == 5) {
            preparedInsertIntoMovieKeywordStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoMovieKeywordStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoMovieKeywordStatement.setLong(3, Long.parseLong(fields[3]));
            preparedInsertIntoMovieKeywordStatement.executeUpdate();
        } else if (rid == 6) {
            preparedInsertIntoPersonInfoStatement.setLong(1, Long.parseLong(fields[1]));
            preparedInsertIntoPersonInfoStatement.setLong(2, Long.parseLong(fields[2]));
            preparedInsertIntoPersonInfoStatement.setLong(3, Long.parseLong(fields[3]));
            preparedInsertIntoPersonInfoStatement.executeUpdate();
        }
    }

    @Override
    public void close() throws SQLException {
        preparedInsertIntoAkaNameStatement.close();
        preparedInsertIntoCastInfoStatement.close();
        preparedInsertIntoMovieCompaniesStatement.close();
        preparedInsertIntoMovieInfoStatement.close();
        preparedInsertIntoMovieInfoIdxStatement.close();
        preparedInsertIntoMovieKeywordStatement.close();
        preparedInsertIntoPersonInfoStatement.close();
        connection.close();
    }

    @Override
    public void printResultSet(ResultSet resultSet) throws SQLException {
        System.out.println(resultSet.getLong(1));
    }
}
