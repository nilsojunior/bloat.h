#ifndef BLOAT_H
#define BLOAT_H

#if defined(BLOAT_IMPLEMENTATION) && defined(BLOAT_STATIC_INLINE)
    #error "Please specify only one between `BLOAT_IMPLEMENTATION` and `BLOAT_STATIC_INLINE`"
#endif

#if defined(BLOAT_STATIC_INLINE)
    #define BLOATDEF static inline
#elif defined(BLOAT_IMPLEMENTATION)
    #define BLOATDEF
#else
    #define BLOATDEF extern
#endif

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdarg.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef size_t usize;
typedef ssize_t isize;

#define DEFER(value) do { result = (value); goto defer; } while(0)

#define SIZEOF(array) sizeof((array)) / sizeof((array[0]))

#define FOREACH(i, array) for (size_t (i) = 0; (i) < sizeof((array)) / sizeof((array)[0]); ++(i))

#define vector_define(T, Name)                  \
    typedef struct {                            \
        (T) *items;                             \
        size_t count;                           \
        size_t cap;                             \
    } (Name);                                   \

#define vector_reserve(v, req_cap)                                        \
do {                                                                      \
    if ((req_cap) > (v)->cap) {                                           \
        if ((v)->cap == 0) {                                              \
            (v)->cap = 256;                                               \
        }                                                                 \
        while ((req_cap) > (v)->cap) {                                    \
            (v)->cap *= 2;                                                \
        }                                                                 \
        (v)->items = realloc((v)->items, (v)->cap * sizeof(*(v)->items)); \
        assert((v)->items != NULL);                                       \
    }                                                                     \
 } while (0)

#define vector_append(v, item)                  \
    do {                                        \
        vector_reserve((v), (v)->count + 1);    \
        (v)->items[(v)->count++] = (item);      \
    } while(0)

#define vector_free(v) free((v).items)

typedef struct {
    const char *str;
    size_t len;
} Slice;

#define SLICE_FMT "%.*s"
#define SLICE_ARG(slice) (int)(slice).len, (slice).str

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
BLOATDEF Slice   slice_size(const char *str, size_t len);
BLOATDEF Slice   slice(const char *str);
BLOATDEF Slice   slice_trim(Slice s);
BLOATDEF Slice   slice_trim_left(Slice s);
BLOATDEF Slice   slice_trim_right(Slice s);
BLOATDEF Slice   slice_split(Slice *s, const char delim);
BLOATDEF Slice   slice_split_whitespace(Slice *s);
BLOATDEF int     slice_parse_int(Slice s);

// Equals
BLOATDEF bool    slice_eq_char(Slice s1, const char s2);
BLOATDEF bool    slice_eq_char_ignorecase(Slice s1, const char s2);
BLOATDEF bool    slice_eq_slice(Slice s1, Slice s2);
BLOATDEF bool    slice_eq_slice_ignorecase(Slice s1, Slice s2);
BLOATDEF bool    slice_eq_str(Slice s1, const char *s2);
BLOATDEF bool    slice_eq_str_ignorecase(Slice s1, const char *s2);

// Starts and ends with
BLOATDEF bool    slice_starts_with_char(Slice s, const char prefix);
BLOATDEF bool    slice_ends_with_char(Slice s, const char suffix);
BLOATDEF bool    slice_starts_with_slice(Slice s, Slice prefix);
BLOATDEF bool    slice_ends_with_slice(Slice s, Slice suffix);
BLOATDEF bool    slice_starts_with_str(Slice s, const char *prefix);
BLOATDEF bool    slice_ends_with_str(Slice s, const char *suffix);

// Print
BLOATDEF void    slice_print(Slice s);
BLOATDEF void    slice_println(Slice s);

// Log
BLOATDEF void    log_info(const char *msg, ...);
BLOATDEF void    log_warn(const char *msg, ...);
BLOATDEF void    log_err(const char *msg, ...);
BLOATDEF void    log_fatal(const char *msg, ...); // Exit program

