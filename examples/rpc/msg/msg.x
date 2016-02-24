/* @(#)msg.x	2.1 88/08/11 4.0 RPCSRC */
/*
 * msg.x: Remote message printing protocol
 */
struct args_in {
	int heights<12>;
	int widths<>;
};

program MESSAGEPROG {
	version MESSAGEVERS {
		int PRINTMESSAGE(string) = 1;
	} = 1;
} = 99;
