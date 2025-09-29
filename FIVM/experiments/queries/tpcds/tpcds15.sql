IMPORT DTREE FROM FILE 'tpcds15.txt';

CREATE STREAM catalog_sales (
    date_sk             INT,
    customer_sk         INT,
    cs_sales_price      DECIMAL)
FROM FILE 'catalog_sales.dat' LINE DELIMITED CSV;

CREATE STREAM customer (
    customer_sk         INT,
    address_sk          INT)
FROM FILE 'customer.dat' LINE DELIMITED CSV;

CREATE STREAM customer_address (
    address_sk          INT,
    ca_zip              CHAR(10))
FROM FILE 'customer_address.dat' LINE DELIMITED CSV;

CREATE STREAM date_dim (
    date_sk             INT)
FROM FILE 'date_dim.dat' LINE DELIMITED CSV;

SELECT ca_zip, SUM(cs_sales_price)
FROM catalog_sales NATURAL JOIN customer NATURAL JOIN customer_address NATURAL JOIN date_dim
GROUP BY ca_zip;