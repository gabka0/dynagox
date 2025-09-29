package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrievePipelineResponse;

public class StartWaitHandler implements RetrievePipelineResponseHandler {
    @Override
    public RetrievePipelineResponseHandler.Action handle(RetrievePipelineResponse response) {
        if (response.deployment_status.equals("Running")) {
            return RetrievePipelineResponseHandler.Action.RETURN_TRUE;
        } else if (response.deployment_status.equals("Failed")) {
            return RetrievePipelineResponseHandler.Action.RETURN_FALSE;
        } else {
            return RetrievePipelineResponseHandler.Action.CONTINUE;
        }
    }
}
