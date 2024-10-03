.POSIX:
.OBJDIR: .
CC = cl
NAME = polyfit
CFLAGS = /Wall /WX /W3
IFLAGS = 
LFLAGS = 

OBJ_FILES = $(NAME).obj

RM = del /s /f

all: $(OBJ_FILES)

check: test.exe
	test

clean:
	$(RM) *.obj *.dll *.exe

test.exe: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJ_FILES) test.c -o $@

.SUFFIXES: .c .obj

.c.obj:
	$(CC) $(CFLAGS) $(IFLAGS) /c $<
