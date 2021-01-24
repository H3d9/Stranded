// Stranded Engine
// 通用数学对象 Matrix 4x4

#pragma once

#include "Vector.h"


struct Matrix {
    Matrix() {
        matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
        matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
        matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
        matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
    }
    Matrix(
        float r11, float r12, float r13, float r14,
        float r21, float r22, float r23, float r24,
        float r31, float r32, float r33, float r34,
        float r41, float r42, float r43, float r44) {
        matrix[0] = r11; matrix[1] = r12; matrix[2] = r13; matrix[3] = r14;
        matrix[4] = r21; matrix[5] = r22; matrix[6] = r23; matrix[7] = r24;
        matrix[8] = r31; matrix[9] = r32; matrix[10] = r33; matrix[11] = r34;
        matrix[12] = r41; matrix[13] = r42; matrix[14] = r43; matrix[15] = r44;
    }

    Matrix operator+(const Matrix& m) const {
        return Matrix(
            matrix[0] + m.matrix[0], matrix[1] + m.matrix[1], matrix[2] + m.matrix[2],
            matrix[3] + m.matrix[3], matrix[4] + m.matrix[4], matrix[5] + m.matrix[5],
            matrix[6] + m.matrix[6], matrix[7] + m.matrix[7], matrix[8] + m.matrix[8],
            matrix[9] + m.matrix[9], matrix[10] + m.matrix[10], matrix[11] + m.matrix[11],
            matrix[12] + m.matrix[12], matrix[13] + m.matrix[13],
            matrix[14] + m.matrix[14], matrix[15] + m.matrix[15]);
    }
    Matrix operator-(const Matrix& m) const {
        return Matrix(
            matrix[0] - m.matrix[0], matrix[1] - m.matrix[1], matrix[2] - m.matrix[2],
            matrix[3] - m.matrix[3], matrix[4] - m.matrix[4], matrix[5] - m.matrix[5],
            matrix[6] - m.matrix[6], matrix[7] - m.matrix[7], matrix[8] - m.matrix[8],
            matrix[9] - m.matrix[9], matrix[10] - m.matrix[10], matrix[11] - m.matrix[11],
            matrix[12] - m.matrix[12], matrix[13] - m.matrix[13],
            matrix[14] - m.matrix[14], matrix[15] - m.matrix[15]);
    }
    Matrix operator*(const Matrix& m) const {
        return Matrix(matrix[0] * m.matrix[0] + matrix[4] * m.matrix[1] + matrix[8] * m.matrix[2] + matrix[12] * m.matrix[3],
            matrix[1] * m.matrix[0] + matrix[5] * m.matrix[1] + matrix[9] * m.matrix[2] + matrix[13] * m.matrix[3],
            matrix[2] * m.matrix[0] + matrix[6] * m.matrix[1] + matrix[10] * m.matrix[2] + matrix[14] * m.matrix[3],
            matrix[3] * m.matrix[0] + matrix[7] * m.matrix[1] + matrix[11] * m.matrix[2] + matrix[15] * m.matrix[3],
            matrix[0] * m.matrix[4] + matrix[4] * m.matrix[5] + matrix[8] * m.matrix[6] + matrix[12] * m.matrix[7],
            matrix[1] * m.matrix[4] + matrix[5] * m.matrix[5] + matrix[9] * m.matrix[6] + matrix[13] * m.matrix[7],
            matrix[2] * m.matrix[4] + matrix[6] * m.matrix[5] + matrix[10] * m.matrix[6] + matrix[14] * m.matrix[7],
            matrix[3] * m.matrix[4] + matrix[7] * m.matrix[5] + matrix[11] * m.matrix[6] + matrix[15] * m.matrix[7],
            matrix[0] * m.matrix[8] + matrix[4] * m.matrix[9] + matrix[8] * m.matrix[10] + matrix[12] * m.matrix[11],
            matrix[1] * m.matrix[8] + matrix[5] * m.matrix[9] + matrix[9] * m.matrix[10] + matrix[13] * m.matrix[11],
            matrix[2] * m.matrix[8] + matrix[6] * m.matrix[9] + matrix[10] * m.matrix[10] + matrix[14] * m.matrix[11],
            matrix[3] * m.matrix[8] + matrix[7] * m.matrix[9] + matrix[11] * m.matrix[10] + matrix[15] * m.matrix[11],
            matrix[0] * m.matrix[12] + matrix[4] * m.matrix[13] + matrix[8] * m.matrix[14] + matrix[12] * m.matrix[15],
            matrix[1] * m.matrix[12] + matrix[5] * m.matrix[13] + matrix[9] * m.matrix[14] + matrix[13] * m.matrix[15],
            matrix[2] * m.matrix[12] + matrix[6] * m.matrix[13] + matrix[10] * m.matrix[14] + matrix[14] * m.matrix[15],
            matrix[3] * m.matrix[12] + matrix[7] * m.matrix[13] + matrix[11] * m.matrix[14] + matrix[15] * m.matrix[15]);
    }

    Matrix operator*(float f) const {
        return Matrix(
            matrix[0] * f, matrix[1] * f, matrix[2] * f, matrix[3] * f,
            matrix[4] * f, matrix[5] * f, matrix[6] * f, matrix[7] * f,
            matrix[8] * f, matrix[9] * f, matrix[10] * f, matrix[11] * f,
            matrix[12] * f, matrix[13] * f, matrix[14] * f, matrix[15] * f);
    }

    float matrix[16];
}; // struct Matrix
