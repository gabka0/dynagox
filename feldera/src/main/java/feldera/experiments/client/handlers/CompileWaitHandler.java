package feldera.experiments.client.handlers;

import feldera.experiments.client.RetrievePipelineResponse;

public class CompileWaitHandler implements RetrievePipelineResponseHandler {
    @Override
    public RetrievePipelineResponseHandler.Action handle(RetrievePipelineResponse response) {
        if (response.program_status.equals("Success")) {
            return RetrievePipelineResponseHandler.Action.RETURN_TRUE;
        } else if (response.program_status.equals("Pending") ||
                response.program_status.equals("CompilingSql") ||
                response.program_status.equals("SqlCompiled") ||
                response.program_status.equals("CompilingRust")) {
            return RetrievePipelineResponseHandler.Action.CONTINUE;
        } else {
            return RetrievePipelineResponseHandler.Action.RETURN_FALSE;
        }
    }
}
