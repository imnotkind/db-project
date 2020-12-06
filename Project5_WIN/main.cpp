#include <stdio.h>
#include "timer.hpp"
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#define INSERT 0
#define READ 1
#define UPDATE 2
#define SCAN 3

/* MY CODE START */
#include <stack>
#include <algorithm>

#define DEBUG false
#define ORDER 300

class Node
{
protected:
	bool is_leaf;
	std::vector<uint64_t> keys;

public:
	bool get_is_leaf() {
		return is_leaf;
	}
	std::vector<uint64_t> get_keys() {
		return keys;
	}

	Node(int order, bool is_leaf_) : is_leaf{ is_leaf_ } {
		keys.reserve(order); //order(branching factor) = max num of children = max num of keys + 1
	}

	//virtual void upsert_key_value(uint64_t key, int8_t value) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual void update_key_value(uint64_t key, int8_t value) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual void insert_key_value(uint64_t key, int8_t value) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual void insert_nonempty_internal(uint64_t key, Node* right_child) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual void insert_empty_internal(uint64_t key, Node* left_child, Node* right_child) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual Node* split(uint64_t* keyToParent, int order) { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual std::vector<Node*> get_children() { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual std::vector<int8_t> get_values() { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1); }
	virtual Node* get_next() { std::cout << "ERROR : UNCALLABLE" << std::endl; exit(1);
	}
};

// internal node
class InternalNode : public Node
{
private:
	std::vector<Node*> children;

public:

	std::vector<Node*> get_children()
	{
		return children;
	}

	InternalNode(int order) : Node{ order, false }
	{
		children.reserve(order);
	}

	void insert_nonempty_internal(uint64_t key, Node* right_child)
	{
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin();

		keys.insert(index, key);
		children.insert(children.begin() + (idx + 1), right_child);
	}

	void insert_empty_internal(uint64_t key, Node* left_child, Node* right_child)
	{
		keys.push_back(key);
		children.push_back(left_child);
		children.push_back(right_child);
	}

	Node* split(uint64_t* key_to_parent, int order)
	{
		uint64_t length = keys.size();

		// create a new right internal node 
		InternalNode* right_node = new InternalNode(order);

		// key to be moved up to the parent is the middle element in the current internal node
		*key_to_parent = keys[length / 2];

		// erase keys and values
		right_node->keys.assign(keys.begin() + (length / 2 + 1), keys.end());
		right_node->children.assign(children.begin() + (length / 2 + 1), children.end()); //one more children than key in right internal node
		keys.erase(keys.begin() + length / 2, keys.end()); //middle key goes to parent without going to left or right internal node
		children.erase(children.begin() + (length / 2 + 1), children.end()); //one more children than key in left internal node

		// return the new right internal node
		return right_node;
	}
};

// leaf node
class LeafNode : public Node
{
private:
	LeafNode* next = nullptr;
	std::vector<int8_t> values;

public:
	std::vector<int8_t> get_values()
	{
		return values;
	}

	Node* get_next()
	{
		return next;
	}

	LeafNode(int order) : Node{ order, true }
	{
		values.reserve(order);
	}

	/* upsert is not needed since it's slower
	void upsert_key_value(uint64_t key, int8_t value)
	{
		// binary search for the key
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin(); // idx is 0 when keys.size() is 0

		// check if updating existing key
		if ((idx != keys.size()) && (key == keys[idx])) //checks when no key (idx is 0), or when new key is bigger than every key (idx is keys.size())
		{
			//update value
			values[idx] = value;
		}

		// if inserting a new key and value
		else
		{
			// insert new key and value
			keys.insert(index, key);
			values.insert(values.begin() + idx, value);
		}
	}
	*/


	void insert_key_value(uint64_t key, int8_t value)
	{
		// binary search for the key
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin(); // idx is 0 when keys.size() is 0

		keys.insert(index, key);
		values.insert(values.begin() + idx, value);
	}

	void update_key_value(uint64_t key, int8_t value)
	{
		// binary search for the key
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin(); // idx is 0 when keys.size() is 0
		values[idx] = value;
	}


	Node* split(uint64_t* key_to_parent, int order)
	{
		// create a new right leaf node
		LeafNode* rightNode = new LeafNode(order);

		// key to be moved up to the parent is the middle element in the current leaf node
		*key_to_parent = keys[keys.size() / 2];

		// erase keys and values
		rightNode->keys.assign(keys.begin() + keys.size() / 2, keys.end()); // middle key is the leftmost key in right leaf node
		rightNode->values.assign(values.begin() + values.size() / 2, values.end());
		keys.erase(keys.begin() + keys.size() / 2, keys.end());
		values.erase(values.begin() + values.size() / 2, values.end());
		
		// link list
		rightNode->next = next;
		next = rightNode;

		// return the new right leaf node
		return rightNode;
	}

	
};

// B+ tree
class BPlusTree
{
private:
	int order;
	Node* root;

