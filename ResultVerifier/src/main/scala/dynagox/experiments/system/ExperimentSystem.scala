package dynagox.experiments.system

trait ExperimentSystem {
    def getName(): String
    def isApproximated(): Boolean = false
    def load(): Unit
    def getResult(): Map[String, List[Double]]
}
