package dynagox.experiments

import dynagox.experiments.system.{CrownDelta, CrownFull, DBToaster, DuckDB, DynaGoxCppDelta, DynaGoxCppFull, DynaGoxFlinkDelta, DynaGoxFlinkFull, ExperimentSystem, FIVM, Feldera}
import org.apache.commons.cli.{DefaultParser, Options}
import org.apache.commons.cli.Option

import scala.collection.mutable.ListBuffer

object MainOptions {
    val QUERY_OPTION = new Option("q", "query", true, "The name of the query.")
    val FIVM_OUT_PATH_OPTION = new Option("a", true, "The path to FIVM verify out file.")
    val DBTOASTER_OUT_PATH_OPTION = new Option("b", true, "The path to DBToaster verify out file.")
    val DYNAGOX_CPP_FULL_OUT_PATH_OPTION = new Option("c", true, "The path to DynaGox(C++)[F] verify out file.")
    val DYNAGOX_CPP_DELTA_OUT_PATH_OPTION = new Option("d", true, "The path to DynaGox(C++)[D] verify out file.")
    val DYNAGOX_FLINK_FULL_OUT_PATH_OPTION = new Option("e", true, "The path to DynaGox(Flink)[F] verify out file.")
    val DYNAGOX_FLINK_DELTA_OUT_PATH_OPTION = new Option("f", true, "The path to DynaGox(Flink)[D] verify out file.")
    val CROWN_FULL_OUT_PATH_OPTION = new Option("g", true, "The path to Crown[F] verify out file.")
    val CROWN_DELTA_OUT_PATH_OPTION = new Option("h", true, "The path to Crown[D] verify out file.")
    val FELDERA_OUT_PATH_OPTION = new Option("i", true, "The path to feldera verify out file.")
    val DUCKDB_OUT_PATH_OPTION = new Option("j", true, "The path to DuckDB verify out file.")

    def getOptions(): Options = {
        val options = new Options
        options.addOption(QUERY_OPTION)
        options.addOption(FIVM_OUT_PATH_OPTION)
        options.addOption(DBTOASTER_OUT_PATH_OPTION)
        options.addOption(DYNAGOX_CPP_FULL_OUT_PATH_OPTION)
        options.addOption(DYNAGOX_CPP_DELTA_OUT_PATH_OPTION)
        options.addOption(DYNAGOX_FLINK_FULL_OUT_PATH_OPTION)
        options.addOption(DYNAGOX_FLINK_DELTA_OUT_PATH_OPTION)
        options.addOption(CROWN_FULL_OUT_PATH_OPTION)
        options.addOption(CROWN_DELTA_OUT_PATH_OPTION)
        options.addOption(FELDERA_OUT_PATH_OPTION)
        options.addOption(DUCKDB_OUT_PATH_OPTION)
        options
    }

    def parse(args: Array[String]): (String, List[ExperimentSystem]) = {
        val defaultParser = new DefaultParser
        val commandLine = defaultParser.parse(MainOptions.getOptions(), args, true)

        if (!commandLine.hasOption(QUERY_OPTION.getOpt)) {
            println("Missing required argument: -q,--query <name>")
            System.exit(1)
        }
        val query = commandLine.getOptionValue(QUERY_OPTION.getOpt)

        val buffer = ListBuffer.empty[ExperimentSystem]
        if (commandLine.hasOption(FIVM_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(FIVM_OUT_PATH_OPTION.getOpt)
            buffer.append(new FIVM(query, file))
        }

        if (commandLine.hasOption(DBTOASTER_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DBTOASTER_OUT_PATH_OPTION.getOpt)
            buffer.append(new DBToaster(query, file))
        }

        if (commandLine.hasOption(DYNAGOX_CPP_FULL_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DYNAGOX_CPP_FULL_OUT_PATH_OPTION.getOpt)
            buffer.append(new DynaGoxCppFull(query, file))
        }

        if (commandLine.hasOption(DYNAGOX_CPP_DELTA_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DYNAGOX_CPP_DELTA_OUT_PATH_OPTION.getOpt)
            buffer.append(new DynaGoxCppDelta(query, file))
        }

        if (commandLine.hasOption(DYNAGOX_FLINK_FULL_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DYNAGOX_FLINK_FULL_OUT_PATH_OPTION.getOpt)
            buffer.append(new DynaGoxFlinkFull(query, file))
        }

        if (commandLine.hasOption(DYNAGOX_FLINK_DELTA_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DYNAGOX_FLINK_DELTA_OUT_PATH_OPTION.getOpt)
            buffer.append(new DynaGoxFlinkDelta(query, file))
        }

        if (commandLine.hasOption(CROWN_FULL_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(CROWN_FULL_OUT_PATH_OPTION.getOpt)
            buffer.append(new CrownFull(query, file))
        }

        if (commandLine.hasOption(CROWN_DELTA_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(CROWN_DELTA_OUT_PATH_OPTION.getOpt)
            buffer.append(new CrownDelta(query, file))
        }

        if (commandLine.hasOption(FELDERA_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(FELDERA_OUT_PATH_OPTION.getOpt)
            buffer.append(new Feldera(query, file))
        }

        if (commandLine.hasOption(DUCKDB_OUT_PATH_OPTION.getOpt)) {
            val file = commandLine.getOptionValue(DUCKDB_OUT_PATH_OPTION.getOpt)
            buffer.append(new DuckDB(query, file))
        }

        (query, buffer.toList)
    }
}