	void search_path(Node* node, uint64_t key, std::stack<Node*>& path)
	{
		// push node to stack
		path.push(node);

		// if internal node, recurse
		if (!node->get_is_leaf())
		{
			std::vector<uint64_t> keys = node->get_keys();
			std::vector<Node*> children = node->get_children();
			std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
			auto idx = index - keys.begin();

			// if key is found : lower_bound finds index of exact position of key
			if (idx != keys.size() && key == keys[idx])
			{
				// go to idx+1 child
				search_path(children[idx + 1], key, path);
			}

			// if key is not found : lower_bound finds min index of key bigger than wanted
			else
			{
				// go to idx child
				search_path(children[idx], key, path);
			}
		}
	}
	void destroy(Node* node)
	{
		if (!node->get_is_leaf())
		{
			std::vector<Node*> children = node->get_children();
			for (std::vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				destroy(*index);
			}
		}
		//delete self
		delete (node);
	}

	void debug_node(Node* node)
	{
		if (NULL == node)
		{
			std::cout << std::endl << "Empty tree";
			return;
		}

		if (node->get_is_leaf())
			std::cout << std::endl << "Leaf Node: ";
		else
			std::cout << std::endl << "Internal Node: ";

		// display the keys
		std::vector<uint64_t> keys = node->get_keys();
		for (std::vector<uint64_t>::iterator index = keys.begin(); index != keys.end(); index++)
		{
			std::cout << *index << " ";
		}
		std::cout << std::endl;

		// recurse in internal node
		if (!node->get_is_leaf())
		{
			std::vector<Node*> children = node->get_children();

			// recurse to children
			for (std::vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				debug_node(*index);
			}
		}
	}

public:
	BPlusTree(int order_) : order{ order_ }, root{ NULL } {}

	/* would it be faster not using stack?
	Node* insert_textbook(Node* node, uint64_t key, int8_t value, Node* split_child) {
		if (node->get_is_leaf()) {
			// leaf node has space : even if we add a key, it still does not reach order
			if (node->get_keys().size() + 1 < order) {
				node->upsert_key_value(key, value); //just insert
				split_child = nullptr;
			}
			// leaf node has no space, split
			else {

			}

		}
		else {

		}
	}
	*/


	void update(uint64_t key, int8_t value) {
		std::stack<Node*> path;
		search_path(root, key, path);

		path.top()->update_key_value(key, value); //update value in corresponding leaf node
	}

	void insert(uint64_t key, int8_t value) {
		// first insertion
		if (root == nullptr)
		{
			// root is leaf
			root = new LeafNode(order);
			root->insert_key_value(key, value);
		}

		// not first insertion
		else
		{
			Node* left_node = nullptr;
			Node* right_node = nullptr;
			uint64_t key_to_parent = 0;

			std::stack<Node*> path;
			search_path(root, key, path);

			// insert the key-value pair in the leaf node
			path.top()->insert_key_value(key, value);

			// keys should be at most [ORDER-1]
			// if there are [ORDER] keys in a node, we need to split
			while (path.top()->get_keys().size() == order)
			{				
				// split node and get key to parent (middle key)
				left_node = path.top();
				right_node = left_node->split(&key_to_parent, order);

				path.pop();
				
				// if split node was not root
				if (!path.empty())
				{
					path.top()->insert_nonempty_internal(key_to_parent, right_node);
				}

				// if split node was root
				else
				{
					// create a new root
					InternalNode* new_root = new InternalNode(order);

					new_root->insert_empty_internal(key_to_parent, left_node, right_node);

					root = new_root;
					break;
				}
			}

		}
	}

	int8_t search(uint64_t key)
	{
		int8_t ret = 0;

		std::stack<Node*> path;
		search_path(root, key, path);

		Node* node = path.top(); // leaf node that has key
		std::vector<uint64_t> keys = node->get_keys();
		std::vector<int8_t> values = node->get_values();
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin();


		/* suppose data is consistent, doesn't READ uninserted keys
		// found
		if ((idx != keys.size()) && key == keys[idx])
		{
			ret = values[idx];
		}
		// not found
		else {
			ret = 0;
		}
		*/
		ret = values[idx];
		
		return ret;

	}
	uint64_t scan(uint64_t key, int8_t num)
	{
		uint64_t ret = 0; //sum
		int8_t count = 0;

		std::stack<Node*> path;
		search_path(root, key, path);

		Node* node = path.top();
		std::vector<uint64_t> keys = node->get_keys();
		std::vector<int8_t> values = node->get_values();
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		auto idx = index - keys.begin();


		while (1)
		{
			// check if end of the current leaf node is reached
			if (idx == keys.size())
			{
				// go to the next leaf node
				node = node->get_next();
				if (!node) break; //if list end, break
				keys = node->get_keys();
				values = node->get_values();
				idx = 0;
			}

			// check if key meets condition (not necessary if tree is consistent)
			//if ((key <= keys[idx]))
			{
				ret += values[idx];
				count += 1;

				if (count == num) {
					break;
				}
			}

			idx++;
		}


		return ret;
	}

