# Voronoi Fracturing TSBK03 Project

## Required libraries
glfw
glew
bullet
***

## Build and run
make -B
./project
***


## Controls

**F**: Fracture and play simulation

**ENTER**: PAUSE / UNPAUSE

**SPACE**: Reset Scene

**P**: Change Pattern

**C**: Switch between random colors and not

**R**: Reset camera

**ARROW-UP**: Increase number of points by 10

**ARROW-DOWN**: Decrease number of points by 10
***

## Makefile Linux
INCLUDE := -I./src -I../common -I"/usr/include/bullet" -I"/usr/include/bullet"

LIBS := -L -lstdc++ -lX11 -lm -lGL -lglfw -lGLEW
LIBS += -L ../common/Bullet/BulletSoftBody -lBulletSoftBody
LIBS += -L ../common/Bullet/BulletDynamics -lBulletDynamics
LIBS += -L ../common/Bullet/BulletCollision -lBulletCollision
LIBS += -L ../common/Bullet/LinearMath -lLinearMath
LIBS += -L "/usr/lib"

all:  project

project:
	g++ -Wall  -o project project.cpp src/*.cpp  $(INCLUDE) $(LIBS)
 
clean :
	rm project

***

## Makefile APPLE

CC=clang++
current_directory=$(shell pwd)

FRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

CFLAGS=-std=c++11
CFLAGS+=-I$(current_directory)
CFLAGS+=-I$(current_directory)/../common
CFLAGS+=-I$(current_directory)/../common/Bullet
#CFLAGS+=-I/usr/local/include

LDFLAGS=-L$(current_directory)/../lib
LDFLAGS+=-L$(current_directory)/../common
LDFLAGS+=-L$(current_directory)/../common/Bullet
LDFLAGS+=-L$(current_directory)/../common/Bullet/BulletCollision -lBulletCollision
LDFLAGS+=-L$(current_directory)/../common/Bullet/BulletDynamics -lBulletDynamics
LDFLAGS+=-L$(current_directory)/../common/Bullet/BulletSoftBody -lBulletSoftBody
LDFLAGS+=-L$(current_directory)/../common/Bullet/LinearMath -lLinearMath
LDFLAGS+=-L/usr/local/lib
LDFLAGS+=-lglfw3
LDFLAGS+=-lGLEW

SOURCES = \$(wildcard *.cpp) \$(wildcard src/*.cpp)
OBJECTS=\$(patsubst %.cpp, %.o, \$(SOURCES))

all: project

%.o: %.cpp
	\$(CC) \$(CFLAGS) -c -o \$@ \$^

default: debug

project: \$(OBJECTS)
	\$(CC) \$(CFLAGS) \$(LDFLAGS) \$(FRAMEWORKS) -o \$@ \$(OBJECTS) -Wno-deprecated-declarations

debug: CFLAGS+=-DDEBUG -g
debug: project

release: project

.PHONY: clean
clean:
	rm -f *.o project

.PHONY: debugger
debugger: debug
	PATH=/usr/bin /usr/bin/lldb ./project
