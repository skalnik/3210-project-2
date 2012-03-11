user: clean_user
	cc -fPIC -c buzzLockU.c
	cc -shared -o buzzLock.so buzzLockU.o

clean: clean_user

clean_user:
	rm -f buzzLockU.o
	rm -f buzzLock.so

.PHONY: clean clean_user
