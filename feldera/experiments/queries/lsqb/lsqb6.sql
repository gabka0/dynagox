CREATE TABLE PERSON_KNOWS_PERSON ( person1_id BIGINT NOT NULL, person2_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/lsqb6_person_knows_person.txt"}
    }
}]');

CREATE TABLE PERSON_HAS_INTEREST_TAG ( person_id BIGINT NOT NULL, tag_id BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/lsqb6_person_has_interest_tag.txt"}
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
FROM PERSON_KNOWS_PERSON AS pkp1, PERSON_KNOWS_PERSON AS pkp2, PERSON_HAS_INTEREST_TAG AS phit
WHERE pkp1.person2_id = pkp2.person1_id AND pkp2.person2_id = phit.person_id;