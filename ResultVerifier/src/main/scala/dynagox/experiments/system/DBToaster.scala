package dynagox.experiments.system

import dynagox.experiments.reader.XMLResultReader

class DBToaster(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def load(): Unit = {
        query match {
            case "line5c" | "line5cd" | "dumbbell" | "lsqb6" | "job8" | "job16" | "job19"
                 | "ceb1" | "ceb2" | "ceb3" | "ceb4" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_COUNT_AGGREGATE_1", List.empty)))
                result = reader.read(file)
            case "line5s" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List.empty)))
                result = reader.read(file)
            case "line5gc" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_COUNT_AGGREGATE_1", List("R3_SRC", "R3_DST"))))
                result = reader.read(file)
            case "tpch3" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("ORDERS_O_ORDERKEY", "ORDERS_O_ORDERDATE", "ORDERS_O_SHIPPRIORITY"))))
                result = reader.read(file)
            case "tpch5" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("NATION_N_NAME"))))
                result = reader.read(file)
            case "tpch8" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("ORDERS_O_YEAR")),
                    ("__SQL_SUM_AGGREGATE_2", List("ORDERS_O_YEAR"))))
                result = reader.read(file)
            case "tpch9" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("NATION_N_NAME", "ORDERS_O_YEAR")),
                    ("__SQL_SUM_AGGREGATE_2", List("NATION_N_NAME", "ORDERS_O_YEAR"))))
                result = reader.read(file)
            case "tpch10" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("CUSTOMER_C_CUSTKEY", "CUSTOMER_C_NAME", "CUSTOMER_C_ADDRESS",
                        "CUSTOMER_C_PHONE", "CUSTOMER_C_ACCTBAL", "CUSTOMER_C_COMMENT", "NATION_N_NAME"))))
                result = reader.read(file)
            case "tpcds15" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("CUSTOMER_ADDRESS_CA_ZIP"))))
                result = reader.read(file)
            case "tpcds19" =>
                val reader = new XMLResultReader(List(
                    ("__SQL_SUM_AGGREGATE_1", List("ITEM_I_BRAND_ID", "ITEM_I_BRAND", "ITEM_I_MANUFACT_ID",
                        "ITEM_I_MANUFACT"))))
                result = reader.read(file)
        }
    }

    override def getName(): String = "DBToaster"

    override def getResult(): Map[String, List[Double]] = result
}
