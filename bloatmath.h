#ifndef BLOATMATH_H
#define BLOATMATH_H

#if defined(BLOATMATH_IMPLEMENTATION) && defined(BLOATMATH_STATIC_INLINE)
    #error "Please specify only one between `BLOATMATH_IMPLEMENTATION` and `BLOATMATH_STATIC_INLINE`"
#endif

#if defined(BLOATMATH_STATIC_INLINE)
    #define BLOATMDEF static inline
#elif defined(BLOATMATH_IMPLEMENTATION)
    #define BLOATMDEF
#else
    #define BLOATMDEF extern
#endif

#include <stdbool.h>
#include <math.h>

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
#define as_degrees(x) ((x)) * (180.0f / PI)

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

BLOATMDEF float        clamp(float value, float min, float max);
BLOATMDEF bool         float_eq(float x, float y);

BLOATMDEF Vector2      vec2(float x, float y);
BLOATMDEF Vector2      vec2_add_vec(Vector2 v1, Vector2 v2);
BLOATMDEF Vector2      vec2_add_value(Vector2 v, float value);
BLOATMDEF Vector2      vec2_sub_vec(Vector2 v1, Vector2 v2);
BLOATMDEF Vector2      vec2_sub_value(Vector2 v, float value);
BLOATMDEF Vector2      vec2_scale(Vector2 v, float scale);
BLOATMDEF Vector2      vec2_normalize(Vector2 v);
BLOATMDEF Vector2      vec2_negate(Vector2 v);
BLOATMDEF Vector2      vec2_radians(Vector2 v); // From degrees to radians
BLOATMDEF Vector2      vec2_degrees(Vector2 v); // From radians to degrees
BLOATMDEF float        vec2_cross(Vector2 v1, Vector2 v2);
BLOATMDEF float        vec2_dot(Vector2 v1, Vector2 v2);
BLOATMDEF float        vec2_length_sqr(Vector2 v);
BLOATMDEF bool         vec2_eq(Vector2 v1, Vector2 v2);
BLOATMDEF bool         vec2_zeroed(Vector2 v);

BLOATMDEF Vector3      vec3(float x, float y, float z);
BLOATMDEF Vector3      vec3_add_vec(Vector3 v1, Vector3 v2);
BLOATMDEF Vector3      vec3_add_value(Vector3 v, float value);
BLOATMDEF Vector3      vec3_sub_vec(Vector3 v1, Vector3 v2);
BLOATMDEF Vector3      vec3_sub_value(Vector3 v, float value);
BLOATMDEF Vector3      vec3_scale(Vector3 v, float scale);
BLOATMDEF Vector3      vec3_normalize(Vector3 v);
BLOATMDEF Vector3      vec3_negate(Vector3 v);
BLOATMDEF Vector3      vec3_cross(Vector3 v1, Vector3 v2);
BLOATMDEF Vector3      vec3_radians(Vector3 v); // From degrees to radians
BLOATMDEF Vector3      vec3_degrees(Vector3 v); // From radians to degrees
BLOATMDEF float        vec3_dot(Vector3 v1, Vector3 v2);
BLOATMDEF float        vec3_length_sqr(Vector3 v);
BLOATMDEF bool         vec3_eq(Vector3 v1, Vector3 v2);
BLOATMDEF bool         vec3_zeroed(Vector3 v);

BLOATMDEF Vector4      vec4(float x, float y, float z, float w);

BLOATMDEF Quaternion   quat(void);
BLOATMDEF Quaternion   quat_add_quat(Quaternion q1, Quaternion q2);
BLOATMDEF Quaternion   quat_add_value(Quaternion q, float value);
BLOATMDEF Quaternion   quat_sub_quat(Quaternion q1, Quaternion q2);
BLOATMDEF Quaternion   quat_sub_value(Quaternion q, float value);
BLOATMDEF Quaternion   quat_scale(Quaternion q, float scale);
BLOATMDEF Quaternion   quat_normalize(Quaternion q);
BLOATMDEF Quaternion   quat_conjugate(Quaternion q);
BLOATMDEF Quaternion   quat_invert(Quaternion q);
BLOATMDEF Quaternion   quat_multiply(Quaternion q1, Quaternion q2);
BLOATMDEF Quaternion   quat_from_axis_angle(Vector3 axis, float angle);
BLOATMDEF Quaternion   quat_from_euler(float pitch, float yaw, float roll);
BLOATMDEF Quaternion   quat_from_vec(Vector3 v1, Vector3 v2);
BLOATMDEF Vector3      quat_rotate_by_vec(Quaternion q, Vector3 v);
BLOATMDEF Matrix       quat_to_matrix(Quaternion q);
BLOATMDEF float        quat_dot(Quaternion q1, Quaternion q2);
BLOATMDEF float        quat_length_sqr(Quaternion q);
BLOATMDEF bool         quat_eq(Quaternion q1, Quaternion q2);
BLOATMDEF bool         quat_zeroed(Quaternion q);

