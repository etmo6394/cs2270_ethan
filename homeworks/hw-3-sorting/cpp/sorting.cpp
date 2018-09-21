/*

  sorting.cpp

  Please refer to sorting.h for documentation on each function.

  Your Name:
  Your TA:
  List O' Collaborators:


   + ------ N O T E: -- Big scary warning! ------------------------------
   |
   |    We clearly can't use unit testing to ensure that you're
   |    actually implementing the correct algorithm. I suppose we could
   |    write some complicated static analysis checker. But it is
   |    easier to just look at it with our eyeballs.
   |
   |    After the assignment is due, we will look at the most recent
   |    version of your implementation that got the highest score and
   |    make sure you're not doing something sneaky like using a
   |    standard library call to sort a vector.
   |
   |    Anyone caught doing this (even though it is kinda funny) will
   |    be given 0/15 on the assignment.
   |
   +-------------------------------------------------------------------- 

 */

#include "sorting.h"

using namespace std;

/**
 * This is the main quicksort function. It should use the
 * quicksort_partition function. It is also recursive, so you will
 * call quicksort from inside the quicksort function.
 * 
 * data: The input is a vector of integers. You can use this in a very
 * similar manner to arrays, but without a lot of the pointer
 * brain-damage. Note that the ampersand in front of the parameter
 * name indicates this is passed by _reference_, which means whatever
 * you do to this vector inside the quicksort function will be visible
 * outside as well.
 *
 * low_idx and high_idx: these are the current indices for the
 * quicksort algorithm, and are normal pass-by-value parameters. Both
 * of these indices are inclusive: they refer to an addressable
 * element of the data vector.
 **/
void quicksort(vector<int> &data, int low_idx, int high_idx) {
  if (high_idx - low_idx < 1) {
    return;
  }
  int pivot_index = low_idx + (high_idx - low_idx)/2; // choose pivot index
  //int pivot_index = high_idx - 1;
  pivot_index = quicksort_partition(data, low_idx, high_idx, pivot_index); // sort into </>= pivot
  quicksort(data, low_idx, pivot_index - 1); // recursively sort [low, pivot)
  quicksort(data, pivot_index + 1, high_idx); // recursively sort (pivot, high]
}

/**
 * This is the partition function for quicksort. Input parameters have
 * same semantics as for quicksort. An additional parameter is given
 * for the pivot location. 
 *
 * The modified pivot index is returned.
 **/
int quicksort_partition(vector<int> &data, int low_idx, int high_idx, int pivot_index) {
  // swap values at pivot and high
  int temp = data[high_idx];
  data[high_idx] = data[pivot_index];
  data[pivot_index] = temp;
  // set pivot to low
  pivot_index = low_idx;
  // sort elements
  for (int i = low_idx; i < high_idx; i++) {
    if (data[i] < data[high_idx]) {
      temp = data[pivot_index];
      data[pivot_index] = data[i];
      data[i] = temp;
      pivot_index++;
    }
  }
  // swap values at pivot and high
  temp = data[pivot_index];
  data[pivot_index] = data[high_idx];
  data[high_idx] = temp;
  return pivot_index;
}

/**
 * The bubblesort is probably the easiest sorting algorithm to
 * conceive of. It is unfortunately not all that fast. Here's your
 * chance to write really inefficient code! There are no related
 * helper functions.
 **/
void bubblesort(vector<int> &data) {
  bool swapped = 0;
  int size = data.size();
  for (int i = 0; i < size; i++) {
    if (data[i+1] < data[i]) {
      int temp = data[i];
      data[i] = data[i+1];
      data[i+1] = temp;
      swapped = 1;
    }
  }
  if (swapped == 1) {
    bubblesort(data);
  }
  return;
}

/**
 * The mergesort is another classic O(n log n) sorting algorithm. This
 * uses the helper function 'merge'.
 **/
void mergesort(vector<int> &data) {
  if (data.size() <= 1) {
    return;
  }
  int middle = data.size()/2; // find middle index
  vector<int> left;
  vector<int> right;
  int i;
  for (i = 0; i < middle; i++) {
    left.push_back(data[i]);
  }
  for (i = middle; i < data.size(); i++) {
    right.push_back(data[i]);
  }
  mergesort(left);
  mergesort(right);
  // for (i = 0; i < left.size();i++) {
  //   cout << left[i] << " ";
  // }
  // cout << endl;
  data = merge(left, right);
}

/**
 * merge is the helper function for mergesort. It returns a vector
 * containing the merged contents of the two input vectors.
 **/ 
vector<int> merge(vector<int> &left, vector<int> &right) {
  vector<int> data;
  int i = 0;
  int j = 0;
  int size_left = left.size();
  int size_right = right.size();
  while (data.size() < size_left + size_right) {
    if (i == size_left) {
      data.push_back(right[j]);
      j++;
    } else if (j == size_right) {
      data.push_back(left[i]);
      i++;
    } else if (left[i] < right[j]) {
      data.push_back(left[i]);
      i++;
    } else {
      data.push_back(right[j]);
      j++;
    }
  }
  return data;
}

/**
 * You may implement any additional sorting function you wish, and
 * call it 'mystery_sort'.
 * 
 * See http://en.wikipedia.org/wiki/Sorting_algorithm for a list.
 *
 * Only rules are: (1) it has to have the following signature, and the
 * calling code will expect 'data' to be in non-decreasing order when
 * it returns, and (2) you have to write something new---no relying on
 * the other functions you've implemented.
 **/
void mystery_sort(vector<int> &data) { // Heap Sort
  vector<int> heap;
  heap.push_back(data[0]);
  for (int i = 1; i < data.size(); i++) {
    insert_heap(heap, data[i]);
  }
  vector<int> sorted;
  while (sorted.size() < data.size()) {
    int temp = heap[0];
    heap[0] = heap[heap.size()-1];
    heap[heap.size()-1] = temp;
    sorted.push_back(heap[heap.size()-1]);
    // cout << heap[heap.back()] << " " << heap[i] << endl;
    heap.pop_back();
    repair_heap(heap, 0);
  }
  for (int i = 0; i < data.size(); i++) {
    data[i] = sorted[data.size()-i-1];
  }
}

/**
 * Insert the new_node in a proper location in the tree indicated by
 * *top_ref. On return, *top_ref points to the root of the tree.
 */
void insert_heap(vector<int> &heap, int new_val) {
  heap.push_back(new_val);
  if (heap.empty()) {
    return;
  }
  int i = heap.size() - 1;
  while (heap[i] > heap[(i-1)/2] && i > 0) {
    int temp = heap[i];
    heap[i] = heap[(i-1)/2];
    heap[(i-1)/2] = temp;
    i = (i-1)/2;
  }
}

void repair_heap(vector<int> &heap, int i) {
  if (heap.empty()) {
    return;
  }
  int largest = i;
  int l = 2*i + 1;
  int r = 2*i + 2;

  if (l < heap.size() && heap[l] > heap[largest]) {
    largest = l;
  }
  if (r < heap.size() && heap[r] > heap[largest]) {
    largest = r;
  }
  if (largest != i) {
    int temp = heap[i];
    heap[i] = heap[largest];
    heap[largest] = temp;
    repair_heap(heap, largest);
  }

}