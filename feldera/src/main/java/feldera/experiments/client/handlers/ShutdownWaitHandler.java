package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrievePipelineResponse;

public class ShutdownWaitHandler implements RetrievePipelineResponseHandler {
    @Override
    public Action handle(RetrievePipelineResponse response) {
        if (response.deployment_status.equals("Shutdown")) {
            return Action.RETURN_TRUE;
        } else {
            return Action.CONTINUE;
        }
    }
}
