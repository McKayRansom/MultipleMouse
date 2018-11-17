keylogger: keylogger.c
	gcc -O0  keylogger.c -o keylogger -lX11
clean:
	rm -f  keylogger
