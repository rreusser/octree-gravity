### Barnes Hut N-body approximation

To compile, you will need the OpenGL and GLUT libraries, but you shouldn't need much of
anything else.  It should be set to go on a Linux platform, but it might take some modification
on Windows.  Honestly, I have no idea, but it works fine on MacOS X and there's no reason it
should require modification to work on Linux beyond library path directories in the Makefile.

To run the program, type:
    cd src; make; make run

To change between regular and octree gravity calculation, simply comment/uncomment the 
'#define OCTREE_GRAVITY' line in common.h.  Be forewarned that it should run, but you 
might get errors if you try to run the regular octree keyboard commands.

I must apologize in advance for the octree data structure.  If this program is slow, I would
blame it on that.  It's not optimal, and since I've never done it before, it's not even
pretty.  There are a couple extra flags that should cease to exist, but you can't say it
doesn't work.  If you get a random segfault in the middle of a run, it's probably because
particles have spiraled in on each other due to the high order integration and the octree
must subdivide infinitely to resolve this.  Just keep the timestep *sane* ane this shouldn't
happen.  Sorry.

To change the initial conditions, alter the initial_conditions.c file.  I have NOT included
the Milky Way initial conditions obtained from the GalactICS program obtainable at
http://www.astro.rug.nl/~kuijken/galactics.html but it's not difficult to get that for yourself.
The main program is already set up to read data from a file with 

m1 x1 y1 z1 u1 v1 w1
m2 x2 y2 z2 u2 v2 w2
...

It's not that impressive to look at, and if this were a more accurate simulation, it might 
be worth putting in more realistic data (see main.c to read from file).  I'll be honest. 
This is really just to look pretty.

Ultimately, it works, but you will have to just dig into the source code and grep everything
if you want to make it work for you.

Enjoy.

 - Ricky
