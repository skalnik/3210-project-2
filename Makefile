user: clean_user simclist
	cc -fPIC -c buzzLockU.c
	cc -shared -o buzzLockU.so buzzLockU.o simclist.o
	cc user_test.c -o user_test buzzLockU.o simclist.o

simclist: clean_simclist
	cc -fPIC -c vendor/simclist.c

clean: clean_user clean_simclist

clean_user:
	rm -f buzzLockU.o
	rm -f buzzLockU.so

clean_simclist:
	rm -rf vendor/simclist.o

.PHONY: clean clean_user clean_simclist
