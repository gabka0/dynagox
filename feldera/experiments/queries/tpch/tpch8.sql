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
        "config": {"path": "${input}/tpch8_part.txt"}
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
        "config": {"path": "${input}/tpch8_supplier.txt"}
    }
}]');

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
        "config": {"path": "${input}/tpch8_customer.txt"}
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
        "config": {"path": "${input}/tpch8_orders.txt"}
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
        "config": {"path": "${input}/tpch8_lineitem.txt"}
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
        "config": {"path": "${input}/tpch8_nation.txt"}
    }
}]');

CREATE TABLE REGION (
    r_regionkey         INTEGER NOT NULL,
    r_name              CHAR(25) NOT NULL,
    r_comment           VARCHAR(152))
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpch8_region.txt"}
    }
}]');

CREATE VIEW RESULT ( o_year, value1, value2 )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
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
    FROM
        PART,
        SUPPLIER,
        LINEITEM,
        ORDERS,
        CUSTOMER,
        NATION n1,
        NATION n2,
        REGION
    WHERE p_partkey = l_partkey
      AND s_suppkey = l_suppkey
      AND l_orderkey = o_orderkey
      AND o_custkey = c_custkey
      AND c_nationkey = n1.n_nationkey
      AND n1.n_regionkey = r_regionkey
      AND r_name = 'AMERICA'
      AND s_nationkey = n2.n_nationkey
      AND o_orderdate between date '1995-01-01' and date '1996-12-31'
      AND p_type = 'ECONOMY ANODIZED STEEL'
) AS ALL_NATIONS
GROUP BY o_year;