package feldera.experiments;

import feldera.experiments.client.FelderaClient;
import feldera.experiments.client.Result;
import org.apache.commons.cli.ParseException;
import org.apache.commons.io.FileUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public final static String PIPELINE_NAME = "feldera";
    public final static long TIMEOUT_IDLE_MS = 30 * 1000;
    public final static long TIMEOUT_TOTAL_MS = 2 * 3600 * 1000;

    public static void main(String[] args) throws ParseException, IOException {
        MainOptions options = MainOptions.parse(args);
        run(options);
    }

    public static void run(MainOptions options) throws IOException {
        boolean success;
        String code = FileUtils.readFileToString(new File(options.code));
        code = code.replaceAll("\\$\\{input\\}", options.input);

        FelderaClient feldera = new FelderaClient();
        success = feldera.deletePipelineIfExist(PIPELINE_NAME);
        if (!success) {
            System.out.println("Delete pipeline if exist failed.");
            System.exit(1);
        }

        success = feldera.createPipeline(PIPELINE_NAME, code);
        if (!success) {
            System.out.println("Create pipeline failed.");
            System.exit(1);
        }

        success = feldera.startPipeline(PIPELINE_NAME);
        if (!success) {
            System.out.println("Start pipeline failed.");
            System.exit(1);
        }

        long startTimeMillis = System.currentTimeMillis();
        Result result = feldera.waitUntilIdle(PIPELINE_NAME, TIMEOUT_IDLE_MS, TIMEOUT_TOTAL_MS);
        long stopTimeMillis = System.currentTimeMillis();
        if (result.isSuccess()) {
            if (options.print) {
                String res = getResult(getContainerId());
                if (res != null) {
                    System.out.println(res);
                }
            }

            System.out.println("Running Time: " + (stopTimeMillis - startTimeMillis - TIMEOUT_IDLE_MS) + " ms");
            System.out.println("Memory Usage: " + result.getMaxUsedMemory() / 1000 + " kB");
        } else {
            System.out.println("Failed After " + (stopTimeMillis - startTimeMillis) + " ms");
            System.out.println("Memory Usage: " + result.getMaxUsedMemory() / 1000 + " kB");
        }

        success = feldera.shutdownPipeline(PIPELINE_NAME);
        if (!success) {
            System.out.println("Shutdown pipeline failed.");
            System.exit(1);
        }

        success = feldera.deletePipeline(PIPELINE_NAME);
        if (!success) {
            System.out.println("Delete pipeline failed.");
            System.exit(1);
        }
    }

    public static String getContainerId() throws IOException {
        List<String> command = new ArrayList<>();
        command.add("docker");
        command.add("ps");
        command.add("--format");
        command.add("{{.ID}}");
        command.add("-f");
        command.add("name=feldera-pipeline-manager-1");
        ProcessBuilder processBuilder = new ProcessBuilder(command);
        Process process = processBuilder.start();
        BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
        StringBuilder builder = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        String containerId = builder.toString();
        if (containerId.isEmpty()) {
            return null;
        }
        return containerId;
    }

    public static String getResult(String containerId) throws IOException {
        if (containerId != null) {
            String command = "docker exec " + containerId + " tail -n1 /home/ubuntu/result.txt";
            Process process = Runtime.getRuntime().exec(command);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String result = reader.readLine();
            if (result == null || result.isEmpty()) {
                return null;
            }
            return result;
        } else {
            return null;
        }
    }
}
