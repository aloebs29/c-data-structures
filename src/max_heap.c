#include "max_heap.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit()

// Private function prototypes
static void sift_up(max_heap_t * heap, int index);
static void sift_down(max_heap_t * heap, int index);
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

  max_heap_t * heap = (max_heap_t *)(malloc(sizeof(max_heap_t)));
  heap->_data = (int *)(sizeof(int) * capacity);
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
  sift_up(heap, heap->_size);
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
  sift_down(heap, 0);

  return return_val;
}

max_heap_t * max_heapify(int * arr, int len);
int * max_heap_sort(int * arr, int len);

// Private function definitions
static void sift_up(max_heap_t * heap, int index)
{
  int parent_index = get_parent_index(index);
  while ((index != 0) && (heap->_data[index] > heap->_data[parent_index]))
  {
    int parent_val = heap->_data[parent_index];
    heap->_data[parent_index] = heap->_data[index];
    heap->_data[index] = parent_val;

    index = parent_index;
    parent_index = get_parent_index(index);
  }
}

static void sift_down(max_heap_t * heap, int index)
{
  int index_of_max = index;
  int left_child_index = get_left_child_index(index);
  if ((left_child_index < heap->_size) && 
      (heap->_data[index_of_max] < heap->_data[left_child_index]))
  {
    index_of_max = left_child_index;
  }
  int right_child_index = get_right_child_index(index);
  if ((right_child_index < heap->_size) && 
      (heap->_data[index_of_max] < heap->_data[right_child_index]))
  {
    index_of_max = right_child_index;
  }

  if (index_of_max != index)
  {
    int max = heap->_data[index_of_max];
    heap->_data[index_of_max] = heap->_data[index];
    heap->_data[index] = max;

    sift_down(heap, index_of_max);
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