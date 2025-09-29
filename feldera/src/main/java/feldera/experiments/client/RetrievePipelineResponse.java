package feldera.experiments.client;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

@JsonIgnoreProperties(ignoreUnknown = true)
public class RetrievePipelineResponse {
    private final static ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    public String id;
    public String name;
    public String program_status;
    public String deployment_status;

    public static RetrievePipelineResponse fromString(String string) throws JsonProcessingException {
        return OBJECT_MAPPER.readValue(string, RetrievePipelineResponse.class);
    }
}
