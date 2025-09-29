package feldera.experiments.client;

public class Result {
    boolean success = false;
    long maxUsedMemory = 0;

    public Result(boolean success, long maxUsedMemory) {
        this.success = success;
        this.maxUsedMemory = maxUsedMemory;
    }

    public boolean isSuccess() {
        return success;
    }

    public long getMaxUsedMemory() {
        return maxUsedMemory;
    }
}
