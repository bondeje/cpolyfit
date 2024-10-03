.POSIX:
.OBJDIR: .
CC = gcc
NAME = polyfit
CFLAGS = -Wall -Werror -Wextra -pedantic -g3 -O2
IFLAGS = 
LFLAGS = 

OBJ_FILES = $(NAME).o

RM = rm -f

all: $(OBJ_FILES)

check: test
	./test

clean:
	$(RM) *.o *.so test

test: $(OBJ_FILES)
	@if [ -n "$(SANITIZE)" ] ; then export DBGOPT="-fsanitize=address,undefined"; else export DBGOPT="" ; fi ; \
	$(CC) $(CFLAGS) $$DBGOPT $(IFLAGS) $(OBJ_FILES) test.c -o $@

.c.o:
	@if [ -n "$(SANITIZE)" ] ; then export DBGOPT="-fsanitize=address,undefined"; else export DBGOPT="" ; fi ; \
	$(CC) $(CFLAGS) $$DBGOPT $(IFLAGS) -c $< -o $@
