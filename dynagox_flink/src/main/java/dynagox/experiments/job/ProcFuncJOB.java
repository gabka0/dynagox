package dynagox.experiments.job;

import dynagox.experiments.common.CommonEvent;
import org.apache.flink.api.common.functions.OpenContext;
import org.apache.flink.streaming.api.functions.ProcessFunction;
import org.apache.flink.util.Collector;

public class ProcFuncJOB extends ProcessFunction<CommonEvent, String> {
    private DynaGoxJOB job = null;

    @Override
    public void open(OpenContext openContext) throws Exception {
        int id = getRuntimeContext().getTaskInfo().getIndexOfThisSubtask();
        job.setId(id);
    }

    public ProcFuncJOB(String query, double epsilon, boolean print) {
        if (query.equals("job8")) {
            job = new DynaGoxJOB8(epsilon, print);
        } else if (query.equals("job16")) {
            job = new DynaGoxJOB16(epsilon, print);
        } else if (query.equals("job19")) {
            job = new DynaGoxJOB19(epsilon, print);
        } else if (query.equals("job8s")) {
            job = new DynaGoxJOB8s(epsilon, print);
        } else if (query.equals("job8m1")) {
            job = new DynaGoxJOB8m1(epsilon, print);
        } else if (query.equals("job8m2")) {
            job = new DynaGoxJOB8m2(epsilon, print);
        }
    }

    @Override
    public void processElement(CommonEvent event, ProcessFunction<CommonEvent, String>.Context context, Collector<String> collector) throws Exception {
        switch (event.rid) {
            case 0:
                job.insert_into_aka_name(event.f0, event.f1);
                break;
            case 1:
                job.insert_into_cast_info(event.f0, event.f1);
                break;
            case 2:
                job.insert_into_movie_companies(event.f0, event.f1);
                break;
            case 3:
                job.insert_into_movie_info(event.f0, event.f1);
                break;
            case 4:
                job.insert_into_movie_keyword(event.f0, event.f1);
                break;
            case -1:
                job.milestone();
                break;
        }
    }
}
