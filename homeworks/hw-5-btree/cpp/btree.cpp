// 
// btree.cpp
//
// Your Name:
// 
// Collaboration is not only encouraged, it is basically
// demanded. Work together! Share code!
//
// Just write down your main collaborators here:
// ...



#include "btree.h"

using namespace std;


// -------------------------------------------- I N S E R T ---------
//
// Insert the given key into a b-tree rooted at 'root'.  If the key is
// already contained in the btree this should do nothing.  
// 
// On exit: 
// -- the 'root' pointer should refer to the root of the
//    tree. (the root may change when we insert or delete)
// -- the btree pointed to by 'root' is valid.
void insert(btree* &root, int key) {
 	if (root->num_keys == 0) { // empty root node
 		root->keys[0] = key;
 		root->num_keys++;
 		return;
 	}
 	btree* node = find_insert(root, key); // find node to insert key
 	btree* parent = NULL;
 	find_parent(root, node, parent); // find parent node

	int order = sizeof(node->keys)/sizeof(node->keys[0]) - 1; // order of root
 	if (node->is_leaf && node->num_keys < order) { // not full leaf node
 		insert_key(node, key);
 	} else if (node->is_leaf && node->num_keys == order && parent == NULL) { // overflow leaf node, root=temp
 		branch(root, key);
 	} else if (node->is_leaf && node->num_keys == order && node != parent && parent->num_keys < order) { // overflow leaf node, parent not full
 		split(parent, node, key);
 	} else if (node->is_leaf && node->num_keys == order && node != parent && parent->num_keys == order) { // overflow leaf node, parent full
 		split2(parent, node, key);
 	}
}


// -------------------------------------------- R E M O V E ---------
//
// Remove the given key from a b-tree rooted at 'root'. If the key is
// not in the btree this should do nothing.
//
// On exit:
// -- the 'root' pointer should refer to the root of the
//    tree. (the root may change when we insert or delete)
// -- the btree pointed to by 'root' is valid.
void remove(btree* &root, int key) {
	btree* node = find_remove(root, key);
	btree* parent = NULL;
	find_parent(root, node, parent);
	int order = sizeof(node->keys)/sizeof(node->keys[0]) - 1; // order of root
	// base case: remove from leaf

}

// ----------------------------------------- C O N T A I N S --------
//
// Return true if any node accessed from the given root node contains
// the provided key value.
bool contains(btree* &root, int key) {
	if (root == NULL) {
  		return false;
  	}
  	if (contains_key(root, key)) {
  		return true;
  	} else {
  		int idx = find_subtree(root, key);
  		if (idx != -1) {
  			contains(root->children[idx], key); // recurse
  		}
 	}
}

// ----------------------------------------- Helper Functions -------------


// Check if root contains key
bool contains_key(btree* &root, int key) {
	for (int i = 0; i < root->num_keys; i++) {
  		if (root->keys[i] == key) { // found key
  			return true;
  		}
  	}
  	return false;
}

// Find subtree of root to search for value
int find_subtree(btree* &root, int key) {
	if (root->is_leaf) {
		return -1;
	}
	int idx = 0;
	for (int i = 0; i < root->num_keys; i++) {
		if (key > root->keys[i]) {
			idx++;
		} else {
			break;
		}
	}
	return idx;
}

// Find node to insert value
btree* find_insert(btree* &root, int key) {
	btree* temp = root;
	if (root->is_leaf) {
		return root;
	} else {
		int idx = 0;
		for (int i = 0; i < root->num_keys; i++) {
			if (key > root->keys[i]) { // keep looking in keys
				idx++;
			} else {
				temp = find_insert(root->children[idx], key);
			}
		}
	}
	return temp;
}

void find_parent(btree* root, btree* node, btree* &parent) {
	if (root->is_leaf) {
		return;
	} else {
		for (int i = 0; i < root->num_keys+1; i++) {
			if (node == root->children[i]) { // keep looking in keys
				parent = root;
			} else {
				find_parent(root->children[i], node, parent);
			}
		}
	}
}

// Insert key value into array of keys
void insert_key(btree* &root, int key) {
	int i = 0;
 	while (key > root->keys[i] && i < root->num_keys) {
 		i++;
 	}
 	root->num_keys++;
 	int temp;
 	for (int j = root->num_keys-1; j > i; j--) {
 		temp = root->keys[j-1];
 		root->keys[j] = temp;
 	}
 	root->keys[i] = key;
}

