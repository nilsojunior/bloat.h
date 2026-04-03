#ifndef BLOAT_MATH_H
#define BLOAT_MATH_H

#define PI 3.14159265358979323846f

#define as_radians(x) ((x)) * (PI / 180.0f)

#define clamp01(value) clamp((value), 0.0f, 1.0f)

#define mat() mat_value(1.0f)

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

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y, z, w;
} Vector4;

// 4x4 Matrix
typedef struct {
    float m0, m4, m8 , m12;
    float m1, m5, m9 , m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
} Matrix;

typedef struct { float mat[16]; } MatrixLayout;

float clamp(float value, float min, float max);

Vector3 vec3(float x, float y, float z);
Vector3 vec3_add_vec(Vector3 v1, Vector3 v2);
Vector3 vec3_add_value(Vector3 v, float value);
Vector3 vec3_sub_vec(Vector3 v1, Vector3 v2);
Vector3 vec3_sub_value(Vector3 v, float value);
Vector3 vec3_scale(Vector3 v, float scale);
Vector3 vec3_normalize(Vector3 v);
Vector3 vec3_cross(Vector3 v1, Vector3 v2);
float   vec3_dot(Vector3 v1, Vector3 v2);
float   vec3_lenght_sqr(Vector3 v);

Vector4 vec4(float x, float y, float z, float w);

Matrix mat_value(float value);
Matrix mat_translate(Vector3 v);
Matrix mat_multiply(Matrix left, Matrix right);
Matrix mat_rotate(Vector3 axis, float angle);
Matrix mat_scale(Vector3 v);
Matrix mat_perspective(float fov, float aspect, float near, float far);
Matrix mat_look_at(Vector3 eye, Vector3 target, Vector3 up);
MatrixLayout mat_flatten(Matrix m);

#endif // BLOAT_MATH_H

#ifdef BLOAT_MATH_IMPLEMENTATION

float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Vector3 vec3(float x, float y, float z)
{
    return (Vector3) {
        .x = x,
        .y = y,
        .z = z,
    };
}
Vector3 vec3_add_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
    };
}

Vector3 vec3_add_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x + value,
        .y = v.y + value,
        .z = v.z + value,
    };
}
Vector3 vec3_sub_vec(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
}

Vector3 vec3_sub_value(Vector3 v, float value)
{
    return (Vector3) {
        .x = v.x - value,
        .y = v.y - value,
        .z = v.z - value,
    };
}

Vector3 vec3_scale(Vector3 v, float scale)
{
    return (Vector3) {
        .x = v.x * scale,
        .y = v.y * scale,
        .z = v.z * scale,
    };
}

float vec3_lenght_sqr(Vector3 v)
{
    return (v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3 vec3_normalize(Vector3 v)
{
    float x = v.x;
    float y = v.y;
    float z = v.z;

    float lenght_sqr = vec3_lenght_sqr(v);

    if ((lenght_sqr != 1.0f) && (lenght_sqr != 0.0f)) {
        float i_lenght = 1.0f/sqrtf(lenght_sqr);
        return (Vector3) {
            .x = x * i_lenght,
            .y = y * i_lenght,
            .z = z * i_lenght,
        };
    }

    return v;
}

Vector3 vec3_cross(Vector3 v1, Vector3 v2)
{
    return (Vector3) {
        .x = v1.y*v2.z - v1.z*v2.y,
        .y = v1.z*v2.x - v1.x*v2.z,
        .z = v1.x*v2.y - v1.y*v2.x,
    };
}

float vec3_dot(Vector3 v1, Vector3 v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vector4 vec4(float x, float y, float z, float w)
{
    return (Vector4) {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
}

Matrix mat_value(float value)
{
    return (Matrix) {
        .m0 = value,
        .m5 = value,
        .m10 = value,
        .m15 = value,
    };
}

Matrix mat_translate(Vector3 v)
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

Matrix mat_multiply(Matrix left, Matrix right)
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

Matrix mat_rotate(Vector3 axis, float angle)
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

Matrix mat_scale(Vector3 v)
{
    return (Matrix) {
        .m0  = v.x,
        .m5  = v.y,
        .m10 = v.z,
        .m15 = 1.0f,
    };
}

Matrix mat_perspective(float fov, float aspect, float near, float far)
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

Matrix mat_look_at(Vector3 eye, Vector3 target, Vector3 up)
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

MatrixLayout mat_flatten(Matrix m)
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