BLOATMDEF Matrix       mat_value(float value);
BLOATMDEF Matrix       mat_translate(Vector3 v);
BLOATMDEF Matrix       mat_multiply(Matrix left, Matrix right);
BLOATMDEF Matrix       mat_rotate(Vector3 axis, float angle);
BLOATMDEF Matrix       mat_scale(Vector3 v);
BLOATMDEF Matrix       mat_transpose(Matrix mat);
BLOATMDEF Matrix       mat_perspective(float fov, float aspect, float near, float far);
BLOATMDEF Matrix       mat_look_at(Vector3 eye, Vector3 target, Vector3 up);
BLOATMDEF MatrixLayout mat_flatten(Matrix m);

#endif // BLOATMATH_H

#if defined(BLOATMATH_STATIC_INLINE) || defined(BLOATMATH_IMPLEMENTATION)

BLOATMDEF float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

BLOATMDEF bool float_eq(float x, float y)
{
    return (fabsf(x - y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));
}

// Vector 2
BLOATMDEF Vector2 vec2(float x, float y)
{
    return (Vector2) {
        .x = x,
        .y = y,
    };
}

BLOATMDEF Vector2 vec2_add_vec(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

BLOATMDEF Vector2 vec2_add_value(Vector2 v, float value)
{
    return (Vector2) {
        .x = v.x + value,
        .y = v.y + value,
    };
}

BLOATMDEF Vector2 vec2_sub_vec(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

BLOATMDEF Vector2 vec2_sub_value(Vector2 v, float value)
{
    return (Vector2) {
        .x = v.x - value,
        .y = v.y - value,
    };
}

BLOATMDEF Vector2 vec2_scale(Vector2 v, float scale)
{
    return (Vector2) {
        .x = v.x * scale,
        .y = v.y * scale,
    };
}

BLOATMDEF float vec2_length_sqr(Vector2 v)
{
    return v.x*v.x + v.y*v.y;
}

BLOATMDEF bool vec2_eq(Vector2 v1, Vector2 v2)
{
    return float_eq(v1.x, v2.x) && float_eq(v2.x, v2.y);
}

BLOATMDEF Vector2 vec2_normalize(Vector2 v)
{
    float length = sqrtf(vec2_length_sqr(v));

    if (length == 0.0f) length = 1.0f;
    float i_length = 1.0f/length;

    return (Vector2) {
        .x = v.x * i_length,
        .y = v.y * i_length,
    };
}

BLOATMDEF Vector2 vec2_negate(Vector2 v)
{
    return (Vector2) {
        .x = -v.x,
        .y = -v.y,
    };
}

BLOATMDEF Vector2 vec2_radians(Vector2 v)
{
    return (Vector2) {
        .x = v.x * (PI / 180.0f),
        .y = v.y * (PI / 180.0f),
    };
}

BLOATMDEF Vector2 vec2_degrees(Vector2 v)
{
    return (Vector2) {
        .x = v.x * (180.0f / PI),
        .y = v.y * (180.0f / PI),
    };
}

BLOATMDEF float vec2_cross(Vector2 v1, Vector2 v2)
{
    return (v1.x*v2.y - v1.y*v2.x);
}

BLOATMDEF float vec2_dot(Vector2 v1, Vector2 v2)
{
    return (v1.x*v2.x + v1.y*v2.y);
}

BLOATMDEF bool vec2_zeroed(Vector2 v)
{
    return v.x == 0.0f && v.y == 0.0f;
}

// Vector 3
BLOATMDEF Vector3 vec3(float x, float y, float z)
{
    return (Vector3) {
        .x = x,
        .y = y,
        .z = z,
    };
}

BLOATMDEF Vector3 vec3_add_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
    };
}

BLOATMDEF Vector3 vec3_add_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x + value,
        .y = v.y + value,
        .z = v.z + value,
    };
}

BLOATMDEF Vector3 vec3_sub_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
}

BLOATMDEF Vector3 vec3_sub_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x - value,
        .y = v.y - value,
        .z = v.z - value,
    };
}

BLOATMDEF Vector3 vec3_scale(Vector3 v, float scale)
{
    return (Vector3) {
        .x = v.x * scale,
        .y = v.y * scale,
        .z = v.z * scale,
    };
}

BLOATMDEF float vec3_length_sqr(Vector3 v)
{
    return (v.x*v.x + v.y*v.y + v.z*v.z);
}

