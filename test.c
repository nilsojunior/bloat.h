#define BLOAT_IMPLEMENTATION
#include "bloat.h"
#include <assert.h>

int main()
{
    {
        Slice s1 = slice_new("slice 1 test from slice 1");
        Slice s2 = slice_new("not slice 1");

        assert(slice_eq(s1, "slice 1 test from slice 1"));
        assert(!slice_eq(s1, s2));

        assert(slice_eq_ignorecase(s1, "slice 1 test from slice 1"));
        assert(slice_eq_ignorecase(s1, "Slice 1 Test From Slice 1"));
        assert(!slice_eq_ignorecase(s1, " slice 1 test from slice 1"));
    }
    {
        Slice s1 = slice_new("s");

        assert(slice_eq(s1, 's'));
        assert(!slice_eq(s1, 'S'));

        assert(slice_eq_ignorecase(s1, 'S'));
        assert(slice_eq_ignorecase(s1, 's'));
        assert(!slice_eq_ignorecase(s1, 'f'));
    }
    {
        Slice s1 = slice_new("slice 1 test from slice 1");
        s1 = slice_until_delim(&s1, ' ');

        assert(slice_starts_with(s1, 's'));
        assert(!slice_starts_with(s1, 'd'));
    }
    {
        Slice s1 = slice_new("slice 1 test from slice 1");

        assert(slice_ends_with(s1, '1'));
        assert(!slice_ends_with(s1, '2'));
    }
    {
        Slice s1 = slice_new("     slice 1 test from slice 1");
        Slice actual = slice_trim(s1, -1);
        Slice expected = slice_new("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice_new("     slice 1 test from slice 1");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice_new("slice 1 test from slice 1     ");
        Slice actual = slice_trim(s1, 1);
        Slice expected = slice_new("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice_new("slice 1 test from slice 1     ");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice_new("     slice 1 test from slice 1     ");
        Slice actual = slice_trim(s1, 0);
        Slice expected = slice_new("slice 1 test from slice 1");

        assert(slice_eq(expected, actual));

        expected = slice_new("     slice 1 test from slice 1     ");
        assert(!slice_eq(expected, actual));
    }
    {
        Slice s1 = slice_new("slice 1 test from slice 1");

        assert(slice_starts_with(s1, slice_new("slice")));
        assert(slice_starts_with(s1, "slice"));

        assert(!slice_starts_with(s1, slice_new("Slice")));
        assert(!slice_starts_with(s1, "Slice"));
    }
    {
        Slice s1 = slice_new("slice 1 test from slice 1");

        assert(slice_ends_with(s1, slice_new("1")));
        assert(slice_ends_with(s1, "1"));

        assert(!slice_ends_with(s1, slice_new("1 ")));
        assert(!slice_ends_with(s1, "1 "));
    }

    return 0;
}
