/*
 * rtele.x: remote telephone number listing protocol
 */
%#define DATABASE "../rdb/personnel.dat"
%#define MAX_STR 256

program RTELEPROG {
	version RTELEVERS {
		string RTELE(string) = 1;
	} = 1;
} = 0x20000001;
