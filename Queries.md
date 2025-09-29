Queries Used in Experiments

```sql
-- L5-CNT
SELECT COUNT(*)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src;

-- L5-SUM
SELECT SUM(g1.src * g5.dst)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src;

-- L5-MAX-PROD
SELECT MAX(g1.src * g5.dst)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src;

-- L5-CNTD
SELECT COUNT(DISTINCT g2.src, g3.src, g4.src, g4.dst)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src;

-- L5-CNT-GB
SELECT g3.src, g3.dst, COUNT(*)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g4.src AND g4.dst = g5.src
GROUP BY g3.src, g3.dst;

-- DUMBBELL
SELECT COUNT(*)
FROM GRAPH AS g1, GRAPH AS g2, GRAPH AS g3, GRAPH AS g4, GRAPH AS g5, GRAPH AS g6, GRAPH AS g7
WHERE g1.dst = g2.src AND g2.dst = g3.src AND g3.dst = g1.src AND g3.src = g4.src
  AND g4.dst = g5.src AND g5.dst = g6.src AND g6.dst = g7.src AND g7.dst = g5.src;

-- LSQB-Q6
SELECT COUNT(*)
FROM PERSON_KNOWS_PERSON AS pkp1, PERSON_KNOWS_PERSON AS pkp2, PERSON_HAS_INTEREST_TAG AS phit
WHERE pkp1.person2_id = pkp2.person1_id AND pkp2.person2_id = phit.person_id;

-- JOB-Q8
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id;

-- JOB-Q8-SUM
SELECT SUM(an.id * mc.id)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id;

-- JOB-Q8-MAX-PROD
SELECT MAX(an.id * mc.id)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id;

-- JOB-Q8-MAX-PLUS
SELECT MAX(an.id + mc.id)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id;

-- JOB-Q16
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_KEYWORD AS mk
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mk.movie_id;

-- JOB-Q19
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi
WHERE an.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id;

-- CEB-Q1
SELECT COUNT(*)
FROM CAST_INFO AS ci, MOVIE_INFO AS mi, PERSON_INFO AS pi
WHERE pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id;

-- CEB-Q2
SELECT COUNT(*)
FROM CAST_INFO AS ci, MOVIE_COMPANIES AS mc, MOVIE_INFO AS mi, PERSON_INFO AS pi
WHERE pi.person_id = ci.person_id AND ci.movie_id = mc.movie_id AND ci.movie_id = mi.movie_id;

-- CEB-Q3
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_INFO AS mi, MOVIE_INFO_IDX AS mii1, 
     MOVIE_INFO_IDX AS mii2, PERSON_INFO AS pi
WHERE an.person_id = ci.person_id AND pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id 
  AND ci.movie_id = mii1.movie_id AND ci.movie_id = mii2.movie_id;

-- CEB-Q4
SELECT COUNT(*)
FROM AKA_NAME AS an, CAST_INFO AS ci, MOVIE_INFO AS mi, MOVIE_INFO_IDX AS mii1, MOVIE_INFO_IDX AS mii2, 
     MOVIE_KEYWORD AS mk, PERSON_INFO AS pi
WHERE an.person_id = ci.person_id AND pi.person_id = ci.person_id AND ci.movie_id = mi.movie_id 
  AND ci.movie_id = mk.movie_id AND ci.movie_id = mii1.movie_id AND ci.movie_id = mii2.movie_id;

-- TPCH-Q3
SELECT
    o_orderkey, o_orderdate, o_shippriority,
    SUM(l_extendedprice * (1 - l_discount)) as revenue
FROM CUSTOMER, ORDERS, LINEITEM
WHERE c_mktsegment = 'BUILDING'
  AND c_custkey = o_custkey
  AND l_orderkey = o_orderkey
  AND o_orderdate < DATE '1995-03-15'
  AND l_shipdate > DATE '1995-03-15'
GROUP BY o_orderkey, o_orderdate, o_shippriority;

-- TPCH-Q5
SELECT
    n_name,
    SUM(l_extendedprice * (1 - l_discount)) AS revenue
FROM CUSTOMER, ORDERS, LINEITEM, SUPPLIER, NATION, REGION
WHERE c_custkey = o_custkey
  AND l_orderkey = o_orderkey
  AND l_suppkey = s_suppkey
  AND c_nationkey = s_nationkey
  AND s_nationkey = n_nationkey
  AND n_regionkey = r_regionkey
  AND r_name = 'ASIA'
  AND o_orderdate >= DATE '1994-01-01'
  AND o_orderdate < DATE '1994-01-01' + INTERVAL '1' YEAR
GROUP BY n_name;

-- TPCH-Q8
SELECT
    o_year,
    SUM(CASE
            WHEN nation = 'BRAZIL' THEN volume
            ELSE 0
        END) AS value1,
    SUM(volume) AS value2
FROM (
         SELECT
             EXTRACT(YEAR FROM o_orderdate) AS o_year,
             l_extendedprice * (1 - l_discount) AS volume,
             n2.n_name AS nation
         FROM PART, SUPPLIER, LINEITEM, ORDERS, CUSTOMER, NATION n1, NATION n2, REGION
         WHERE p_partkey = l_partkey
           AND s_suppkey = l_suppkey
           AND l_orderkey = o_orderkey
           AND o_custkey = c_custkey
           AND c_nationkey = n1.n_nationkey
           AND n1.n_regionkey = r_regionkey
           AND r_name = 'AMERICA'
           AND s_nationkey = n2.n_nationkey
           AND o_orderdate BETWEEN DATE '1995-01-01' and DATE '1996-12-31'
           AND p_type = 'ECONOMY ANODIZED STEEL'
     ) AS ALL_NATIONS
GROUP BY o_year;

-- TPCH-Q9
SELECT
    nation, o_year,
    SUM(v1) AS value1,
    SUM(v2) AS value2
FROM (
         SELECT
             n_name AS nation,
             EXTRACT(YEAR FROM o_orderdate) AS o_year,
             l_extendedprice * (1 - l_discount) AS v1,
             ps_supplycost * l_quantity AS v2
         FROM PART, SUPPLIER, LINEITEM, PARTSUPP, ORDERS, NATION
         WHERE s_suppkey = l_suppkey
           AND ps_suppkey = l_suppkey
           AND ps_partkey = l_partkey
           AND p_partkey = l_partkey
           AND o_orderkey = l_orderkey
           AND s_nationkey = n_nationkey
           AND p_name LIKE '%green%'
     ) AS PROFIT
GROUP BY nation, o_year;

-- TPCH-Q10
SELECT
    c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name,
    SUM(l_extendedprice * (1 - l_discount)) AS revenue
FROM CUSTOMER, ORDERS, LINEITEM, NATION
WHERE c_custkey = o_custkey
  AND l_orderkey = o_orderkey
  AND o_orderdate >= DATE '1993-10-01'
  AND o_orderdate < DATE '1993-10-01' + INTERVAL '3' MONTH
  AND l_returnflag = 'R'
  AND c_nationkey = n_nationkey
GROUP BY c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name;

-- TPCDS-Q15
SELECT
    ca_zip, SUM(cs_sales_price)
FROM catalog_sales, customer, customer_address, date_dim
WHERE cs_bill_customer_sk = c_customer_sk
  AND c_current_addr_sk = ca_address_sk
  AND cs_sold_date_sk = d_date_sk
GROUP BY ca_zip;

-- TPCDS-Q19
SELECT
    i_brand_id, i_brand, i_manufact_id, i_manufact,
    SUM(ss_ext_sales_price)
FROM store_sales, store, customer, customer_address, date_dim, item
WHERE d_date_sk = ss_sold_date_sk
  AND ss_item_sk = i_item_sk
  AND ss_customer_sk = c_customer_sk
  AND c_current_addr_sk = ca_address_sk
  AND ss_store_sk = s_store_sk
GROUP BY i_brand_id, i_brand, i_manufact_id, i_manufact;
```



