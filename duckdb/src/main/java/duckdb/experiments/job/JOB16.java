package duckdb.experiments.job;

public class JOB16 extends JOBExperiment {
    public JOB16(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_KEYWORD AS mk " +
                "WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mk.movie_id";
    }
}
