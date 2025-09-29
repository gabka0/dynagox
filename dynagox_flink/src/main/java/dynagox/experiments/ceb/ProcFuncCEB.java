package dynagox.experiments.ceb;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncCEB extends ProcessFunction<CommonEvent, String> {
    private DynaGoxCEB ceb = null;

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        ceb.setId(id);
    }

    public ProcFuncCEB(String query, double epsilon, boolean print) {
        if (query.equals("ceb1")) {
            ceb = new DynaGoxCEB1(epsilon, print);
        } else if (query.equals("ceb2")) {
            ceb = new DynaGoxCEB2(epsilon, print);
        } else if (query.equals("ceb3")) {
            ceb = new DynaGoxCEB3(epsilon, print);
        } else if (query.equals("ceb4")) {
            ceb = new DynaGoxCEB4(epsilon, print);
        }
    }

    @Override
    public void processElement(CommonEvent event, ProcessFunction<CommonEvent, String>.Context context, Collector<String> collector) throws Exception {
        switch (event.rid) {
            case 0:
                ceb.insert_into_aka_name(event.f0, event.f1);
                break;
            case 1:
                ceb.insert_into_cast_info(event.f0, event.f1);
                break;
            case 2:
                ceb.insert_into_movie_companies(event.f0, event.f1);
                break;
            case 3:
                ceb.insert_into_movie_info(event.f0, event.f1);
                break;
            case 4:
                ceb.insert_into_movie_info_idx1(event.f0, event.f1);
                ceb.insert_into_movie_info_idx2(event.f0, event.f1);
                break;
            case 5:
                ceb.insert_into_movie_keyword(event.f0, event.f1);
                break;
            case 6:
                ceb.insert_into_person_info(event.f0, event.f1);
                break;
            case -1:
                ceb.milestone();
                break;
        }
    }
}
