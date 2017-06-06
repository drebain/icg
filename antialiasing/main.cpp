#include "icg_common.h"
#include "ScreenQuad.h"

int width=720, height=720;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;


static const float SpeedFactor = 1;
ScreenQuad squad;


void init(){
    glClearColor(0,0,0, /*solid*/1.0 );

    squad.init();
}

void display() {

    glViewport(0,0,width,height);    

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    squad.draw();

}

int main(int, char**) {
    OpenGP::glfwInitWindowSize(512, 512);
    OpenGP::glfwMakeWindow("FrameBuffer");
    OpenGP::glfwDisplayFunc(display);
    init();
    OpenGP::glfwMainLoop();

    exit( EXIT_SUCCESS );
}
