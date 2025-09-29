package dynagox.experiments.tpcds;

import org.apache.flink.api.java.functions.KeySelector;

public class TPCDSEventKeySelector implements KeySelector<TPCDSEvent, Integer> {
    @Override
    public Integer getKey(TPCDSEvent event) throws Exception {
        return event.key;
    }
}
