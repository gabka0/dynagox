CREATE STREAM aka_name(id long, person_id long) FROM FILE 'aka_name.dat' LINE DELIMITED CSV;
CREATE STREAM cast_info(person_id long, movie_id long) FROM FILE 'cast_info.dat' LINE DELIMITED CSV;
CREATE STREAM movie_info(id long, movie_id long) FROM FILE 'movie_info.dat' LINE DELIMITED CSV;
CREATE STREAM movie_info_idx1(id long, movie_id long) FROM FILE 'movie_info_idx1.dat' LINE DELIMITED CSV;
CREATE STREAM movie_info_idx2(id long, movie_id long) FROM FILE 'movie_info_idx2.dat' LINE DELIMITED CSV;
CREATE STREAM movie_keyword(id long, movie_id long) FROM FILE 'movie_keyword.dat' LINE DELIMITED CSV;
CREATE STREAM person_info(id long, person_id long) FROM FILE 'person_info.dat' LINE DELIMITED CSV;

SELECT COUNT(*)
FROM aka_name AS an, cast_info AS ci, movie_info AS mi,
     movie_info_idx1 AS mii1, movie_info_idx2 AS mii2, movie_keyword AS mk, person_info AS pi
WHERE an.person_id = ci.person_id AND pi.person_id = ci.person_id
  AND ci.movie_id = mi.movie_id AND ci.movie_id = mii1.movie_id
  AND ci.movie_id = mii2.movie_id AND ci.movie_id = mk.movie_id;