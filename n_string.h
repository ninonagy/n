#ifndef N_STRING_H
#define N_STRING_H

#include <assert.h>
#include <string.h>
#include <malloc.h>

#if !defined(__cplusplus)
typedef int bool;
#define true 1
#define false 0
#endif

static const int npos = -1;

enum white_chars
{
    char_space =    ' ',
    char_htab =     '\t',
    char_newline =  '\n',
    char_vtab =     '\v',
    char_feed =     '\f',
    char_return =   '\r',
};

static bool is_white(char c)
{
    bool result = ((c == char_space) ||
        	       (c == char_htab) ||
        	       (c == char_newline) ||
        	       (c == char_vtab) ||
        	       (c == char_feed) ||
        	       (c == char_return));
    return result;
}

static bool is_end_of_line(char c)
{
    bool result = ((c == char_newline) ||
                   (c == char_return));
    return result;
}

static bool is_letter(char a)
{
    if((a >= 'a' && a <= 'z') ||
       (a >= 'A' && a <= 'Z'))
        return true;
    return false;
}

static bool is_number(char a)
{
    if(a >= '0' && a <= '9')
        return true;
	return false;
}

// TODO: char_to_upper
static char 
char_to_lower(char c)
{
    if(c >= 'A' && c <= 'Z') return (c - ('Z'-'z'));
    else return c; 
}

// TODO: Make faster? use memcpy instead
static void 
copy_string(char const *source,
            char *dest, int length)
{
    assert(source && dest);
    for(int i = 0; i < length; ++i)
    {
        *(dest+i) = *(source+i);
    }
}

static int cstring_len(const char *str)
{
    assert(str);
    int count = 0;
    while(*str++) ++count;
    return count;
}

#define N_FREE(ptr) free(ptr)

typedef char *n_string;

typedef struct
{
	size_t len;
	size_t cap;
} n_string_header;

#define N_STRING_HEADER(str) ((n_string_header *)str - 1)

// typedef struct n_string
// {
	// char *ptr;
	// int length;
// #if defined(__cplusplus)
	// n_string(char const *string, int length = 0)
	// {
		//TODO: Case for null pointer
		// if (length == 0) length = string_len(string); // use string length
		// return new_string_len(string, length);
	// };
// #endif
// } n_string;

static n_string new_string(const char *string);
static n_string new_string_len(const char *string, int length);
inline n_string add_string(n_string str, n_string other);
static n_string add_string_len(n_string str, const char *other, size_t other_len);
inline n_string add_cstring(n_string str, const char *other);
static void string_to_lower(n_string str);
static bool strings_are_equal(const char *A, const char *B);
static bool strings_are_equal_to(const char *A, const char *B, int length);
static bool string_starts_with(n_string str, const char *other);
static void string_erase_len(n_string str, int at, int len);
inline void string_erase_at(n_string str, int at);
static bool string_contains(n_string str, const char *other);
static int string_find_from(n_string str, char const *cstr, int from);
static int string_find(n_string str, char const *cstr);
static n_string set_string(n_string str, const char *cstr);
static size_t string_len(n_string str);
static size_t string_cap(n_string str);
inline void set_string_len(n_string str, size_t len);
inline void set_string_cap(n_string str, size_t cap);
static n_string string_new_space_for(n_string str, size_t add_len);

#if 0
#if defined(__cplusplus)
inline n_string
operator + (n_string A, n_string B)
{
    return add_string(A, B);
}

inline bool
operator == (const n_string& A, const n_string& B)
{
    bool result = false;
    if(strings_are_equal(A, B)) 
		result = true;
    return result;
}

inline bool
operator != (const n_string& A, const n_string& B)
{
    bool result = false;
    if(!(A == B)) result = true;
    return result;
}
#endif
#endif

static size_t string_len(n_string str)
{
    return N_STRING_HEADER(str)->len;
}

static size_t string_cap(n_string str)
{
    return N_STRING_HEADER(str)->cap;
}

