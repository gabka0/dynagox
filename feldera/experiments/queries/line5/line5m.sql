CREATE TABLE GRAPH ( src BIGINT NOT NULL, dst BIGINT NOT NULL )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/graph.txt"}
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
SELECT MAX(g1.src * g5.dst)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src;