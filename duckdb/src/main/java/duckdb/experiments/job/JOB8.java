package duckdb.experiments.job;

public class JOB8 extends JOBExperiment {
    public JOB8(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc " +
                "WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id";
    }
}
