package dynagox.experiments;

import org.apache.commons.cli.*;

public class MainOptions {
    public String query;
    public String input;
    public boolean print = false;
    public boolean is_delta = false;
    public double epsilon;

    public final static Option QUERY_OPTION =
            new Option("q", "query", true, "The name of the query.");

    public final static Option INPUT_OPTION =
            new Option("i", "input", true, "The path to the input data.");

    public final static Option PRINT_OPTION =
            new Option("p", "print", false, "Print the last message in the result topic.");

    public final static Option MODE_OPTION =
            new Option("m", "mode", true, "Enumeration mode.");

    public final static Option EPSILON_OPTION =
            new Option("e", "epsilon", true, "Epsilon value.");

    public static Options getOptions() {
        Options options = new Options();
        options.addOption(QUERY_OPTION);
        options.addOption(INPUT_OPTION);
        options.addOption(PRINT_OPTION);
        options.addOption(MODE_OPTION);
        options.addOption(EPSILON_OPTION);
        return options;
    }

    public static MainOptions parse(String[] args) throws ParseException {
        DefaultParser defaultParser = new DefaultParser();
        CommandLine commandLine = defaultParser.parse(MainOptions.getOptions(), args, true);
        MainOptions mainOptions = new MainOptions();

        if (!commandLine.hasOption(MainOptions.QUERY_OPTION.getOpt())) {
            System.out.println("Missing required argument: -q,--query <name>");
            System.exit(1);
        }
        mainOptions.query = commandLine.getOptionValue(MainOptions.QUERY_OPTION.getOpt());

        if (!commandLine.hasOption(MainOptions.INPUT_OPTION.getOpt())) {
            System.out.println("Missing required argument: -i,--input <file>");
            System.exit(1);
        }
        mainOptions.input = commandLine.getOptionValue(MainOptions.INPUT_OPTION.getOpt());

        mainOptions.print = commandLine.hasOption(MainOptions.PRINT_OPTION.getOpt());

        if (!commandLine.hasOption(MainOptions.MODE_OPTION.getOpt())) {
            mainOptions.is_delta = false;
        } else {
            String mode = commandLine.getOptionValue(MainOptions.MODE_OPTION.getOpt());
            if (!mode.equals("full") && !mode.equals("delta")) {
                System.out.println("Unknown mode: " + mode);
                System.exit(1);
            }

            mainOptions.is_delta = (mode.equals("delta"));
        }

        if (!commandLine.hasOption(MainOptions.EPSILON_OPTION.getOpt())) {
            mainOptions.epsilon = 0.1;
        } else {
            String e = commandLine.getOptionValue(MainOptions.EPSILON_OPTION.getOpt());
            mainOptions.epsilon = Double.parseDouble(e);
        }

        return mainOptions;
    }
}
