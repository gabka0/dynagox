package duckdb.experiments;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Utils {
    public static long getMemoryUsage() {
        try (BufferedReader reader = new BufferedReader(new FileReader("/proc/self/status"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.startsWith("VmRSS")) {
                    String[] parts = line.split("\\s+");
                    if (parts.length >= 2) {
                        try {
                            return Long.parseLong(parts[1]);
                        } catch (NumberFormatException e) {
                            System.err.println("Error parsing VmRSS value: " + e.getMessage());
                            return -1;
                        }
                    }
                }
            }
            System.err.println("Error: VmRSS not found in /proc/self/status");
        } catch (IOException e) {
            System.err.println("Error reading /proc/self/status: " + e.getMessage());
        }
        return -1;
    }
}
