CREATE TABLE AKA_NAME ( id BIGINT NOT NULL, person_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job19_aka_name.txt"}
    }
}]');

CREATE TABLE CAST_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, movie_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job19_cast_info.txt"}
    }
}]');

CREATE TABLE MOVIE_COMPANIES ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, company_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job19_movie_companies.txt"}
    }
}]');

CREATE TABLE MOVIE_INFO ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, info_type_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job19_movie_info.txt"}
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
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id;