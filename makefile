CC=gcc
CFLAGS=-lX11 -lXi
DEPS = mouse.h
OBJ = keylogger.o multipleMouse.o 


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clean keylogger multipleMouse

keylogger: $(OBJ)
	$(CC)  $@.o -o $@ $(CFLAGS)

multipleMouse: $(OBJ)
	$(CC)  $@.o -o $@  $(CFLAGS)

.PHONE: clean

clean:
		rm -f $(OBJ)/*.o keylogger multipleMouse
