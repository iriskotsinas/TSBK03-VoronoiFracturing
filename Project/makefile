all :  project

project: project.cpp ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/LittleOBJLoader.c ../common/Linux/MicroGlut.c
	gcc -Wall -o project -DGL_GLEXT_PROTOTYPES project.cpp ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/LittleOBJLoader.c ../common/Linux/MicroGlut.c -I../common -I../common/Linux -lX11 -lm -lGL -lglfw3
 
clean :
	rm project

