#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstddef>
#include <stddef.h>
// Include GLEW
#include <GL/glew.h>
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		// #include <stdio.h>
		#include <GL/gl.h>
		// #include "MicroGlut.h"
		// #include <GL/glut.h>
	#endif
#endif

#define DEBUG

#include <GLFW/glfw3.h>
#include <iostream>

#include "src/Scene.h"
#include "src/VoronoiDiagram.h"
#include "src/Plane.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
Scene* scene;
int pattern = 0;
int numPoints = 50;
bool randomColor = false;

GLFWwindow* InitWindow()
{
    // Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Voronoi Fracturing TSBK03", NULL, NULL);
    if ( window == NULL )
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

static void mouse_cursor_callback( GLFWwindow * window, int button, int action, int mods)  
{

  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
        if(GLFW_PRESS == action)
        {
            double x,y;
            glfwGetCursorPos(window, &x, &y);
            scene->setWindowPressed(x,y);

        }
        else if(GLFW_RELEASE == action)
            scene->setWindowReleased();
    }
}

void mouseDragged(GLFWwindow* window, double x, double y)
{
    // std::cout<<"dragged: x: "<<x<<", y: "<<y<<std::endl;
    scene->setCameraRotation(x, y);
}

void mouseScroll(GLFWwindow* window, double x, double y) {
    scene->setCameraZoom(x, y);
}

void createScene()
{
    scene = new Scene();

    Plane* wall = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), "wall");
    wall->setColor(glm::vec4(0.7f, 0.7f, 0.9f, 1.0f));
    wall->generateCube(2.0f, 2.0f, 0.5f);
    scene->addGeometry(wall, 0.0, 0);
    
    Plane* groundPlane = new Plane(glm::vec3(0.0f, -1.01f, 0.0f), "groundplane");
    groundPlane->generatePlaneXZ(100.0f, 100.0f);
    scene->addGeometry(groundPlane, 0.0f, 0);
    scene->initialize();
    
}
void fracturePlane(){
    scene->deleteGeometryByName("wall");
    Plane* plane = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), "voronoiPlane");
    plane->generatePlaneXY(2.0f, 2.0f);
    VoronoiDiagram* vd = new VoronoiDiagram(plane);

    switch (pattern)
    {
        case 0:
            vd->sampleUniformPoints(numPoints);
            break;
        case 1:
            vd->sampleCrackPoints(numPoints);
            break;
        case 2:
            vd->sampleHolePoints(numPoints);
            break;
        default:
            break;
    }

    std::vector<Geometry*> fractures = vd->fracture(randomColor);
    for (auto g : fractures)
    {
        scene->addGeometry(g, 1.0f, 1);
    }
    delete vd;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_R:
                scene->resetCamera();
                break;
            case GLFW_KEY_SPACE:
                createScene();
                break;
            case GLFW_KEY_P:
                if (pattern >= 2)
                    pattern = 0;
                else
                    pattern ++;
                createScene();
                break;
            case GLFW_KEY_UP:
                if (numPoints <= 200)
                    numPoints += 10;
                createScene();
                break;
            case GLFW_KEY_DOWN:
                if (numPoints >= 20)
                    numPoints -= 10;
                createScene();
                break;
            case GLFW_KEY_C:
                randomColor = !randomColor;
                createScene();
                break;
            case GLFW_KEY_F:
                fracturePlane();
                scene->applyForce(glm::vec3(0,0,0), 0.1);
                break;
            default:
                break;
        }
    }
}

int main( void )
{
    GLFWwindow* window = InitWindow();
    if (!window)
        return -1;

    {
        createScene();

        glfwSetCursorPosCallback(window, mouseDragged);
        glfwSetScrollCallback(window, mouseScroll);
        glfwSetMouseButtonCallback(window, mouse_cursor_callback);

        glfwSetKeyCallback(window, key_callback);

        do {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // renderer.Clear();
            // shader.Bind();
            // shader.SetUniform4f("u_Color", 1.0, 1.0, 1.0, 1.0);
            // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            // renderer.Draw(va, ib, shader);
            
            scene->render();
            scene->stepSimulation();
            // Swap buffers
            glFlush();
            glfwSwapBuffers(window);
            glfwPollEvents();
        } // Check if the ESC key was pressed or the window was closed
        while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );
    // delete scene;
    }
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