void branch(btree* &root, int key) {
	insert_key(root, key); // insert key into key array
	int median = root->keys[2];// find median
	btree* left = new btree; // left child
	btree* right = new btree; // left child
	left->num_keys = 2;
	right->num_keys = 2;
	left->keys[0] = root->keys[0];// place first 2 keys in left
	left->keys[1] = root->keys[1];
	right->keys[0] = root->keys[3];// place last 2 keys in right
	right->keys[1] = root->keys[4];
	root->keys[0] = median;// place median in first slot of root
	for (int i = 1; i < root->num_keys; i++) {
		root->keys[i] = 0;
	}
	for (int i = 0; i < root->num_keys+1; i++) {
		root->children[i] = NULL;
	}
	root->num_keys = 1;
	left->is_leaf = true;
	right->is_leaf = true;
	root->is_leaf = false;
	root->children[0] = left;
	root->children[1] = right;
	return;
}

void split(btree* &parent, btree* &root, int key) {
	int order = sizeof(root->keys)/sizeof(root->keys[0]) - 1; // order of root
	insert_key(root, key); // insert key into key array
	int median = root->keys[(order+1)/2];// find median
	// set up new node
	btree* new_node = new btree;
	new_node->num_keys = (order+1)/2;
	new_node->is_leaf = true;
	for (int j = 0; j < (order+1)/2 ; j++) {
		new_node->keys[j] = root->keys[(order+1)/2 + 1 + j];
	}
	if (parent->num_keys < order) { // parent not full
		insert_key(parent, median); // insert median value into parent 

		int i = 0;
 		while (key != parent->keys[i] && i < root->num_keys) {
 			i++;
 		}

 		btree* temp2;
 		for (int k = parent->num_keys+1; k > i; k--) {
 			temp2 = parent->children[k-1];
 			parent->children[k] = temp2;
 		}
 		parent->children[i+1] = new_node;

		root->num_keys = (order+1)/2; // set up root
	}
	for (int i = (order+1)/2; i < root->num_keys; i++) {
		root->keys[i] = 0; // delete extra values from root
	}
}

void split2(btree* &parent, btree* &root, int key) {
	int order = sizeof(root->keys)/sizeof(root->keys[0]) - 1; // order of root
	insert_key(root, key); // insert key into key array
	int median = root->keys[(order+1)/2];// find median
	// set up new node
	btree* new_node = new btree;
	new_node->num_keys = (order+1)/2;
	new_node->is_leaf = true;
	for (int j = 0; j < (order+1)/2 ; j++) {
		new_node->keys[j] = root->keys[(order+1)/2 + 1 + j];
	}
	btree* parent_orig = copy_node(parent);
	branch(parent, median);// branch parent
	// fix original node
	for (int i = (order+1)/2; i < root->num_keys; i++) {
		root->keys[i] = 0; // delete extra values from root
	}
	root->num_keys = (order+1)/2;

	// add new_node to parent orig children
	int i = 0;
 	while (new_node->keys[0] > parent_orig->children[i]->keys[0]) {
 		i++;
 	}
 	btree* temp;
 	for (int j = order+2; j > i; j--) {
 		temp = parent_orig->children[j-1];
 		parent_orig->children[j] = temp;
 	}
 	parent_orig->children[i] = new_node;

	for (int i = 0; i < (order+1)/2 + 1; i++) {
		parent->children[0]->children[i] = parent_orig->children[i];
		parent->children[1]->children[i] = parent_orig->children[(order+1)/2 + i + 1];
	}
	parent->children[0]->is_leaf = false;
	parent->children[1]->is_leaf = false;
}

btree* copy_node(btree* node) {
	btree* new_node = new btree;
	new_node->num_keys = node->num_keys;
	new_node->is_leaf = node->is_leaf;
	int i = 0;
	for (i = 0; i < node->num_keys; i++) {
		new_node->keys[i] = node->keys[i];
		new_node->children[i] = node->children[i];
	}
	new_node->children[i] = node->children[i];
	return new_node;
}

// Find node to remove value
btree* find_remove(btree* &root, int key) {
	btree* temp = root;
	if (root == NULL) {
  		return NULL;
  	}
  	if (contains_key(root, key)) {
  		return root;
  	} else {
  		int idx = find_subtree(root, key);
  		if (idx != -1) {
  			temp = find_remove(root->children[idx], key); // recurse
  		}
 	}
 	return temp;
}

void combine() {

}