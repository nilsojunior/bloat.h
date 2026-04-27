#define BLOAT_IMPLEMENTATION
#include "bloat.h"
#include <assert.h>

int main()
{
    {
        Slice s1 = slice("slice 1 test from slice 1");
        Slice s2 = slice("not slice 1");

        assert(slice_eq(s1, "slice 1 test from slice 1"));
        assert(!slice_eq(s1, s2));

        assert(slice_eq_ignorecase(s1, "slice 1 test from slice 1"));
        assert(slice_eq_ignorecase(s1, "Slice 1 Test From Slice 1"));
        assert(!slice_eq_ignorecase(s1, " slice 1 test from slice 1"));
    }
    {
        Slice s1 = slice("s");

        assert(slice_eq(s1, 's'));
        assert(!slice_eq(s1, 'S'));

        assert(slice_eq_ignorecase(s1, 'S'));
        assert(slice_eq_ignorecase(s1, 's'));
        assert(!slice_eq_ignorecase(s1, 'f'));
    }
    {
        Slice s1 = slice("slice 1 test from slice 1");
        s1 = slice_split(&s1, ' ');

        assert(slice_starts_with(s1, 's'));
        assert(!slice_starts_with(s1, 'd'));
    }
    {
        Slice s1 = slice("slice 1 test from slice 1");

        assert(slice_ends_with(s1, '1'));
        assert(!slice_ends_with(s1, '2'));
    }
    {
        Slice s1 = slice("     slice 1 test from slice 1");
        Slice actual = slice_trim_left(s1);
        Slice expected = slice("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice("     slice 1 test from slice 1");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice("slice 1 test from slice 1     ");
        Slice actual = slice_trim_right(s1);
        Slice expected = slice("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice("slice 1 test from slice 1     ");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice("     slice 1 test from slice 1     ");
        Slice actual = slice_trim(s1);
        Slice expected = slice("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice("     slice 1 test from slice 1     ");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice("slice 1 test from slice 1");

        assert(slice_starts_with(s1, slice("slice")));
        assert(slice_starts_with(s1, "slice"));

        assert(!slice_starts_with(s1, slice("Slice")));
        assert(!slice_starts_with(s1, "Slice"));
    }
    {
        Slice s1 = slice("slice 1 test from slice 1");

        assert(slice_ends_with(s1, slice("1")));
        assert(slice_ends_with(s1, "1"));

        assert(!slice_ends_with(s1, slice("1 ")));
        assert(!slice_ends_with(s1, "1 "));
    }
    {
        FILE *f = fs_open("testfile", "rb");
        size_t size = fs_get_size(f);
        char buffer[size + 1];
        fs_to_string(f, buffer);

        assert(string_eq(buffer, "This is a testfile\n"));

        fclose(f);
    }
    {
        Slice s1 = slice("123");

        assert(slice_parse_int(s1) == 123);
    }

    return 0;
}
