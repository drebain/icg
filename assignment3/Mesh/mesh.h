#pragma once
#include "icg_common.h"
#include "../noise.h"

class Mesh{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID
    GLuint _tex;

    const std::string filename = "Mesh/grid.obj";
    OpenGP::SurfaceMesh mesh;
    GLuint _vpoint;    ///< memory buffer
    GLuint _vnormal;   ///< memory buffer

public:
    GLuint getProgramID(){ return _pid; }

    void init(){
        ///--- Compile the shaders
        _pid = OpenGP::load_shaders("Mesh/Mesh_vshader.glsl", "Mesh/Mesh_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        check_error_gl();
        bool success = mesh.read(filename.c_str());
        assert(success);
        mesh.triangulate();
        mesh.update_vertex_normals();
        printf("Loaded mesh '%s' (#V=%d, #F=%d)\n", filename.c_str(), mesh.n_vertices(), mesh.n_faces());

        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Buffer
        OpenGP::SurfaceMesh::Vertex_property<OpenGP::Point> vpoints = mesh.get_vertex_property<OpenGP::Point>("v:point");
        glGenBuffers(ONE, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
        check_error_gl();

        ///--- Normal Buffer
        OpenGP::SurfaceMesh::Vertex_property<OpenGP::Normal> vnormals = mesh.get_vertex_property<OpenGP::Normal>("v:normal");
        glGenBuffers(ONE, &_vnormal);
        glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vnormals.data(), GL_STATIC_DRAW);
        check_error_gl();

        ///--- Index Buffer
        std::vector<unsigned int> indices;
        for(OpenGP::SurfaceMesh::Face_iterator fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit) {
            unsigned int n = mesh.valence(*fit);
            OpenGP::SurfaceMesh::Vertex_around_face_circulator vit = mesh.vertices(*fit);
            for(unsigned int v = 0; v < n; ++v) {
                indices.push_back((*vit).idx());
                ++vit;
            }
        }

        GLuint _vbo_indices;
        glGenBuffers(ONE, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        check_error_gl();

        glUseProgram(_pid);

        _tex = generate_noise();

        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        check_error_gl();
        glUniform1i(tex_id, 0);
        check_error_gl();

        glUseProgram(0);

    }

    void draw(mat4 Model, mat4 View, mat4 Projection){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Positions
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        glEnableVertexAttribArray(vpoint_id);

        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        ///--- Vertex Attribute ID for Normals
        GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
        glEnableVertexAttribArray(vnormal_id);
        glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
        glVertexAttribPointer(vnormal_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _tex);

        ///--- Set the MVP to vshader
        glUniformMatrix4fv(glGetUniformLocation(_pid, "MODEL"), 1, GL_FALSE, Model.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "VIEW"), 1, GL_FALSE, View.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "PROJ"), 1, GL_FALSE, Projection.data());

        check_error_gl();
        ///--- Draw
        glDrawElements(GL_TRIANGLES,
                    /*#vertices*/ 3*mesh.n_faces(),
                    GL_UNSIGNED_INT,
                    ZERO_BUFFER_OFFSET);
        check_error_gl();

        ///--- Clean up
        glBindVertexArray(0);
        glUseProgram(0);
    }
};