// C Strings
BLOATDEF void    string_to_lowercase(char *s);
BLOATDEF void    string_to_uppercase(char *s);
BLOATDEF bool    string_eq(const char *s1, const char *s2);
BLOATDEF bool    string_eq_ignorecase(const char *s1, const char *s2);

// Memory
BLOATDEF bool memory_eq(const void *s1, const void *s2, size_t size);

// Char
BLOATDEF char to_lowercase(char c);
BLOATDEF char to_uppercase(char c);
BLOATDEF bool is_whitespace(char c);
BLOATDEF bool is_digit(char c);

// FS
#define fs_to_string(stream, buffer) fs_to_string_size((stream), (buffer), sizeof(buffer) - 1) // This expect the size of the buffer to be the file size + null terminator

BLOATDEF FILE*   fs_open(const char *file_name, const char *mode); // TODO: Maybe add a should crash flag or another function to do that
BLOATDEF long    fs_get_size(FILE *stream); // Returns the size of the file, without the null terminator
BLOATDEF bool    fs_to_string_size(FILE *stream, char *buffer, size_t size); // Returns a null terminated str with the contents of the file

#endif // BLOAT_H

#if defined(BLOAT_STATIC_INLINE) || defined(BLOAT_IMPLEMENTATION)

BLOATDEF char to_lowercase(char c)
{
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
}

BLOATDEF char to_uppercase(char c)
{
    return c >= 'A' && c <= 'Z' ? c : c - 32;
}

BLOATDEF bool is_whitespace(char c)
{
    return c == ' '  || // Space
           c == '\f' || // Form feed
           c == '\n' || // Line feed
           c == '\r' || // Carriage return
           c == '\t' || // Horizontal tab
           c == '\v';   // Vertical tab
}

BLOATDEF bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

BLOATDEF Slice slice_size(const char *str, size_t len)
{
    return (Slice) {
        .str = str,
        .len = len,
    };
}

BLOATDEF Slice slice(const char *str)
{
    return slice_size(str, strlen(str));
}

BLOATDEF bool slice_eq_slice(Slice s1, Slice s2)
{
    return s1.len == s2.len && memory_eq(s1.str, s2.str, s1.len);
}

BLOATDEF bool slice_eq_slice_ignorecase(Slice s1, Slice s2)
{
    if (s1.len != s2.len) return false;
    for (size_t i = 0; i < s1.len; ++i) {
        char c1 = to_lowercase(s1.str[i]);
        char c2 = to_lowercase(s2.str[i]);
        if (c1 != c2) return false;
    }
    return true;
}

BLOATDEF bool slice_eq_str_ignorecase(Slice s1, const char *s2) {
    return slice_eq_slice_ignorecase(s1, slice(s2));
}

BLOATDEF bool slice_eq_char_ignorecase(Slice s1, const char s2) {
    return s1.len == 1 && s1.str[0] == to_lowercase(s2);
}

BLOATDEF bool slice_eq_str(Slice s1, const char *s2)
{
    return slice_eq_slice(s1, slice(s2));
}

BLOATDEF bool slice_eq_char(Slice s1, const char s2)
{
    return s1.len == 1 && s1.str[0] == s2;
}

BLOATDEF bool slice_starts_with_char(Slice s, const char prefix)
{
    return s.len > 0 && s.str[0] == prefix;
}

BLOATDEF bool slice_ends_with_char(Slice s, const char suffix)
{
    return s.len > 0 && s.str[s.len - 1] == suffix;
}

BLOATDEF bool slice_starts_with_slice(Slice s, Slice prefix)
{
    if (prefix.len > s.len) return false;
    Slice actual = slice_size(s.str, prefix.len);
    return slice_eq_slice(prefix, actual);
}

