package dynagox.experiments;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Utils {
    public static double get_b(double epsilon, double n) {
        return Math.pow((1 + epsilon), 1.0 / n);
    }

    public static long getMaxMemoryUsage() {
        try (BufferedReader reader = new BufferedReader(new FileReader("/proc/self/status"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.startsWith("VmPeak")) {
                    String[] parts = line.split("\\s+");
                    if (parts.length >= 2) {
                        try {
                            return Long.parseLong(parts[1]);
                        } catch (NumberFormatException e) {
                            System.err.println("Error parsing VmPeak value: " + e.getMessage());
                            return -1;
                        }
                    }
                }
            }
            System.err.println("Error: VmPeak not found in /proc/self/status");
        } catch (IOException e) {
            System.err.println("Error reading /proc/self/status: " + e.getMessage());
        }
        return -1;
    }
}
