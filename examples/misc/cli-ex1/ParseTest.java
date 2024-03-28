import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

/**
 * Example with short and long options with one or more required values. Make sure to
 * include the commons-cli jar file in the classpath. Test it with the following command:
 * 
 * java -cp .:../commons-cli-1.4/commons-cli-1.4.jar ParseTest -l amit -m  amit timal  -n 5005 -p test123
 */

public class ParseTest
{

    public static void printUsage(Options options) {
        HelpFormatter formatter = new HelpFormatter();
        String header = "       Identity Server - phase I: <authors>";
        String footer = "\n Please report issues to youremail@boisestate.edu";
        formatter.printHelp("java ParseTest <options>", header, options, footer);
    }


    public static Options setupOptions() {
        // create the Options
        Options options = new Options();

        // option with no value after it
        options.addOption("h", "help", false, "");

        // one option with optional value
        Option portOption = new Option("n", "numport", true, "optional port number on server");
        portOption.setOptionalArg(true);
        options.addOption(portOption);

        // this option requires one value
        options.addOption("l", "lookup", true, "lookup an account with the given login name");

        // this option requires one value (but we will only look for it if a password is required)
        Option passwordOption = new Option("p", "password", true, "supply password (if needed)");
        options.addOption(passwordOption);
        
        // one way to create an option that requires multiple values
        Option modifyOption = new Option("m", "modify", true, "modify existing login name");
        modifyOption.setArgs(2);
        options.addOption(modifyOption);

        return options;
    }


    /**
     * @param args
     */
    public static void main(String[] args) {

        // create the command line parser
        CommandLineParser parser = new DefaultParser();

        Options options = setupOptions();

        if (args.length == 0) {
            printUsage(options);
        }

        try {
            // parse the command line arguments
            CommandLine line = parser.parse(options, args);

            if (line.hasOption("d")) {
                System.out.println("Option -d [--debug] chosen");
            }

            if (line.hasOption("n")) {
                String value = line.getOptionValue("n");
                System.out.print("Option -n  [--numport] chosen");
                if (value != null) {
                    System.out.println(" with optional port# " + line.getOptionValue("n"));
                } else {
                    System.out.println(" without an optional port number");
                }
            }

            if (line.hasOption("l")) {
                System.out.print("Option -l ");
                System.out.println("with parameter " + line.getOptionValue("l") + " chosen");
            }

            if (line.hasOption("m")) {
                System.out.print("Option -m with parameters ");

                String[] values = line.getOptionValues("m");
                for (String s : values) {
                    System.out.print(s + " ");
                }
                System.out.println(" chosen");
                if (line.hasOption("p")) {
                    System.out.println("with password " + line.getOptionValue("p"));
                }
            }
            // process other options...

        } catch (ParseException exp) {
            System.out.println("ParseTest: " + exp.getMessage());
            printUsage(options);
        }
    }

}
