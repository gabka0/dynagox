package duckdb.experiments;

import duckdb.experiments.graph.*;
import duckdb.experiments.lsqb.LSQB6;
import duckdb.experiments.ceb.CEB1;
import duckdb.experiments.ceb.CEB2;
import duckdb.experiments.ceb.CEB3;
import duckdb.experiments.ceb.CEB4;
import duckdb.experiments.tpch.*;
import duckdb.experiments.job.JOB16;
import duckdb.experiments.job.JOB19;
import duckdb.experiments.job.JOB8;
import duckdb.experiments.tpcds.TPCDS15;
import duckdb.experiments.tpcds.TPCDS19;
import org.apache.commons.cli.ParseException;

import java.io.IOException;
import java.sql.*;

public class Main {
    public static void main(String[] args) throws SQLException, IOException, ParseException {
        MainOptions options = MainOptions.parse(args);
        Experiment experiment = null;
        switch (options.query) {
            case "line5c":
                experiment = new Line5c(options.print);
                break;
            case "line5s":
                experiment = new Line5s(options.print);
                break;
            case "line5m":
                experiment = new Line5m(options.print);
                break;
            case "line5cd":
                experiment = new Line5cd(options.print);
                break;
            case "line5gc":
                experiment = new Line5gc(options.print);
                break;
            case "dumbbell":
                experiment = new Dumbbell(options.print);
                break;
            case "lsqb6":
                experiment = new LSQB6(options.print);
                break;
            case "job8":
                experiment = new JOB8(options.print);
                break;
            case "job16":
                experiment = new JOB16(options.print);
                break;
            case "job19":
                experiment = new JOB19(options.print);
                break;
            case "ceb1":
                experiment = new CEB1(options.print);
                break;
            case "ceb2":
                experiment = new CEB2(options.print);
                break;
            case "ceb3":
                experiment = new CEB3(options.print);
                break;
            case "ceb4":
                experiment = new CEB4(options.print);
                break;
            case "tpch3":
                experiment = new TPCH3(options.print);
                break;
            case "tpch5":
                experiment = new TPCH5(options.print);
                break;
            case "tpch8":
                experiment = new TPCH8(options.print);
                break;
            case "tpch9":
                experiment = new TPCH9(options.print);
                break;
            case "tpch10":
                experiment = new TPCH10(options.print);
                break;
            case "tpcds15":
                experiment = new TPCDS15(options.print);
                break;
            case "tpcds19":
                experiment = new TPCDS19(options.print);
                break;
            case "line5c_p":
                experiment = new Line5c(options.print);
                experiment.K = 10;
                break;
            default:
                System.out.println("Unknown query: " + options.query);
                System.exit(1);
        }

        experiment.init();

        experiment.run(options.input);

        experiment.close();
    }


}