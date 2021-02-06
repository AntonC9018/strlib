# strlib - A C-style string library for C++

`strlib` is a really simple library for C-style string manipulation in C++. Includes types for a string with a malloc-ed buffer `str_t`, a string view `str_view_t` (either over literal or over a str_t) and a string builder `str_builder_t` and some useful functions over that.

Every string is both null terminated and has a length field, which makes it very comfortable to use with C's string.h functions.

## Demo

```c++
#include "strlib.h"
#include "strlib.cpp"
#include <stdio.h>

int main()
{
    {
        str_t s = str_copy_lit("Hello");
        puts(s.chars); // Hello

        str_t s2 = str_copy(s);
        str_free(s);
        puts(s2.chars); // Hello

        str_view_t sv = str_view(s2);
        puts(sv.chars); // Hello
        str_free(s2);

        str_view_t sv2 = str_lit("Hello2");
        printf("%s | %zu\n", sv2.chars, sv2.length); // Hello2 | 6
    }
    {
        str_builder_t sb = strb_create(5);
        strb_cat(sb, "Hello"); // the size if set to 5, but the buffer
                               // is rescaled when that limit is reached.
        strb_chr(sb, ',');
        strb_chr(sb, ' ');
        strb_cat(sb, "World!");

        str_t str = strb_build(sb);
        puts(str.chars); // Hello, World!
    }
    return 0;
}
```