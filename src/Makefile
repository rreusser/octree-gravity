PROD =		../bin/nbody
LIB =		-lm -lgl -lglu -lglut
LIBDIR =	-L/usr/local/lib -L/usr/X11R6/lib
INCDIR =	-I/usr/local/include -I/usr/X11R6/include
CC =		cc
CFLAGS = 	-Wall $(INCDIR) -O3  -g
LD =		cc
LDFLAGS =	-Wall $(LIBDIR) $(LIB) -O3 -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib -g
OBJ =		main.o particles.o regular_gravity.o \
		common.o array.o rk.o point.o \
		glinterface.o vector3f.o initial_conditions.o \
		camera.o func.o gldisplay.o octree_draw.o \
		glkeys.o glmouse.o octree_gravity.o iterate.o

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
