#! /bin/sh
#
# sstart - manual startup of computation servers, RPC or otherwise
# 
# usage: sstart [-n] [-v] [-s] server [hosts...]
#
#	-v means print the rsh commands before executing
# 	-n means print the rsh commands but don't execute them
# 	-s silence - means throw-away rsh command results
#

SILENCE=0
while true
do
	case $1 in
		-v) SHOPTS="${SHOPTS}v"; shift ;;
		-n) SHOPTS="${SHOPTS}nv"; shift ;;
		-s) SILENCE=1; shift ;;
		*) break ;;
	esac
done

SERVER=$1; shift ;
if test -s $HOME/.$SERVER; then 
	echo reading hosts from $HOME/.$SERVER
	else 
	echo $0: $HOME/.$SERVER not found
	exit
fi

tmp=/tmp/sstart_s$$
tmp3=/tmp/sstart_u$$

sort -b $HOME/.$SERVER | egrep -v '^#' >$tmp

# if arguments supplied, extract the named hosts
if test -n "$*"; then
	tmp2=/tmp/sstart_t$$
	for host in $*
	do 
		echo $host
	done | sort -b >$tmp2
	cat $tmp | awk '{print $1}' | comm -13 - $tmp2 |
        awk '{printf "'$0': host %s not in $HOME/.$SERVER, ignored\n", $1}' 1>&2
	join $tmp $tmp2
	rm -f $tmp2
else
	cat $tmp
fi >$tmp3

cat $tmp3 |
  awk '
  { opts=""
     for(i=3; i<=NF; i++) opts=opts" "$i
     if (SILENCE == 1) {
	printf "%s \"%s%s%s >/dev/null </dev/null &\"\n", \
		$1, $2, SERVER, opts
    } else {
	printf "%s \"%s%s%s </dev/null &\"\n", \
		$1, $2, SERVER, opts
    }
  }' SERVER=$SERVER SILENCE=$SILENCE >$tmp

# Check for a different login name in the ~/.rhosts file

sort -b $HOME/.rhosts |
  # if multiple usernames for the same host, chooses one arbitrarily
  awk '{print $2, $1}' | uniq -1 | 
  awk '{print $2, "-l", $1}' | join -a2 - $tmp | 

# There is now a complete rsh command in the pipe for each host found.
# We'll attempt to start all the servers in parallel, delaying some
# between each process creation to allow a process to get out onto the
# network before we load things down further. We send each cammand
# into the background to attempt to start them all in parallel.
# Because most servers and system error messages will come-back without
# identification of the server (host), we'll prepend that.

(
while read host rest
do
	# The quoted newline is necessary to get newlines in the output
	# when the -v and -n options are used. The redirect from </dev/null
	# is not just nice but essential here, as it would use the
	# standard input stream intended the above read.
	sh -c$SHOPTS "exec rsh $host $rest 2>&1 | sed -e 's/^/$host: /' 1>&2"'
' </dev/null &
	sleep 2
done 

wait	# wait for all messages to come back before exiting
)

rm -f $tmp $tmp3
