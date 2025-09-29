package dynagox.experiments.ceb;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncCEB implements FlatMapFunction<String, CommonEvent> {
    @Override
    public void flatMap(String value, Collector<CommonEvent> out) throws Exception {
        if (value.equals("*")) {
            for (int key = 0; key < 16; key++) {
                out.collect(new CommonEvent(key, -1, 0, 0));
            }
        } else {
            String[] fields = value.split("\\|");
            int rid = Integer.parseInt(fields[0]);
            int f0 = Integer.parseInt(fields[1]);
            int f1 = Integer.parseInt(fields[2]);
            int f2 = fields.length > 3 ? Integer.parseInt(fields[3]) : 0;
            switch (rid) {
                case 0:
                    for (int key = 0; key < 16; key++) {
                        out.collect(new CommonEvent(key, 0, f0, f1));
                    }
                    break;
                case 1:
                    out.collect(new CommonEvent((f2 % 16), 1, f1, f2));
                    break;
                case 2:
                    out.collect(new CommonEvent((f1 % 16), 2, f0, f1));
                    break;
                case 3:
                    out.collect(new CommonEvent((f1 % 16), 3, f0, f1));
                    break;
                case 4:
                    out.collect(new CommonEvent((f1 % 16), 4, f0, f1));
                    break;
                case 5:
                    out.collect(new CommonEvent((f1 % 16), 5, f0, f1));
                    break;
                case 6:
                    for (int key = 0; key < 16; key++) {
                        out.collect(new CommonEvent(key, 6, f0, f1));
                    }
                    break;
            }
        }
    }
}
