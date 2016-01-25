#
#  Sample sstart .server file
#
#  Note that you should not include here machines already running the
# server either manually or thru inetd (creates multiple copies of
# the service).
#
# The format of this file is "host server_directory arguements"
# do not specify the server name, that's tacked-on later.  Just
# for kicks, this $HOME/.ls file will run a couple different versions
# of ls on three machines with different args
vision		 	/usr/bin/ -F
pyramid 		/usr/5bin/ -g
rodson	 		/usr/5bin/ -l
cortex
bach
