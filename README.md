# VoronoiFracturing
TSBK03 Project

## Required libraries
glfw
glew
bullet


## Build and run
make -B
./project


## Controls

F: Fracture and play simulation

ENTER: PAUSE / UNPAUSE

SPACE: Reset Scene

P: Change Pattern

C: Switch between random colors and not

R: Reset camera

ARROW-UP: Increase number of points by 10

ARROW-DOWN: Decrease number of points by 10

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

#Makefile APPLE
