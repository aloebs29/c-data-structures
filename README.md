# c-data-structures
Implementations of the following data structures in C:
  - Dynamic array
  - Singly-linked list
  - Qeue with array or singly-linked list backing
  - Singly-linked key/value list for use in hash table
  - Hash table with string keys and integer values, implemented with chaining, djb2 hash

Each module has a test file associated with it (test_\<module name\>.c). Tests were written using the Unity unit testing framework (via ceedling). Ceedling will need to be installed according to the instructions [here](https://github.com/ThrowTheSwitch/Ceedling) in order to run the tests.

## Module interfaces:
  - Dynamic array (dvector_t):
    - dvector_new - creates new array
    - dvector_destroy - frees array
    - dvector_capacity - returns the current capacity of the array
    - dvector_size - returns the number of elements in the array
    - dvector_is_empty - returns true if array is empty
    - dvector_at - returns list element at a given index
    - dvector_push - adds item to end of vector
    - dvector_insert - inserts item at selected index
    - dvector_prepend - inserts item at beginning of vector
    - dvector_pop - removes item from end of vector, returns item
    - dvector_delete - removes item at index (and returns item), shifts all other items left
    - dvector_remove - removes every item matching the specified value
    - dvector_find - looks for value and returns index (-1 if not found)
    
  - Singly-linked list (dforward_list_t):
    - dforward_list_new - creates new list
    - dforward_list_destroy - frees list
    - dforward_list_is_empty - returns true if list is empty
    - dforward_list_size - returns number of elements in list
    - dforward_list_at - returns value of item at index
    - dforward_list_push_back - adds item to end of list
    - dforward_list_at_from_end - returns value of item at index (starting from the end of the list)
    - dforward_list_pop_front - removes item from front of list and returns value
    - dforward_list_pop_back - removes item from back of list and returns value
    - dforward_list_front - returns item at front of list (peek)
    - dforward_list_back - returns item at back of list (peek)
    - dforward_list_insert - inserts item at given index
    - dforward_list_delete - deletes item at given index and returns value
    - dforward_list_reverse - reverses the list
    - dforward_list_remove - removes all items matching the specified value
    
  - Qeue (dqeue_t - yes, I know that my 'd' prefix is unfortunate here)
    - dqeue_new_as_array - creates new qeue with array backing with given capacity
    - dqeue_new_as_list - creates new qeue with singly-linked list backing
    - dqeue_destroy - frees qeue
    - dqeue_enqeue - adds item to back of qeue
    - dqeue_deqeue - removes item from front of qeue, returns value
    - dqeue_is_empty - returns true if qeue is empty
    - dqeue_is_full - returns true if qeue is full (only happens with array backing)
    
  - Singly-linked key/value list -- interface specifically designed for use in hash table (dkey_val_list_t):
    - dkey_val_list_new - creates new list
    - dkey_val_list_destroy - frees list
    - dkey_val_list_is_empty - returns true if list is empty
    - dkey_val_list_push_back - adds item to end of list
    - dkey_val_list_pop_front - removes item from front of list and returns value
    - dkey_val_list_try_get - attemps to get value associated with given key
    - dkey_val_list_try_update - attempts to update a value associated with given key
    - dkey_val_list_remove - removes item from list with given key
    
  - Hash table (dhash_table_t)
    - dhash_table_new - creates new hash table
    - dhash_table_destroy - frees hash table
    - dhash_table_add - adds key/value pair to the table (updates if existing)
    - dhash_table_exists - checks if key/value pair exists
    - dhash_table_try_get - attempts to get value associated with given key
    - dhash_table_remove - removes key/value pair from hash table
