//
// binary_search_tree.cpp
//
// Please refer to binary_search_tree.h for documentation.
//

#include "binary_search_tree.h"

/**
 * Initialize a bt_node with the given data, and two NULL children.
 */
bt_node* init_node(int data) {
  bt_node* new_node = new bt_node;
  new_node->data = data;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

/**
 * Insert the new_node in a proper location in the tree indicated by
 * *top_ref. On return, *top_ref points to the root of the tree.
 */
void insert(bt_node** top_ref, bt_node* new_node) {
  if (*top_ref == NULL) {
    *top_ref = new_node;
    return;
  } else {
    bt_node* temp = *top_ref;
    if (new_node->data < temp->data) {
      insert(&temp->left, new_node);
    } else {
      insert(&temp->right, new_node);
    }
  }
  return;
}

/**
 * Insert a new node with the given data value similarly to insert().
 */
void insert_data(bt_node** top_ref, int data) {
  bt_node* new_node = new bt_node;
  new_node->data = data;
  new_node->left = NULL;
  new_node->right = NULL;
  insert(top_ref, new_node);
}

/**
 * Removes a node from the tree whose data value is the same as the
 * input. On return, *top_ref points to the root of the tree. If no
 * node in the tree contains the given data, this function has no
 * effect.
 */
void remove(bt_node** top_ref, int data) {
  if (*top_ref == NULL) { // BT is empty
    return;
  } else if (data == (*top_ref)->data) { // node to remove
    if ((*top_ref)->left == NULL) { // left child empty
      if ((*top_ref)->right == NULL) { // left and right child empty
        *top_ref = NULL;
        return;
      } else { // only left child empty
        (*top_ref) = (*top_ref)->right;
        return;
      }
    } else if ((*top_ref)->right == NULL) { // right child empty
      if ((*top_ref)->left == NULL) { // right and left child empty
        *top_ref = NULL;
        return;
      } else { // only right child empty
        (*top_ref) = (*top_ref)->left;
        return;
      }
    } else { // neither child empty
      bt_node* temp = (*top_ref)->left;
      while (temp->right != NULL) {
        temp = temp->right;
      }
      (*top_ref)->data = temp->data;
      remove(&(*top_ref)->left, temp->data);
      delete temp;
      return;
    }
  } else { // not node to remove
      if (data < (*top_ref)->data) {
        remove(&(*top_ref)->left, data);
      } else {
        remove(&(*top_ref)->right, data);
      }
  }
  return;
}



/**
 * Return true if any node in the tree pointed to by top contains the
 * given data value.
 */
bool contains(bt_node* top, int data) {
  if (top == NULL) {
    return false;
  } else {
    if (data == top->data) {
      return true;
    } else if (data < top->data) {
      get_node(top->left, data);
    } else {
      get_node(top->right, data);
    }
  }
}

/**
 * Searches through the tree pointed to by top for a node that
 * contains the given data value. If such a node is found, a pointer
 * to it is returned. Otherwise this function returns NULL.
 */
bt_node* get_node(bt_node* top, int data) {
  if (top == NULL) {
    return NULL;
  } else {
    if (data == top->data) {
      return top;
    } else if (data < top->data) {
      get_node(top->left, data);
    } else {
      get_node(top->right, data);
    }
  }
}

/**
 * Return the size of the tree pointed to by top. In other words, it
 * returns the number of nodes that are reachable by traversing the
 * tree pointed to by top. If the tree is empty (top is NULL), it
 * returns zero.
 */
int size(bt_node* top) {
  if (top == NULL) {
    return 0;
  } else {
    return 1 + size(top->left) + size(top->right);
  }
}

/**
 * Fill an integer array to reflect the contents of this binary
 * tree. The filled array will be the same as the tree's size (found
 * with the size() function), and the order of the array elements are
 * the same that is found during an inorder traversal of the tree.
 *
 * Note: the array is created and sized before this function is
 * called. You will have at least size(top) elements available, so you
 * may safely index [0..size(top)-1].
 */
void to_array(bt_node* top, int arr[]) {
  if (top == NULL) {
    return;
  } else {
    to_array(top->left, arr);
    int i = 0;
    while (arr[i] != -1) {
      i++;
    }
    arr[i] = top->data;
    to_array(top->right, arr);
  }
  return;
}
