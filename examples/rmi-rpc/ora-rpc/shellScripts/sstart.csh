#!/bin/csh -f
#
# sstart.csh - establish the server processes 
#
# Usage: sstart.csh service [host host host...]
# 	without hosts, kills all services it can find, with
#	hosts, it starts the named service on the host

# Where is the server executable?  All sorts of NFS differences may
# exist,...
set Server="$1"

#If no args, blast existing daemons.
if ($#argv == 1) then
  foreach i \
  (`ps g|egrep "$Server"|awk '\\!/egrep/&&\\!/awk/&&\\!/sstart.csh/{print $1}'`)
    kill $i
  end
endif

#Lauch the processes
foreach i ($2 $3 $4$5 $6 $7 $8 $9 $10)
  rsh $i $Server < /dev/null &
end
exit
