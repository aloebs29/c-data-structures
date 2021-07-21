/**
 * @file		darray.h
 * @author		Andrew Loebs
 * @brief		Dynamically sized array.
 *
 */

#ifndef DARRAY_H_
#define DARRAY_H_

/**
 * @brief Dynamic array.
 * @note Should not be created/deleted/modified directly; use functions provided in this module
 *       to interact with this data structure. 
 */
struct darray {
    void *(*_allocator)(unsigned long);
    void (*_deallocator)(void *);
    void *_data;
    unsigned int _item_size;
    unsigned long _size;
    unsigned long _capacity;
};

/**
 * @brief Create a new dynamic array.
 *
 * @param item_size Size (in bytes) of the items to be stored in the array.
 * @param allocator Memory allocator to be used by the array; use NULL for stdlib malloc().
 * @param deallocator Deallocator to use for freeing memory; use NULL for stdlib free().
 *
 * @note Only supports storage of homogenous items.
 *
 * @return Pointer to the newly created dynamic array.
 */
struct darray *darray_new(unsigned long item_size, void *(*allocator)(unsigned long),
        void (*deallocator)(void *));

/**
 * @brief Deallocate a dynamic array.
 *
 * @param d Dynamic array to be destroyed.
 */
void darray_destroy(struct darray *d);

/**
 * @brief Extend the dynamic array by adding new elements to the end of the array.
 *
 * @param d Dynamic array to be extended.
 * @param data Pointer to the data to copy into the dynamic array.
 * @param count Number of items pointed to by data. Note: this is not size in bytes, but number of
 *              items (per the item_size passed in on creation).
 * 
 * @return 0 on success; negative value on failure.
 */
int darray_extend(struct darray *d, void *data, unsigned long count);

/**
 * @brief Appends one item to the end of the array.
 *
 * @param d Dynamic array to be appended.
 * @param data Pointer to the data to copy into the dynamic array.
 *
 * @return 0 on success; negative value on failure.
 */
int darray_append(struct darray *d, void *data);

/**
 * @brief Inserts one item into the array.
 *
 * @param d Dynamic array to insert into.
 * @param data Pointer to the data to copy into the dynamic array.
 * @param index Index of the array to insert the new data into. Must be less than or equal to the
 *              current number of items in the array (see DARRAY_COUNT).
 *
 * @note Prefer extending/appending to the array when possible, this function must shift elements
 *       to the right, which incurs a performance penalty.
 * 
 * @return 0 on success; negative value on failure.
 */
int darray_insert(struct darray *d, void *data, unsigned long index);

/**
 * @brief Removes one item from the end of the array.
 *
 * @param d Dynamic array to remove an item from.
 */
void darray_pop(struct darray *d);

/**
 * @brief Removes one item from the array.
 *
 * @param d Dynamic array to remove an item from.
 * @param index Index at which to remove an item. No operation will be performed if the index
 *              exceeds the number of items in the array.
 */
void darray_remove(struct darray *d, unsigned long index);

/**
 * @brief Clears all items from the array.
 *
 * @param d Dynamic array to be cleared.
 */
void darray_clear(struct darray *d);

/**
 * @brief Returns the number of items in the array.
 *
 * @param d Dynamic array to return the count of.
 *
 * @return Number of items in the array.
 */
#define DARRAY_COUNT(d) (DARRAY_I_FROM_OFF((d), (d)->_size))

/**
 * @brief Returns the item at a given index in the array.
 *
 * @param d Dynamic array storing the item to be retrieved.
 * @param index Index of the item to be retrieved (WARNING: no bounds checking).
 * @param type Type of the item to be retrieved.
 *
 * @return The item at the given index.
 */
#define DARRAY_AT(d, index, type) (*DARRAY_PAT((d), (index), type))

/**
 * @brief Returns a pointer to the item at a given index in the array.
 *
 * @param d Dynamic array storing the item to be retrieved.
 * @param index Index of the item to retrieve a pointer of (WARNING: no bounds checking).
 * @param type Type of the item to be pointed to by the return.
 *
 * @return Pointer to the item at the given index.
 */
#define DARRAY_PAT(d, index, type) ((type *)DARRAY_VPAT((d), (index)))

/**
 * @brief Returns a void pointer to the item at a given index in the array.
 *
 * @param d Dynamic array storing the item to be retrieved.
 * @param index Index of the item to retrieve a pointer of (WARNING: no bounds checking).
 *
 * @return Pointer to the item at the given index.
 */
#define DARRAY_VPAT(d, index) ((void *)((char *)(d)->_data + DARRAY_OFF_AT((d), (index))))

/**
 * @brief Converts an index to a byte-offset based on the array's item size.
 *
 * @param d Dynamic array to retrieve a byte-offset of.
 * @param index Index to be converted into a byte-offset.
 *
 * @return Byte-offset of the given index.
 */
#define DARRAY_OFF_AT(d, index) ((index) * (d)->_item_size)

/**
 * @brief Converts a byte-offset to an index based on the array's item size.
 *
 * @param d Dynamic array to retrieve an index of.
 * @param off Byte-offset to be converted to an index.
 *
 * @return Index at the given byte-offset.
 */
#define DARRAY_I_FROM_OFF(d, off) ((off) / (d)->_item_size)

/**
 * @brief Loops over all of the items in a dynamic array.
 *
 * @param d Dynamic array to loop over.
 * @param pvar Name of the loop variable to store a pointer to each item (must not conflict with
 *             any names in the current scope; will be declared by this macro).
 * @param type Type to be pointed to by pvar.
 */
#define DARRAY_PFOREACH(d, pvar, type) \
    for (type *pvar = DARRAY_PAT(d, 0, type); \
            pvar != DARRAY_PAT(d, DARRAY_COUNT(d), type); \
            pvar++)

#endif // DARRAY_H_
