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

#define NUM_ARGS 2
#define MIN_ORDER 3
#define OUTPUT_FILE "output_file.txt"

using namespace std;

// generic node
class Node
{
protected:
	bool isLeaf;
	vector<uint64_t> keys;

public:
	bool Get_IsLeaf() {
		// return whether leaf or internal node
		return isLeaf;
	}
	vector<uint64_t> Get_Keys() {
		// return the vector of keys
		return keys;
	}
	virtual void Insert(uint64_t key, int8_t value) {}
	virtual void Insert(uint64_t key, Node* rightChild) {}
	virtual void Insert(uint64_t key, Node* leftChild, Node* rightChild) {}
	virtual void Search(uint64_t key) {}
	virtual void Search(uint64_t key1, uint64_t key2) {}
	virtual Node* Split(uint64_t* keyToParent) { return NULL; }
	virtual vector<Node*> Get_Children() { vector<Node*> t;  return t; }
	virtual vector<int8_t> Get_Values() { vector<int8_t> t;  return t; }
	virtual Node* Get_Next() { return NULL; }
};

// internal node
class InternalNode : public Node
{
private:
	vector<Node*> children;

public:
	InternalNode()
	{
		isLeaf = false;
	}

	void Insert(uint64_t key, Node* rightChild)
	{
		// insert key in to suitable position in the given internal node
		vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
		keys.insert(index, key);

		// insert right child in the immediately next index in the children vector
		index = lower_bound(keys.begin(), keys.end(), key);
		children.insert(children.begin() + (index - keys.begin() + 1), rightChild);
	}

	void Insert(uint64_t key, Node* leftChild, Node* rightChild)
	{
		// insert key, left child and right child
		keys.push_back(key);
		children.push_back(leftChild);
		children.push_back(rightChild);
	}

	Node* Split(uint64_t* keyToParent)
	{

		uint64_t length = keys.size();

		// create a new right internal node
		InternalNode* rightNode = new InternalNode;

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

	vector<Node*> Get_Children()
	{
		// return the children vector
		return children;
	}
};

// leaf node
class LeafNode : public Node
{
private:
	LeafNode* prev;
	LeafNode* next;
	vector<int8_t> values;

public:
	LeafNode()
	{
		isLeaf = true;
		prev = this;
		next = this;
	}

	void Insert(uint64_t key, int8_t value)
	{
		// search for the key in the given leaf node
		vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);

		// check if inserting a duplicate value for an existing key
		if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && (key == keys[index - keys.begin()]))
		{
			//update value! 
			//cout << "FOUND : " << (int)values[index - keys.begin()] << endl;
			values[index - keys.begin()] = value;
			//cout << "CHANGED : " << (int)values[index - keys.begin()] << endl;
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

	Node* Split(uint64_t* keyToParent)
	{
		// create a new right leaf node
		LeafNode* rightNode = new LeafNode;

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

	vector<int8_t> Get_Values()
	{
		// return the vector of values
		return values;
	}

	Node* Get_Next()
	{
		// return the pointer to the next leaf node
		return next;
	}
};

// B+ tree
class BPlusTree
{
private:
	int order;
	Node* root;

	void Search_Path(Node* node, uint64_t key, stack<Node*>* path)
	{
		// push node to stack
		path->push(node);

		// check if the node pushed to stack is an internal node
		if (!node->Get_IsLeaf())
		{
			// search for the given key in the current node
			vector<uint64_t> keys = node->Get_Keys();
			vector<Node*> children = node->Get_Children();
			vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);

			// check if key is found
			if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && key == keys[index - keys.begin()])
			{
				// recursively repeat by searching the path through the corresponding right child index
				Search_Path(children[(index - keys.begin()) + 1], key, path);
			}

			// if key is not found
			else
			{
				// recursively repeat by searching the path through the corresponding left child index
				Search_Path(children[index - keys.begin()], key, path);
			}
		}
	}
	void Destroy(Node* node)
	{
		// recursively repeat the function to delete all the nodes level by level, startin g with the leaf nodes
		if (!node->Get_IsLeaf())
		{
			vector<Node*> children = node->Get_Children();
			for (vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				Destroy(*index);
			}
		}
		delete (node);
	}

