package dynagox.experiments;

import dynagox.experiments.ceb.FlatMapFuncCEB;
import dynagox.experiments.ceb.ProcFuncCEB;
import dynagox.experiments.common.CommonEventKeySelector;
import dynagox.experiments.common.CommonEventLKeySelector;
import dynagox.experiments.dumbbell.FlatMapFuncDumbbell;
import dynagox.experiments.dumbbell.ProcFuncDumbbell;
import dynagox.experiments.job.FlatMapFuncJOB;
import dynagox.experiments.job.ProcFuncJOB;
import dynagox.experiments.line5.FlatMapFuncLine5;
import dynagox.experiments.line5.ProcFuncLine5;
import dynagox.experiments.lsqb.FlatMapFuncLSQB;
import dynagox.experiments.lsqb.ProcFuncLSQB;
import dynagox.experiments.tpcds.TPCDSEventKeySelector;
import dynagox.experiments.tpcds.tpcds15.FlatMapFuncTPCDS15;
import dynagox.experiments.tpcds.tpcds15.ProcFuncTPCDS15;
import dynagox.experiments.tpcds.tpcds19.FlatMapFuncTPCDS19;
import dynagox.experiments.tpcds.tpcds19.ProcFuncTPCDS19;
import dynagox.experiments.tpch.TPCHEventKeySelector;
import dynagox.experiments.tpch.tpch10.FlatMapFuncTPCH10;
import dynagox.experiments.tpch.tpch10.ProcFuncTPCH10;
import dynagox.experiments.tpch.tpch3.FlatMapFuncTPCH3;
import dynagox.experiments.tpch.tpch3.ProcFuncTPCH3;
import dynagox.experiments.tpch.tpch5.FlatMapFuncTPCH5;
import dynagox.experiments.tpch.tpch5.ProcFuncTPCH5;
import dynagox.experiments.tpch.tpch8.FlatMapFuncTPCH8;
import dynagox.experiments.tpch.tpch8.ProcFuncTPCH8;
import dynagox.experiments.tpch.tpch9.FlatMapFuncTPCH9;
import dynagox.experiments.tpch.tpch9.ProcFuncTPCH9;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.connector.file.src.FileSource;
import org.apache.flink.connector.file.src.reader.TextLineInputFormat;
import org.apache.flink.core.fs.Path;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.functions.sink.v2.DiscardingSink;

public class Main {

    public static void main(String[] args) throws Exception {
        MainOptions options = MainOptions.parse(args);

        final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
        env.setParallelism(16);

        final FileSource<String> source = FileSource.forRecordStreamFormat(
                new TextLineInputFormat(),
                new Path(options.input)).build();
        final DataStream<String> stream = env.fromSource(source, WatermarkStrategy.noWatermarks(),
                options.query + "-source");

        if (options.query.startsWith("line5")) {
            stream.flatMap(new FlatMapFuncLine5())
					.partitionCustom(new Partitioner(), new CommonEventKeySelector())
                    .process(new ProcFuncLine5(options.query, options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.startsWith("dumbbell")) {
            stream.flatMap(new FlatMapFuncDumbbell())
                    .partitionCustom(new Partitioner(), new CommonEventKeySelector())
                    .process(new ProcFuncDumbbell(options.query, options.epsilon, options.print))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.startsWith("lsqb")) {
            stream.flatMap(new FlatMapFuncLSQB())
                    .partitionCustom(new Partitioner(), new CommonEventLKeySelector())
                    .process(new ProcFuncLSQB(options.query, options.epsilon, options.print))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.startsWith("ceb")) {
            stream.flatMap(new FlatMapFuncCEB())
                    .partitionCustom(new Partitioner(), new CommonEventKeySelector())
                    .process(new ProcFuncCEB(options.query, options.epsilon, options.print))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.startsWith("job")) {
            stream.flatMap(new FlatMapFuncJOB())
                    .partitionCustom(new Partitioner(), new CommonEventKeySelector())
                    .process(new ProcFuncJOB(options.query, options.epsilon, options.print))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpch3")) {
            stream.flatMap(new FlatMapFuncTPCH3())
                    .partitionCustom(new Partitioner(), new TPCHEventKeySelector())
                    .process(new ProcFuncTPCH3(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpch5")) {
            stream.flatMap(new FlatMapFuncTPCH5())
                    .partitionCustom(new Partitioner(), new TPCHEventKeySelector())
                    .process(new ProcFuncTPCH5(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpch8")) {
            stream.flatMap(new FlatMapFuncTPCH8())
                    .partitionCustom(new Partitioner(), new TPCHEventKeySelector())
                    .process(new ProcFuncTPCH8(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpch9")) {
            stream.flatMap(new FlatMapFuncTPCH9())
                    .partitionCustom(new Partitioner(), new TPCHEventKeySelector())
                    .process(new ProcFuncTPCH9(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpch10")) {
            stream.flatMap(new FlatMapFuncTPCH10())
                    .partitionCustom(new Partitioner(), new TPCHEventKeySelector())
                    .process(new ProcFuncTPCH10(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpcds15")) {
            stream.flatMap(new FlatMapFuncTPCDS15())
                    .partitionCustom(new Partitioner(), new TPCDSEventKeySelector())
                    .process(new ProcFuncTPCDS15(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        } else if (options.query.equals("tpcds19")) {
            stream.flatMap(new FlatMapFuncTPCDS19())
                    .partitionCustom(new Partitioner(), new TPCDSEventKeySelector())
                    .process(new ProcFuncTPCDS19(options.epsilon, options.print, options.is_delta))
                    .sinkTo(new DiscardingSink<>());
        }

        long t0 = System.currentTimeMillis();
        env.execute(options.query);
        long t1 = System.currentTimeMillis();
        System.out.println("Running Time: " + (t1 - t0) + " ms");
        System.out.println("Memory Usage: " + Utils.getMaxMemoryUsage() + " kB");
    }
}
