package dynagox.experiments.system

import dynagox.experiments.reader.XMLResultReader

class FIVM(val query: String, val file: String) extends ExperimentSystem {
    var result: Map[String, List[Double]] = Map.empty

    override def getName(): String = "F-IVM"

    override def load(): Unit = {
        query match {
            case "line5c" | "line5s" | "line5m" =>
                val reader = new XMLResultReader(List((
                    "V_C_RRRRR1",
                    List.empty)))
                result = reader.read(file)
            case "line5gc" =>
                val reader = new XMLResultReader(List((
                    "V_C_RRRRR1",
                    List("C", "D"))))
                result = reader.read(file)
            case "dumbbell" =>
                val reader = new XMLResultReader(List((
                    "V_A_RRRRRRR1",
                    List.empty)))
                result = reader.read(file)
            case "lsqb6" =>
                val reader = new XMLResultReader(List((
                    "V_B_PPP1",
                    List.empty)))
                result = reader.read(file)
            case "job8" =>
                val reader = new XMLResultReader(List((
                    "V_B_AMC1",
                    List.empty)))
                result = reader.read(file)
            case "job16" =>
                val reader = new XMLResultReader(List((
                    "V_B_AMMC1",
                    List.empty)))
                result = reader.read(file)
            case "job19" =>
                val reader = new XMLResultReader(List((
                    "V_B_AMMC1",
                    List.empty)))
                result = reader.read(file)
            case "ceb1" =>
                val reader = new XMLResultReader(List((
                    "V_B_PMC1",
                    List.empty)))
                result = reader.read(file)
            case "ceb2" =>
                val reader = new XMLResultReader(List((
                    "V_B_PMMC1",
                    List.empty)))
                result = reader.read(file)
            case "ceb3" =>
                val reader = new XMLResultReader(List((
                    "V_B_APMMMC1",
                    List.empty)))
                result = reader.read(file)
            case "ceb4" =>
                val reader = new XMLResultReader(List((
                    "V_B_APMMMMC1",
                    List.empty)))
                result = reader.read(file)
            case "tpch3" =>
                val reader = new XMLResultReader(List((
                    "V_orderkey_LOC1",
                    List("orderkey", "o_orderdate", "o_shippriority"))))
                result = reader.read(file)
            case "tpch5" =>
                val reader = new XMLResultReader(List((
                    "V_suppkey_CSRNOL1",
                    List("n_name"))))
                result = reader.read(file)
            case "tpch10" =>
                val reader = new XMLResultReader(List((
                    "V_orderkey_LOCN1",
                    List("custkey", "c_name", "c_address", "c_phone", "c_acctbal", "c_comment", "n_name"))))
                result = reader.read(file)
            case "tpcds15" =>
                val reader = new XMLResultReader(List((
                    "V_customer_sk_cdcc1",
                    List("ca_zip"))))
                result = reader.read(file)
            case "tpcds19" =>
                val reader = new XMLResultReader(List((
                    "V_date_sk_ssccid1",
                    List("i_brand_id", "i_brand", "i_manufact_id", "i_manufact"))))
                result = reader.read(file)
        }
    }

    override def getResult(): Map[String, List[Double]] = result
}
