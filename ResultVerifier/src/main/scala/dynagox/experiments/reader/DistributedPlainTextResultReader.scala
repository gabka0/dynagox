package dynagox.experiments.reader

import scala.collection.mutable
import scala.io.Source

class DistributedPlainTextResultReader(groupBy: Boolean, size: Int, func: String) extends ResultReader {
    override def read(file: String): Map[String, List[Double]] = {
        val source = Source.fromFile(file)
        val lines = source.getLines().toList
        source.close()

        val hashMaps = List.fill(16)(new mutable.HashMap[String, List[Double]]())
        if (groupBy) {
            if (size == 1) {
                val pattern = """^(\d{1,2})> \((.*?)\) -> (.*?)""".r


                for (line <- lines) {
                    line match {
                        case pattern(id, tuple, annotation) =>
                            if (Math.abs(annotation.toDouble) > 0.001)
                                hashMaps(id.toInt)(tuple) = List(annotation.toDouble)
                        case _ =>
                    }
                }
            } else {
                val pattern = """^(\d{1,2})> \((.*?)\) -> \((.*?)\)""".r

                for (line <- lines) {
                    line match {
                        case pattern(id, tuple, annotations) =>
                            hashMaps(id.toInt)(tuple) = annotations.split(",").map(a => a.toDouble).toList
                        case _ =>
                    }
                }
            }
        } else {
            assert(size == 1)
            val pattern = """^(\d{1,2})> (.*?)""".r
            for (line <- lines) {
                line match {
                    case pattern(id, annotation) =>
                        hashMaps(id.toInt)("val") = List(annotation.toDouble)
                    case _ =>
                }
            }
        }

        val result = new mutable.HashMap[String, List[Double]]()
        for (id <- (0 until 16)) {
            for (entry <- hashMaps(id)) {
                val tuple = entry._1
                val annotations = entry._2
                if (result.contains(tuple)) {
                    result(tuple) = merge(result(tuple), annotations)
                } else {
                    result(tuple) = annotations
                }
            }
        }

        result.toMap
    }

    private def merge(a: List[Double], b: List[Double]): List[Double] = {
        func match {
            case "SUM" =>
                a.zip(b).map(t => t._1 + t._2)
            case "MAX" =>
                a.zip(b).map(t => t._1 max t._2)
        }
    }
}
