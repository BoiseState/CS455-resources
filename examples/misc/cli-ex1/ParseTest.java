import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

/**
 * Example with short and long options with required values.
 */

/**
 * @author amit
 *
 */
public class ParseTest
{
    
    public static void printUsage(Options options) {
	HelpFormatter formatter = new HelpFormatter();
	formatter.printHelp( "ls", options );
    }
    
    
    /**
     * @param args
     */
    public static void main(String[] args) {

	// create the command line parser
	CommandLineParser parser = new DefaultParser();

	// create the Options
	Options options = new Options();
	options.addOption( "a", "all", false, "do not hide entries starting with ." );
	options.addOption( "A", "almost-all", false, "do not list implied . and .." );
	options.addOption( "b", "escape", false, "print octal escapes for nongraphic " + "characters" );
	options.addOption("B", "ignore-backups", false, "do not list implied entried " + "ending with ~");
	options.addOption("C", false, "list entries by columns");
	options.addOption("c", "create", true, " create a new account with the given login name");
	
	if (args.length == 0) {
	    printUsage(options);
	}
	
	try {
	    // parse the command line arguments
	    CommandLine line = parser.parse(options, args);

	    // validate that block-size has been set
	    if (line.hasOption("c")) {
		// print the value of block-size
		System.out.println(line.getOptionValue("c"));
	    }
	} catch (ParseException exp) {
	    System.out.println("ParseTest: " + exp.getMessage());
	    printUsage(options);
	}
    }

}
