CREATE TABLE catalog_sales (
    cs_sold_date_sk     INTEGER NOT NULL,
    cs_bill_customer_sk INTEGER NOT NULL,
    cs_sales_price      DECIMAL(7,2) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds15_catalog_sales.txt"}
    }
}]');

CREATE TABLE customer (
    c_customer_sk       INTEGER NOT NULL,
    c_current_addr_sk   INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds15_customer.txt"}
    }
}]');

CREATE TABLE customer_address (
    ca_address_sk       INTEGER NOT NULL,
    ca_zip              CHAR(10) NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds15_customer_address.txt"}
    }
}]');

CREATE TABLE date_dim (
    d_date_sk           INTEGER NOT NULL)
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_input",
        "config": {"path": "${input}/tpcds15_date_dim.txt"}
    }
}]');

CREATE VIEW RESULT ( ca_zip, val )
WITH ('connectors' = '[{
    "format": {"name": "json"},
    "transport": {
        "name": "file_output",
        "config": {"path": "/home/ubuntu/result.txt"}
    }
}]')
AS
SELECT ca_zip, SUM(cs_sales_price)
FROM catalog_sales,
     customer,
     customer_address,
     date_dim
WHERE cs_bill_customer_sk = c_customer_sk
  AND c_current_addr_sk = ca_address_sk
  AND cs_sold_date_sk = d_date_sk
GROUP BY ca_zip;