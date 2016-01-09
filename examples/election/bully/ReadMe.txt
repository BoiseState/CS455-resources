Bully Algorithm

A coordinator is elected among a group of processes.  The coordinator's only
job is to respond to AYA (Are You Alive) messages.  The group is defined to
be processes listening at the same port.  This means only process per
machine at a time.  The processes must be on machines that receive broadcasts
from each other.  (Discovery is the only thing done using a broadcast.)

bully works on Unix, Linux and Win32.  It should be able to handle
heterogeneous groups.

Byte Ordering:  I have changed the OpCode field to be a single byte to avoid
byte-ordering problems.  The group addresses are also treated as byte-strings
(using memcmp).  The lParam is treated as opaque data by the receiver so it
also is not a problem.

OS differences:  Win32 ignores the first parameter to select so I was passing
zero.  Once I corrected that, the program worked fine on onyx.  Linux has two
other differences.  (1) A socket won't receive broadcasts if it has been
bound to a particular address.  (2) When a datagram can't be delivered the
next call to send or recv fails with ECONNREFUSED.  I work around this by
ignoring ECONNREFUSED on these calls.

Group Membership:  I don't implement complete group membership.  In
particular I don't have any way to tell if someone besides the coordinator
is removed from the group.  (I implement a LEAVE message which a member can
send but there is currently no way to send it.)  What this means is that as
processes leave and new processes join the group will grow without bound.
This should only be a problem with the array which holds the address list.
It will overflow after a time.

