package dynagox.experiments.line5;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncLine5 implements FlatMapFunction<String, CommonEvent> {
    @Override
    public void flatMap(String value, Collector<CommonEvent> out) throws Exception {
        if (value.equals("*")) {
            for (int key = 0; key < 16; key++) {
                out.collect(new CommonEvent(key, -1, 0, 0));
            }
        } else {
            String[] fields = value.split("\\|");
            boolean isInsert = (fields[0].equals("+"));
            int src = Integer.parseInt(fields[1]);
            int dst = Integer.parseInt(fields[2]);

            for (int c = 0; c < 2; c++) {
                for (int d = 0; d < 2; d++) {
                    for (int e = 0; e < 2; e++) {
                        out.collect(new CommonEvent(getKey(dst % 2, c, d, e), isInsert, 0, src, dst));
                    }
                }
            }

            for (int d = 0; d < 2; d++) {
                for (int e = 0; e < 2; e++) {
                    out.collect(new CommonEvent(getKey(src % 2, dst % 2, d, e), isInsert, 1, src, dst));
                }
            }

            for (int b = 0; b < 2; b++) {
                for (int e = 0; e < 2; e++) {
                    out.collect(new CommonEvent(getKey(b, src % 2, dst % 2, e), isInsert, 2, src, dst));
                }
            }

            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    out.collect(new CommonEvent(getKey(b, c, src % 2, dst % 2), isInsert, 3, src, dst));
                }
            }

            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    for (int d = 0; d < 2; d++) {
                        out.collect(new CommonEvent(getKey(b, c, d, src % 2), isInsert, 4, src, dst));
                    }
                }
            }
        }
    }

    private int getKey(int b, int c, int d, int e) {
        return b * 8 + c * 4 + d * 2 + e;
    }
}
