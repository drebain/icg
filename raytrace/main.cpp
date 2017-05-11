#include "icg_common.h"
#include "imshow.h"
#include <Eigen/Geometry>


Colour red() { return Colour(255, 0, 0); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

int main(int, char**){

    /// Rays and vectors represented with Eigen

    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;

    int wResolution = 640;
    int hResolution = 480;
    
    Image image(wResolution, hResolution);
    
    /// TODO: define camera position and sphere position here

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            /// TODO: build primary rays
            // vec3 o = vec3(0,0,0);
            // vec3 d = vec3(1,1,0).normalized();
            // ray3 r(o,d);
            
            /// EXAMPLE: using "image(row,col)"
            if(row>100 && row<200 && col>200 && col<500)  {
                image.set(row, col, red());
            }
            if(row>140 && row<240 && col>240 && col<340) {
                image.set(row, col, Colour(0,0,127));
            }
       }
    }

    image.save("out.bmp");
    image.show();

    return EXIT_SUCCESS;
}
