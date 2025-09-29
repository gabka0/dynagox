package feldera.experiments.client;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

@JsonIgnoreProperties(ignoreUnknown = true)
public class RetrieveStatisticsResponse {
    private final static ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    private long total_input_records = 0;
    private long total_processed_records = 0;
    private long rss_bytes = 0;

    public long getTotalInputRecords() {
        return total_input_records;
    }

    public long getTotalProcessedRecords() {
        return total_processed_records;
    }

    public long getRssBytes() {
        return rss_bytes;
    }

    public static RetrieveStatisticsResponse fromString(String string) throws JsonProcessingException {
        RetrieveStatisticsResponse response = new RetrieveStatisticsResponse();
        JsonNode node =  OBJECT_MAPPER.readTree(string);

        response.total_input_records = node.get("global_metrics").get("total_input_records").asLong();
        response.total_processed_records = node.get("global_metrics").get("total_processed_records").asLong();
        response.rss_bytes = node.get("global_metrics").get("rss_bytes").asLong();

        return response;
    }
}


