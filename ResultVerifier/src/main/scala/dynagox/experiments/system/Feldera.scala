package dynagox.experiments.system

import dynagox.experiments.reader.JSONResultReader

class Feldera(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def getName(): String = "feldera"

    override def load(): Unit = {
        query match {
            case "line5c" | "line5s" | "line5m" | "line5cd" | "dumbbell" | "lsqb6" | "job8" | "job16" | "job19"
                 | "ceb1" | "ceb2" | "ceb3" | "ceb4" =>
                val reader = new JSONResultReader(
                    List.empty, List("val"))
                result = reader.read(file)
            case "line5gc" =>
                val reader = new JSONResultReader(
                    List(("src", "Int"),
                        ("dst", "Int")),
                    List("val"))
                result = reader.read(file)
            case "tpch3" =>
                val reader = new JSONResultReader(
                    List(("o_orderkey", "Int"),
                        ("o_orderdate", "Date"),
                        ("o_shippriority", "Int")),
                    List("revenue"))
                result = reader.read(file)
            case "tpch5" =>
                val reader = new JSONResultReader(
                    List(("n_name", "String")),
                    List("revenue"))
                result = reader.read(file)
            case "tpch8" =>
                val reader = new JSONResultReader(
                    List(("o_year", "Int")),
                    List("value1", "value2"))
                result = reader.read(file)
            case "tpch9" =>
                val reader = new JSONResultReader(
                    List(("nation", "String"),
                        ("o_year", "Int")),
                    List("value1", "value2"))
                result = reader.read(file)
            case "tpch10" =>
                val reader = new JSONResultReader(
                    List(("c_custkey", "Int"),
                        ("c_name", "String"),
                        ("c_address", "String"),
                        ("c_phone", "String"),
                        ("c_acctbal", "Double"),
                        ("c_comment", "String"),
                        ("n_name", "String")),
                    List("revenue"))
                result = reader.read(file)
            case "tpcds15" =>
                val reader = new JSONResultReader(
                    List(("ca_zip", "String")),
                    List("val"))
                result = reader.read(file)
            case "tpcds19" =>
                val reader = new JSONResultReader(
                    List(("i_brand_id", "Int"),
                        ("i_brand", "String"),
                        ("i_manufact_id", "Int"),
                        ("i_manufact", "String")),
                    List("val"))
                result = reader.read(file)
        }
    }

    override def getResult(): Map[String, List[Double]] = result
}
