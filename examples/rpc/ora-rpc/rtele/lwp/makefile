LFLAGS = -llwp -lnbio
CFLAGS = -I../

ALL = rteleLWP rteleLWP_svc

all: $(ALL)
rteleLWP: rteleLWP.c  ../rtele_clnt.c
	$(CC) $(CFLAGS) -o rteleLWP rteleLWP.c ../rtele_clnt.c $(LFLAGS)

rteleLWP_svc: ../rtele_svc.c rteleLWP_svc_proc.c svc_runLWP.c
	$(CC) $(CFLAGS) -o rteleLWP_svc ../rtele_svc.c rteleLWP_svc_proc.c svc_runLWP.c $(LFLAGS)

clean:
	$(RM) $(ALL) *.o
