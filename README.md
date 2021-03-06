# c-data-structures
Implementations of the following data structures in C:
  - Dynamic array
  - Singly-linked list
  - queue with array or singly-linked list backing
  - Singly-linked key/value list for use in hash table
  - Hash table with string keys and integer values, implemented with chaining, djb2 hash

Other:
  - Heap sort, quick sort, and merge sort

Each module has a test file associated with it (test_\<module name\>.c). Tests were written using the Unity unit testing framework (via ceedling). Ceedling will need to be installed according to the instructions [here](https://github.com/ThrowTheSwitch/Ceedling) in order to run the tests.

These were written for the purpose of practice, and are not intended to be used in an actual project (especially not an embedded project due to the usage of malloc).

## Module interfaces:
  - Dynamic array (vector_t):
    - vector_new - creates new array
    - vector_destroy - frees array
    - vector_capacity - returns the current capacity of the array
    - vector_size - returns the number of elements in the array
    - vector_is_empty - returns true if array is empty
    - vector_at - returns list element at a given index
    - vector_push - adds item to end of vector
    - vector_insert - inserts item at selected index
    - vector_prepend - inserts item at beginning of vector
    - vector_pop - removes item from end of vector, returns item
    - vector_delete - removes item at index (and returns item), shifts all other items left
    - vector_remove - removes every item matching the specified value
    - vector_find - looks for value and returns index (-1 if not found)
    
  - Singly-linked list (forward_list_t):
    - forward_list_new - creates new list
    - forward_list_destroy - frees list
    - forward_list_is_empty - returns true if list is empty
    - forward_list_size - returns number of elements in list
    - forward_list_at - returns value of item at index
    - forward_list_push_back - adds item to end of list
    - forward_list_at_from_end - returns value of item at index (starting from the end of the list)
    - forward_list_pop_front - removes item from front of list and returns value
    - forward_list_pop_back - removes item from back of list and returns value
    - forward_list_front - returns item at front of list (peek)
    - forward_list_back - returns item at back of list (peek)
    - forward_list_insert - inserts item at given index
    - forward_list_delete - deletes item at given index and returns value
    - forward_list_reverse - reverses the list
    - forward_list_remove - removes all items matching the specified value
    
  - Queue (queue_t)
    - queue_new_as_array - creates new queue with array backing with given capacity
    - queue_new_as_list - creates new queue with singly-linked list backing
    - queue_destroy - frees queue
    - queue_enqueue - adds item to back of queue
    - queue_dequeue - removes item from front of queue, returns value
    - queue_is_empty - returns true if queue is empty
    - queue_is_full - returns true if queue is full (only happens with array backing)
    
  - Singly-linked key/value list -- interface specifically designed for use in hash table (key_val_list_t):
    - key_val_list_new - creates new list
    - key_val_list_destroy - frees list
    - key_val_list_is_empty - returns true if list is empty
    - key_val_list_push_back - adds item to end of list
    - key_val_list_pop_front - removes item from front of list and returns value
    - key_val_list_try_get - attemps to get value associated with given key
    - key_val_list_try_update - attempts to update a value associated with given key
    - key_val_list_remove - removes item from list with given key
    
  - Hash table (hash_table_t)
    - hash_table_new - creates new hash table
    - hash_table_destroy - frees hash table
    - hash_table_add - adds key/value pair to the table (updates if existing)
    - hash_table_exists - checks if key/value pair exists
    - hash_table_try_get - attempts to get value associated with given key
    - hash_table_remove - removes key/value pair from hash table

  - Binary search tree (bst_t)
    - bst_new - creates new binary search tree
    - bst_destroy - frees tree
    - bst_insert - inserts value into tree
    - bst_get_count - returns the number of items in the tree (includes duplicates)
    - bst_get_node_count - returns the number of nodes in the tree (this will implicitly exclude duplicates)
    - bst_is_in_tree - searches for value, returns true if value is in the tree
    - bst_get_height - returns height of the tree
    - bst_get_min - returns the minimum value in the tree
    - bst_get_max - returns the maximum value in the tree
    - bst_flatten - returns an array containing the values in the tree in ascending order
    - bst_remove - removes one instance of the given value from the tree (if it exists)

  - Max heap (max_heap_t)
    - max_heap_new - creates new max heap with a given capacity
    - max_heap_destroy - frees heap
    - max_heap_is_empty - returns true if heap is empty
    - max_heap_try_insert - adds value to heap and returns true if heap has capacity
    - max_heap_get_max - returns the max value (without removing it from the heap)
    - max_heap_extract_max - returns the max value, removes it from the heap
    - max_heap_sort - uses heap sort to sort the input array in place

  - Sort
    - heap_sort - wrapper for max_heap_sort function from max heap module
    - quick_sort - implementation using Hoare partition
    - merge_sort
