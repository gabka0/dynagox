CREATE STREAM aka_name(id long, person_id long) FROM FILE 'aka_name.dat' LINE DELIMITED CSV;
CREATE STREAM cast_info(person_id long, movie_id long) FROM FILE 'cast_info.dat' LINE DELIMITED CSV;
CREATE STREAM movie_companies(id long, movie_id long) FROM FILE 'movie_companies.dat' LINE DELIMITED CSV;
CREATE STREAM movie_keyword(id long, movie_id long) FROM FILE 'movie_keyword.dat' LINE DELIMITED CSV;

SELECT COUNT(*)
FROM aka_name AS an, cast_info AS ci, movie_companies AS mc, movie_keyword AS mk
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mk.movie_id;