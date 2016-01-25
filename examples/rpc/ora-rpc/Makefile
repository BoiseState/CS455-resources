all:
	for i in `find . -name makefile -print | sed 's/makefile//'` ; do \
	  cd $$i ; \
	  ${MAKE} ${MFLAGS}; \
	  cd $(PWD) ; \
	done

clean:
	for i in `find . -name makefile -print | sed 's/makefile//'` ; do \
	  cd $$i ; \
	  ${MAKE} ${MFLAGS} clean ; \
	  cd $(PWD) ; \
	done
