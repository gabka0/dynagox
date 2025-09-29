CREATE TABLE AKA_NAME ( id BIGINT NOT NULL, person_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_aka_name.txt"}
    }
}]');

CREATE TABLE CAST_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, movie_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_cast_info.txt"}
    }
}]');

CREATE TABLE MOVIE_INFO ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_movie_info.txt"}
    }
}]');

CREATE TABLE MOVIE_INFO_IDX ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_movie_info_idx.txt"}
    }
}]');

CREATE TABLE MOVIE_KEYWORD ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, keyword_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_movie_keyword.txt"}
    }
}]');

CREATE TABLE PERSON_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb4_person_info.txt"}
    }
}]');

CREATE VIEW RESULT ( val )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_INFO AS mi,
     MOVIE_INFO_IDX AS mii1, MOVIE_INFO_IDX AS mii2, MOVIE_KEYWORD AS mk, PERSON_INFO AS pi
WHERE an.person_id = ci.person_id AND pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id
  AND ci.movie_id = mk.movie_id AND ci.movie_id = mii1.movie_id AND ci.movie_id = mii2.movie_id;