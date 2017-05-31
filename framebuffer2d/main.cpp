#include "icg_common.h"
#include "FrameBuffer.h"
#include "Quad.h"
#include "ScreenQuad.h"

int width=720, height=720;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

Quad background;
Quad foreground;
static const float SpeedFactor = 1;
// TODO: declare Framebuffer + ScreenQuad (see slides)


void init(){
    glClearColor(1,1,1, /*solid*/1.0 );
    foreground.init("earth.tga");
    background.init("sun.tga");

    // TODO: initialize framebuffer (see slides)
    // TODO: initialize fullscreen quad (see slides)

}

void drawScene(float timeCount)
{

}

void display(){ 
//    OpenGP::update_title_fps("FrameBuffer");
    glViewport(0,0,width,height);    

    // TODO: First draw the scene onto framebuffer, 
    // then use screen quad to add effects!
    drawScene(glfwGetTime());
}

int main(int, char**){
    OpenGP::glfwInitWindowSize(512, 512);
    OpenGP::glfwMakeWindow("FrameBuffer");
    OpenGP::glfwDisplayFunc(display);
    init();
    OpenGP::glfwMainLoop();

    exit( EXIT_SUCCESS );
}
