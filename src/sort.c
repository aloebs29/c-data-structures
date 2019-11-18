#include "sort.h"
#include <stdlib.h> // malloc(), free(), rand()
#include <limits.h>
#include "max_heap.h"

// Private function prototypes
static int sort_by_pivot(int * arr, int len, int pivot); // returns number of elements less than pivot
static void merge(int * src, int * dest, int start_index, int offset1, 
    int offset2);
static int min(int a, int b);

// Public functions
void quick_sort(int * arr, int len)
{
  if (len <= 1)
  {
    return;
  }
  int pivot = arr[rand() % len];
  int num_below_pivot = sort_by_pivot(arr, len, pivot);

  // Recursively call on the two unsorted sub-arrays
  quick_sort(arr, num_below_pivot);
  quick_sort(&arr[num_below_pivot], (len - num_below_pivot));
}

void merge_sort(int * arr, int len)
{
  if (len <= 1)
  {
    return;
  }

  // Create shadow array to swap between
  int * temp = (int *)malloc(sizeof(int) * len);
  bool into_temp = false;
  for (int merge_size = 1; merge_size < len; merge_size *= 2)
  {
    into_temp = !into_temp;
    for (int merge_start = 0; 
        merge_start < len; 
        merge_start += 2 * merge_size)
    {
      if (into_temp)
      {
        merge(arr, temp, merge_start, merge_size, 
            min(2 * merge_size, len - merge_start));
      }
      else
      {
        merge(temp, arr, merge_start, merge_size, 
            min(2 * merge_size, len - merge_start));
      }
    }
  }

  // If our last merge was into temp, swap back to source
  if (into_temp)
  {
    for (int i = 0; i < len; i++)
    {
      arr[i] = temp[i];
    }
  }
  free(temp);
}

void heap_sort(int * arr, int len)
{
  max_heap_sort(arr, len);
}

// Private function definitions
static int sort_by_pivot(int * arr, int len, int pivot)
{
  int left_index = -1;
  int right_index = len;

  while (true)
  {
    do
    {
      left_index++;
    } while (arr[left_index] < pivot);
    do
    {
      right_index--;
    } while (arr[right_index] > pivot);

    if (left_index >= right_index)
    {
      return left_index;
    }

    int temp = arr[left_index];
    arr[left_index] = arr[right_index];
    arr[right_index] = temp;
  }
}

static void merge(int * src, int * dest, int start_index, int offset1, 
    int offset2)
{
  int dest_index = start_index;
  int index1 = start_index;
  int index2 = start_index + offset1;
  int end_index1 = index2;
  int end_index2 = start_index + offset2;
  // Until one of the to-be-merged sections is empty
  while ((index1 < end_index1) && (index2 < end_index2))
  {
    if (src[index1] < src[index2])
    {
      dest[dest_index] = src[index1];
      index1++; 
    }
    else
    {
      dest[dest_index] = src[index2];
      index2++;
    }
    dest_index++;
  }
  // Empty whichever section remains
  while (index1 < end_index1)
  {
    dest[dest_index] = src[index1];
    index1++;
    dest_index++;
  }
  while (index2 < end_index2)
  {
    dest[dest_index] = src[index2];
    index2++;
    dest_index++;
  }
}

static int min(int a, int b)
{
  return a < b ? a : b;
}