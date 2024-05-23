#if !defined(__LIBTTT_IMPL_H__)
#define __LIBTTT_IMPL_H__

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>

#include "ttt-defs.h"
#include "ttt.h"

/**
 * @defgroup ttt_internal_api_macros Internal API macros
 *
 * @brief Internal API macros defined in `ttt-impl.h`.
 */

/**
 * @defgroup ttt_internal_api_functions Internal API functions
 *
 * @brief Internal API functions defined in `ttt-impl.h`.
 */

/**
 * ttt handle returned by ttt_init().
 */
struct ttt {
  unsigned verbose; /**< Verbosity level. */
};

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Macro for allocating memory. This macro calls C standard library
 * function `calloc()` and casts the returned pointer as a pointer to type `T`.
 *
 * @param T Type of the memory to be allocated.
 * @param n Number of elements of type T to be allocated.
 *
 * @return Pointer to the allocated memory.
 */
#define ttt_calloc(T, n) ((T *)calloc(n, sizeof(T)))

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Macro for re-allocating memory. This macro calls C standard library
 * function `realloc()` and casts the returned pointer as a pointer to type `T`.
 *
 * @param T Type of the memory to be allocated.
 * @param ptr Pointer to already allocated memory.
 * @param n Number of elements of type T to be allocated.
 *
 * @return Pointer to the allocated memory.
 */
#define ttt_realloc(T, ptr, n) ((T *)realloc(ptr, sizeof(T) * (n)))

TTT_INTERN void ttt_free_(void **ptr);

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Macro for freeing memory. This macro calls ttt_free_() function.
 *
 * @param p Pointer to the memory to be freed.
 */
#define ttt_free(p) ttt_free_((void **)p)

/**
 * @defgroup ttt_internal_types Internal types
 *
 * @brief Internal types (structures and enums) used by ttt.
 */

/**
 * @ingroup ttt_internal_types
 *
 * @brief Defines ttt log levels. A log can be an error, warning or general
 * information.
 */
typedef enum {
  TTT_NONE    = 0, /**< No output generated. */
  TTT_ERROR   = 1, /**< Error type. */
  TTT_WARNING = 2, /**< Warning type. */
  TTT_INFO    = 3  /**< Information type. */
} ttt_log_type_t;

TTT_INTERN void ttt_log_init(ttt_log_type_t level);

TTT_INTERN int ttt_log_(ttt_log_type_t type, int error_no, const char *fmt,
                        ...);

#define TTT_VA_REST_CASE_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define TTT_VA_REST_CASE(...)                                                  \
  TTT_VA_REST_CASE_IMPL(__VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 1, 0)

#define TTT_VA_REST_IMPL_2(first, ...) , __VA_ARGS__
#define TTT_VA_REST_IMPL_1(...)
#define TTT_VA_REST_IMPL_(num, ...) TTT_VA_REST_IMPL_##num(__VA_ARGS__)
#define TTT_VA_REST_IMPL(num, ...)  TTT_VA_REST_IMPL_(num, __VA_ARGS__)
#define TTT_VA_REST(...)                                                       \
  TTT_VA_REST_IMPL(TTT_VA_REST_CASE(__VA_ARGS__), __VA_ARGS__)

#define TTT_VA_FIRST_IMPL(first, ...) first
#define TTT_VA_FIRST(...)             TTT_VA_FIRST_IMPL(__VA_ARGS__, throwaway)

#define ttt_log(type, error_no, ...)                                           \
  ttt_log_(type, error_no, TTT_VA_FIRST(__VA_ARGS__), __FILE__,                \
           __LINE__ TTT_VA_REST(__VA_ARGS__))

TTT_INTERN int ttt_log_finalize(void);

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Check the return value of an internal ttt call. Return the error
 * id if it is not equal to TTT_SUCCESS. ttt internal API functions return
 * error ids generated by ttt_log() function.
 *
 * @param call Return value from an internal ttt call.
 */

#define ttt_check(call)                                                        \
  {                                                                            \
    int status = (call);                                                       \
    if (status != TTT_SUCCESS) return status;                                  \
  }

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Check if the given pointer is NULL.
 *
 * @param ptr Pointer to be checked.
 */
#define ttt_check_ptr(ptr)                                                     \
  {                                                                            \
    if (ptr == NULL) return TTT_INVALID_USER_INPUT;                            \
  }

/**
 * @ingroup ttt_internal_api_macros
 *
 * @brief Check if the dereferenced value of a pointer is NULL.
 *
 * @param ptr Address of the pointer to be checked.
 */
#define ttt_check_ptr2(ptr)                                                    \
  {                                                                            \
    if (*ptr == NULL) return TTT_INVALID_USER_INPUT;                           \
  }

#endif // __LIBTTT_IMPL_H__
