CREATE TABLE CAST_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, movie_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb2_cast_info.txt"}
    }
}]');

CREATE TABLE MOVIE_COMPANIES ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, company_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb2_movie_companies.txt"}
    }
}]');

CREATE TABLE MOVIE_INFO ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb2_movie_info.txt"}
    }
}]');

CREATE TABLE PERSON_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/ceb2_person_info.txt"}
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
FROM CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi, PERSON_INFO AS pi
WHERE pi.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id;