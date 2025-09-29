package dynagox.experiments.system

import dynagox.experiments.reader.PlainTextResultReader

class DuckDB(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def getName(): String = "DuckDB"

    override def load(): Unit = {
        query match {
            case "line5c" | "line5s" | "line5m" | "line5cd" | "dumbbell" | "lsqb6" | "job8" | "job16" | "job19"
                 | "ceb1" | "ceb2" | "ceb3" | "ceb4" =>
                val reader = new PlainTextResultReader(false, 1)
                result = reader.read(file)
            case "line5gc" | "tpch3" | "tpch5" | "tpch10" | "tpcds15" | "tpcds19" =>
                val reader = new PlainTextResultReader(true, 1)
                result = reader.read(file)
            case "tpch8" | "tpch9" =>
                val reader = new PlainTextResultReader(true, 2)
                result = reader.read(file)
        }
    }

    override def getResult(): Map[String, List[Double]] = result
}
