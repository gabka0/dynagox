package dynagox.experiments.dumbbell;

import dynagox.experiments.common.CommonEvent;
import dynagox.experiments.line5.*;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncDumbbell extends ProcessFunction<CommonEvent, String> {
    private DynaGoxDumbbell dumbbell = null;

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        dumbbell.setId(id);
    }

    public ProcFuncDumbbell(String query, double epsilon, boolean print) {
        if (query.equals("dumbbell")) {
            dumbbell = new DynaGoxDumbbell(epsilon, print);
        }
    }

    @Override
    public void processElement(CommonEvent event, ProcessFunction<CommonEvent, String>.Context context, Collector<String> collector) throws Exception {
        switch (event.rid) {
            case 0:
                if (event.isInsert) {
                    dumbbell.insert_into_r1(event.f0, event.f1);
                }
                break;
            case 1:
                if (event.isInsert) {
                    dumbbell.insert_into_r2(event.f0, event.f1);
                }
                break;
            case 2:
                if (event.isInsert) {
                    dumbbell.insert_into_r3(event.f0, event.f1);
                }
                break;
            case 3:
                if (event.isInsert) {
                    dumbbell.insert_into_r4(event.f0, event.f1);
                }
                break;
            case 4:
                if (event.isInsert) {
                    dumbbell.insert_into_r5(event.f0, event.f1);
                }
                break;
            case 5:
                if (event.isInsert) {
                    dumbbell.insert_into_r6(event.f0, event.f1);
                }
                break;
            case 6:
                if (event.isInsert) {
                    dumbbell.insert_into_r7(event.f0, event.f1);
                }
                break;
            case -1:
                dumbbell.milestone();
                break;
        }
    }
}
