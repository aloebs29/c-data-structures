/**
 * @file		darray.h
 * @author		Andrew Loebs
 * @brief		Dynamically sized array.
 *
 */

#ifndef DARRAY_H_
#define DARRAY_H_

struct darray {
    void *_data;
    unsigned int _item_size;
    unsigned int _size;
    unsigned int _capacity;
};

struct darray *darray_new(unsigned int item_size);

void darray_destroy(struct darray *d);

int darray_extend(struct darray *d, void *data, unsigned int count);

int darray_append(struct darray *d, void *data);

int darray_insert(struct darray *d, void *data, unsigned int index);

void darray_pop(struct darray *d);

void darray_remove(struct darray *d, unsigned int index);

void darray_clear(struct darray *d);

#define DARRAY_COUNT(d) (DARRAY_I_FROM_OFF((d), (d)->_size))

#define DARRAY_AT(d, index, type) (*DARRAY_PAT((d), (index), type))

#define DARRAY_PAT(d, index, type) ((type *)DARRAY_VPAT((d), (index)))

#define DARRAY_VPAT(d, index) ((void *)((char *)(d)->_data + DARRAY_OFF_AT((d), (index))))

#define DARRAY_OFF_AT(d, index) ((index) * (d)->_item_size)

#define DARRAY_I_FROM_OFF(d, off) ((off) / (d)->_item_size)

#define DARRAY_PFOREACH(d, pvar, type) \
    for (type *pvar = DARRAY_PAT(d, 0, type); \
            pvar != DARRAY_PAT(d, DARRAY_COUNT(d), type); \
            *pvar++)


#endif // DARRAY_H_
