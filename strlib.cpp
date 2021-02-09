#include "strlib.h"

char& str_t::operator[](size_t index)
{
    return chars[index];
}

char str_t::operator[](size_t index) const
{
    return chars[index];
}

bool str_t::operator<(str_t v) const
{
    return str_view(v) < str_view(*this);
}

bool str_view_t::operator<(str_view_t v) const
{
    return str_compare(*this, v) < 0;
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
    char* data = str_alloc_buffer(length);
    memcpy(data, source, length);
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
    sb.capacity = sb.capacity * 2;
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

int str_compare(str_view_t str1, str_view_t str2)
{
    if (str1.length == str2.length)
    {
        if (str1.chars == str2.chars)
            return 0;
        return memcmp(str1.chars, str2.chars, str1.length);
    }
    return str1.length - str2.length;
}

namespace std
{
    template<> 
    struct hash<str_view_t>
    {
        size_t operator()(str_view_t str) const noexcept
        {
            size_t h = FNV_HASH_OFFSET_BASIS;
            size_t last_size_index = std::max(str.length - sizeof(size_t), (size_t)0);
            for (size_t i = 0; i < last_size_index; i += sizeof(size_t))
            {
                h ^= *(size_t*)(str.chars + i);
                h *= FNV_HASH_PRIME;
            }
            for (size_t i = last_size_index; i < str.length; i++)
            {
                h ^= (size_t)(str.chars[i]) << i * 8;
            }
            if (last_size_index > 0)
            {
                h *= FNV_HASH_PRIME;
            }
            return h;
        }
    };

    template<> 
    struct hash<str_t>
    {
        size_t operator()(str_t str) const noexcept
        {
            return hash<str_view_t>()(str_view(str));
        }
    };
}