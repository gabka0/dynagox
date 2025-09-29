package dynagox.experiments.reader

import scala.collection.mutable
import scala.io.Source

class PlainTextResultReader(groupBy: Boolean, size: Int) extends ResultReader {
    override def read(file: String): Map[String, List[Double]] = {
        val source = Source.fromFile(file)
        val lines = source.getLines().toList
        source.close()

        if (groupBy) {
            if (size == 1) {
                val pattern = """\((.*?)\) -> (.*?)""".r
                val hashMap = new mutable.HashMap[String, List[Double]]()

                for (line <- lines) {
                    line match {
                        case pattern(tuple, annotation) =>
                            if (Math.abs(annotation.toDouble) > 0.001)
                                hashMap(tuple) = List(annotation.toDouble)
                        case _ =>
                    }
                }

                hashMap.toMap
            } else {
                val pattern = """\((.*?)\) -> \((.*?)\)""".r
                val hashMap = new mutable.HashMap[String, List[Double]]()

                for (line <- lines) {
                    line match {
                        case pattern(tuple, annotations) =>
                            hashMap(tuple) = annotations.split(",").map(a => a.toDouble).toList
                        case _ =>
                    }
                }

                hashMap.toMap
            }
        } else {
            assert(size == 1)
            val matched = lines.filter(s => s.nonEmpty && s.charAt(0).isDigit && !s.contains("ms") && !s.contains("kB"))
            val value = matched.last.toDouble
            Map("val" -> List(value))
        }
    }
}
