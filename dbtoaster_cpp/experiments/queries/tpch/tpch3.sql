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
    o_comment           VARCHAR(79))
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

SELECT o_orderkey, o_orderdate, o_shippriority, SUM(l_extendedprice * (1 - l_discount))
FROM CUSTOMER, ORDERS, LINEITEM
WHERE l_orderkey = o_orderkey AND o_custkey = c_custkey
AND c_mktsegment = 'BUILDING'
AND o_orderdate < DATE ('1995-03-15')
AND l_shipdate > DATE ('1995-03-15')
GROUP BY o_orderkey, o_orderdate, o_shippriority;