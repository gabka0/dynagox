package dynagox.experiments.system

import dynagox.experiments.reader.PlainTextResultReader

class CrownDelta(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def getName(): String = "Crown[D]"

    override def load(): Unit = {
        query match {
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
