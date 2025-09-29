IMPORT DTREE FROM FILE 'tpcds19.txt';

CREATE STREAM store_sales (
    date_sk             INT,
    item_sk             INT,
    customer_sk         INT,
    store_sk            INT,
    ss_ext_sales_price  DECIMAL)
FROM FILE 'store_sales.dat' LINE DELIMITED CSV;

CREATE STREAM store (
    store_sk            INT)
FROM FILE 'store.dat' LINE DELIMITED CSV;

CREATE STREAM customer (
    customer_sk         INT,
    address_sk          INT)
FROM FILE 'customer.dat' LINE DELIMITED CSV;

CREATE STREAM customer_address (
    address_sk          INT)
FROM FILE 'customer_address.dat' LINE DELIMITED CSV;

CREATE STREAM date_dim (
    date_sk             INT)
FROM FILE 'date_dim.dat' LINE DELIMITED CSV;

CREATE STREAM item (
    item_sk             INT,
    i_brand_id          INT,
    i_brand             CHAR(50),
    i_manufact_id       INT,
    i_manufact          CHAR(50))
FROM FILE 'item.dat' LINE DELIMITED CSV;

SELECT i_brand_id,
       i_brand,
       i_manufact_id,
       i_manufact,
       SUM(ss_ext_sales_price)
FROM store_sales NATURAL JOIN store NATURAL JOIN customer
    NATURAL JOIN customer_address NATURAL JOIN date_dim NATURAL JOIN item
GROUP BY i_brand_id, i_brand, i_manufact_id, i_manufact;