#ifndef BLOAT_MATH_H
#define BLOAT_MATH_H

#include <stdbool.h>
#include <math.h>

#ifndef BLOATDEF
#define BLOATDEF static inline
#endif // BLOATDEF

typedef struct {
    float x, y;
} Vector2;

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y, z, w;
} Vector4;

typedef Vector4 Quaternion;

// 4x4 Matrix
typedef struct {
    float m0, m4, m8 , m12;
    float m1, m5, m9 , m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
} Matrix;

typedef struct { float mat[16]; } MatrixLayout;

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#define as_radians(x) ((x)) * (PI / 180.0f)

#define clamp01(value) clamp((value), 0.0f, 1.0f)

#define mat() mat_value(1.0f)

#define vec2_add(v, value)                      \
    _Generic((value),                           \
             float: vec2_add_value,             \
             Vector2: vec2_add_vec              \
             )(v, value)

#define vec2_sub(v, value)                      \
    _Generic((value),                           \
             float: vec2_sub_value,             \
             Vector2: vec2_sub_vec              \
             )(v, value)

#define vec3_add(v, value)                      \
    _Generic((value),                           \
             float: vec3_add_value,             \
             Vector3: vec3_add_vec              \
             )(v, value)

#define vec3_sub(v, value)                      \
    _Generic((value),                           \
             float: vec3_sub_value,             \
             Vector3: vec3_sub_vec              \
             )(v, value)

BLOATDEF float        clamp(float value, float min, float max);
BLOATDEF bool         float_eq(float x, float y);

BLOATDEF Vector2      vec2(float x, float y);
BLOATDEF Vector2      vec2_add_vec(Vector2 v1, Vector2 v2);
BLOATDEF Vector2      vec2_add_value(Vector2 v, float value);
BLOATDEF Vector2      vec2_sub_vec(Vector2 v1, Vector2 v2);
BLOATDEF Vector2      vec2_sub_value(Vector2 v, float value);
BLOATDEF Vector2      vec2_scale(Vector2 v, float scale);
BLOATDEF Vector2      vec2_normalize(Vector2 v);
BLOATDEF Vector2      vec2_negate(Vector2 v);
BLOATDEF float        vec2_cross(Vector2 v1, Vector2 v2);
BLOATDEF float        vec2_dot(Vector2 v1, Vector2 v2);
BLOATDEF float        vec2_length_sqr(Vector2 v);
BLOATDEF bool         vec2_eq(Vector2 v1, Vector2 v2);
BLOATDEF bool         vec2_zeroed(Vector2 v);

BLOATDEF Vector3      vec3(float x, float y, float z);
BLOATDEF Vector3      vec3_add_vec(Vector3 v1, Vector3 v2);
BLOATDEF Vector3      vec3_add_value(Vector3 v, float value);
BLOATDEF Vector3      vec3_sub_vec(Vector3 v1, Vector3 v2);
BLOATDEF Vector3      vec3_sub_value(Vector3 v, float value);
BLOATDEF Vector3      vec3_scale(Vector3 v, float scale);
BLOATDEF Vector3      vec3_normalize(Vector3 v);
BLOATDEF Vector3      vec3_negate(Vector3 v);
BLOATDEF Vector3      vec3_cross(Vector3 v1, Vector3 v2);
BLOATDEF float        vec3_dot(Vector3 v1, Vector3 v2);
BLOATDEF float        vec3_length_sqr(Vector3 v);
BLOATDEF bool         vec3_eq(Vector3 v1, Vector3 v2);
BLOATDEF bool         vec3_zeroed(Vector3 v);

BLOATDEF Vector4      vec4(float x, float y, float z, float w);

BLOATDEF Quaternion   quat(void);
BLOATDEF Quaternion   quat_add_quat(Quaternion q1, Quaternion q2);
BLOATDEF Quaternion   quat_add_value(Quaternion q, float value);
BLOATDEF Quaternion   quat_sub_quat(Quaternion q1, Quaternion q2);
BLOATDEF Quaternion   quat_sub_value(Quaternion q, float value);
BLOATDEF Quaternion   quat_scale(Quaternion q, float scale);
BLOATDEF Quaternion   quat_normalize(Quaternion q);
BLOATDEF Quaternion   quat_negate(Quaternion q);
BLOATDEF float        quat_dot(Quaternion q1, Quaternion q2);
BLOATDEF float        quat_length_sqr(Quaternion q);
BLOATDEF bool         quat_eq(Quaternion q1, Quaternion q2);
BLOATDEF bool         quat_zeroed(Quaternion q);

