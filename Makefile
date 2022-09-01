CC = g++
CFLAGS = -Wall 
DEPS = myHeaderFiles.h 
OBJ = nonCanonicalMode.o listDirectory.o keyPressActions.o utilities.o commandMode.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clear:
	rm -rf *o main