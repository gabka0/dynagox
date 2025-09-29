package dynagox.experiments.common;

import org.apache.flink.api.java.functions.KeySelector;

public class CommonEventKeySelector implements KeySelector<CommonEvent, Integer> {
    @Override
    public Integer getKey(CommonEvent event) throws Exception {
        return event.key;
    }
}
