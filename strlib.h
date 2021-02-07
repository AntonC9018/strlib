#ifndef STRLIB_INCLUDE
#define STRLIB_INCLUDE 1
#include <stdlib.h>
#include <string.h>

// String with static literal as its buffer or points at another str_t's buffer.
// Should not be freed.
struct str_view_t
{
    const char* chars; // A null terminated char array.
    size_t length;     // Size of the data buffer, excluding the null character.

    char operator[](size_t index) const;
};

struct str_t
{
    char* chars;       // A null terminated char array.
    size_t length;     // Size of the data buffer, excluding the null character.

    char &operator[](size_t index);
};

#define STR_NULL { NULL, 0 }

inline void str_free(str_t str)
{
    free((void*)str.chars);
}

// Allocates length bytes and fills them with zeros.
inline char* str_calloc_buffer(size_t length)
{
    return (char*) calloc(1, length + 1);
}
// Allocates length bytes.
inline char* str_malloc_buffer(size_t length)
{
    return (char*) malloc(length + 1);
}
// Allocates length bytes and inserts a null character at the end.
inline char* str_alloc_buffer(size_t length)
{
    char* data = (char*) malloc(length + 1);
    data[length] = 0;
    return data;
}

// Allocates a buffer, filling it with 0, and wraps it in a string
inline str_t str_make_zeros(size_t length)
{
    return { str_calloc_buffer(length), length };
}
// Allocates a buffer, and null terminates it
inline str_t str_make(size_t length)
{
    return { str_alloc_buffer(length), length };
}
// Allocates a buffer but leaves all the garbage in it
inline str_t make_unitialized(size_t length)
{
    return { str_malloc_buffer(length), length };
}
// Allocates a buffer big enough to hold the source string.
// Copies the source string into that buffer.
str_t str_copy(str_t source);
// Use this if you don't know the size of the buffer.
// This function finds out the size of the buffer with strlen().
str_t str_copy(const char* source);
str_t str_copy(const char* source, size_t length);
// Copy from a string view.
inline str_t str_copy_lit(str_view_t str)
{
    return str_copy(str.chars, str.length);
}
// Use this to copy a string literal.
#define str_copy_lit(string) str_copy((string), sizeof((string)) - 1)
// Wraps a string literal in a strview_t.
#define str_lit(string) str_view_t{ (string), sizeof((string)) - 1 }

inline str_view_t str_view(str_t str)
{
    return { str.chars, str.length };
}

// Increments the pointer until the first occurence of a character which isn't whitespace.
str_view_t str_trim_front(str_view_t str, char to_trim = ' ');

// Returns a pointer to the character with highest value.
const char* str_max_char(str_view_t str);

inline bool str_has_char(str_t str, char ch)
{
    return strchr(str.chars, ch) == 0;
}

inline bool str_has_char(str_view_t str, char ch)
{
    return strchr(str.chars, ch) == 0;
}

inline char* str_find_char(str_t str, char ch)
{
    return strchr(str.chars, ch);
}

inline const char* str_find_char(str_view_t str, char ch)
{
    return strchr(str.chars, ch);
}

inline size_t str_find_char_index(str_view_t str, char ch)
{
    const char* p = strchr(str.chars, ch);
    return p != NULL ? p - str.chars : -1;
}


struct str_builder_t
{
    char* chars;
    size_t char_count;
    size_t capacity;
};

str_builder_t strb_create(size_t size = 64);
str_builder_t strb_from(str_view_t str);
void strb_free(str_builder_t& sb);
void strb_cat(str_builder_t& sb, const char* src);
void strb_chr(str_builder_t& sb, char ch);
void strb_null_terminate(str_builder_t& sb);
void strb_reset(str_builder_t& sb);
void strb_cat_until(str_builder_t& sb, const char* src, char stop);
// void strb_trim_front(str_builder_t& sb, char to_trim = ' ');
void strb_trim_back(str_builder_t& sb, char to_trim = ' ');
str_t strb_build(str_builder_t& sb);

#endif