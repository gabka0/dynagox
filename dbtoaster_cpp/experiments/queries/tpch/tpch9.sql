CREATE STREAM PART (
    p_partkey           INT,
    p_name              VARCHAR(55),
    p_mfgr              CHAR(25),
    p_brand             CHAR(10),
    p_type              VARCHAR(25),
    p_size              INT,
    p_container         CHAR(10),
    p_retailprice       DECIMAL,
    p_comment           VARCHAR(23))
FROM FILE 'part.dat' LINE DELIMITED CSV;

CREATE STREAM SUPPLIER (
    s_suppkey           INT,
    s_name              VARCHAR(25),
    s_address           VARCHAR(40),
    s_nationkey         INT,
    s_phone             CHAR(15),
    s_acctbal           DECIMAL,
    s_comment           VARCHAR(101))
FROM FILE 'supplier.dat' LINE DELIMITED CSV;

CREATE STREAM PARTSUPP (
    ps_partkey          INT,
    ps_suppkey          INT,
    ps_availqty         INT,
    ps_supplycost       DECIMAL,
    ps_comment          VARCHAR(199))
FROM FILE 'partsupp.dat' LINE DELIMITED CSV;

CREATE STREAM ORDERS (
    o_orderkey          INT,
    o_custkey           INT,
    o_orderstatus       CHAR(1),
    o_totalprice        DECIMAL,
    o_orderdate         DATE,
    o_orderpriority     CHAR(15),
    o_clerk             CHAR(15),
    o_shippriority      INT,
    o_comment           VARCHAR(79),
    o_year              INT)
FROM FILE 'orders.dat' LINE DELIMITED CSV;

CREATE STREAM LINEITEM (
    l_orderkey          INT,
    l_partkey           INT,
    l_suppkey           INT,
    l_linenumber        INT,
    l_quantity          DECIMAL,
    l_extendedprice     DECIMAL,
    l_discount          DECIMAL,
    l_tax               DECIMAL,
    l_returnflag        CHAR(1),
    l_linestatus        CHAR(1),
    l_shipdate          DATE,
    l_commitdate        DATE,
    l_receiptdate       DATE,
    l_shipinstruct      CHAR(25),
    l_shipmode          CHAR(10),
    l_comment           VARCHAR(44))
FROM FILE 'lineitem.dat' LINE DELIMITED CSV;

CREATE STREAM NATION (
    n_nationkey         INT,
    n_name              CHAR(25),
    n_regionkey         INT,
    n_comment           VARCHAR(152))
FROM FILE 'nation.dat' LINE DELIMITED CSV;

SELECT
    n_name,
    o_year,
    SUM(l_extendedprice * (1-l_discount)),
    SUM(ps_supplycost * l_quantity)
FROM PART, SUPPLIER, PARTSUPP, ORDERS, LINEITEM, NATION
WHERE s_suppkey = l_suppkey
AND ps_suppkey = l_suppkey
AND ps_partkey = l_partkey
AND p_partkey = l_partkey
AND o_orderkey = l_orderkey
AND s_nationkey = n_nationkey
AND p_name LIKE '%green%'
GROUP BY n_name, o_year