user : clean
	cc -c buzzLockU.c
	cc -shared -o buzzLock.so buzzLockU.c

clean:
	rm -f buzzLockU.o
	rm -f buzzLock.so
