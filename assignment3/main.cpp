#include "icg_common.h"
#include "imshow.h"

#include <math.h>
#include <OpenGP/GL/Eigen.h>

#include "Mesh/mesh.h"

using namespace OpenGP;

Mesh grid;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
int window_width = 1280;
int window_height = 1024;

void display() {
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ratio = window_width / (float) window_height;
    Transform modelTransform = Transform::Identity();
    mat4 model = modelTransform.matrix();
    mat4 projection = OpenGP::perspective(45.0f, ratio, 0.1f, 10.0f);

    //camera movement
    float time = .5 * glfwGetTime();
    vec3 cam_pos(2*cos(time), 2.0, 2*sin(time));
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 1.0f, 0.0f);
    mat4 view = OpenGP::lookAt(cam_pos, cam_look, cam_up);

    grid.draw(model, view, projection);
}

///
/// You can use this sub-project as a  starting point for your second
/// assignemnt. See the files triangle.h and quad.h for examples of
/// basic OpenGL code.
///

int main(int, char**) {

    OpenGP::glfwInitWindowSize(window_width, window_height);
    OpenGP::glfwMakeWindow("Assignment 3");

    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    grid.init();

    OpenGP::glfwDisplayFunc(&display);

    OpenGP::glfwMainLoop();

    return EXIT_SUCCESS;

}
