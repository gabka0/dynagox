package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrieveStatisticsResponse;

public class RunWaitHandler implements RetrieveStatisticsResponseHandler {
    long timeoutIdle;
    long previousTotalInputRecords = 0;
    long previousTotalProcessedRecords = 0;
    long idleTimeMillis = -1;
    long maxUsedMemory = 0;

    public RunWaitHandler(long timeoutIdle) {
        this.timeoutIdle = timeoutIdle;
    }

    @Override
    public Action handle(RetrieveStatisticsResponse response) {
        if (response.getRssBytes() > maxUsedMemory) {
            maxUsedMemory = response.getRssBytes();
        }

        boolean unchanged = (response.getTotalInputRecords() == previousTotalInputRecords
                && response.getTotalProcessedRecords() == previousTotalProcessedRecords);
        boolean equal = (response.getTotalInputRecords() == response.getTotalProcessedRecords());

        previousTotalInputRecords = response.getTotalInputRecords();
        previousTotalProcessedRecords = response.getTotalProcessedRecords();

        if (unchanged && equal) {
            if (idleTimeMillis < 0) {
                idleTimeMillis = System.currentTimeMillis();
            }
        } else {
            idleTimeMillis = -1;
        }

        if (idleTimeMillis > 0 && System.currentTimeMillis() - idleTimeMillis > timeoutIdle) {
            return RetrieveStatisticsResponseHandler.Action.RETURN_TRUE;
        } else {
            return RetrieveStatisticsResponseHandler.Action.CONTINUE;
        }
    }

    public long getMaxUsedMemory() {
        return maxUsedMemory;
    }
}
