/**
 * @file		darray.c
 * @author		Andrew Loebs
 * @brief		Dynamically sized array.
 *
 */

#include "darray.h"

#include <stdlib.h>
#include <string.h>


#define DEFAULT_CAPACITY        16
#define GROWTH_FACTOR           2
#define SHRINK_AT_DIVISOR       4

#define LAST_INDEX(d) (DARRAY_COUNT(d) - 1)

// Private function prototypes
static int resize(struct darray *d, unsigned int new_capacity);
static int ensure_capacity_for_add(struct darray *d, unsigned int add_count);
static void reduce_capacity_if_able(struct darray *d);

// Private function implementations
static int resize(struct darray *d, unsigned int new_capacity)
{
    void *new_data = malloc(new_capacity);
    if (!new_data) return -1;

    memcpy(new_data, d->_data, d->_size);
    free(d->_data);
    d->_data = new_data;
    d->_capacity = new_capacity;
    return 0;
}

static int ensure_capacity_for_add(struct darray *d, unsigned int add_count)
{
    // figure out new capacity
    const unsigned int required_capacity = d->_size + DARRAY_OFF_AT(d, add_count);
    unsigned int new_capacity = d->_capacity;
    while (required_capacity > new_capacity) new_capacity *= GROWTH_FACTOR;

    if (new_capacity != d->_capacity) 
        return resize(d, new_capacity);
    
    return 0;
}

static void reduce_capacity_if_able(struct darray *d)
{
    // figure out minimum capacity
    unsigned int new_capacity = d->_capacity;
    while ((d->_size <= (new_capacity / SHRINK_AT_DIVISOR)) && 
            (new_capacity != DARRAY_OFF_AT(d, DEFAULT_CAPACITY)))
        new_capacity /= GROWTH_FACTOR;

    if (new_capacity != d->_capacity)
        // NOTE (aloebs): If resize fails, the old data array will be intact, which is fine here.
        resize(d, new_capacity);
}

// Public function implementations
struct darray *darray_new(unsigned int item_size)
{
    struct darray *d = (struct darray *)malloc(sizeof(struct darray));
    if (!d) return NULL;

    d->_item_size = item_size;
    d->_size = 0;
    d->_capacity = DARRAY_OFF_AT(d, DEFAULT_CAPACITY);
    d->_data = malloc(d->_capacity);
    if (!d->_data) {
        free(d);
        return NULL;
    }

    return d;
}

void darray_destroy(struct darray *d)
{
    free(d->_data);
    free(d);
}

int darray_extend(struct darray *d, void *data, unsigned int count)
{
    if (ensure_capacity_for_add(d, count) < 0) return -1;

    memcpy(DARRAY_VPAT(d, DARRAY_COUNT(d)), data, DARRAY_OFF_AT(d, count));
    d->_size += DARRAY_OFF_AT(d, count);
    return 0;
}

int darray_append(struct darray *d, void *data)
{
    return darray_extend(d, data, 1);
}

int darray_insert(struct darray *d, void *data, unsigned int index)
{
    if (index > DARRAY_COUNT(d)) return -1; // TODO return codes?
    if (ensure_capacity_for_add(d, 1) < 0) return -1;

    // shift right
    for (int i = DARRAY_COUNT(d); i > index; i--)
        memcpy(DARRAY_VPAT(d, i), DARRAY_VPAT(d, i - 1), d->_item_size);

    memcpy(DARRAY_VPAT(d, index), data, d->_item_size);
    d->_size += d->_item_size;
    return 0;
}

void darray_pop(struct darray *d)
{
    if (d->_size) d->_size -= d->_item_size;
    reduce_capacity_if_able(d);
}

void darray_remove(struct darray *d, unsigned int index)
{
    if (index >= DARRAY_COUNT(d)) return;

    for (int i = index; i < (DARRAY_COUNT(d) - 1); i++)
        memcpy(DARRAY_VPAT(d, i), DARRAY_VPAT(d, i + 1), d->_item_size);

    d->_size -= d->_item_size;
    reduce_capacity_if_able(d);
}

void darray_clear(struct darray *d)
{
    d->_size = 0;
    reduce_capacity_if_able(d);
}
