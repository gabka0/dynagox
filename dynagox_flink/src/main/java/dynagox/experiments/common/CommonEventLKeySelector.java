package dynagox.experiments.common;

import org.apache.flink.api.java.functions.KeySelector;

public class CommonEventLKeySelector implements KeySelector<CommonEventL, Integer> {
    @Override
    public Integer getKey(CommonEventL event) throws Exception {
        return (int) event.key;
    }
}
