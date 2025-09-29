package dynagox.experiments.lsqb;

import dynagox.experiments.common.CommonEvent;
import dynagox.experiments.common.CommonEventL;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncLSQB implements FlatMapFunction<String, CommonEventL> {
    @Override
    public void flatMap(String value, Collector<CommonEventL> out) throws Exception {
        if (value.equals("*")) {
            for (int key = 0; key < 16; key++) {
                out.collect(new CommonEventL(key, -1, 0, 0));
            }
        } else {
            String[] fields = value.split("\\|");
            int rid = Integer.parseInt(fields[0]);
            long f0 = Long.parseLong(fields[1]);
            long f1 = Long.parseLong(fields[2]);
            switch (rid) {
                case 0:
                    for (int key = 0; key < 16; key++) {
                        out.collect(new CommonEventL(key, 0, f0, f1));
                    }
                    out.collect(new CommonEventL((f1 % 16), 1, f0, f1));
                    break;
                case 1:
                    out.collect(new CommonEventL((f0 % 16), 2, f0, f1));
                    break;
            }
        }
    }
}
