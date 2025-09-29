package dynagox.experiments.dumbbell;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.util.Collector;

public class FlatMapFuncDumbbell implements FlatMapFunction<String, CommonEvent> {
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

            for (int key = 0; key < 16; key++) {
                out.collect(new CommonEvent(key, isInsert, 0, src, dst));
            }

            for (int d = 0; d < 4; d++) {
                out.collect(new CommonEvent(getKey(dst % 4, d), isInsert, 1, src, dst));
            }

            for (int d = 0; d < 4; d++) {
                out.collect(new CommonEvent(getKey(src % 4, d), isInsert, 2, src, dst));
            }

            out.collect(new CommonEvent(getKey(src % 4, dst % 4), isInsert, 3, src, dst));

            for (int c = 0; c < 4; c++) {
                out.collect(new CommonEvent(getKey(c, src % 4), isInsert, 4, src, dst));
            }

            for (int key = 0; key < 16; key++) {
                out.collect(new CommonEvent(key, isInsert, 5, src, dst));
            }

            for (int c = 0; c < 4; c++) {
                out.collect(new CommonEvent(getKey(c, dst % 4), isInsert, 6, src, dst));
            }
        }
    }

    private int getKey(int c, int d) {
        return c * 4 + d;
    }
}
