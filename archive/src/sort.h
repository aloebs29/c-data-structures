#ifndef _SORT_H
#define _SORT_H

// Sort the input arrays in ascending order
void quick_sort(int * arr, int len); // unstable
void merge_sort(int * arr, int len); // stable
// Just a wrapper for the sort function in the max_heap module
void heap_sort(int * arr, int len); // stable
void radix_sort(int * arr, int len); // stable

#endif // _SORT_H
