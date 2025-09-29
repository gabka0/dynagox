package dynagox.experiments.system

import dynagox.experiments.reader.DistributedPlainTextResultReader

class DynaGoxFlinkDelta(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def getName(): String = "DynaGox(Flink)[D]"

    override def isApproximated(): Boolean = true

    override def load(): Unit = {
        query match {
            case "line5gc" | "tpch3" | "tpch5" | "tpch10" | "tpcds15" | "tpcds19" =>
                val reader = new DistributedPlainTextResultReader(true, 1, "SUM")
                result = reader.read(file)
            case "tpch8" | "tpch9" =>
                val reader = new DistributedPlainTextResultReader(true, 2, "SUM")
                result = reader.read(file)
        }
    }

    override def getResult(): Map[String, List[Double]] = result
}
