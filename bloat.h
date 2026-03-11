#ifndef BLOAT_H
#define BLOAT_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DEFER(value) do { return (value); goto defer; } while(0)

#define PI 3.14159265358979323846f

#define vector_define(T, Name)                  \
    typedef struct {                            \
        T *items;                               \
        size_t count;                           \
        size_t capacity;                        \
    } Name;                                     \

#define vector_append(T, item)                                          \
    do {                                                                \
        if ((T).count >= (T).capacity) {                                \
            if ((T).capacity == 0) (T).capacity = 256;                  \
            else (T).capacity *= 2;                                     \
            (T).items = realloc((T).items, (T).capacity * sizeof(*(T).items)); \
        }                                                               \
        (T).items[(T).count++] = item;                                  \
    } while(0)                                                          \

#define vector_free(T) free((T).items)

typedef struct {
    const char *string;
    size_t len;
} Slice;

#define SLICE_FMT "%s.*"
#define SLICE_ARG(slice) (int)slice.len, slice.string

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
             int: slice_eq_char,                \
             char: slice_eq_char,               \
             char *: slice_eq_str,              \
             Slice: slice_eq_slice              \
             )(s1, s2)

#define slice_eq_ignorecase(s1, s2)             \
    _Generic((s2),                              \
             int: slice_eq_char_ignorecase,     \
             char: slice_eq_char_ignorecase,    \
             char *: slice_eq_str_ignorecase,   \
             Slice: slice_eq_slice_ignorecase   \
             )(s1, s2)

// Slice
Slice   slice_new_size(const char *string, size_t len);
Slice   slice_new(const char *string);
Slice   slice_trim(Slice slice, int direction); // -1 To trim left, +1 to trim right and 0 to trim both
Slice   slice_until_delim(Slice *slice, const char delim);

// Equals
bool    slice_eq_char(Slice s1, const char s2);
bool    slice_eq_char_ignorecase(Slice s1, const char s2);
bool    slice_eq_slice(Slice s1, Slice s2);
bool    slice_eq_slice_ignorecase(Slice s1, Slice s2);
bool    slice_eq_str(Slice s1, const char *s2);
bool    slice_eq_str_ignorecase(Slice s1, const char *s2);

// Starts and ends with
bool    slice_starts_with_char(Slice slice, const char prefix);
bool    slice_ends_with_char(Slice slice, const char suffix);
bool    slice_starts_with_slice(Slice slice, Slice prefix);
bool    slice_ends_with_slice(Slice slice, Slice suffix);
bool    slice_starts_with_str(Slice slice, const char *prefix);
bool    slice_ends_with_str(Slice slice, const char *suffix);

// Print
void    slice_print(Slice slice);
void    slice_println(Slice slice);

// C Strings
void    string_to_lowercase(char *string);
void    string_to_uppercase(char *string);
bool    string_eq(const char *s1, const char *s2);
bool    string_eq_ignorecase(const char *s1, const char *s2);

// FS
#define fs_to_string(stream, buffer) fs_to_string_size((stream), (buffer), sizeof(buffer) - 1) // This expect the size of the buffer to be the file size + null terminator

FILE*   fs_open(const char *file_name, const char *mode); // TODO: Maybe add a should crash flag or another function to do that
long    fs_get_size(FILE *stream); // Returns the size of the file, without the null terminator
bool    fs_to_string_size(FILE *stream, char *buffer, size_t size); // Returns a null terminated string with the contents of the file

#endif // BLOAT_H

#ifdef BLOAT_IMPLEMENTATION

static inline char char_to_lowercase(char c)
{
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
}

static inline char char_to_uppercase(char c)
{
    return c >= 'A' && c <= 'Z' ? c : c + 32;
}

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

bool slice_eq_slice_ignorecase(Slice s1, Slice s2)
{
    if (s1.len != s2.len) return false;
    for (size_t i = 0; i < s1.len; i++) {
        char c1 = char_to_lowercase(s1.string[i]);
        char c2 = char_to_lowercase(s2.string[i]);
        if (c1 != c2) return false;
    }
    return true;
}

bool slice_eq_str_ignorecase(Slice s1, const char *s2) {
    return slice_eq_slice_ignorecase(s1, slice_new(s2));
}

bool slice_eq_char_ignorecase(Slice s1, const char s2) {
    return s1.len == 1 && s1.string[0] == char_to_lowercase(s2);
}

bool slice_eq_str(Slice s1, const char *s2)
{
    return slice_eq_slice(s1, slice_new(s2));
}

bool slice_eq_char(Slice s1, const char s2)
{
    return s1.len == 1 && s1.string[0] == s2;
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

void string_to_uppercase(char *string)
{
    while (*string) {
        *string = char_to_uppercase(*string);
        string++;
    }
}

void string_to_lowercase(char *string)
{
    while (*string) {
        *string = char_to_lowercase(*string);
        string++;
    }
}

bool string_eq(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        if (*s1 != *s2) return false;
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

bool string_eq_ignorecase(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        char c1 = char_to_lowercase(*s1);
        char c2 = char_to_lowercase(*s2);
        if (c1 != c2) return false;
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

FILE* fs_open(const char *file_name, const char *mode)
{
    FILE *f = fopen(file_name, mode);
    if (f == NULL) {
        fprintf(stderr, "ERROR: Failed to open file `%s`: %s\n", file_name, strerror(errno));
        return NULL;
    }
    return f;
}

long fs_get_size(FILE *stream)
{
    long result = 0;

    if (fseek(stream, 0, SEEK_END)) DEFER(0);

    result = ftell(stream);
    if (result < 0) DEFER(0);
    rewind(stream);

 defer:
    if (result == 0) fprintf(stderr, "ERROR: Failed to get file size: %s\n", strerror(errno));

    return result;
}

bool fs_to_string_size(FILE *stream, char *buffer, size_t size)
{
    if (fread(buffer, 1, size, stream) != size) {
        fprintf(stderr, "ERROR: Failed to read file: %s\n", strerror(errno));
        return false;
    }
    if (ferror(stream)) {
        fprintf(stderr, "ERROR: Failed to read file\n");
        return false;
    }

    buffer[size] = '\0';
    return true;
}

#endif // BLOAT_IMPLEMENTATION
