package dynagox.experiments;

public class Partitioner implements org.apache.flink.api.common.functions.Partitioner<Integer> {
    @Override
    public int partition(Integer key, int numPartitions) {
        return key % numPartitions;
    }
}