BLOATDEF Matrix       mat_value(float value);
BLOATDEF Matrix       mat_translate(Vector3 v);
BLOATDEF Matrix       mat_multiply(Matrix left, Matrix right);
BLOATDEF Matrix       mat_rotate(Vector3 axis, float angle);
BLOATDEF Matrix       mat_scale(Vector3 v);
BLOATDEF Matrix       mat_perspective(float fov, float aspect, float near, float far);
BLOATDEF Matrix       mat_look_at(Vector3 eye, Vector3 target, Vector3 up);
BLOATDEF MatrixLayout mat_flatten(Matrix m);

#endif // BLOAT_MATH_H

#ifdef BLOAT_MATH_IMPLEMENTATION

BLOATDEF float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

BLOATDEF bool float_eq(float x, float y)
{
    return (fabsf(x - y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));
}

// Vector 2
BLOATDEF Vector2 vec2(float x, float y)
{
    return (Vector2) {
        .x = x,
        .y = y,
    };
}

BLOATDEF Vector2 vec2_add_vec(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

BLOATDEF Vector2 vec2_add_value(Vector2 v, float value)
{
    return (Vector2) {
        .x = v.x + value,
        .y = v.y + value,
    };
}

BLOATDEF Vector2 vec2_sub_vec(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

BLOATDEF Vector2 vec2_sub_value(Vector2 v, float value)
{
    return (Vector2) {
        .x = v.x - value,
        .y = v.y - value,
    };
}

BLOATDEF Vector2 vec2_scale(Vector2 v, float scale)
{
    return (Vector2) {
        .x = v.x * scale,
        .y = v.y * scale,
    };
}

BLOATDEF float vec2_length_sqr(Vector2 v)
{
    return v.x*v.x + v.y*v.y;
}

BLOATDEF bool vec2_eq(Vector2 v1, Vector2 v2)
{
    return float_eq(v1.x, v2.x) && float_eq(v2.x, v2.y);
}

BLOATDEF Vector2 vec2_normalize(Vector2 v)
{
    float length = sqrtf(vec2_length_sqr(v));

    if (length == 0.0f) lenght = 1.0f;
    float i_length = 1.0f/length;

        return (Vector2) {
            .x = v.x * i_length,
            .y = v.y * i_length,
        };
}

BLOATDEF Vector2 vec2_negate(Vector2 v)
{
    return (Vector2) {
        .x = -v.x,
        .y = -v.y,
    };
}

BLOATDEF float vec2_cross(Vector2 v1, Vector2 v2)
{
    return (v1.x*v2.y - v1.y*v2.x);
}

BLOATDEF float vec2_dot(Vector2 v1, Vector2 v2)
{
    return (v1.x*v2.x + v1.y*v2.y);
}

BLOATDEF bool vec2_zeroed(Vector2 v)
{
    return v.x == 0.0f && v.y == 0.0f;
}

// Vector 3
BLOATDEF Vector3 vec3(float x, float y, float z)
{
    return (Vector3) {
        .x = x,
        .y = y,
        .z = z,
    };
}

BLOATDEF Vector3 vec3_add_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
    };
}

BLOATDEF Vector3 vec3_add_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x + value,
        .y = v.y + value,
        .z = v.z + value,
    };
}

BLOATDEF Vector3 vec3_sub_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
}

BLOATDEF Vector3 vec3_sub_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x - value,
        .y = v.y - value,
        .z = v.z - value,
    };
}

BLOATDEF Vector3 vec3_scale(Vector3 v, float scale)
{
    return (Vector3) {
        .x = v.x * scale,
        .y = v.y * scale,
        .z = v.z * scale,
    };
}

BLOATDEF float vec3_length_sqr(Vector3 v)
{
    return (v.x*v.x + v.y*v.y + v.z*v.z);
}

BLOATDEF bool vec3_eq(Vector3 v1, Vector3 v2)
{
    return float_eq(v1.x, v2.x) && float_eq(v1.y, v2.y) && float_eq(v1.z, v2.z);
}

BLOATDEF bool vec3_zeroed(Vector3 v)
{
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
}

