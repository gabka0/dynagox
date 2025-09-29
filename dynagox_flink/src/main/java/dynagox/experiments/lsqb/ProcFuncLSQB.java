package dynagox.experiments.lsqb;

import dynagox.experiments.common.CommonEventL;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncLSQB extends ProcessFunction<CommonEventL, String> {
    private DynaGoxLSQB6 lsqb6 = null;

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        lsqb6.setId(id);
    }

    public ProcFuncLSQB(String query, double epsilon, boolean print) {
        if (query.equals("lsqb6")) {
            lsqb6 = new DynaGoxLSQB6(epsilon, print);
        }
    }

    @Override
    public void processElement(CommonEventL event, ProcessFunction<CommonEventL, String>.Context context, Collector<String> collector) throws Exception {
        switch (event.rid) {
            case 0:
                lsqb6.insert_into_pkp1(event.f0, event.f1);
                break;
            case 1:
                lsqb6.insert_into_pkp2(event.f0, event.f1);
                break;
            case 2:
                lsqb6.insert_into_phit(event.f0, event.f1);
                break;
            case -1:
                lsqb6.milestone();
                break;
        }
    }
}
