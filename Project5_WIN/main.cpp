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
 

#include <stack>
#include <algorithm>


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
		keys.reserve(order); //order : max num of keys
	}

	virtual void insert_value(uint64_t key, int8_t value) {}
	virtual void insert_right(uint64_t key, Node* rightChild) {}
	virtual void insert_left_right(uint64_t key, Node* leftChild, Node* rightChild) {}
	virtual Node* split(uint64_t* keyToParent, int order) { return NULL; }
	virtual std::vector<Node*> get_children() { std::vector<Node*> t;  return t; }
	virtual std::vector<int8_t> get_values() { std::vector<int8_t> t;  return t; }
	virtual Node* get_next() { return NULL; }
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

	void insert_right(uint64_t key, Node* rightChild)
	{
		// insert key in to suitable position in the given internal node
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		keys.insert(index, key);

		// insert right child in the immediately next index in the children std::vector
		index = lower_bound(keys.begin(), keys.end(), key);
		children.insert(children.begin() + (index - keys.begin() + 1), rightChild);
	}

	void insert_left_right(uint64_t key, Node* leftChild, Node* rightChild)
	{
		// insert key, left child and right child
		keys.push_back(key);
		children.push_back(leftChild);
		children.push_back(rightChild);
	}

	Node* split(uint64_t* keyToParent, int order)
	{

		uint64_t length = keys.size();

		// create a new right internal node
		InternalNode* rightNode = new InternalNode(order);

		// key to be moved up to the parent is the middle element in the current internal node
		*keyToParent = keys[length / 2];

		// Copy the second half of the current internal node excluding the middle element to the
		// new right internal node. Erase the second half of the current internal node including
		// the middle element, and thus current internal node becomes the left internal node.
		rightNode->keys.assign(keys.begin() + (length / 2 + 1), keys.end());
		rightNode->children.assign(children.begin() + (length / 2 + 1), children.end());
		keys.erase(keys.begin() + length / 2, keys.end());
		children.erase(children.begin() + (length / 2 + 1), children.end());

		// return the new right internal node
		return rightNode;
	}
};

// leaf node
class LeafNode : public Node
{
private:
	LeafNode* prev = this;
	LeafNode* next = this;
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

	void insert_value(uint64_t key, int8_t value)
	{
		// search for the key in the given leaf node
		std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);

		// check if inserting a duplicate value for an existing key
		if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && (key == keys[index - keys.begin()]))
		{
			//update value! 
			//std::cout << "FOUND : " << (int)values[index - keys.begin()] << std::endl;
			values[index - keys.begin()] = value;
			//std::cout << "CHANGED : " << (int)values[index - keys.begin()] << std::endl;
		}

		// if inserting a new key and value
		else
		{
			// insert the new key
			keys.insert(index, key);

			//insert the corresponding value
			index = lower_bound(keys.begin(), keys.end(), key); //necessary?
			values.insert(values.begin() + (index - keys.begin()), value);
		}
	}

	Node* split(uint64_t* keyToParent, int order)
	{
		// create a new right leaf node
		LeafNode* rightNode = new LeafNode(order);

		// key to be moved up to the parent is the middle element in the current leaf node
		*keyToParent = keys[keys.size() / 2];

		// Copy the second half of the current leaf node to the new right leaf node. Erase the second
		// half of the current leaf node, and thus the current leaf node becomes the left leaf node.
		rightNode->keys.assign(keys.begin() + keys.size() / 2, keys.end());
		rightNode->values.assign(values.begin() + values.size() / 2, values.end());
		keys.erase(keys.begin() + keys.size() / 2, keys.end());
		values.erase(values.begin() + values.size() / 2, values.end());

		// link the leaf nodes to form a doubly linked list
		rightNode->next = next;
		rightNode->prev = this;
		next = rightNode;
		(rightNode->next)->prev = rightNode;

		// return the right leaf node
		return rightNode;
	}

	
};

// B+ tree
class BPlusTree
{
private:
	int order;
	Node* root;

