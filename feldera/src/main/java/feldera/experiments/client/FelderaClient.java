package feldera.experiments.client;

import feldera.experiments.client.handlers.*;
import org.apache.http.client.methods.*;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;

public class FelderaClient {
    public final static String BASE_URI = "http://localhost:8080/v0/";
    public final static int PIPELINE_CORES_MAX = 1;
    public final static int PIPELINE_MEMORY_MAX = 1024 * 1024;
    public final static int WAIT_INTERVAL_MS = 500;
    public final static int CREATE_TIMEOUT_MS = 60 * 1000;
    public final static int START_TIMEOUT_MS = 30 * 1000;
    public final static int SHUTDOWN_TIMEOUT_MS = 30 * 1000;

    public boolean createPipeline(String name, String code) {
        CreatePipelineRequest request = new CreatePipelineRequest(name, code, PIPELINE_CORES_MAX, PIPELINE_MEMORY_MAX);

        try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
            HttpPut httpPut = new HttpPut(BASE_URI + "/pipelines/" + name);
            String body = request.toString();
            StringEntity stringEntity = new StringEntity(body);
            httpPut.setEntity(stringEntity);
            httpPut.setHeader("Content-Type", "application/json");

            try (CloseableHttpResponse httpResponse = httpClient.execute(httpPut)) {
                if (httpResponse.getStatusLine().getStatusCode() != 201) {
                    System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                    return false;
                } else {
                    return wait(name, new CompileWaitHandler(), CREATE_TIMEOUT_MS);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean startPipeline(String name) {
        try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
            HttpPost httpPost = new HttpPost(BASE_URI + "/pipelines/" + name + "/start");

            try (CloseableHttpResponse httpResponse = httpClient.execute(httpPost)) {
                if (httpResponse.getStatusLine().getStatusCode() != 202) {
                    System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                    return false;
                } else {
                    return wait(name, new StartWaitHandler(), START_TIMEOUT_MS);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean shutdownPipeline(String name) {
        try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
            HttpPost httpPost = new HttpPost(BASE_URI + "/pipelines/" + name + "/shutdown");

            try (CloseableHttpResponse httpResponse = httpClient.execute(httpPost)) {
                if (httpResponse.getStatusLine().getStatusCode() != 202) {
                    System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                    return false;
                } else {
                    return wait(name, new ShutdownWaitHandler(), SHUTDOWN_TIMEOUT_MS);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean deletePipeline(String name) {
        try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
            HttpDelete httpDelete = new HttpDelete(BASE_URI + "/pipelines/" + name);

            try (CloseableHttpResponse httpResponse = httpClient.execute(httpDelete)) {
                if (httpResponse.getStatusLine().getStatusCode() != 200) {
                    System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                    return false;
                } else {
                    return true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public Result waitUntilIdle(String name, long timeoutIdle, long timeoutTotal) {
        RunWaitHandler handler = new RunWaitHandler(timeoutIdle);
        boolean success = wait(name, handler, timeoutTotal);
        if (success) {
            return new Result(true, handler.getMaxUsedMemory());
        } else {
            return new Result(false, handler.getMaxUsedMemory());
        }
    }

    public boolean deletePipelineIfExist(String name) {
        try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
            HttpGet httpGet = new HttpGet(BASE_URI + "/pipelines/" + name);

            try (CloseableHttpResponse httpResponse = httpClient.execute(httpGet)) {
                if (httpResponse.getStatusLine().getStatusCode() == 404) {
                    return true;
                } else if (httpResponse.getStatusLine().getStatusCode() == 200) {
                    String content = EntityUtils.toString(httpResponse.getEntity());
                    RetrievePipelineResponse response = RetrievePipelineResponse.fromString(content);
                    if (!response.deployment_status.equals("Shutdown")) {
                        return shutdownPipeline(name) && deletePipeline(name);
                    } else {
                        return deletePipeline(name);
                    }
                } else {
                    System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                    return false;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private boolean wait(String name, RetrievePipelineResponseHandler handler, long timeout) {
        long startTimeMillis = System.currentTimeMillis();
        while (System.currentTimeMillis() - startTimeMillis < timeout) {
            try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
                HttpGet httpGet = new HttpGet(BASE_URI + "/pipelines/" + name);

                try (CloseableHttpResponse httpResponse = httpClient.execute(httpGet)) {
                    if (httpResponse.getStatusLine().getStatusCode() != 200) {
                        System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                        return false;
                    } else {
                        String content = EntityUtils.toString(httpResponse.getEntity());
                        RetrievePipelineResponse response = RetrievePipelineResponse.fromString(content);
                        RetrievePipelineResponseHandler.Action action = handler.handle(response);
                        if (action == RetrievePipelineResponseHandler.Action.RETURN_TRUE) {
                            return true;
                        } else if (action == RetrievePipelineResponseHandler.Action.RETURN_FALSE) {
                            return false;
                        } else if (action == RetrievePipelineResponseHandler.Action.CONTINUE) {
                            Thread.sleep(WAIT_INTERVAL_MS);
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }

        System.out.println("Wait timeout.");
        return false;
    }

    private boolean wait(String name, RetrieveStatisticsResponseHandler handler, long timeout) {
        long startTimeMillis = System.currentTimeMillis();
        long reportedTime = 0;
        while (System.currentTimeMillis() - startTimeMillis < timeout) {
            long currentTime = (System.currentTimeMillis() - startTimeMillis) / 1000;
            if (currentTime > reportedTime + 60) {
                reportedTime = currentTime;
                System.out.println("The query has been running for " + currentTime + " seconds.");
            }

            try (CloseableHttpClient httpClient = HttpClientBuilder.create().build()) {
                HttpGet httpGet = new HttpGet(BASE_URI + "/pipelines/" + name + "/stats");

                try (CloseableHttpResponse httpResponse = httpClient.execute(httpGet)) {
                    if (httpResponse.getStatusLine().getStatusCode() != 200) {
                        System.out.println(EntityUtils.toString(httpResponse.getEntity()));
                        return false;
                    } else {
                        String content = EntityUtils.toString(httpResponse.getEntity());
                        RetrieveStatisticsResponse response = RetrieveStatisticsResponse.fromString(content);
                        RetrieveStatisticsResponseHandler.Action action = handler.handle(response);
                        if (action == RetrieveStatisticsResponseHandler.Action.RETURN_TRUE) {
                            return true;
                        } else {
                            Thread.sleep(WAIT_INTERVAL_MS);
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }

        System.out.println("Wait timeout.");
        return false;
    }
}