inline void set_string_len(n_string str, size_t len)
{
    N_STRING_HEADER(str)->len = len;
}

inline void set_string_cap(n_string str, size_t cap)
{
    N_STRING_HEADER(str)->cap = cap;
}

static bool string_end(char *cstr)
{
    if (*cstr == '\0') return true;
    return false;
}

// TODO: Rework?
static int 
naive_string_search(n_string str, const char *other)
{
    int pos = 0;
    char *on = (char *)str;

    while(!string_end(on))
    {
        // TODO: Making new string for 'on' seems unefficient
        if(!string_starts_with((char*)on, (char*)other)) {
            ++on;
            ++pos;
        }
        else return pos;
    }
    return -1;
}

static bool 
string_contains(n_string str, const char *other)
{
    bool result = false;
    if(naive_string_search(str, other) >= 0)
        result = true;
    return result;
}

static n_string
new_string_len(const char *str, int length)
{
    n_string result;
    n_string_header *header;
    size_t header_size = sizeof(n_string_header);
    void *ptr = malloc(header_size + length + 1);
    // TODO: str null case
    if(ptr == 0) return 0;

    result = (char *)ptr + header_size;
    header = N_STRING_HEADER(result);
    header->len = length;
    header->cap = length;
    if(length && str)
        copy_string(str, result, length);
    result[length] = '\0';
    
    return result;
}

static n_string
new_string(const char *str)
{
    int len = str ? cstring_len(str) : 0;
    return new_string_len(str, len);
}

void free_string(n_string str)
{
    if(str == 0) return;
    N_FREE(N_STRING_HEADER(str));
}

inline void empty_string(n_string str)
{
    set_string_len(str, 0);
    str[0] = '\0';
}

static n_string
set_string(n_string str, const char *cstr)
{
    size_t len = cstring_len(cstr);
    if(string_cap(str) < len)
    {
        str = string_new_space_for(str, len - string_len(str));
        if(str == 0) return 0;
    }

    memcpy(str, cstr, len);
    str[len] = '\0';
    set_string_len(str, len);

    return str;
}

static void *
string_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;
    if(ptr == 0) return malloc(new_size);

    if(new_size < old_size) new_size = old_size;
    if(old_size == new_size) return ptr;

    new_ptr = malloc(new_size);
    if(new_ptr == 0) return 0;

    memcpy(new_ptr, ptr, old_size);

    N_FREE(ptr);
    
    return new_ptr;
}

static n_string
string_new_space_for(n_string str, size_t add_len)
{
    size_t len = string_len(str);
    size_t new_len = len + add_len;
    size_t available, old_size, new_size;

    available = string_cap(str) - len;
    if(available >= add_len) return str; // no space left

    void *ptr, *new_ptr;
    ptr = (char *)str - sizeof(n_string_header);
    old_size = sizeof(n_string_header) + string_len(str) + 1;
	new_size = sizeof(n_string_header) + new_len + 1;

    new_ptr = string_realloc(ptr, old_size, new_size);
    if(new_ptr == 0) return 0;
    str = (char *)new_ptr + sizeof(n_string_header);

    set_string_cap(str, new_len);

    return str;
}

static n_string
add_string_len(n_string str, const char *other, size_t other_len)
{
    size_t str_len = string_len(str);
    
    str = string_new_space_for(str, other_len);
    if(str == 0) return 0;

    memcpy(str + str_len, other, other_len);
    str[str_len + other_len] = '\0';
    set_string_len(str, str_len + other_len);

    return str;
}

inline n_string add_string(n_string str, n_string other)
{
    return add_string_len(str, other, string_len(other));
}

inline n_string add_cstring(n_string str, char const *other)
{
    return add_string_len(str, other, cstring_len(other));
}

static void string_to_lower(n_string str)
{
    char *at = str;
    int counter = 0;
	int len = cstring_len(str);
    while(counter++ < len)
    {
        if (is_letter(*at)) {
            *at = char_to_lower(*at);
        }
        at++;
    }
}

