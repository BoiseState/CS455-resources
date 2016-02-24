#!/bin/sh

if test "$1" == ""
then
	serverhost=localhost
else
	serverhost=$1
fi

client $serverhost 1 &
client $serverhost 2 &
client $serverhost 3 &
client $serverhost 4 &
client $serverhost 5 &
client $serverhost 6 &
client $serverhost 7 &
client $serverhost 8 &

