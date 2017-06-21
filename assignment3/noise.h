#pragma once

#include "icg_common.h"

inline float mix(float x, float y, float alpha) {
    return y * alpha + x * (1.0f - alpha);
}


inline float f(float t) {
    float t_3 = t * t * t;
    return 6 * t * t * t_3 - 15 * t * t_3 + 10 * t_3;
}

inline float rand01() {
    return ((float) std::rand())/((float) RAND_MAX);
}

inline GLuint generate_noise() {

    int width = 512;
    int height = 512;

    float *gradients = new float[2 * width * height];
    auto sample_gradient = [&](int i, int j) {
        float x = gradients[2*i + 2*j*height];
        float y = gradients[2*i + 2*j*height];
        return vec2(x,y);
    };
    float *noise_data = new float[width * height];

    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {

            float angle = rand01();

            gradients[2*i + 2*j*height]     = cos(2 * angle * M_PI);
            gradients[1 + 2*i + 2*j*height] = sin(2 * angle * M_PI);

        }
    }

    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {
            noise_data[i + j * height] = 0;
        }
    }

    int period = 64;
    float frequency = 1.0f / period;

    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {
            int left = (i / period) * period;
            int right = (left + period) % width;
            float dx = (i - left) * frequency;

            int top = (j / period) * period;
            int bottom = (top + period) % height;
            float dy = (j - top) * frequency;

            vec2 a(dx, -dy);
            vec2 b(dx - 1, -dy);
            vec2 c(dx - 1, 1 - dy);
            vec2 d(dx, 1 - dy);

            vec2 topleft = sample_gradient(left, top);
            vec2 topright = sample_gradient(right, top);
            vec2 bottomleft = sample_gradient(left, bottom);
            vec2 bottomright = sample_gradient(right, bottom);

            /// TODO: add dot products and bilinear interpolation
            /// Hint: use f(t) and mix(x, y, alpha)

            float noise = 0;

            noise_data[i + j * height] += noise;
        }
    }

    GLuint _tex;

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);

    check_error_gl();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    check_error_gl();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
                 width, height, 0,
                 GL_RED, GL_FLOAT, noise_data);
    check_error_gl();

    delete gradients;
    delete noise_data;

    return _tex;
}
