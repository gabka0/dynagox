CREATE TABLE PART (
    P_PARTKEY           INTEGER NOT NULL,
    P_NAME              VARCHAR(55) NOT NULL,
    P_MFGR              CHAR(25) NOT NULL,
    P_BRAND             CHAR(10) NOT NULL,
    P_TYPE              VARCHAR(25) NOT NULL,
    P_SIZE              INTEGER NOT NULL,
    P_CONTAINER         CHAR(10) NOT NULL,
    P_RETAILPRICE       DECIMAL(15,2) NOT NULL,
    P_COMMENT           VARCHAR(23) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch9_part.txt"}
    }
}]');

CREATE TABLE SUPPLIER (
    s_suppkey           INTEGER NOT NULL,
    s_name              CHAR(25) NOT NULL,
    s_address           VARCHAR(40) NOT NULL,
    s_nationkey         INTEGER NOT NULL,
    s_phone             CHAR(15) NOT NULL,
    s_acctbal           DECIMAL(15,2) NOT NULL,
    s_comment           VARCHAR(101) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch9_supplier.txt"}
    }
}]');

CREATE TABLE PARTSUPP (
    ps_partkey          INTEGER NOT NULL,
    ps_suppkey          INTEGER NOT NULL,
    ps_availqty         INTEGER NOT NULL,
    ps_supplycost       DECIMAL(15,2)  NOT NULL,
    ps_comment          VARCHAR(199) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch9_partsupp.txt"}
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
        "config": {"path": "${input}/tpch9_orders.txt"}
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
        "config": {"path": "${input}/tpch9_lineitem.txt"}
    }
}]');

CREATE TABLE NATION (
    n_nationkey         INTEGER NOT NULL,
    n_name              CHAR(25) NOT NULL,
    n_regionkey         INTEGER NOT NULL,
    n_comment           VARCHAR(152))
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch9_nation.txt"}
    }
}]');

CREATE VIEW RESULT ( nation, o_year, value1, value2 )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
SELECT
    nation,
    o_year,
    SUM(v1) AS value1,
    SUM(v2) AS value2
FROM (
    SELECT
        n_name AS nation,
        EXTRACT(YEAR FROM o_orderdate) AS o_year,
        l_extendedprice * (1 - l_discount) AS v1,
        ps_supplycost * l_quantity AS v2
    FROM
        PART,
        SUPPLIER,
        LINEITEM,
        PARTSUPP,
        ORDERS,
        NATION
    WHERE s_suppkey = l_suppkey
      AND ps_suppkey = l_suppkey
      AND ps_partkey = l_partkey
      AND p_partkey = l_partkey
      AND o_orderkey = l_orderkey
      AND s_nationkey = n_nationkey
      AND p_name LIKE '%green%'
) AS PROFIT
GROUP BY nation, o_year;