	void Reveal_Tree(Node* node)
	{
		// check if tree is empty
		if (NULL == node)
		{
			cout << endl
				<< "Root Node: Null";
			return;
		}

		// check if current node is a leaf node
		if (node->Get_IsLeaf())
		{
			cout << endl
				<< "Leaf Node: ";
		}

		// if current node is a internal node
		else
		{
			cout << endl
				<< "Internal Node: ";
		}

		// display the keys
		vector<uint64_t> keys = node->Get_Keys();
		for (vector<uint64_t>::iterator index = keys.begin(); index != keys.end(); index++)
		{
			cout << *index << " ";
		}
		cout << endl;

		// check if internal node to continue revelation of the next level
		if (!node->Get_IsLeaf())
		{
			// display the keys in the children of the current internal node
			vector<Node*> children = node->Get_Children();
			cout << "children" << endl
				<< "--------" << endl;
			for (vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				vector<uint64_t> childKeys = (*index)->Get_Keys();
				for (vector<uint64_t>::iterator i = childKeys.begin(); i != childKeys.end(); i++)
				{
					cout << *i << " ";
				}
				cout << endl;
			}

			// recursively repeat revelation of the next level
			for (vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
			{
				Reveal_Tree(*index);
			}
		}
	}

public:
	BPlusTree(int m) {
		order = m;
		root = NULL;
	}

	void Insert(uint64_t key, int8_t value)
	{
		// check if tree is empty
		if (NULL == root)
		{
			// Irrespective of the order, root is always a leaf node for
			// the first insertion. So, create a new leaf node.
			root = new LeafNode;
			root->Insert(key, value);
		}

		// if it is a subsequent insertion
		else
		{
			Node* leftNode = NULL;
			Node* rightNode = NULL;
			uint64_t* keyToParent = new uint64_t;
			bool rootPopped = false;

			// obtain the search path from the root to leaf node and push it on to a stack
			stack<Node*>* path = new stack<Node*>;
			Search_Path(root, key, path);

			// insert the key-value pair in the leaf node
			path->top()->Insert(key, value);

			// Split the current node and insert the middle key & children in to the parent. Perform
			// this as long as there is an imbalance in the tree, moving up the stack every iteration.
			while (path->top()->Get_Keys().size() == order)
			{
				// Update the current node as the left half and return the right half. Also
				// obtain the middle element, which is the key to be moved up to the parent.
				leftNode = path->top();
				rightNode = leftNode->Split(keyToParent);

				// check if currently split node is not the root node
				path->pop();
				if (!path->empty())
				{
					// Insert the middle key and the right half in to
					// the parent. The parent will be an internal node.
					path->top()->Insert(*keyToParent, rightNode);
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
				InternalNode* tempRoot = new InternalNode;

				// insert the left and the right halves as the children of this new internal node
				tempRoot->Insert(*keyToParent, leftNode, rightNode);

				// mark this new internal node as the root of the tree
				root = tempRoot;
			}

			delete (keyToParent);
			delete (path);
		}
	}
	int8_t Search(uint64_t key)
	{
		int8_t ret = 0;

		// check if tree is empty
		if (NULL == root)
		{
			//outputFile << "Null" << endl;
		}

		// if it is a vaild search
		else
		{
			int i = 0;

			// obtain the search path from root to leaf node and push it on to a stack
			stack<Node*>* path = new stack<Node*>;
			Search_Path(root, key, path);

			// search for the key in the leaf node, which is at the top of the stack
			vector<uint64_t> keys = path->top()->Get_Keys();
			vector<int8_t> values = path->top()->Get_Values();
			vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key);
			auto b = index - keys.begin();

			// check if key is found
			if ((0 != keys.size()) && (index - keys.begin() != keys.size()) && key == keys[index - keys.begin()])
			{
				// display the values
				//outputFile << values[index - keys.begin()] << ",";
				//cout << "FOUND : " << (int)values[index - keys.begin()] << endl;
				ret = values[index - keys.begin()];

			}

			// if key is not found
			else
			{
				//outputFile << "Null" << endl;
			}

			delete (path);

		}
		return ret;

	}
	uint64_t Search(uint64_t key1, int8_t num)
	{
		uint64_t ret = 0; //sum
		int8_t count = 0;

		// check if tree is empty
		if (NULL == root)
		{
			//outputFile << "Null" << endl;
		}

		// if it is a valid range search
		else
		{
			int i = 0;
			bool firstPass = true;
			uint64_t firstKey = -1;

			// obtain the search path from root to leaf node and push it on to a stack
			stack<Node*>* path = new stack<Node*>;
			Search_Path(root, key1, path);

			// search for the key in the leaf node, which is at the top of the stack
			vector<uint64_t> keys = path->top()->Get_Keys();
			vector<int8_t> values = path->top()->Get_Values();
			Node* next = path->top()->Get_Next();
			vector<uint64_t>::iterator index = lower_bound(keys.begin(), keys.end(), key1);

			// display all the keys in the search range, along with their corresponding values
			while (1)
			{
				// check if end of the current leaf node is reached
				if ((index - keys.begin()) == keys.size())
				{
					// go to the next leaf node
					keys = next->Get_Keys();
					values = next->Get_Values();
					next = next->Get_Next();
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
						//outputFile << endl;
					}

					// if no keys belonged within the search range
					else
					{
						//outputFile << "Null" << endl;
					}

					// exit the loop
					break;
				}

				firstPass = false;
				index++;
			}

			delete (path);
		}

		return ret;
	}

	~BPlusTree()
	{
		Destroy(root);
	}

	void Print_Tree()
	{
		Reveal_Tree(root);
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
		//cout << "INSERT " << key << "," << (int)value << endl;
		tree.Insert(key, value);
	}

	void update(uint64_t key, int8_t value)
	{
		//cout << "UPDATE " << key << "," << (int)value << endl;
		tree.Insert(key, value);
	}

	int8_t read(uint64_t key)
	{
		//cout << "READ " << key << endl;
		return tree.Search(key);
	}

	uint64_t scan(uint64_t key, int8_t num)
	{
		return tree.Search(key, num);
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