BLOATMDEF bool vec3_eq(Vector3 v1, Vector3 v2)
{
    return float_eq(v1.x, v2.x) && float_eq(v1.y, v2.y) && float_eq(v1.z, v2.z);
}

BLOATMDEF bool vec3_zeroed(Vector3 v)
{
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
}

BLOATMDEF Vector3 vec3_normalize(Vector3 v)
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

BLOATMDEF Vector3 vec3_negate(Vector3 v)
{
    return (Vector3) {
        .x = -v.x,
        .y = -v.y,
        .z = -v.z,
    };
}

BLOATMDEF Vector3 vec3_cross(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.y*v2.z - v1.z*v2.y,
        .y = v1.z*v2.x - v1.x*v2.z,
        .z = v1.x*v2.y - v1.y*v2.x,
    };
}

BLOATMDEF Vector3 vec3_radians(Vector3 v)
{
    return (Vector3) {
        .x = v.x * (PI / 180.0f),
        .y = v.y * (PI / 180.0f),
        .z = v.z * (PI / 180.0f),
    };
}

BLOATMDEF Vector3 vec3_degrees(Vector3 v)
{
    return (Vector3) {
        .x = v.x * (180.0f / PI),
        .y = v.y * (180.0f / PI),
        .z = v.z * (180.0f / PI),
    };
}

