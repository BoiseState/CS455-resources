import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

/**
 * Example with short and long options with one or more required values.
 */

/**
 * @author amit
 *
 */
public class ParseTest
{
    
    public static void printUsage(Options options) {
	HelpFormatter formatter = new HelpFormatter();
	formatter.printHelp( "ParseTest ", options );
    }
    
    
    public static Options setupOptions() {
	// create the Options
	Options options = new Options();

	// option with no value after it
	options.addOption("d", "debug", false, "debug level");

	// one option with optional value
	Option portOption = new Option("n", "numport", true, "port number on server");
	portOption.setOptionalArg(true);
	options.addOption(portOption);

	// this option requires one value
	options.addOption("l", "lookup", true, "lookup an account with the given login name");

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

	    if (line.hasOption("n")) {
		String value = line.getOptionValue("n");
		if (value != null) {
		    System.out.println(line.getOptionValue("n"));
		}
	    }
	    
	    if (line.hasOption("l")) {
		System.out.println(line.getOptionValue("l"));
	    }
	    
	    if (line.hasOption("m")) {
		String[] values = line.getOptionValues("m");
		for (String s: values) {
		    System.out.println(s);
		}
	    }
	    // process other options...
	    
	} catch (ParseException exp) {
	    System.out.println("ParseTest: " + exp.getMessage());
	    printUsage(options);
	}
    }

}
