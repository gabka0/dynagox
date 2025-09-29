package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrievePipelineResponse;

public interface RetrievePipelineResponseHandler {
    enum Action {
        RETURN_TRUE,
        RETURN_FALSE,
        CONTINUE
    }

    Action handle(RetrievePipelineResponse response);
}
