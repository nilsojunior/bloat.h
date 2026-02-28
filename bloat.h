#ifndef BLOAT_H
#define BLOAT_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define Defer(value) do { return (value); goto defer; } while(0)

#define PI 3.14159265358979323846f

#define VectorInit(T, Name)                     \
    typedef struct {                            \
        T *items;                               \
        size_t count;                           \
        size_t capacity;                        \
    } Name;                                     \

#define VectorAppend(T, item)                                           \
    do {                                                                \
        if ((T).count >= (T).capacity) {                                \
            if ((T).capacity == 0) (T).capacity = 256;                  \
            else (T).capacity *= 2;                                     \
            (T).items = realloc((T).items, (T).capacity * sizeof(*(T).items)); \
        }                                                               \
        (T).items[(T).count++] = item;                                  \
    } while(0)                                                          \

#define VectorFree(T) free((T).items)

typedef struct {
    const char *string;
    size_t len;
} Slice;

#define slice_starts_with(s1, s2)               \
    _Generic((s2),                              \
             int: slice_starts_with_char,       \
             char: slice_starts_with_char,      \
             char *: slice_starts_with_str,     \
             Slice: slice_starts_with_slice     \
             )(s1, s2)

#define slice_ends_with(s1, s2)                 \
    _Generic((s2),                              \
             int: slice_ends_with_char,         \
             char: slice_ends_with_char,        \
             char *: slice_ends_with_str,       \
             Slice: slice_ends_with_slice       \
             )(s1, s2)

#define slice_eq(s1, s2)                        \
    _Generic((s2),                              \
             char *: slice_eq_str,              \
             Slice: slice_eq_slice              \
             )(s1, s2)

Slice   slice_new_size(const char *string, size_t len);
Slice   slice_new(const char *string);
Slice   slice_trim(Slice slice, int direction); // -1 To trim left, +1 to trim right and 0 to trim both
Slice   slice_until_delim(Slice *slice, const char delim);

bool    slice_eq_slice(Slice s1, Slice s2);
bool    slice_eq_str(Slice s1, const char *s2);

bool    slice_starts_with_char(Slice slice, const char prefix);
bool    slice_ends_with_char(Slice slice, const char suffix);

bool    slice_starts_with_slice(Slice slice, Slice prefix);
bool    slice_ends_with_slice(Slice slice, Slice suffix);

bool    slice_starts_with_str(Slice slice, const char *prefix);
bool    slice_ends_with_str(Slice slice, const char *suffix);

void    slice_print(Slice slice);
void    slice_println(Slice slice);

#endif // BLOAT_H

#ifdef BLOAT_IMPLEMENTATION

Slice slice_new_size(const char *string, size_t len)
{
    return (Slice) {
        .string = string,
        .len = len,
    };
}

Slice slice_new(const char *string)
{
    return slice_new_size(string, strlen(string));
}

bool slice_eq_slice(Slice s1, Slice s2)
{
    return s1.len == s2.len && memcmp(s1.string, s2.string, s1.len) == 0;
}

bool slice_eq_str(Slice s1, const char *s2)
{
    return slice_eq_slice(s1, slice_new(s2));
}

bool slice_starts_with_char(Slice slice, const char prefix)
{
    return slice.len > 0 && slice.string[0] == prefix;
}

bool slice_ends_with_char(Slice slice, const char suffix)
{
    return slice.len > 0 && slice.string[slice.len - 1] == suffix;
}

bool slice_starts_with_slice(Slice slice, Slice prefix)
{
    if (prefix.len > slice.len) return false;
    Slice actual = slice_new_size(slice.string, prefix.len);
    return slice_eq_slice(prefix, actual);
}

bool slice_ends_with_slice(Slice slice, Slice suffix)
{
    if (suffix.len > slice.len) return false;
    Slice actual = slice_new_size(slice.string + (slice.len - suffix.len), suffix.len);
    return slice_eq_slice(suffix, actual);
}

bool slice_starts_with_str(Slice slice, const char *prefix)
{
    return slice_starts_with_slice(slice, slice_new(prefix));
}

bool slice_ends_with_str(Slice slice, const char *suffix)
{
    return slice_ends_with_slice(slice, slice_new(suffix));
}

Slice slice_until_delim(Slice *slice, const char delim)
{
    size_t i = 0;
    while (i < slice->len && slice->string[i] != delim) i++;

    Slice s = slice_new_size(slice->string, i);

    if (i < slice->len) i++;
    slice->len -= i;
    slice->string += i;

    return s;
}

// If direction is positive trim to the right, if negative trim to the left and if 0 trim both
Slice slice_trim(Slice slice, int direction)
{
    size_t start = 0;
    size_t end = slice.len;

    if (direction <= 0) {
        while (start < end && isspace(slice.string[start])) start++;
    }

    if (direction >= 0) {
        while (start < end && isspace(slice.string[end - 1])) end--;
    }

    return slice_new_size(slice.string + start, end - start);
}

void slice_print(Slice slice)
{
    for (size_t i = 0; i < slice.len; i++) putchar(slice.string[i]);
}

void slice_println(Slice slice)
{
    slice_print(slice); putchar('\n');
}

#endif // BLOAT_IMPLEMENTATION
