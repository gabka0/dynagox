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

CREATE STREAM CUSTOMER (
    c_custkey           INT,
    c_name              VARCHAR(25),
    c_address           VARCHAR(40),
    c_nationkey         INT,
    c_phone             CHAR(15),
    c_acctbal           DECIMAL,
    c_mktsegment        CHAR(10),
    c_comment           VARCHAR(117))
FROM FILE 'customer.dat' LINE DELIMITED CSV;

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

CREATE STREAM NATION1 (
    n1_nationkey        INT,
    n1_name             CHAR(25),
    n1_regionkey        INT,
    n1_comment          VARCHAR(152))
FROM FILE 'nation1.dat' LINE DELIMITED CSV;

CREATE STREAM NATION2 (
    n2_nationkey        INT,
    n2_name             CHAR(25),
    n2_regionkey        INT,
    n2_comment          VARCHAR(152))
FROM FILE 'nation2.dat' LINE DELIMITED CSV;

CREATE STREAM REGION (
    r_regionkey         INT,
    r_name              CHAR(25),
    r_comment           VARCHAR(152))
FROM FILE 'region.dat' LINE DELIMITED CSV;

SELECT
    o_year,
    SUM(CASE
            WHEN n2_name = 'BRAZIL'
                THEN l_extendedprice * (1-l_discount)
            ELSE 0
        END),
    SUM(l_extendedprice * (1-l_discount))
FROM PART, SUPPLIER, CUSTOMER, ORDERS, LINEITEM, NATION1, NATION2, REGION
WHERE p_partkey = l_partkey
AND s_suppkey = l_suppkey
AND l_orderkey = o_orderkey
AND o_custkey = c_custkey
AND c_nationkey = n1_nationkey
AND n1_regionkey = r_regionkey
AND r_name = 'AMERICA'
AND s_nationkey = n2_nationkey
AND o_orderdate >= DATE ('1995-01-01')
AND o_orderdate <= DATE ('1996-12-31')
AND p_type = 'ECONOMY ANODIZED STEEL'
GROUP BY o_year