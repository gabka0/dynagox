package duckdb.experiments.ceb;

public class CEB1 extends CEBExperiment {
    public CEB1(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM CAST_INFO AS ci, MOVIE_INFO AS mi, PERSON_INFO AS pi " +
                "WHERE pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id";
    }
}
