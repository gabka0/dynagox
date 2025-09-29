CREATE STREAM cast_info(person_id long, movie_id long) FROM FILE 'cast_info.dat' LINE DELIMITED CSV;
CREATE STREAM movie_companies(id long, movie_id long) FROM FILE 'movie_companies.dat' LINE DELIMITED CSV;
CREATE STREAM movie_info(id long, movie_id long) FROM FILE 'movie_info.dat' LINE DELIMITED CSV;
CREATE STREAM person_info(id long, person_id long) FROM FILE 'person_info.dat' LINE DELIMITED CSV;

SELECT COUNT(*)
FROM cast_info AS ci, movie_companies AS mc, movie_info AS mi, person_info AS pi
WHERE pi.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id;