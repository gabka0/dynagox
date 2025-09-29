package dynagox.experiments.reader

import com.fasterxml.jackson.databind.ObjectMapper

import scala.collection.mutable
import scala.collection.mutable.ListBuffer
import scala.io.Source

class JSONResultReader(columns: List[(String, String)], values: List[String]) extends ResultReader {
    override def read(file: String): Map[String, List[Double]] = {
        val source = Source.fromFile(file)
        val lines = source.getLines().toList
        source.close()

        val hashMap = new mutable.HashMap[String, List[Double]]()
        val mapper = new ObjectMapper()
        for (line <- lines) {
            val node = mapper.readTree(line)
            val tuple = node.get("insert")
            if (tuple != null) {
                val buffer = new ListBuffer[String]()
                if (columns.nonEmpty) {
                    for ((columnName, columnType) <- columns) {
                        if (columnType == "Date") {
                            val field = tuple.get(columnName).asText()
                            buffer.append(field.replaceAll("-", ""))
                        } else if (columnType == "Int") {
                            val field = tuple.get(columnName).asInt()
                            buffer.append(field.toString)
                        } else if (columnType == "Double") {
                            val field = tuple.get(columnName).asDouble().toString
                                .replaceAll("\\.0+$", "")
                                .replaceAll("(\\.\\d*[1-9])0+$", "$1")
                            buffer.append(field)
                        } else if (columnType == "String") {
                            val field = tuple.get(columnName).asText()
                            buffer.append(field)
                        } else {
                            val field = tuple.get(columnName).asText()
                            buffer.append(field)
                        }
                    }
                    val annotations = values.map(v => tuple.get(v).asDouble())
                    hashMap(buffer.mkString(",")) = annotations
                } else {
                    assert(values.size == 1)
                    val annotation = tuple.get(values.head).asDouble()
                    hashMap("val") = List(annotation)
                }
            }
        }

        hashMap.toMap
    }
}