static bool 
strings_are_equal(const char *A, const char *B)
{
    while(*A && *B && *A == *B)
    {
        ++A;
        ++B;
    }
    bool result = (*A == 0 && *B == 0);
    return result;
}

static bool 
strings_are_equal_to(const char *A, const char *B, int length)
{
    // TODO: length < a_len && b_len
    bool result = false;
    int counter = 0;
    while(*A && *B && *A == *B)
    {
        if(counter++ >= length-1)
        {
            result = true;
            break;
        }
        
        ++A;
        ++B;
    }
    return result;
}

static bool 
string_starts_with(n_string str, const char *other)
{
    bool result = false;
    if(strings_are_equal_to(str, other, cstring_len(other))) {
        result = true;
    }
    return result;
}

static bool 
string_contains_number(n_string str)
{
    for(int i = 0;
        i < string_len(str);
        ++i)
    {
        if(is_number(str[i])) return true;
    }
    return false;
}

static char
string_at(n_string str, int at)
{
    assert(at < string_len(str));
    return *(str + at);
}

static int 
string_find_from(n_string str, char const *cstr, int from)
{
    assert(from < string_len(str));
    return naive_string_search(str + from, cstr);
}

static int string_find(n_string str, char const *cstr)
{
    return string_find_from(str, cstr, 0);
}

static void
string_erase_len(n_string str, int at, int len)
{
    /*  0 |1 2 3 4 5 . . .|  */
    size_t str_len = string_len(str);
    if((at + len) > str_len)
        // len = len - ((at + len) - str_len);
        len = str_len - at;

    int other_size = str_len - (at + len);
    if(other_size)
    {
        // TODO: Use memmove();
        char *tmp = (char *)malloc(other_size);
        copy_string(str + at + len, tmp, other_size);
        copy_string(tmp, str + at, other_size);
    }
    str[str_len - len] = '\0';

    set_string_len(str, str_len - len);
}

inline void
string_erase_at(n_string str, int at)
{
    string_erase_len(str, at, 1);
}

static int
string_find_chars(n_string str, char const *chars, int from)
{
    int pos = npos;
    int len = cstring_len(chars);

    for(int i = 0; i < len; ++i)
    {
        n_string c = new_string_len(chars + i, 1);
        pos = string_find_from(str, c, from);
        if(pos != npos) break;
    }

    return pos;
}

static void
string_remove_chars(n_string str, char const *chars)
{
#if 1
    int pos = 0;

    while(pos < string_len(str))
	{
        bool found = false;
        
        pos = string_find_chars(str, chars, 0);
        if(pos >= 0)
        {
            found = true;
            string_erase_at(str, pos);
        }
        
        if(!found) break;
	}
#else
    int pos = 0;
    int len = cstring_len(chars);

	while(pos < string_len(str))
	{
        char char_at = str[pos];
        bool found = false;

		for(int i = 0; i < len; ++i) {
			if(char_at == chars[i]) {
                string_erase_at(str, pos);
                found = true;
                break;
            }
		}

        if(!found) ++pos;
	}
#endif
}

static void
string_replace(n_string str, char const *cstr_a, char const *cstr_b)
{
    int at = 0;
    n_string string = str;
    
    while(at != npos)
    {
        at = string_find(string, cstr_a);
        if(at != npos)
        {
            // NOTE: Make copy of right string
            n_string str_d = new_string(string + at + cstring_len(cstr_a));
            
            string_erase_len(string, at, string_len(string));
            string = add_cstring(string, cstr_b);
        
            string = add_string(string, str_d);
        }
    }
}

static n_string
string_consume_while(char **source, const char *string)
{
    // TODO: Rework
    char *at = *source;
    int length = 0;
    size_t len = cstring_len(string);
    while(!string_end(at) && strncmp(at, string, len) != 0)
    {
        ++length;
        ++at;
    }

    n_string result = new_string_len(*source, length);
	*source += length;

	return result;
}

#endif // N_STRING_H