BLOATDEF Vector3 vec3_normalize(Vector3 v)
{
    float length = sqrtf(vec3_length_sqr(v));

    if (length == 0.0f) length = 1.0f;
    float i_length = 1.0f/length;

    return (Vector3) {
        .x = v.x * i_length,
        .y = v.y * i_length,
        .z = v.z * i_length,
    };
}

BLOATDEF Vector3 vec3_negate(Vector3 v)
{
    return (Vector3) {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z,
    };
}

BLOATDEF Vector3 vec3_cross(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.y*v2.z - v1.z*v2.y,
        .y = v1.z*v2.x - v1.x*v2.z,
        .z = v1.x*v2.y - v1.y*v2.x,
    };
}

BLOATDEF float vec3_dot(Vector3 v1, Vector3 v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

// Vector 4
BLOATDEF Vector4 vec4(float x, float y, float z, float w)
{
    return (Vector4) {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
}

// Quaternion
BLOATDEF Quaternion quat(void)
{
    return (Quaternion) {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
        .w = 1.0f,
    };
}

BLOATDEF Quaternion quat_add_quat(Quaternion q1, Quaternion q2)
{
    return (Quaternion) {
        .x = q1.x + q2.x,
        .y = q1.y + q2.y,
        .z = q1.z + q2.z,
        .w = q1.w + q2.w,
    };
}

BLOATDEF Quaternion quat_add_value(Quaternion q, float value)
{
    return (Quaternion) {
        .x = q.x + value,
        .y = q.y + value,
        .z = q.z + value,
        .w = q.w + value,
    };
}

BLOATDEF Quaternion quat_sub_quat(Quaternion q1, Quaternion q2)
{
    return (Quaternion) {
        .x = q1.x - q2.x,
        .y = q1.y - q2.y,
        .z = q1.z - q2.z,
        .w = q1.w - q2.w,
    };
}

BLOATDEF Quaternion quat_sub_value(Quaternion q, float value)
{
    return (Quaternion) {
        .x = q.x - value,
        .y = q.y - value,
        .z = q.z - value,
        .w = q.w - value,
    };
}

BLOATDEF Quaternion quat_scale(Quaternion q, float scale)
{
    return (Quaternion) {
        .x = q.x * scale,
        .y = q.y * scale,
        .z = q.z * scale,
        .w = q.w * scale,
    };
}

BLOATDEF float quat_length_sqr(Quaternion q)
{
    return (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

BLOATDEF Quaternion quat_normalize(Quaternion q)
{
    float length = sqrtf(quat_length_sqr(q));

    if (length == 0.0f) length = 1.0f; 
    float i_length = 1.0f/length;

    return (Quaternion) {
        .x = q.x * i_length,
        .y = q.y * i_length,
        .z = q.z * i_length,
        .w = q.w * i_length,
    };
}
BLOATDEF Quaternion quat_negate(Quaternion q)
{
    return (Quaternion) {
        .x = -q.x,
        .y = -q.y,
        .z = -q.z,
        .w = -q.w,
    };
}

BLOATDEF float quat_dot(Quaternion q1, Quaternion q2)
{
    return (q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w);
}

BLOATDEF bool quat_eq(Quaternion q1, Quaternion q2)
{
    return float_eq(q1.x, q2.x) && float_eq(q1.y, q2.y) && float_eq(q1.z, q2.z) && float_eq(q1.w, q2.w);
}

BLOATDEF bool quat_zeroed(Quaternion q)
{
    return q.x == 0.0f && q.y == 0.0f && q.z == 0.0f && q.w == 0.0f;
}

// Matrix
BLOATDEF Matrix mat_value(float value)
{
    return (Matrix) {
        .m0 = value,
        .m5 = value,
        .m10 = value,
        .m15 = value,
    };
}

BLOATDEF Matrix mat_translate(Vector3 v)
{
    return (Matrix) {
        .m0  = 1.0f,
        .m5  = 1.0f,
        .m10 = 1.0f,
        .m15 = 1.0f,

        .m12 = v.x,
        .m13 = v.y,
        .m14 = v.z,
    };
}

BLOATDEF Matrix mat_multiply(Matrix left, Matrix right)
{
    return (Matrix) {
        .m0  = left.m0 *right.m0  + left.m1 *right.m4 + left.m2*right.m8   + left.m3*right.m12,
        .m1  = left.m0 *right.m1  + left.m1 *right.m5 + left.m2*right.m9   + left.m3*right.m13,
        .m2  = left.m0 *right.m2  + left.m1 *right.m6 + left.m2*right.m10  + left.m3*right.m14,
        .m3  = left.m0 *right.m3  + left.m1 *right.m7 + left.m2*right.m11  + left.m3*right.m15,

        .m4  = left.m4 *right.m0  + left.m5 *right.m4 + left.m6*right.m8   + left.m7*right.m12,
        .m5  = left.m4 *right.m1  + left.m5 *right.m5 + left.m6*right.m9   + left.m7*right.m13,
        .m6  = left.m4 *right.m2  + left.m5 *right.m6 + left.m6*right.m10  + left.m7*right.m14,
        .m7  = left.m4 *right.m3  + left.m5 *right.m7 + left.m6*right.m11  + left.m7*right.m15,

        .m8  = left.m8 *right.m0  + left.m9 *right.m4 + left.m10*right.m8  + left.m11*right.m12,
        .m9  = left.m8 *right.m1  + left.m9 *right.m5 + left.m10*right.m9  + left.m11*right.m13,
        .m10 = left.m8 *right.m2  + left.m9 *right.m6 + left.m10*right.m10 + left.m11*right.m14,
        .m11 = left.m8 *right.m3  + left.m9 *right.m7 + left.m10*right.m11 + left.m11*right.m15,

        .m12 = left.m12*right.m0  + left.m13*right.m4 + left.m14*right.m8  + left.m15*right.m12,
        .m13 = left.m12*right.m1  + left.m13*right.m5 + left.m14*right.m9  + left.m15*right.m13,
        .m14 = left.m12*right.m2  + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14,
        .m15 = left.m12*right.m3  + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15,
    };
}

BLOATDEF Matrix mat_rotate(Vector3 axis, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    float _c = 1.0f - c;

    Vector3 v = vec3_normalize(axis);
    float x = v.x;
    float y = v.y;
    float z = v.z;

    return (Matrix) {
        .m0  = x*x*_c + c,
        .m1  = y*x*_c + z*s,
        .m2  = z*x*_c - y*s,
        .m3  = 0.0f,

        .m4  = x*y*_c - z*s,
        .m5  = y*y*_c + c,
        .m6  = z*y*_c + x*s,
        .m7  = 0.0f,

        .m8  = x*z*_c + y*s,
        .m9  = y*z*_c - x*s,
        .m10 = z*z*_c + c,
        .m11 = 0.0f,

        .m12 = 0.0f,
        .m13 = 0.0f,
        .m14 = 0.0f,
        .m15 = 1.0f,
    };
}

BLOATDEF Matrix mat_scale(Vector3 v)
{
    return (Matrix) {
        .m0  = v.x,
        .m5  = v.y,
        .m10 = v.z,
        .m15 = 1.0f,
    };
}

BLOATDEF Matrix mat_perspective(float fov, float aspect, float near, float far)
{
    float top = near*tanf(fov * 0.5f);
    float bottom = -top;
    float right = top*aspect;
    float left = -right;

    // Frustum
    float x = right - left;
    float y = top - bottom;
    float z = far - near;

    return (Matrix) {
        .m0  = (near * 2.0f) / x,
        .m5  = (near * 2.0f) / y,
        .m8  = (right + left) / x,
        .m9  = (top + bottom) / y,
        .m10 = -(far + near) / z,
        .m11 = -1.0f,
        .m14 = -(far * near * 2.0f) / z,
    };
}

BLOATDEF Matrix mat_look_at(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 d = vec3_normalize(vec3_sub(eye, target));
    Vector3 r = vec3_normalize(vec3_cross(d, up));
    Vector3 u = vec3_cross(r, d);

    return (Matrix) {
        r.x , r.y , r.z , -vec3_dot(r, eye),
        u.x , u.y , u.z , -vec3_dot(u, eye),
        d.x , d.y , d.z , -vec3_dot(d, eye),
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

BLOATDEF MatrixLayout mat_flatten(Matrix m)
{
    return (MatrixLayout) {
        .mat = {
            m.m0 , m.m1 , m.m2 , m.m3,
            m.m4 , m.m5 , m.m6 , m.m7,
            m.m8 , m.m9 , m.m10, m.m11,
            m.m12, m.m13, m.m14, m.m15,
        }
    };
}

#endif // BLOAT_MATH_IMPLEMENTATION
