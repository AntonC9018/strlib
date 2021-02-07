#include "strlib.h"

char& str_t::operator[](size_t index)
{
    return chars[index];
}

char str_t::operator[](size_t index) const
{
    return chars[index];
}

char str_view_t::operator[](size_t index) const
{
    return chars[index];
}

str_t str_copy(str_t source)
{
    return str_copy(source.chars, source.length); 
}

str_t str_copy(const char* source)
{
    size_t length = strlen(source);
    char* chars = str_malloc_buffer(length);
    memcpy(chars, source, length + 1);
    return { chars, length };
}

str_t str_copy(const char* source, size_t length)
{
    char* data = str_malloc_buffer(length);
    memcpy(data, source, length + 1);
    return { data, length };
}

str_view_t str_trim_front(str_view_t str, char to_trim)
{
    const char* data = str.chars;
    while (*data != 0 && *data == to_trim)
    {
        data++;
    }
    return { data, str.length };
}

const char* str_max_char(str_view_t str)
{
    const char* result = str.chars;
    const char* data = str.chars;
    data++;
    while (*data != 0)
    {
        if (*data > *result)
        {
            result = data;
        }
        data++;
    }
    return result;
}


str_builder_t strb_make(size_t length)
{
    char* data = str_malloc_buffer(length);
    return { data, 0, length };
}

void strb_free(str_builder_t& sb)
{
    free(sb.chars);
}

void strb_double_buffer(str_builder_t& sb)
{
    char* data = str_malloc_buffer(sb.capacity * 2);
    memcpy(data, sb.chars, sb.char_count);
    free(sb.chars);
    sb.chars = data;
}

#define maybe_double(sb) if \
    ((sb).char_count >= (sb).capacity - 1) \
        strb_double_buffer((sb));

#define strb_append_char(sb, chr) \
    maybe_double((sb)); \
    (sb).chars[(sb).char_count] = (chr); \
    (sb).char_count++

void strb_cat(str_builder_t& sb, const char* src)
{
    while (*src != 0)
    {
        strb_append_char(sb, *src);
        src++;
    }
}

void strb_chr(str_builder_t& sb, char ch)
{
    strb_append_char(sb, ch);
}

void strb_null_terminate(str_builder_t& sb)
{
    sb.chars[sb.char_count] = 0;
}

void strb_reset(str_builder_t& sb)
{
    sb.char_count = 0;
}

void strb_cat_until(str_builder_t& sb, const char* src, char stop)
{
    while (*src != 0 && *src != stop)
    {
        strb_append_char(sb, *src);
        src++;
    }
}

// void strb_trim_front(str_builder_t& sb, char to_trim);

void strb_trim_back(str_builder_t& sb, char to_trim)
{
    do
    {
        sb.char_count--;
    }
    while (sb.char_count > 0 && sb.chars[sb.char_count] != to_trim);
}

str_t strb_build(str_builder_t& sb)
{
    strb_null_terminate(sb);
    return { sb.chars, sb.char_count };
}

bool strb_has_char(str_builder_t& sb, char ch)
{
    for (size_t i = 0; i < sb.char_count; i++)
    {
        if (sb.chars[i] == ch)
        {
            return true;
        }
    }
    return false;
}