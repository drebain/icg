#include "icg_common.h"
#include "imshow.h"

#include <math.h>
#include <OpenGP/GL/Eigen.h>

#include "Triangle/Triangle.h"
#include "Quad/Quad.h"


using namespace OpenGP;

Triangle triangle;
Quad quad;

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    triangle.draw();
    quad.draw();

}

///
/// You can use this sub-project as a  starting point for your second
/// assignemnt. See the files triangle.h and quad.h for examples of
/// basic OpenGL code.
///

int main(int, char**) {

    OpenGP::glfwInitWindowSize(640, 480);
    OpenGP::glfwMakeWindow("Assignment 2");

    glClearColor(0.0,0.0,0.0,0.0);

    triangle.init();
    quad.init();

    OpenGP::glfwDisplayFunc(&display);

    OpenGP::glfwMainLoop();

    return EXIT_SUCCESS;

}
