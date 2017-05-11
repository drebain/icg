#pragma once

#include <memory>

#include "icg_common.h"

typedef Eigen::Matrix<uint8_t, 3, 1> Colour;


class Image {
private:

    GLuint gpuTex;
    GLuint program;

    const char *fshader_source =
            "#version 330 core"
            "out vec3 color;"
            "in vec2 uv;"
            "uniform sampler2D tex;"
            "void main() {"
            "    color = vec3(1,0,0);//texture(tex,uv).rgb;"
            "}";

    const char *vshader_source =
            "#version 330 core"
            "in vec3 vpoint;"
            "in vec2 vtexcoord;"
            "out vec2 uv;"
            "void main() {"
            "    gl_Position = vec4(vpoint, 1.0);"
            "    uv = vtexcoord;"
            "}";

public:

    int rows, cols;

    std::unique_ptr<uint8_t[]> data;

    Image(int cols, int rows) {

        this->rows = rows;
        this->cols = cols;

        data = std::unique_ptr<uint8_t[]>(new uint8_t[3 * cols * rows]);

    }

    Colour get(int row, int col) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        auto r = data[col * rows + row];
        auto g = data[col * rows + row + 1];
        auto b = data[col * rows + row + 2];

        return Colour(r, g, b);

    }

    void set(int row, int col, Colour colour) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        data[row * cols + col]     = colour(0);
        data[row * cols + col + 1] = colour(1);
        data[row * cols + col + 2] = colour(2);

    }

    void save(std::string name) {

        /// http://stackoverflow.com/a/2654860

        FILE *f;
        unsigned char *img = data.get();

        int w = cols;
        int h = rows;

        int filesize = 54 + 3*w*h;

        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
        unsigned char bmppad[3] = {0,0,0};

        bmpfileheader[ 2] = (unsigned char)(filesize    );
        bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
        bmpfileheader[ 4] = (unsigned char)(filesize>>16);
        bmpfileheader[ 5] = (unsigned char)(filesize>>24);

        bmpinfoheader[ 4] = (unsigned char)(       w    );
        bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
        bmpinfoheader[ 6] = (unsigned char)(       w>>16);
        bmpinfoheader[ 7] = (unsigned char)(       w>>24);
        bmpinfoheader[ 8] = (unsigned char)(       h    );
        bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
        bmpinfoheader[10] = (unsigned char)(       h>>16);
        bmpinfoheader[11] = (unsigned char)(       h>>24);

        f = fopen(name.c_str(),"wb");
        fwrite(bmpfileheader,1,14,f);
        fwrite(bmpinfoheader,1,40,f);
        for(int i=0; i<h; i++)
        {
            fwrite(img+(w*(h-i-1)*3),3,w,f);
            fwrite(bmppad,1,(4-(w*3)%4)%4,f);
        }
        fclose(f);

    }

    void show() {

        glfwInit();

        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        auto window = glfwCreateWindow(cols, rows, "Output Image", nullptr, nullptr);

        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        glewInit();

        std::cout << "TEST2" << std::endl;
/*
        /// Create the Vertex Shader
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

        /// Compile Vertex Shader
        glShaderSource(VertexShaderID, 1, &vshader_source , NULL);
        glCompileShader(VertexShaderID);

        /// Create the Fragment Shader
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        /// Compile Fragment Shader
        glShaderSource(FragmentShaderID, 1, &fshader_source , NULL);
        glCompileShader(FragmentShaderID);

        /// Link the program
        program = glCreateProgram();
        glAttachShader(program, VertexShaderID);
        glAttachShader(program, FragmentShaderID);
        glLinkProgram(program);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);*/

        program = OpenGP::compile_shaders(vshader_source, fshader_source);

        glUseProgram(program);

        /// Texture
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &gpuTex);
        glBindTexture(GL_TEXTURE_2D, gpuTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, cols, rows, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        GLuint tex_id = glGetUniformLocation(program, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

        /// Vertex Data
        GLuint _vao, _vbo_vpoint, _vbo_vtexcoord;

        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { -1.0f, -1.0f, 0.0f,
                                       +1.0f, -1.0f, 0.0f,
                                       -1.0f, +1.0f, 0.0f,
                                       +1.0f, +1.0f, 0.0f };
            ///--- Buffer
            glGenBuffers(1, &_vbo_vpoint);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(program, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { 0.0f, 0.0f,
                                          1.0f, 0.0f,
                                          0.0f, 1.0f,
                                          1.0f, 1.0f};

            ///--- Buffer
            glGenBuffers(1, &_vbo_vtexcoord);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_vtexcoord);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);

            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(program, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.0,0.0,0.0,0.0);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(_vao);
            glUseProgram(program);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gpuTex);

            ///--- Draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }

};
