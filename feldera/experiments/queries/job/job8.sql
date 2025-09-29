CREATE TABLE AKA_NAME ( id BIGINT NOT NULL, person_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job8_aka_name.txt"}
    }
}]');

CREATE TABLE CAST_INFO ( id BIGINT NOT NULL, person_id BIGINT NOT NULL, movie_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job8_cast_info.txt"}
    }
}]');

CREATE TABLE MOVIE_COMPANIES ( id BIGINT NOT NULL, movie_id BIGINT NOT NULL, company_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/job8_movie_companies.txt"}
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
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id;