CREATE TABLE CUSTOMER (
    c_custkey           INTEGER NOT NULL,
    c_name              VARCHAR(25) NOT NULL,
    c_address           VARCHAR(40) NOT NULL,
    c_nationkey         INTEGER NOT NULL,
    c_phone             CHAR(15) NOT NULL,
    c_acctbal           DECIMAL(15,2)   NOT NULL,
    c_mktsegment        CHAR(10) NOT NULL,
    c_comment           VARCHAR(117) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch3_customer.txt"}
    }
}]');

CREATE TABLE ORDERS (
    o_orderkey          INTEGER NOT NULL,
    o_custkey           INTEGER NOT NULL,
    o_orderstatus       CHAR(1) NOT NULL,
    o_totalprice        DECIMAL(15,2) NOT NULL,
    o_orderdate         DATE NOT NULL,
    o_orderpriority     CHAR(15) NOT NULL,
    o_clerk             CHAR(15) NOT NULL,
    o_shippriority      INTEGER NOT NULL,
    o_comment           VARCHAR(79) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch3_orders.txt"}
    }
}]');

CREATE TABLE LINEITEM (
    l_orderkey          INTEGER NOT NULL,
    l_partkey           INTEGER NOT NULL,
    l_suppkey           INTEGER NOT NULL,
    l_linenumber        INTEGER NOT NULL,
    l_quantity          DECIMAL(15,2) NOT NULL,
    l_extendedprice     DECIMAL(15,2) NOT NULL,
    l_discount          DECIMAL(15,2) NOT NULL,
    l_tax               DECIMAL(15,2) NOT NULL,
    l_returnflag        CHAR(1) NOT NULL,
    l_linestatus        CHAR(1) NOT NULL,
    l_shipdate          DATE NOT NULL,
    l_commitdate        DATE NOT NULL,
    l_receiptdate       DATE NOT NULL,
    l_shipinstruct      CHAR(25) NOT NULL,
    l_shipmode          CHAR(10) NOT NULL,
    l_comment           VARCHAR(44) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch3_lineitem.txt"}
    }
}]');

create view RESULT ( o_orderkey, o_orderdate, o_shippriority, revenue )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
SELECT
    o_orderkey,
    o_orderdate,
    o_shippriority,
    sum(l_extendedprice * (1 - l_discount)) as revenue
FROM CUSTOMER, ORDERS, LINEITEM
WHERE c_mktsegment = 'BUILDING'
AND c_custkey = o_custkey
AND l_orderkey = o_orderkey
AND o_orderdate < DATE '1995-03-15'
AND l_shipdate > DATE '1995-03-15'
GROUP BY o_orderkey, o_orderdate, o_shippriority;