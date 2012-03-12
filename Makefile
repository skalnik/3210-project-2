user: clean_user simclist
	gcc -fPIC -c buzzLockU.c
	gcc -shared -o buzzLockU.so buzzLockU.o vendor/simclist.o
	gcc -fopenmp user_test.c -o user_test buzzLockU.o vendor/simclist.o

simclist: clean_simclist
	gcc -g -fPIC -DSIMCLIST_WITH_THREADS -o vendor/simclist.o -c vendor/simclist.c -std=c99

clean: clean_user clean_simclist

clean_user:
	rm -f buzzLockU.o
	rm -f buzzLockU.so
	rm -rf user_test

clean_simclist:
	rm -rf vendor/simclist.o

.PHONY: clean clean_user clean_simclist
