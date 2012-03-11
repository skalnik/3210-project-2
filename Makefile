user: clean_user simclist
	cc -fPIC -c buzzLockU.c
	cc -shared -o buzzLock.so buzzLockU.o simclist.o

simclist:
	cc -fPIC -c vendor/simclist.c

clean: clean_user

clean_user: clean_simclist
	rm -f buzzLockU.o
	rm -f buzzLock.so

clean_simclist:
	rm -rf vendor/simclist.o


.PHONY: simclist clean clean_user
