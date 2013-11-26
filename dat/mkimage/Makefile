PROD =		mkimage
LIB =		-lm -lgl -lglu -lglut -lfftw3
LIBDIR =	-L/usr/local/lib -L/usr/X11R6/lib
INCDIR =	-I/usr/local/include -I/usr/X11R6/include
CC =		cc
CFLAGS = 	-Wall $(INCDIR) -O3 
LD =		cc
LDFLAGS =	-Wall $(LIBDIR) $(LIB) -O3 -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib
OBJ =		mkimage.o

$(PROD): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

.c.o:
	$(CC) -c $< $(CFLAGS) -o $@
clean:
	rm -f *.o $(PROD)
run:
	./$(PROD)
debug:
	gdb $(PROD)
