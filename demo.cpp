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
        strb_cat(sb, "Hello");
        strb_chr(sb, ',');
        strb_chr(sb, ' ');
        strb_cat(sb, "World!");

        str_t str = strb_build(sb);
        puts(str.chars); // Hello, World!
    }
    return 0;
}