	void search_path(Node* node, uint64_t key, std::stack<Node*>* path)
	{
		// push node to stack
		path->push(node);

		// check if the node pushed to stack is an internal node
		if (!node->get_is_leaf())
		{
			// search for the given key in the current node
			std::vector<uint64_t> keys = node->get_keys();
			std::vector<Node*> children = node->get_children();
			std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);

			// check if key is found
			if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && key == keys[index - keys.begin()])
			{
				// recursively repeat by searching the path through the corresponding right child index
				search_path(children[(index - keys.begin()) + 1], key, path);
			}

			// if key is not found
			else
			{
				// recursively repeat by searching the path through the corresponding left child index
				search_path(children[index - keys.begin()], key, path);
			}
		}
	}
	void destroy(Node* node)
	{
		// recursively repeat the function to delete all the nodes level by level, startin g with the leaf nodes
		if (!node->get_is_leaf())
		{
			std::vector<Node*> children = node->get_children();
			for (std::vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				destroy(*index);
			}
		}
		delete (node);
	}

	void debug_node(Node* node)
	{
		// check if tree is empty
		if (NULL == node)
		{
			std::cout << std::endl
				<< "Root Node: Null";
			return;
		}

		// check if current node is a leaf node
		if (node->get_is_leaf())
		{
			std::cout << std::endl
				<< "Leaf Node: ";
		}

		// if current node is a internal node
		else
		{
			std::cout << std::endl
				<< "Internal Node: ";
		}

		// display the keys
		std::vector<uint64_t> keys = node->get_keys();
		for (std::vector<uint64_t>::iterator index = keys.begin(); index != keys.end(); index++)
		{
			std::cout << *index << " ";
		}
		std::cout << std::endl;

		// check if internal node to continue revelation of the next level
		if (!node->get_is_leaf())
		{
			// display the keys in the children of the current internal node
			std::vector<Node*> children = node->get_children();
			std::cout << "children" << std::endl
				<< "--------" << std::endl;
			for (std::vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				std::vector<uint64_t> childKeys = (*index)->get_keys();
				for (std::vector<uint64_t>::iterator i = childKeys.begin(); i != childKeys.end(); i++)
				{
					std::cout << *i << " ";
				}
				std::cout << std::endl;
			}

			// recursively repeat revelation of the next level
			for (std::vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				debug_node(*index);
			}
		}
	}

