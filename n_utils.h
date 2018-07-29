#ifndef N_UTILS_H
#define N_UTILS_H

#include <stdint.h>

typedef uint8_t uint8;
typedef uint8_t u8;
typedef int8_t int8;
typedef int8_t i8;

typedef uint16_t uint16;
typedef uint16_t u16;
typedef int16_t int16;
typedef int16_t i16;

typedef uint32_t uint32;
typedef uint32_t u32;
typedef int32_t int32;
typedef int32_t i32;

typedef uint64_t uint64;
typedef uint64_t u64;
typedef int64_t int64;
typedef int64_t i64;

typedef int32 bool32;
typedef float r32;
typedef double r64;
typedef float real32;
typedef double real64;

#if !defined(true) && !defined(false)
typedef int bool;
#define true 1
#define false 0
#endif

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

#define for_each(ptr, type) type *it = 0; for(it = ptr; it; it = it->next)

#include <stdio.h>
// https://www.softwariness.com/articles/assertions-in-cpp/
#ifndef assert
#define assert(expression) if(!(expression)) { _assert(#expression, __FILE__, __LINE__); }
inline void _assert(const char *expression, const char *file, int line)
{
    fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", expression, file, line);
    // abort();
}
#endif

// @file
#ifdef ENABLE_FILE
#include <malloc.h>

#ifndef print
#define print(...) printf(__VA_ARGS__);
#endif

static char *
read_entire_file(char *file_name)
{
    char *result = 0;
    FILE *file = fopen(file_name, "rb");
    
    if(file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result = (char *)malloc(file_size + 1);
        fread(result, file_size, 1, file);
        result[file_size] = 0;

        fclose(file);
    }

    return result;
}

#endif // ENABLE_FILE

// @random_generator
#ifdef ENABLE_RANDOM_GENERATOR
#include <math.h> // for pow, abs

static int n_seed = 123456789;
// https://en.wikipedia.org/wiki/Linear_congruential_generator
inline int n_rand() {
    n_seed = (1103515245 * n_seed + 12345) % ((int)pow(2,31)-1);
    return n_seed;
}

inline int n_randi(int min, int max) {
    int rand_val = (abs(n_rand()) % (max + 1 - min)) + min;
    return rand_val;
};

inline float n_randf(float min, float max) {
    float rand_val = n_randi(min*100, max*100) / 100.0f;
    return rand_val;
};

#endif // ENABLE_RANDOM_GENERATOR

#endif // N_UTILS_H