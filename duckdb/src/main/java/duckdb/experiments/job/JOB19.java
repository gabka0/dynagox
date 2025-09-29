package duckdb.experiments.job;

public class JOB19 extends JOBExperiment {
    public JOB19(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi " +
                "WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id";
    }
}
