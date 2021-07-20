#include "max_heap.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit()

// Private function prototypes
static void heapify(int * arr, int len);
static void sift_up(int * arr, int index);
static void sift_down(int * arr, int index, int len);
static int get_parent_index(int index);
static int get_left_child_index(int index);
static int get_right_child_index(int index);

// Public functions
max_heap_t * max_heap_new(int capacity)
{
  if (capacity == 0)
  {
    error_print_and_quit("max_heap_new capacity must be greater than 0.");
  }

  max_heap_t * heap = (max_heap_t *)malloc(sizeof(max_heap_t));
  heap->_data = (int *)(malloc(sizeof(int) * capacity));
  heap->_capacity = capacity;
  heap->_size = 0;

  return heap;
}

void max_heap_destroy(max_heap_t * heap)
{
  free(heap->_data);
  free(heap);
}

bool max_heap_is_empty(max_heap_t * heap)
{
  return (heap->_size == 0);
}

bool max_heap_try_insert(max_heap_t * heap, int value)
{
  if (heap->_size == heap->_capacity)
  {
    return false;
  }

  heap->_data[heap->_size] = value;
  sift_up(heap->_data, heap->_size);
  heap->_size++;

  return true;
}

int max_heap_get_max(max_heap_t * heap)
{
  if (heap->_size == 0)
  {
    error_print_and_quit("max_heap_get_max heap must have non-zero size.");
  }
  return heap->_data[0];
}

int max_heap_extract_max(max_heap_t * heap)
{
  if (heap->_size == 0)
  {
    error_print_and_quit("max_heap_extract_max heap must have non-zero size.");
  }
  int return_val = heap->_data[0];
  heap->_data[0] = heap->_data[heap->_size - 1];
  heap->_size--;
  sift_down(heap->_data, 0, heap->_size);

  return return_val;
}

void max_heap_sort(int * arr, int len)
{
  if (len <= 1)
  {
    return;
  }
  heapify(arr, len);
  for (int i = (len - 1); i > 0; i--)
  {
    // Swap 
    int max = arr[0];
    arr[0] = arr[i];
    arr[i] = max;

    sift_down(arr, 0, i);
  }
}

// Private function definitions
static void heapify(int * arr, int len)
{
  for (int i = (len / 2); i >= 0;  i--)
  {
    sift_down(arr, i, len);
  }
}

static void sift_up(int * arr, int index)
{
  int parent_index = get_parent_index(index);
  while ((index != 0) && (arr[index] > arr[parent_index]))
  {
    int parent_val = arr[parent_index];
    arr[parent_index] = arr[index];
    arr[index] = parent_val;

    index = parent_index;
    parent_index = get_parent_index(index);
  }
}

static void sift_down(int * arr, int index, int len)
{
  int index_of_max = index;
  int left_child_index = get_left_child_index(index);
  if ((left_child_index < len) && 
      (arr[index_of_max] < arr[left_child_index]))
  {
    index_of_max = left_child_index;
  }
  int right_child_index = get_right_child_index(index);
  if ((right_child_index < len) && 
      (arr[index_of_max] < arr[right_child_index]))
  {
    index_of_max = right_child_index;
  }

  if (index_of_max != index)
  {
    int max = arr[index_of_max];
    arr[index_of_max] = arr[index];
    arr[index] = max;

    sift_down(arr, index_of_max, len);
  }
}

static int get_parent_index(int index)
{
  return (index - 1) / 2;
}

static int get_left_child_index(int index)
{
  return (2 * index) + 1;
}

static int get_right_child_index(int index)
{
  return (2 * index) + 2;
}