public:
	BPlusTree(int order_) : order{ order_ }, root{ NULL } {

	}

	void insert(uint64_t key, int8_t value)
	{
		// check if tree is empty
		if (NULL == root)
		{
			// Irrespective of the order, root is always a leaf node for
			// the first insertion. So, create a new leaf node.
			root = new LeafNode(order);
			root->insert_value(key, value);
		}

		// if it is a subsequent insertion
		else
		{
			Node* leftNode = NULL;
			Node* rightNode = NULL;
			uint64_t key_to_parent = 0;
			bool rootPopped = false;

			// obtain the search path from the root to leaf node and push it on to a stack
			std::stack<Node*> path;
			search_path(root, key, &path);

			// insert the key-value pair in the leaf node
			path.top()->insert_value(key, value);

			// Split the current node and insert the middle key & children in to the parent. Perform
			// this as long as there is an imbalance in the tree, moving up the stack every iteration.
			while (path.top()->get_keys().size() == order)
			{
				// Update the current node as the left half and return the right half. Also
				// obtain the middle element, which is the key to be moved up to the parent.
				leftNode = path.top();
				rightNode = leftNode->split(&key_to_parent, order);

				// check if currently split node is not the root node
				path.pop();
				if (!path.empty())
				{
					// Insert the middle key and the right half in to
					// the parent. The parent will be an internal node.
					path.top()->insert_right(key_to_parent, rightNode);
				}

				// if currently split node is the root node
				else
				{
					// set flag indicating that the root has popped from the stack
					rootPopped = true;
					break;
				}
			}

			// new internal node needs to be created and assigned as the root
			if (rootPopped)
			{
				// create a new internal node
				InternalNode* tempRoot = new InternalNode(order);

				// insert the left and the right halves as the children of this new internal node
				tempRoot->insert_left_right(key_to_parent, leftNode, rightNode);

				// mark this new internal node as the root of the tree
				root = tempRoot;
			}

		
		}
	}
	int8_t search(uint64_t key)
	{
		int8_t ret = 0;

		// check if tree is empty
		if (NULL == root)
		{
			//outputFile << "Null" << std::endl;
		}

		// if it is a vaild search
		else
		{
			int i = 0;

			// obtain the search path from root to leaf node and push it on to a stack
			std::stack<Node*> path;
			search_path(root, key, &path);

			// search for the key in the leaf node, which is at the top of the stack
			std::vector<uint64_t> keys = path.top()->get_keys();
			std::vector<int8_t> values = path.top()->get_values();
			std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
			auto b = index - keys.begin();

			// check if key is found
			if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && key == keys[index - keys.begin()])
			{
				// display the values
				//outputFile << values[index - keys.begin()] << ",";
				//std::cout << "FOUND : " << (int)values[index - keys.begin()] << std::endl;
				ret = values[index - keys.begin()];

			}

			// if key is not found
			else
			{
				//outputFile << "Null" << std::endl;
			}

		}
		return ret;

	}
	uint64_t scan(uint64_t key1, int8_t num)
	{
		uint64_t ret = 0; //sum
		int8_t count = 0;

		// check if tree is empty
		if (NULL == root)
		{
			return ret;
		}

		// if it is a valid range search
		else
		{
			int i = 0;
			bool firstPass = true;
			uint64_t firstKey = -1;

			// obtain the search path from root to leaf node and push it on to a stack
			std::stack<Node*> path;
			search_path(root, key1, &path);

			// search for the key in the leaf node, which is at the top of the stack
			std::vector<uint64_t> keys = path.top()->get_keys();
			std::vector<int8_t> values = path.top()->get_values();
			Node* next = path.top()->get_next();
			std::vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key1);

			// display all the keys in the search range, along with their corresponding values
			while (1)
			{
				// check if end of the current leaf node is reached
				if ((index - keys.begin()) == keys.size())
				{
					// go to the next leaf node
					keys = next->get_keys();
					values = next->get_values();
					next = next->get_next();
					index = keys.begin();
				}

				// save the smallest key in the given search range
				if (firstPass)
				{
					firstKey = keys[index - keys.begin()];
				}

				// check if already iterated through the doubly linked list once
				if (!(firstPass || (keys[index - keys.begin()] != firstKey)))
				{
					// exit the loop
					break;
				}

				// check if key is within the search range
				if ((key1 <= keys[index - keys.begin()]))
				{
					if (!firstPass)
					{
						//outputFile << ",";
					}

					// display the key and its corresponding values
					//outputFile << values[index - keys.begin()] << ",";
					ret += values[index - keys.begin()];
					count += 1;

					if (count == num) {
						break;
					}

				}

				// if key is not within the search range
				else
				{
					// check if atleast one key was in the search range
					if (!firstPass)
					{
						//outputFile << std::endl;
					}

					// if no keys belonged within the search range
					else
					{
						//outputFile << "Null" << std::endl;
					}

					// exit the loop
					break;
				}

				firstPass = false;
				index++;
			}

		}

		return ret;
	}

	~BPlusTree()
	{
		destroy(root);
	}

	void debug_tree()
	{
		debug_node(root);
	}
};


class InMemoryIndex
{
	BPlusTree tree;

public:
	InMemoryIndex() : tree(300){ //order 300 b+ tree
	}

	void insert(uint64_t key, int8_t value)
	{
		//std::cout << "INSERT " << key << "," << (int)value << std::endl;
		tree.insert(key, value);
	}

	void update(uint64_t key, int8_t value)
	{
		//std::cout << "UPDATE " << key << "," << (int)value << std::endl;
		tree.insert(key, value);
	}

	int8_t read(uint64_t key)
	{
		//std::cout << "READ " << key << std::endl;
		return tree.search(key);
	}

	uint64_t scan(uint64_t key, int8_t num)
	{
		//std::cout << "SCAN " << key << "," << (int)num << std::endl;
		return tree.scan(key, num);
	}
};


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
