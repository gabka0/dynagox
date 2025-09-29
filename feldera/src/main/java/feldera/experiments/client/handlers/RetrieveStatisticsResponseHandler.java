package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrieveStatisticsResponse;

public interface RetrieveStatisticsResponseHandler {
    enum Action {
        RETURN_TRUE,
        RETURN_FALSE,
        CONTINUE
    }

    Action handle(RetrieveStatisticsResponse response);
}
