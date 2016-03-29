.PHONY: all
all: TimeServer TimeClient

.PHONY: timeserver
timeserver: TimeServer

.PHONY: timeclient
timeclient: TimeClient

TimeServer: 
	cd timeserver ; mix deps.get ; mix escript.build
	mv timeserver/TimeServer .
	chmod 755 TimeServer

TimeClient:
	cd timeclient ; mix deps.get ; mix escript.build
	mv timeclient/TimeClient .
	chmod 755 TimeClient

.PHONY: clean
clean: clean-timeserver clean-timeclient

.PHONY: clean-timeserver
clean-timeserver:
	cd timeserver ; mix clean ; rm -rf _build
	rm TimeServer

.PHONY: clean-timeclient
clean-timeclient:
	cd timeclient ; mix clean ; rm -rf _build
	rm TimeClient

