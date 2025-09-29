CREATE STREAM catalog_sales (
    cs_sold_date_sk     INT,
    cs_bill_customer_sk INT,
    cs_sales_price      DECIMAL)
FROM FILE 'catalog_sales.dat' LINE DELIMITED CSV;

CREATE STREAM customer (
    c_customer_sk       INT,
    c_current_addr_sk   INT)
FROM FILE 'customer.dat' LINE DELIMITED CSV;

CREATE STREAM customer_address (
    ca_address_sk       INT,
    ca_zip              CHAR(10))
FROM FILE 'customer_address.dat' LINE DELIMITED CSV;

CREATE STREAM date_dim (
    d_date_sk       INT)
FROM FILE 'date_dim.dat' LINE DELIMITED CSV;

SELECT ca_zip, SUM(cs_sales_price)
FROM catalog_sales,
     customer,
     customer_address,
     date_dim
WHERE cs_bill_customer_sk = c_customer_sk
  AND c_current_addr_sk = ca_address_sk
  AND cs_sold_date_sk = d_date_sk
GROUP BY ca_zip;