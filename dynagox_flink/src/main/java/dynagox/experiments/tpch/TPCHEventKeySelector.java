package dynagox.experiments.tpch;

import org.apache.flink.api.java.functions.KeySelector;

public class TPCHEventKeySelector implements KeySelector<TPCHEvent, Integer> {
    @Override
    public Integer getKey(TPCHEvent event) throws Exception {
        return event.key;
    }
}