BLOATMDEF float vec3_dot(Vector3 v1, Vector3 v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

// Vector 4
BLOATMDEF Vector4 vec4(float x, float y, float z, float w)
{
    return (Vector4) {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
}

// Quaternion
BLOATMDEF Quaternion quat(void)
{
    return (Quaternion) {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
        .w = 1.0f,
    };
}

BLOATMDEF Quaternion quat_add_quat(Quaternion q1, Quaternion q2)
{
    return (Quaternion) {
        .x = q1.x + q2.x,
        .y = q1.y + q2.y,
        .z = q1.z + q2.z,
        .w = q1.w + q2.w,
    };
}

BLOATMDEF Quaternion quat_add_value(Quaternion q, float value)
{
    return (Quaternion) {
        .x = q.x + value,
        .y = q.y + value,
        .z = q.z + value,
        .w = q.w + value,
    };
}

BLOATMDEF Quaternion quat_sub_quat(Quaternion q1, Quaternion q2)
{
    return (Quaternion) {
        .x = q1.x - q2.x,
        .y = q1.y - q2.y,
        .z = q1.z - q2.z,
        .w = q1.w - q2.w,
    };
}

BLOATMDEF Quaternion quat_sub_value(Quaternion q, float value)
{
    return (Quaternion) {
        .x = q.x - value,
        .y = q.y - value,
        .z = q.z - value,
        .w = q.w - value,
    };
}

BLOATMDEF Quaternion quat_scale(Quaternion q, float scale)
{
    return (Quaternion) {
        .x = q.x * scale,
        .y = q.y * scale,
        .z = q.z * scale,
        .w = q.w * scale,
    };
}

BLOATMDEF float quat_length_sqr(Quaternion q)
{
    return (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

BLOATMDEF Quaternion quat_normalize(Quaternion q)
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

BLOATMDEF Quaternion quat_conjugate(Quaternion q)
{
    return (Quaternion) {
        .x = -q.x,
        .y = -q.y,
        .z = -q.z,
        .w = q.w,
    };
}

BLOATMDEF Quaternion quat_invert(Quaternion q)
{
    float length = sqrtf(quat_length_sqr(q));

    if (length == 0.0f) return q;

    float inverse_length = 1.0f/length;

    return (Quaternion) {
        .x = q.x * -inverse_length,
        .y = q.y * -inverse_length,
        .z = q.z * -inverse_length,
        .w = q.w *  inverse_length,
    };
}

BLOATMDEF Quaternion quat_multiply(Quaternion q1, Quaternion q2)
{
    return (Quaternion) {
        .x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
        .y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
        .z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
        .w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
    };
}

BLOATMDEF Quaternion quat_from_axis_angle(Vector3 axis, float angle)
{
    float length = vec3_length_sqr(axis);
    if (length == 0.0f) return quat();

    angle *= 0.5;

    axis = vec3_normalize(axis);
    float sin_half = sinf(angle);

    return (Quaternion) {
        .x = axis.x * sin_half,
        .y = axis.y * sin_half,
        .z = axis.z * sin_half,
        .w = cosf(angle),
    };
}

// NOTE: ZYX in radians
BLOATMDEF Quaternion quat_from_euler(float pitch, float yaw, float roll)
{
    pitch *= 0.5f;
    yaw   *= 0.5f;
    roll  *= 0.5f;

    float xx = cosf(pitch);
    float xy = sinf(pitch);
    float yx = cosf(yaw);
    float yy = sinf(yaw);
    float zx = cosf(roll);
    float zy = sinf(roll);

    return (Quaternion) {
        .x = xy*yx*zx - xx*yy*zy,
        .y = xx*yy*zx + xy*yx*zy,
        .z = xx*yx*zy - xy*yy*zx,
        .w = xx*yx*zx + xy*yy*zy,
    };
}

BLOATMDEF Quaternion quat_from_vec(Vector3 v1, Vector3 v2)
{
    float theta   = vec3_dot(v1, v2);
    Vector3 cross = vec3_cross(v1, v2);
    float length  = vec3_length_sqr(cross);

    Quaternion q = {
        .x = cross.x,
        .y = cross.y,
        .z = cross.z,
        .w = sqrtf(length + theta*theta) + theta,
    };

    return quat_normalize(q);
}

BLOATMDEF Vector3 quat_rotate_by_vec(Quaternion q, Vector3 v)
{
    // TODO: Maybe refactor this into a function
    Quaternion vq = { v.x, v.y, v.z, 0.0f };

    Quaternion rotated = quat_multiply(quat_multiply(q, vq), quat_conjugate(q));

    return (Vector3) {
        .x = rotated.x,
        .y = rotated.y,
        .z = rotated.z,
    };
}

BLOATMDEF Matrix quat_to_matrix(Quaternion q)
{
    Matrix mat = mat();

    float x = q.x;
    float y = q.y;
    float z = q.z;
    float w = q.w;

    float xx = x*x;
    float yy = y*y;
    float zz = z*z;
    float xy = x*y;
    float xz = x*z;
    float yz = y*z;
    float wx = w*x;
    float wy = w*y;
    float wz = w*z;

    mat.m0 = 1.0f - 2.0f * (yy + zz);
    mat.m1 = 2.0f * (xy + wz);
    mat.m2 = 2.0f * (xz - wy);

    mat.m4 = 2.0f * (xy - wz);
    mat.m5 = 1.0f - 2.0f * (xx + zz);
    mat.m6 = 2.0f * (yz + wx);

    mat.m8  = 2.0f * (xz + wy);
    mat.m9  = 2.0f * (yz - wx);
    mat.m10 = 1.0f - 2.0f * (xx + yy);

    return mat;
}

BLOATMDEF float quat_dot(Quaternion q1, Quaternion q2)
{
    return (q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w);
}

BLOATMDEF bool quat_eq(Quaternion q1, Quaternion q2)
{
    return float_eq(q1.x, q2.x) && float_eq(q1.y, q2.y) && float_eq(q1.z, q2.z) && float_eq(q1.w, q2.w);
}

BLOATMDEF bool quat_zeroed(Quaternion q)
{
    return q.x == 0.0f && q.y == 0.0f && q.z == 0.0f && q.w == 0.0f;
}

// Matrix
BLOATMDEF Matrix mat_value(float value)
{
    return (Matrix) {
        .m0 = value,
        .m5 = value,
        .m10 = value,
        .m15 = value,
    };
}

BLOATMDEF Matrix mat_translate(Vector3 v)
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

BLOATMDEF Matrix mat_multiply(Matrix left, Matrix right)
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

BLOATMDEF Matrix mat_rotate(Vector3 axis, float angle)
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

BLOATMDEF Matrix mat_scale(Vector3 v)
{
    return (Matrix) {
        .m0  = v.x,
        .m5  = v.y,
        .m10 = v.z,
        .m15 = 1.0f,
    };
}

BLOATMDEF Matrix mat_transpose(Matrix mat)
{
    return (Matrix) {
        .m0  = mat.m0,
        .m1  = mat.m4,
        .m2  = mat.m8,
        .m3  = mat.m12,

        .m4  = mat.m1,
        .m5  = mat.m5,
        .m6  = mat.m9,
        .m7  = mat.m13,

        .m8  = mat.m2,
        .m9  = mat.m6,
        .m10 = mat.m10,
        .m11 = mat.m14,

        .m12 = mat.m3,
        .m13 = mat.m7,
        .m14 = mat.m11,
        .m15 = mat.m15,
    };
}

BLOATMDEF Matrix mat_perspective(float fov, float aspect, float near, float far)
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

BLOATMDEF Matrix mat_look_at(Vector3 eye, Vector3 target, Vector3 up)
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

BLOATMDEF MatrixLayout mat_flatten(Matrix m)
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

#endif // BLOATMATH_STATIC_INLINE || BLOATMATH_IMPLEMENTATION
