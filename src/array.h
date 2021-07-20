/**
 * @file		array.h
 * @author		Andrew Loebs
 * @brief		Common utilities for C-arrays.
 *
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#define ARRAY_COUNT(a) (sizeof((a)) / sizeof(*(a)))

#define ARRAY_PFOREACH(a, pvar, type) \
    for (type *pvar = a; pvar != (a + ARRAY_COUNT(a)); pvar++)

#endif // ARRAY_H_
