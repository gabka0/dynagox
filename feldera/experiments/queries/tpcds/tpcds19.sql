CREATE TABLE store_sales (
    ss_sold_date_sk     INTEGER NOT NULL,
    ss_item_sk          INTEGER NOT NULL,
    ss_customer_sk      INTEGER NOT NULL,
    ss_store_sk         INTEGER NOT NULL,
    ss_ext_sales_price  DECIMAL(7,2) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_store_sales.txt"}
    }
}]');

CREATE TABLE store (
    s_store_sk          INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_store.txt"}
    }
}]');

CREATE TABLE customer (
    c_customer_sk       INTEGER NOT NULL,
    c_current_addr_sk   INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_customer.txt"}
    }
}]');

CREATE TABLE customer_address (
    ca_address_sk       INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_customer_address.txt"}
    }
}]');

CREATE TABLE date_dim (
    d_date_sk           INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_date_dim.txt"}
    }
}]');

CREATE TABLE item (
    i_item_sk           INTEGER NOT NULL,
    i_brand_id          INTEGER NOT NULL,
    i_brand             CHAR(50) NOT NULL,
    i_manufact_id       INTEGER NOT NULL,
    i_manufact          CHAR(50) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds19_item.txt"}
    }
}]');

CREATE VIEW RESULT ( i_brand_id, i_brand, i_manufact_id, i_manufact, val )
        WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
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