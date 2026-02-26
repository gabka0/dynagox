-- Flink SQL template for Line(k,a,b) COUNT(*)
-- Assumptions:
-- 1) Input file has columns: seq, op, rid, src, dst, is_milestone
-- 2) Initial baseline handles inserts (+) only. Deletions (-) require retract-aware setup.

-- Replace ${PATH} with your CSV path
CREATE TABLE updates (
  seq BIGINT,
  op STRING,
  rid INT,
  src BIGINT,
  dst BIGINT,
  is_milestone INT
) WITH (
  'connector' = 'filesystem',
  'path' = '${PATH}',
  'format' = 'csv',
  'csv.ignore-parse-errors' = 'true'
);

CREATE VIEW r1 AS SELECT src, dst FROM updates WHERE rid = 1 AND op = '+';
CREATE VIEW r2 AS SELECT src, dst FROM updates WHERE rid = 2 AND op = '+';
CREATE VIEW r3 AS SELECT src, dst FROM updates WHERE rid = 3 AND op = '+';
CREATE VIEW r4 AS SELECT src, dst FROM updates WHERE rid = 4 AND op = '+';
CREATE VIEW r5 AS SELECT src, dst FROM updates WHERE rid = 5 AND op = '+';
CREATE VIEW r6 AS SELECT src, dst FROM updates WHERE rid = 6 AND op = '+';

-- Example query: Line(3,2,3) -> group by (x2,x3)
SELECT
  r2.src AS x2,
  r2.dst AS x3,
  COUNT(*) AS cnt
FROM r1
JOIN r2 ON r1.dst = r2.src
JOIN r3 ON r2.dst = r3.src
GROUP BY r2.src, r2.dst;

-- Example query: Line(4,2,4) -> group by (x2,x3,x4)
SELECT
  r2.src AS x2,
  r2.dst AS x3,
  r3.dst AS x4,
  COUNT(*) AS cnt
FROM r1
JOIN r2 ON r1.dst = r2.src
JOIN r3 ON r2.dst = r3.src
JOIN r4 ON r3.dst = r4.src
GROUP BY r2.src, r2.dst, r3.dst;
