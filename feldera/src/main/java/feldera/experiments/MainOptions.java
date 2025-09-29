package feldera.experiments;

import org.apache.commons.cli.*;

public class MainOptions {
    public String code;
    public String input;
    public boolean print = false;

    public final static Option CODE_OPTION =
            new Option("c", "code", true, "The path to the code.");

    public final static Option INPUT_OPTION =
            new Option("i", "input", true, "The path to the input data.");

    public final static Option PRINT_OPTION =
            new Option("p", "print", false, "Print the last message in the result topic.");

    public static Options getOptions() {
        Options options = new Options();
        options.addOption(CODE_OPTION);
        options.addOption(INPUT_OPTION);
        options.addOption(PRINT_OPTION);
        return options;
    }

    public static MainOptions parse(String[] args) throws ParseException {
        DefaultParser defaultParser = new DefaultParser();
        CommandLine commandLine = defaultParser.parse(MainOptions.getOptions(), args, true);
        MainOptions mainOptions = new MainOptions();

        if (!commandLine.hasOption(MainOptions.CODE_OPTION.getOpt())) {
            System.out.println("Missing required argument: -c,--code <file>");
            System.exit(1);
        }
        mainOptions.code = commandLine.getOptionValue(MainOptions.CODE_OPTION.getOpt());

        if (!commandLine.hasOption(MainOptions.INPUT_OPTION.getOpt())) {
            System.out.println("Missing required argument: -i,--input <file>");
            System.exit(1);
        }
        mainOptions.input = commandLine.getOptionValue(MainOptions.INPUT_OPTION.getOpt());

        mainOptions.print = commandLine.hasOption(MainOptions.PRINT_OPTION.getOpt());

        return mainOptions;
    }
}
