package dynagox.experiments.reader

trait ResultReader {
    def read(file: String): Map[String, List[Double]]
}
