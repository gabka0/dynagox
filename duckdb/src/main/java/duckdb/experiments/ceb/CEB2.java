package duckdb.experiments.ceb;

public class CEB2 extends CEBExperiment {
    public CEB2(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi, PERSON_INFO AS pi " +
                "WHERE pi.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id";
    }
}
