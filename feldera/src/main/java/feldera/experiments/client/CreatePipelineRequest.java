package feldera.experiments.client;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.HashMap;
import java.util.Map;

public class CreatePipelineRequest {
    private final static ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    @JsonProperty(value = "name")
    public String name;

    @JsonProperty(value = "program_code")
    public String code;

    @JsonProperty(value = "runtime_config")
    public Map<String, Object> runtimeConfig;

    public CreatePipelineRequest(String name, String code, long cores, long memory) {
        this.name = name;
        this.code = code;
        this.runtimeConfig = new HashMap<>();

        Map<String, Object> resources = new HashMap<>();
        resources.put("cpu_cores_max", cores);
        resources.put("memory_mb_max", memory);
        this.runtimeConfig.put("resources", resources);
    }

    @Override
    public String toString() {
        try {
            return OBJECT_MAPPER.writeValueAsString(this);
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }
}
