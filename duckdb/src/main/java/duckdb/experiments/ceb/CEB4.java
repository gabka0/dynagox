package duckdb.experiments.ceb;

public class CEB4 extends CEBExperiment {
    public CEB4(boolean print) {
        super(print);
    }

    @Override
    public String getQuery() {
        return "SELECT COUNT(*) " +
                "FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_INFO AS mi, " +
                "     MOVIE_INFO_IDX AS mii1, MOVIE_INFO_IDX AS mii2, MOVIE_KEYWORD AS mk, PERSON_INFO AS pi " +
                "WHERE an.person_id = ci.person_id AND pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id " +
                "  AND ci.movie_id = mk.movie_id AND ci.movie_id = mii1.movie_id AND ci.movie_id = mii2.movie_id";
    }
}