BLOATDEF bool slice_ends_with_slice(Slice s, Slice suffix)
{
    if (suffix.len > s.len) return false;
    Slice actual = slice_size(s.str + (s.len - suffix.len), suffix.len);
    return slice_eq_slice(suffix, actual);
}

BLOATDEF bool slice_starts_with_str(Slice s, const char *prefix)
{
    return slice_starts_with_slice(s, slice(prefix));
}

BLOATDEF bool slice_ends_with_str(Slice s, const char *suffix)
{
    return slice_ends_with_slice(s, slice(suffix));
}

BLOATDEF Slice slice_split(Slice *s, const char delim)
{
    size_t i = 0;
    while (i < s->len && s->str[i] != delim) ++i;

    Slice remainder = slice_size(s->str, i);

    if (i < s->len) ++i;
    s->len -= i;
    s->str += i;

    return remainder;
}

BLOATDEF Slice slice_split_whitespace(Slice *s)
{
    return slice_split(s, ' ');
}

BLOATDEF int slice_parse_int(Slice s)
{
    int value = 0;
    int sign  = 1;
    size_t i  = 0;

    if (s.len == 0) return value;

    if (s.str[0] == '+' || s.str[0] == '-') {
        if (s.str[0] == '-') sign = -1;
        ++i;
    }

    while (i < s.len && is_digit(s.str[i])) {
        value = value * 10 + (s.str[i] - '0');
        ++i;
    }

    return value * sign;
}

BLOATDEF Slice slice_trim_left(Slice s)
{
    size_t i = 0;
    while (i < s.len && is_whitespace(s.str[i])) ++i;
    return slice_size(s.str + i, s.len - i);
}

BLOATDEF Slice slice_trim_right(Slice s)
{
    size_t i = 0;
    while (i < s.len && is_whitespace(s.str[s.len - 1 - i])) ++i;
    return slice_size(s.str, s.len - i);
}

BLOATDEF Slice slice_trim(Slice s)
{
    return slice_trim_left(slice_trim_right(s));
}

BLOATDEF void slice_print(Slice s)
{
    for (size_t i = 0; i < s.len; ++i) putchar(s.str[i]);
}

BLOATDEF void slice_println(Slice s)
{
    slice_print(s); putchar('\n');
}

// Log
void log_info(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "INFO: ");
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

void log_warn(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "WARNING: ");
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

void log_err(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

void log_fatal(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "FATAL: ");
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);

    exit(EXIT_FAILURE);
}

BLOATDEF void string_to_uppercase(char *s)
{
    while (*s) {
        *s = to_uppercase(*s);
        ++s;
    }
}

BLOATDEF void string_to_lowercase(char *s)
{
    while (*s) {
        *s = to_lowercase(*s);
        ++s;
    }
}

BLOATDEF bool string_eq(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return *s1 == *s2;
}

BLOATDEF bool string_eq_ignorecase(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        char c1 = to_lowercase(*s1);
        char c2 = to_lowercase(*s2);
        if (c1 != c2) return false;
        ++s1;
        ++s2;
    }
    return *s1 == *s2;
}

// Memory
BLOATDEF bool memory_eq(const void *s1, const void *s2, size_t size)
{
    const u8 *p1 = (const u8*) s1;
    const u8 *p2 = (const u8*) s2;

    for (u8 i = 0; i < size; ++i) {
        if (p1[i] != p2[i]) return false;
    }

    return true;
}

BLOATDEF FILE* fs_open(const char *file_name, const char *mode)
{
    FILE *f = fopen(file_name, mode);
    if (f == NULL) {
        fprintf(stderr, "ERROR: Failed to open file `%s`: %s\n", file_name, strerror(errno));
        return NULL;
    }
    return f;
}

BLOATDEF long fs_get_size(FILE *stream)
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

BLOATDEF bool fs_to_string_size(FILE *stream, char *buffer, size_t size)
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

#endif // BLOAT_STATIC_INLINE || BLOAT_IMPLEMENTATION