	~BPlusTree()
	{
		destroy(root);
	}

	void debug_tree()
	{
		std::cout << std::endl << "DEBUG TREE" << std::endl;
		debug_node(root);
	}
};


class InMemoryIndex
{
	BPlusTree tree;

public:
	InMemoryIndex() : tree(ORDER){ // b+ tree order(branching factor) : max num of keys = order - 1
	}

	void insert(uint64_t key, int8_t value)
	{
		if(DEBUG) std::cout << std::endl << "INSERT " << key << "," << (int)value << std::endl;
		tree.insert(key, value);
		if (DEBUG) tree.debug_tree();
	}

	void update(uint64_t key, int8_t value)
	{
		if (DEBUG) std::cout << "UPDATE " << key << "," << (int)value << std::endl << std::endl;
		tree.update(key, value);
	}

	int8_t read(uint64_t key)
	{
		if (DEBUG) std::cout << std::endl << "READ " << key << std::endl;
		return tree.search(key);
	}

	uint64_t scan(uint64_t key, int8_t num)
	{
		if (DEBUG) std::cout << "SCAN " << key << "," << (int)num << std::endl << std::endl;
		return tree.scan(key, num);
	}
};


/* MY CODE END */


/*
class InMemoryIndex {
public:
	std::map<uint64_t, int8_t> index;
	
	void insert(uint64_t key, int8_t value) {
		index[key] = value;
		// Impleent this function
	}

	void update(uint64_t key, int8_t value) {
		index[key] = value;
		// Impleent this function
	}

	int8_t read(uint64_t key) {
		return index[key];
		// Impleent this function
	}

	uint64_t scan(uint64_t key, int8_t num) {
		auto it = index.lower_bound(key);
		uint64_t result = 0;
		for (int8_t i = 0; i < num; ++i) {
			if (it == index.end()) break;
			result += it->second;
			++it;
		}
		// Impleent this function
		return result;
	}
};
*/

/* no need to change from here */

struct Operation {
	int type;
	uint64_t key;
	int num;

	Operation() {
		type = -1; key = 0; num = 0;
	}
	Operation(int t, uint64_t k, int n) {
		type = t; key = k; num = n;
	}
};

InMemoryIndex tree;
std::vector<Operation> inits;
std::vector<Operation> txns;

double load (std::string &fname, std::vector<Operation> &ops, uint64_t limit) {
	Timer timer;
	timer.start();
	ops.reserve(limit);
	std::ifstream infile(fname);
	std::string op;
	uint64_t key;
	int num;
	for (uint64_t count = 0; count < limit && infile.good(); ++count) {
		infile >> op >> key;
		if (op.compare("INSERT") == 0) {
			ops.emplace_back(INSERT, key, 0); 
		} else if (op.compare("READ") == 0) {
			ops.emplace_back(READ, key, 0); 
		} else if (op.compare("UPDATE") == 0) {
			ops.emplace_back(UPDATE, key, 0); 
		} else if (op.compare("SCAN") == 0) {
			infile >> num;
			ops.emplace_back(SCAN, key, num); 
		}
	}
	infile.close();
	return timer.get_elapsed_time();
}

double exec_loads (std::vector<Operation> &ops) {
	Timer timer;
	timer.start();
	for (auto op = ops.begin(); op != ops.end(); ++op) {
		if (op->type == INSERT) {
			tree.insert(op->key, 1);
		} 
	}
	return timer.get_elapsed_time();
}

uint64_t read_result = 0;
uint64_t scan_result = 0;

double exec_txns (std::vector<Operation> &ops) {
	Timer timer;
	timer.start();
	for (auto op = ops.begin(); op != ops.end(); ++op) {
		if (op->type == INSERT) {
			tree.insert(op->key, 1);
		} else if (op->type == READ) {
			read_result += tree.read(op->key);
		} else if (op->type == UPDATE) {
			tree.update(op->key, 2);
		} else if (op->type == SCAN) {
			scan_result += tree.scan(op->key, op->num);
		}
	}
	return timer.get_elapsed_time();
}

int main(int argc, char* argv[]) {
	std::string load_fname(argv[1]);
	std::string txns_fname(argv[2]);
	size_t load_size = std::stoi(argv[3]);
	size_t txns_size = std::stoi(argv[4]);

	std::cout << "load_fname: " << load_fname << "\n";
	std::cout << "txns_fname: " << txns_fname << "\n";

	load(load_fname, inits, load_size);
	std::cout << "Execute " << load_size << " operations\n";
	double load_time = exec_loads(inits);
	std::cout << "load time(ms): " << load_time << "\n";
	inits.clear();

	load(txns_fname, txns, txns_size);
	std::cout << "Execute " << txns_size << " operations\n";
	double txns_time = exec_txns(txns);
	std::cout << "txns time(ms): " << txns_time << "\n";
	std::cout << "sum of values of read entries: " << read_result << "\n";
	std::cout << "sum of values of scanned entries: " << scan_result << "\n";
	return 1;
}
