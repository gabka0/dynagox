package dynagox.experiments.reader

import scala.collection.mutable
import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.xml.XML

class XMLResultReader(val tags: List[(String, List[String])]) extends ResultReader {
    override def read(file: String): Map[String, List[Double]] = {
        val source = Source.fromFile(file)
        val result = source.mkString
        source.close()

        val beginIndex = result.indexOf(s"<${tags.head._1}>")
        val endIndex = result.indexOf(s"</${tags.last._1}>") + s"</${tags.last._1}>".length
        val content = result.substring(beginIndex, endIndex)

        val hashMap = new mutable.HashMap[String, ListBuffer[Double]]()
        val xml = XML.loadString(s"<root>${content}</root>")

        for ((aggregate, columns) <- tags) {
            if (columns.nonEmpty) {
                val items = ((xml \ aggregate) \\ "item").toList
                for (item <- items) {
                    val tuple = new ListBuffer[String]
                    for (column <- columns) {
                        tuple.append((item \ column).text)
                    }
                    val annotation = (item \ "__av").text.toDouble

                    if (Math.abs(annotation) > 0.001) {
                        val key = tuple.mkString(",")
                        if (hashMap.contains(key)) {
                            hashMap(key).append(annotation)
                        } else {
                            val buffer = new ListBuffer[Double]
                            buffer.append(annotation)
                            hashMap(key) = buffer
                        }
                    }
                }
            } else {
                val value = (xml \\ aggregate).text.toDouble
                val buffer = new ListBuffer[Double]
                buffer.append(value)
                hashMap("val") = buffer
            }
        }

        assert(hashMap.forall(t => t._2.size == tags.size))
        hashMap.map(t => (t._1, t._2.toList)).toMap
    }
}
