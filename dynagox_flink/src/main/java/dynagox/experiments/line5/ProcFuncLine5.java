package dynagox.experiments.line5;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncLine5 extends ProcessFunction<CommonEvent, String> {
    private DynaGoxLine5 line5 = null;

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        line5.setId(id);
    }

    public ProcFuncLine5(String query, double epsilon, boolean print, boolean is_delta) {
        if (query.equals("line5c")) {
            line5 = new DynaGoxLine5c(epsilon, print);
        } else if (query.equals("line5s")) {
            line5 = new DynaGoxLine5s(epsilon, print);
        } else if (query.equals("line5m")) {
            line5 = new DynaGoxLine5m(epsilon, print);
        } else if (query.equals("line5cd")) {
            line5 = new DynaGoxLine5cd(epsilon, print);
        } else if (query.equals("line5gc")) {
            line5 = new DynaGoxLine5gc(epsilon, print, is_delta);
        }
    }

    @Override
    public void processElement(CommonEvent event, ProcessFunction<CommonEvent, String>.Context context, Collector<String> collector) throws Exception {
        switch (event.rid) {
            case 0:
                if (event.isInsert) {
                    line5.insert_into_r1(event.f0, event.f1);
                } else {
                    line5.delete_from_r1(event.f0, event.f1);
                }
                break;
            case 1:
                if (event.isInsert) {
                    line5.insert_into_r2(event.f0, event.f1);
                } else {
                    line5.delete_from_r2(event.f0, event.f1);
                }
                break;
            case 2:
                if (event.isInsert) {
                    line5.insert_into_r3(event.f0, event.f1);
                } else {
                    line5.delete_from_r3(event.f0, event.f1);
                }
                break;
            case 3:
                if (event.isInsert) {
                    line5.insert_into_r4(event.f0, event.f1);
                } else {
                    line5.delete_from_r4(event.f0, event.f1);
                }
                break;
            case 4:
                if (event.isInsert) {
                    line5.insert_into_r5(event.f0, event.f1);
                } else {
                    line5.delete_from_r5(event.f0, event.f1);
                }
                break;
            case -1:
                line5.milestone();
                break;
        }
    }
}
