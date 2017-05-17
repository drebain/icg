#include "icg_common.h"
#include "imshow.h"

#include <math.h>
#include <OpenGP/GL/Eigen.h>


using namespace OpenGP;

struct Triangle {
    vec3 v1, v2, v3, n1, n2, n3;
};

float triangleArea(vec3 v1, vec3 v2, vec3 v3) {

    return 0; /// TODO: Calculate the area of the triangle

}

Colour shadeFragment(vec3 normal) {
    float l = std::min(std::max(normal.dot(vec3(0,1,0)) + 0.3f, 0.0f), 1.0f);
    vec3 c = vec3(127,255,0) * l;
    return c.cast<uint8_t>();
}

void rasterize(Triangle t, Image &image) {

    // Vectors projected onto z=0 plane
    vec3 s1 = vec3(t.v1(0), t.v1(1), 0);
    vec3 s2 = vec3(t.v2(0), t.v2(1), 0);
    vec3 s3 = vec3(t.v3(0), t.v3(1), 0);

    /// TODO: Restrict these bounds to only rasterize triangle bounding box (Optional Optimization)
    int iMin = 0;
    int iMax = image.cols;
    int jMin = 0;
    int jMax = image.rows;

    float totalArea = triangleArea(s1, s2, s3);

    for (int i = iMin;i < iMax;i++) {
        for (int j = jMin;j < jMax;j++) {

            // Location of fragment in image space
            vec3 pt = vec3(((float)i) / image.cols, ((float)j) / image.rows, 0.0);
            pt = 2 * (pt - vec3(0.5,0.5,0));

            /// TODO: Calculate barycentric coordinates of the fragment within current triangle
            float alpha = 0;
            float beta = 0;
            float gamma = 0;

            /// TODO: Calculate depth value of fragment (Use z-component of vertex positions)
            float depth = 0;

            if (false /** TODO: check if fragment is inside triangle
                                and not behind previous fragment **/) {

                /// TODO: Calculate normal vector of fragment (very similar to depth calculation)
                vec3 normal = vec3(0, 0, 0);
                Colour c = shadeFragment(normal);

                image.set(j, i, c);
                image.setDepth(j, i, depth);

            }
        }
    }

}

int main(int, char**){

    std::string meshName = "tet.obj";
    //std::string meshName = "bunny.obj";

    SurfaceMesh mesh;
    if (!mesh.read(meshName)) {
        std::cerr << "Failed to read input mesh" << std::endl;
        return EXIT_FAILURE;
    }

    mesh.update_vertex_normals();

    auto vpoint = mesh.vertex_property<vec3>("v:point");
    auto vnormal = mesh.vertex_property<vec3>("v:normal");

    int wResolution = 500;
    int hResolution = 500;

    Image image(wResolution, hResolution);

    // Set initial depth to large value
    for (int i = 0;i < image.cols;i++) {
        for (int j = 0;j < image.rows;j++) {
            image.setDepth(j, i, std::numeric_limits<float>::max());
        }
    }

    float fieldOfView = 80; // degrees
    float near = 0.01;
    float far = 10;

    mat4 projection = OpenGP::perspective(fieldOfView, (float)wResolution/(float)hResolution, 0.01, 5);

    vec3 eye = vec3(0,0.3,3);
    vec3 focus = vec3(0,0,0);
    vec3 up = vec3(0,1,0);

    mat4 view = OpenGP::lookAt(eye, focus, up);

    mat4 model = mat4::Identity();

    mat4 mvp = projection * view * model;

    for (SurfaceMesh::Face tri : mesh.faces()) {
        auto h1 = mesh.halfedge(tri);
        auto h2 = mesh.next_halfedge(h1);
        auto h3 = mesh.next_halfedge(h2);

        SurfaceMesh::Halfedge halfedges[3] = {
            h1, h2, h3
        };

        vec3 vertices[3] = {
            vec3(0,0,0), vec3(0,0,0), vec3(0,0,0)
        };

        vec3 normals[3] = {
            vec3(0,0,0), vec3(0,0,0), vec3(0,0,0)
        };

        for (int i = 0;i < 3;i++) {
            auto vi = mesh.to_vertex(halfedges[i]);
            vec3 v = vpoint[vi];
            vec4 ssv = mvp * vec4(v(0), v(1), v(2), 1);
            vertices[i] = vec3(ssv(0) / ssv(3), ssv(1) / ssv(3), ssv(2) / ssv(3));
            normals[i] = vnormal[vi];
        }

        Triangle t = {
            vertices[0], vertices[1], vertices[2],
            normals[0], normals[1], normals[2]
        };

        rasterize(t, image);
    }

    image.save("../../out.bmp");
    image.show();

    return EXIT_SUCCESS;
}
