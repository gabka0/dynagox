CREATE STREAM store_sales (
    ss_sold_date_sk     INT,
    ss_item_sk          INT,
    ss_customer_sk      INT,
    ss_store_sk         INT,
    ss_ext_sales_price  DECIMAL)
FROM FILE 'store_sales.dat' LINE DELIMITED CSV;

CREATE STREAM store (
    s_store_sk          INT)
FROM FILE 'store.dat' LINE DELIMITED CSV;

CREATE STREAM customer (
    c_customer_sk       INT,
    c_current_addr_sk   INT)
FROM FILE 'customer.dat' LINE DELIMITED CSV;

CREATE STREAM customer_address (
    ca_address_sk       INT)
FROM FILE 'customer_address.dat' LINE DELIMITED CSV;

CREATE STREAM date_dim (
    d_date_sk       INT)
FROM FILE 'date_dim.dat' LINE DELIMITED CSV;

CREATE STREAM item (
    i_item_sk       INT,
    i_brand_id      INT,
    i_brand         CHAR(50),
    i_manufact_id   INT,
    i_manufact      CHAR(50))
FROM FILE 'item.dat' LINE DELIMITED CSV;

SELECT i_brand_id,
       i_brand,
       i_manufact_id,
       i_manufact,
       SUM(ss_ext_sales_price)
FROM store_sales,
     store,
     customer,
     customer_address,
     date_dim,
     item
WHERE d_date_sk = ss_sold_date_sk
  AND ss_item_sk = i_item_sk
  AND ss_customer_sk = c_customer_sk
  AND c_current_addr_sk = ca_address_sk
  AND ss_store_sk = s_store_sk
GROUP BY i_brand_id, i_brand, i_manufact_id, i_manufact;