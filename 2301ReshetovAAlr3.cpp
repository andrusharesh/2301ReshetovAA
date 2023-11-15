#include <algorithm>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <queue>
#include <vector>
#include <stack>
#include <chrono>
#include <ctime>
#include <random>
#include <cstdlib>
#define LANGUAGE 1251
using namespace std;
using namespace std::chrono;

class BinarySearchTreeClass {
	struct Node
	{
		int key;
		Node* left_child = nullptr;
		Node* right_child = nullptr;
		Node(int key) {
			this->key = key;
		}
	} *root = nullptr;

	bool findKey(Node* node, int key) const {
		while ((node != nullptr) and (key != node->key)) {
			if (key > node->key)
				node = node->right_child;
			else if (key < node->key)
				node = node->left_child;
			if (node == nullptr)
				return false;
			if (key == node->key)
				return true;
		}
	}

	Node* insert(Node* node, int key) {
		if (node == nullptr) {
			return new Node(key);
		}

		Node* ptr = node;
		Node* parent_ptr = nullptr;

		while (ptr != nullptr) {
			parent_ptr = ptr;
			if (key < ptr->key)
				ptr = ptr->left_child;
			else
				ptr = ptr->right_child;
		}

		if (key < parent_ptr->key)
			parent_ptr->left_child = new Node(key);
		else
			parent_ptr->right_child = new Node(key);

		return node;
	}

	void preorderPrint(Node* node) const {
		if (node != nullptr) {
			cout << node->key << " ";		//сначала выводим значение
			preorderPrint(node->left_child);		//идем в самое лево (параллельно выводя)
			preorderPrint(node->right_child);		//идем по правым веткам (пока не встретится левое)

		}
	}

	void postorderPrint(Node* node) const {
		if (node == nullptr)		//если дошли до конца возврат
			return;

		postorderPrint(node->left_child);	//идем по левой до макс
		postorderPrint(node->right_child);	//идем по правой до макс
		cout << node->key << " ";	//выводим значение
	}

	void inorderPrint(Node* node) const {
		if (node == nullptr)
			return;
		inorderPrint(node->left_child);
		cout << node->key << " ";
		inorderPrint(node->right_child);
	}

	void widthPrint(Node* root) {
		if (root == nullptr) return;

		queue<Node*> vector;
		vector.push(root);

		while (!vector.empty()) {
			Node* node = vector.front();
			cout << node->key << " ";
			vector.pop();

			if (node->left_child != nullptr)
				vector.push(node->left_child);

			if (node->right_child != nullptr)
				vector.push(node->right_child);
		}
	}

	void destroy(Node* node) {
		if (node == nullptr)
			return;

		std::stack<Node*> nodeStack;
		Node* current = node;

		while (current != nullptr || !nodeStack.empty()) {
			while (current != nullptr) {
				if (current->right_child)
					nodeStack.push(current->right_child);

				nodeStack.push(current);
				current = current->left_child;
			}

			current = nodeStack.top();
			nodeStack.pop();

			if (!nodeStack.empty() && current->right_child && nodeStack.top() == current->right_child) {
				nodeStack.pop();
				nodeStack.push(current);
				current = current->right_child;
			}
			else {
				delete current;
				current = NULL;
			}
		}
	}

	Node* findMin(Node* node) {
		while (node->left_child != nullptr)
			node = node->left_child;
		return node;
	}

	Node* deleteNode(Node* node, int key) {
		while ((node != nullptr) and (key != node->key)) {
			if (key > node->key)
				node = node->right_child;
			else if (key < node->key)
				node = node->left_child;
			if (node == nullptr)
				return node;
			if (key == node->key) {
				// нет детей
				if (node->left_child == nullptr && node->right_child == nullptr) {
					delete node;
					node = NULL;
				}

				//один ребенок
				else if (node->left_child == nullptr) {
					Node* memory = node;
					node = node->right_child;
					delete memory;
				}

				else if (node->right_child == nullptr) {
					Node* memory = node;
					node = node->left_child;
					delete memory;
				}
				//два ребенка
				else {
					Node* memory = findMin(node->right_child);
					node->key = memory->key;
					node->right_child = deleteNode(node->right_child, memory->key);
				}
			}
		}
		return node;
	}

	int hight(Node* node) {
		vector<Node*> top;		//вектор вершин (ссылки)
		vector<Node*> tops_;	//промежуточный вектор
		if (!node) {
			return 0;
		}
		int hight = 0;
		top.push_back(node);	//присваиваем первую вершину
		while (!top.empty()) {	//пока есть вершины
			for (int i = 0; i < top.size(); i++) {	//проходим по всем вершинам уровня
				if (top[i]->left_child)				//формируем веришны след уровня
					tops_.push_back(top[i]->left_child);
				if (top[i]->right_child)
					tops_.push_back(top[i]->right_child);
			}
			hight++;
			top.clear();
			top = tops_;	//вектор вершин переходит на след уровень
			tops_.clear();
		}
		return hight;
	}

public:
	bool findKey(int key) const {
		return findKey(root, key);
	}
	void insert(int key) {
		root = insert(root, key);
	}
	void preorderPrint() {
		preorderPrint(root);
	}
	void postorderPrint() {
		postorderPrint(root);
	}
	void inorderPrint() {
		inorderPrint(root);
	}
	void deleteNode(int key) {
		root = deleteNode(root, key);
	}
	void widthPrint() {
		widthPrint(root);
	}
	void destroy() {
		destroy(root);
	}
	void hight() {
		cout << hight(root);
	}
};

void bstTree() {
	BinarySearchTreeClass bsttree;

	int key;
	int choice;

	do {
		cout << endl << "----------------------------Двоичное дерево----------------------------" << endl;
		cout << "1. Добавить элемент" << endl;
		cout << "2. Проверить, есть ли элемент в дереве" << endl;
		cout << "3. Вывести результат ПРЯМОГО обхода по дереву (preorder)" << endl;
		cout << "4. Вывести результат ОБРАТНОГО обхода по дереву (postorder)" << endl;
		cout << "5. Вывести результат СИММЕТРИЧНОГО обхода по дереву (inorder)" << endl;
		cout << "6. Вывести результат обхода по дереву В ШИРИНУ" << endl;
		cout << "7. Удалить элемент по ключу" << endl;
		cout << "8. Вывести высоту дерева" << endl;
		cout << "0. Вернуться в главное меню" << endl;
		cout << "------------------------------------------------------------------" << endl;
		cout << endl << "Выберите действие: ";
		cin >> choice;
		cout << endl << "------------------------------------------------------------------" << endl;

		switch (choice)
		{
		case 1:
			cout << endl << "Введите элемент, который хотите добавить: ";
			cin >> key;
			bsttree.insert(key);
			break;
		case 2:
			cout << endl << "Введите элемент, который хотите найти: ";
			cin >> key;
			if (bsttree.findKey(key))
				cout << "Элемент присутствует в дереве." << endl;
			else
				cout << "Элемент отсутствует в дереве." << endl;
			break;
		case 3:
			cout << endl << "Список: " << endl;
			bsttree.preorderPrint();
			break;
		case 4:
			cout << endl << "Список: " << endl;
			bsttree.postorderPrint();
			break;
		case 5:
			cout << endl << "Список: " << endl;
			bsttree.inorderPrint();
			break;
		case 6:
			cout << endl << "Список: " << endl;
			bsttree.widthPrint();
			break;
		case 7:
			cout << endl << "Введите значение элемента, который хотите удалить: ";
			int deleteKey;
			cin >> deleteKey;
			bsttree.deleteNode(deleteKey);
			break;
		case 8:
			cout << endl << "Высота дерева: ";
			bsttree.hight();
			break;
		case 9:
			int k;
			k = 10;
			bsttree.insert(k);
			k = 7;
			bsttree.insert(k);
			k = 5;
			bsttree.insert(k);
			k = 8;
			bsttree.insert(k);
			k = 9;
			bsttree.insert(k);
			k = 20;
			bsttree.insert(k);
			k = 30;
			bsttree.insert(k);
			k = 40;
			bsttree.insert(k);
			k = 35;
			bsttree.insert(k);
			break;

		case 0:
			bsttree.destroy();
			break;
		default:
			break;
		}
	} while (choice != 0);

}

class AVLtreeClass {
	struct Node {
		int key, height;
		Node* left_child = nullptr;
		Node* right_child = nullptr;
		Node(int key) {
			this->key = key;
			this->height = 1;
		}
	} *root = nullptr;

	void swap(Node* a, Node* b) {	//обмен ключами для поворота
		int temp = a->key;
		a->key = b->key;
		b->key = temp;
	}

	int height(Node* node) {
		if (node == nullptr)
			return 0;
		return node->height;
	}

	void updateheight(Node* n) {	//обнововление высоты родителя
		n->height = max(height(n->left_child), height(n->right_child)) + 1;
	}

	bool findKey(Node* node, int key) const {
		while ((node != nullptr) and (key != node->key)) { //поиск по ключу
			if (key > node->key)
				node = node->right_child;
			else if (key < node->key)
				node = node->left_child;
			if (node == nullptr)
				return false;
			if (key == node->key)
				return true;
		}
	}

	Node* rightRotate(Node* node) {
		swap(node, node->left_child);
		Node* memory = node->right_child;
		node->right_child = node->left_child;
		node->left_child = node->right_child->left_child;
		node->right_child->left_child = node->right_child->right_child;
		node->right_child->right_child = memory;
		updateheight(node->right_child);
		updateheight(node);
		return node;

	}
	Node* leftRotate(Node* node) {
		swap(node, node->right_child);
		Node* memory = node->left_child;
		node->left_child = node->right_child;
		node->right_child = node->left_child->right_child;
		node->left_child->right_child = node->left_child->left_child;
		node->left_child->left_child = memory;
		updateheight(node->left_child);
		updateheight(node);
		return node;
	}

	int getBalance(Node* node) { //разность высот 
		if (node == nullptr)
			return 0;
		return height(node->left_child) - height(node->right_child);
	}

	Node* insert(Node* node, int key) {
		if (node == nullptr)
			return new Node(key);
		if (key < node->key)
			node->left_child = insert(node->left_child, key);
		else if (key > node->key)
			node->right_child = insert(node->right_child, key);
		else
			return node;

		node->height = 1 + max(height(node->left_child), height(node->right_child));
		int balance = getBalance(node);

		if (balance > 1 && key < node->left_child->key)
			return rightRotate(node);

		if (balance < -1 && key > node->right_child->key)
			return leftRotate(node);

		if (balance > 1 && key > node->left_child->key) {
			node->left_child = leftRotate(node->left_child);
			return rightRotate(node);
		}

		if (balance < -1 && key < node->right_child->key) {
			node->right_child = rightRotate(node->right_child);
			return leftRotate(node);
		}

		return node;
	}

	Node* minValueNode(Node* node) {
		Node* current = node;
		while (current->left_child != NULL)
			current = current->left_child;
		return current;
	}

	Node* deleteNode(Node* node, int key) {
		if (node == nullptr)
			return node;
		if (key < node->key)
			node->left_child = deleteNode(node->left_child, key);
		else if (key > node->key)
			node->right_child = deleteNode(node->right_child, key);
		else {
			if ((node->left_child == nullptr) || (node->right_child == nullptr)) {
				Node* temp = node->left_child ? node->left_child : node->right_child;
				if (temp == nullptr) {
					temp = node;
					node = nullptr;
				}
				else
					*node = *temp;
				free(temp);
			}
			else {
				Node* temp = minValueNode(node->right_child);
				node->key = temp->key;
				node->right_child = deleteNode(node->right_child, temp->key);
			}
		}

		if (node == nullptr)
			return node;

		node->height = 1 + max(height(node->left_child), height(node->right_child));
		int balance = getBalance(node);

		if (balance > 1 && getBalance(node->left_child) >= 0)
			return rightRotate(node);

		if (balance > 1 && getBalance(node->left_child) < 0) {
			node->left_child = leftRotate(node->left_child);
			return rightRotate(node);
		}

		if (balance < -1 && getBalance(node->right_child) <= 0)
			return leftRotate(node);

		if (balance < -1 && getBalance(node->right_child) > 0) {
			node->right_child = rightRotate(node->right_child);
			return leftRotate(node);
		}

		return node;
	}

	void destroy(Node* node) {		//удалить дерево
		if (node == nullptr) return;
		destroy(node->left_child);
		destroy(node->right_child);
		delete node;
	}

	void preorderPrint(Node* node) const {
		if (node != nullptr) {
			cout << node->key << " ";
			preorderPrint(node->left_child);
			preorderPrint(node->right_child);
		}
	}

	void postorderPrint(Node* node) const {
		if (node == nullptr)		//если дошли до конца возврат
			return;

		postorderPrint(node->left_child);	//идем по левой до макс
		postorderPrint(node->right_child);	//идем по правой до макс
		cout << node->key << " ";	//выводим значение
	}

	void inorderPrint(Node* node) const {
		if (node == nullptr)
			return;
		inorderPrint(node->left_child);
		cout << node->key << " ";
		inorderPrint(node->right_child);
	}

	void widthPrint(Node* root) {
		if (root == nullptr) return;

		queue<Node*> vector;
		vector.push(root);

		while (!vector.empty()) {
			Node* node = vector.front();
			cout << node->key << " ";
			vector.pop();

			if (node->left_child != nullptr)
				vector.push(node->left_child);

			if (node->right_child != nullptr)
				vector.push(node->right_child);
		}
	}

	int hight(Node* node) {
		vector<Node*> top;		//вектор вершин (ссылки)
		vector<Node*> tops_;	//промежуточный вектор
		if (!node) {
			return 0;
		}
		int hight = 0;
		top.push_back(node);	//присваиваем первую вершину
		while (!top.empty()) {	//пока есть вершины
			for (int i = 0; i < top.size(); i++) {	//проходим по всем вершинам уровня
				if (top[i]->left_child)				//формируем веришны след уровня
					tops_.push_back(top[i]->left_child);
				if (top[i]->right_child)
					tops_.push_back(top[i]->right_child);
			}
			hight++;
			top.clear();
			top = tops_;	//вектор вершин переходит на след уровень
			tops_.clear();
		}
		return hight;
	}

public:
	bool findKey(int key) const {
		return(root, key);
	}
	void insert(int key) {
		root = insert(root, key);
	}
	void preorderPrint() {
		preorderPrint(root);
	}
	void postorderPrint() {
		postorderPrint(root);
	}
	void inorderPrint() {
		inorderPrint(root);
	}
	void widthPrint() {
		widthPrint(root);
	}
	void deleteNode(int key) {
		root = deleteNode(root, key);
	}
	void destroy() {
		destroy(root);
	}
	void hight() {
		cout << hight(root);
	}
};

void avlTree() {
	AVLtreeClass avltree;

	int key;
	int choice;
	do {
		cout << endl << "----------------------------AVL дерево----------------------------" << endl;
		cout << "1. Добавить элемент" << endl;
		cout << "2. Проверить, есть ли элемент в дереве" << endl;
		cout << "3. Вывести результат ПРЯМОГО обхода по дереву (preorder)" << endl;
		cout << "4. Вывести результат ОБРАТНОГО обхода по дереву (postorder)" << endl;
		cout << "5. Вывести результат СИММЕТРИЧНОГО обхода по дереву (inorder)" << endl;
		cout << "6. Вывести результат обхода по дереву В ШИРИНУ" << endl;
		cout << "7. Удалить элемент по ключу" << endl;
		cout << "8. Вывести высоту дерева" << endl;
		cout << "0. Вернуться в главное меню" << endl;
		cout << "------------------------------------------------------------------" << endl;
		cout << endl << "Выберите действие: ";
		cin >> choice;
		cout << endl << "------------------------------------------------------------------" << endl;

		switch (choice) {
		case 1:
			cout << endl << "Введите элемент, который хотите добавить: ";
			cin >> key;
			avltree.insert(key);
			break;
		case 2:
			cout << endl << "Введите элемент, который хотите найти: ";
			cin >> key;
			if (avltree.findKey(key))
				cout << "Элемент присутствует в дереве." << endl;
			else
				cout << "Элемент отсутствует в дереве." << endl;
			break;
		case 3:
			cout << endl << "Список: " << endl;
			avltree.preorderPrint();
			break;
		case 4:
			cout << endl << "Список: " << endl;
			avltree.postorderPrint();
			break;
		case 5:
			cout << endl << "Список: " << endl;
			avltree.inorderPrint();
			break;
		case 6:
			cout << endl << "Список: " << endl;
			avltree.widthPrint();
			break;
		case 7:
			cout << endl << "Введите значение элемента, который хотите удалить: ";
			int deleteKey;
			cin >> deleteKey;
			avltree.deleteNode(deleteKey);
			break;
		case 8:
			cout << endl << "Высота дерева: ";
			avltree.hight();
			break;
		case 0:
			avltree.destroy();
			break;
		default:
			break;
		}
	} while (choice != 0);
}

class RedBlackTreeClass {
	struct Node {
		int key;
		Node* parent;
		Node* left_child;
		Node* right_child;
		int color;
	};

	typedef Node* Node_star;

private:
	Node_star root;
	Node_star TNULL;

	Node_star minimum(Node_star node) {
		while (node->left_child != TNULL) {
			node = node->left_child;
		}
		return node;
	}

	Node_star maximum(Node_star node) {
		while (node->right_child != TNULL) {
			node = node->right_child;
		}
		return node;
	}

	void initializeNULLNode(Node_star node, Node_star parent) {
		node->key = 0;
		node->parent = parent;
		node->left_child = nullptr;
		node->right_child = nullptr;
		node->color = 0;
	}

	void preorderPrint(Node_star node) {
		if (node != TNULL) {
			cout << node->key << " ";
			preorderPrint(node->left_child);
			preorderPrint(node->right_child);
		}
	}

	void inorderPrint(Node_star node) {
		if (node != TNULL) {
			inorderPrint(node->left_child);
			cout << node->key << " ";
			inorderPrint(node->right_child);
		}
	}

	void postorderPrint(Node_star node) {
		if (node != TNULL) {
			postorderPrint(node->left_child);
			postorderPrint(node->right_child);
			cout << node->key << " ";
		}
	}

	void widthPrint(Node_star node) {
		if (node == nullptr) {
			return;
		}

		std::queue<Node_star> q;
		q.push(node);

		while (!q.empty()) {
			int size = q.size();

			while (size > 0) {
				Node* temp = q.front();
				q.pop();

				std::cout << temp->key << " ";

				if (temp->left_child) {
					q.push(temp->left_child);
				}

				if (temp->right_child) {
					q.push(temp->right_child);
				}

				size--;
			}

			std::cout << std::endl;
		}
	}

	void leftRotate(Node_star first) {
		Node_star second = first->right_child;
		first->right_child = second->left_child;
		if (second->left_child != TNULL) {
			second->left_child->parent = first;
		}
		second->parent = first->parent;
		if (first->parent == nullptr) {
			this->root = second;
		}
		else if (first == first->parent->left_child) {
			first->parent->left_child = second;
		}
		else {
			first->parent->right_child = second;
		}
		second->left_child = first;
		first->parent = second;
	}

	void rightRotate(Node_star first) {
		Node_star second = first->left_child;
		first->left_child = second->right_child;
		if (second->right_child != TNULL) {
			second->right_child->parent = first;
		}
		second->parent = first->parent;
		if (first->parent == nullptr) {
			this->root = second;
		}
		else if (first == first->parent->right_child) {
			first->parent->right_child = second;
		}
		else {
			first->parent->left_child = second;
		}
		second->right_child = first;
		first->parent = second;
	}

	//для балансировки дерева после удаления
	void deleteFix(Node_star x) {
		Node_star s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left_child) {
				s = x->parent->right_child;
				if (s->color == 1) {
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right_child;
				}

				if (s->left_child->color == 0 && s->right_child->color == 0) {
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right_child->color == 0) {
						s->left_child->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right_child;
					}

					s->color = x->parent->color;
					x->parent->color = 0;
					s->right_child->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left_child;
				if (s->color == 1) {
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left_child;
				}

				if (s->right_child->color == 0 && s->right_child->color == 0) {
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left_child->color == 0) {
						s->right_child->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left_child;
					}

					s->color = x->parent->color;
					x->parent->color = 0;
					s->left_child->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}

	//используется для замены одного поддерева в качестве дочернего по отношению к его родительскому на другое поддерево
	void rbTransplant(Node_star first_subtree, Node_star second_subtree) {
		if (first_subtree->parent == nullptr) {
			root = second_subtree;
		}
		else if (first_subtree == first_subtree->parent->left_child) {
			first_subtree->parent->left_child = second_subtree;
		}
		else {
			first_subtree->parent->right_child = second_subtree;
		}
		second_subtree->parent = first_subtree->parent;
	}

	void deleteNode(Node_star node, int key) {
		Node_star z = TNULL;
		Node_star x, y;
		while (node != TNULL) {
			if (node->key == key) {
				z = node;
			}

			if (node->key <= key) {
				node = node->right_child;
			}
			else {
				node = node->left_child;
			}
		}

		if (z == TNULL) {
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left_child == TNULL) {
			x = z->right_child;
			rbTransplant(z, z->right_child);
		}
		else if (z->right_child == TNULL) {
			x = z->left_child;
			rbTransplant(z, z->left_child);
		}
		else {
			y = minimum(z->right_child);
			y_original_color = y->color;
			x = y->right_child;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right_child);
				y->right_child = z->right_child;
				y->right_child->parent = y;
			}

			rbTransplant(z, y);
			y->left_child = z->left_child;
			y->left_child->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			deleteFix(x);
		}
	}

	// балансировка после вставки
	void insertFix(Node_star k) {
		Node_star u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right_child) {
				u = k->parent->parent->left_child;
				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left_child) {
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right_child;

				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right_child) {
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void destroy(Node_star node) {
		if (node != TNULL) {
			destroy(node->left_child);
			destroy(node->right_child);
			delete node;
		}
	}

	int hight(Node_star node) {
		vector<Node_star> top;		//вектор вершин (ссылки)
		vector<Node_star> tops_;	//промежуточный вектор
		if (!node) {
			return 0;
		}
		int hight = 0;
		top.push_back(node);	//присваиваем первую вершину
		while (!top.empty()) {	//пока есть вершины
			for (int i = 0; i < top.size(); i++) {	//проходим по всем вершинам уровня
				if (top[i]->left_child)				//формируем веришны след уровня
					tops_.push_back(top[i]->left_child);
				if (top[i]->right_child)
					tops_.push_back(top[i]->right_child);
			}
			hight++;
			top.clear();
			top = tops_;	//вектор вершин переходит на след уровень
			tops_.clear();
		}
		return hight;
	}

	bool findKey(Node_star node, int key) {
		while (node != TNULL) {
			if (node->key == key) {
				return true;
			}
			if (node->key > key) {
				node = node->left_child;
			}
			else {
				node = node->right_child;
			}
		}
		return false;
	}

public:
	RedBlackTreeClass() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left_child = nullptr;
		TNULL->right_child = nullptr;
		root = TNULL;
	}

	Node_star getRoot() {
		return this->root;
	}

	void preorderPrint() {
		preorderPrint(this->root);
	}

	void inorderPrint() {
		inorderPrint(this->root);
	}

	void postorderPrint() {
		postorderPrint(this->root);
	}

	void widthPrint() {
		widthPrint(this->root);
	}

	bool findKey(int key) {
		return (getRoot(), key);
	}

	void insert(int key) {
		Node_star node = new Node;
		node->parent = nullptr;
		node->key = key;
		node->left_child = TNULL;
		node->right_child = TNULL;
		node->color = 1;

		Node_star y = nullptr;
		Node_star x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->key < x->key) {
				x = x->left_child;
			}
			else {
				x = x->right_child;
			}
		}

		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->key < y->key) {
			y->left_child = node;
		}
		else {
			y->right_child = node;
		}

		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		if (node->parent->parent == nullptr) {
			return;
		}

		insertFix(node);
	}

	void deleteNode(int key) {
		deleteNode(this->root, key);
	}

	void destroy() {
		destroy(this->root);
	}

	void hight() {
		cout << hight(this->root);
	}
};

void rbTree() {
	RedBlackTreeClass rbtree;

	int key;
	int choice;
	do {
		cout << endl << "-----------------------Красно-черное дерево-----------------------" << endl;
		cout << "1. Добавить элемент" << endl;
		cout << "2. Проверить, есть ли элемент в дереве" << endl;
		cout << "3. Вывести результат ПРЯМОГО обхода по дереву (preorder)" << endl;
		cout << "4. Вывести результат ОБРАТНОГО обхода по дереву (postorder)" << endl;
		cout << "5. Вывести результат СИММЕТРИЧНОГО обхода по дереву (inorder)" << endl;
		cout << "6. Вывести результат обхода по дереву В ШИРИНУ" << endl;
		cout << "7. Удалить элемент по ключу" << endl;
		cout << "8. Вывести высоту дерева" << endl;
		cout << "0. Вернуться в главное меню" << endl;
		cout << "------------------------------------------------------------------" << endl;
		cout << endl << "Выберите действие: ";
		cin >> choice;
		cout << endl << "------------------------------------------------------------------" << endl;

		switch (choice) {
		case 1:
			cout << endl << "Введите элемент, который хотите добавить: ";
			cin >> key;
			rbtree.insert(key);
			break;
		case 2:
			cout << endl << "Введите элемент, который хотите найти: ";
			cin >> key;
			if (rbtree.findKey(key)) {
				cout << "Элемент присутствует в дереве." << endl;
			}
			else
				cout << "Элемент отсутствует в дереве." << endl;
			break;
		case 3:
			cout << endl << "Список: " << endl;
			rbtree.preorderPrint();
			break;
		case 4:
			cout << endl << "Список: " << endl;
			rbtree.postorderPrint();
			break;
		case 5:
			cout << endl << "Список: " << endl;
			rbtree.inorderPrint();
			break;
		case 6:
			cout << endl << "Список: " << endl;
			rbtree.widthPrint();
			break;
		case 7:
			cout << endl << "Введите значение элемента, который хотите удалить: ";
			int deleteKey;
			cin >> deleteKey;
			rbtree.deleteNode(deleteKey);
			break;
		case 8:
			cout << endl << "Высота дерева: ";
			rbtree.hight();
			break;
		case 0:
			rbtree.destroy();
			break;
		default:
			break;
		}
	} while (choice != 0);
}

void RandomInsertResults() {
	srand(time(0));
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;
	int* numbers = new int[tree_max];

	for (int i = 0; i < tree_max; i++) { //массив случайных чисел для вставки в дерево
		numbers[i] = rand() % tree_max;
	}

	auto begin = steady_clock::now();
	auto end = steady_clock::now();
	cout << endl << "%Среднее время добавления случайных элементов" << endl;

	cout << "AVLrandomInsert=["; //вывод времени вставки рандомного порядка 
	int k = 0; //счетчик запятых в выводе
	int sum = 0;//время
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				begin = steady_clock::now();
				avltree.insert(numbers[i + j]);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
				int mid_time = sum / i;
				cout << mid_time;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				begin = steady_clock::now();
				avltree.insert(numbers[i + j]);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
			}
		}
	}
	cout << "];" << endl;

	cout << "RBrandomInsert=["; //вывод времени вставки рандомного порядка 
	k = 0; //счетчик запятых в выводе
	sum = 0;//время
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				begin = steady_clock::now();
				rbtree.insert(numbers[i + j]);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
				int mid_time = sum / i;
				cout << mid_time;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				begin = steady_clock::now();
				rbtree.insert(numbers[i + j]);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
			}
		}
	}
	cout << "];" << endl;

	avltree.destroy();
	rbtree.destroy();
}

void SortedInsertResults() {
	srand(time(0));
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;

	auto begin = steady_clock::now();
	auto end = steady_clock::now();

	cout << endl << "%Среднее время добавления элементов, отсортированных по возрастанию" << endl;
	cout << "AVLsortedInsert=[";//вывод времени вставки возрастающего порядка
	int k = 0; //счетчик запятых в выводе
	int sum = 0;//время
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			int element = i + j;
			if (i == tree_step - 1) {
				begin = steady_clock::now();
				avltree.insert(element);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
				int mid_time = sum / i;
				cout << mid_time;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				begin = steady_clock::now();
				avltree.insert(element);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
			}
		}
	}
	cout << "];" << endl;

	cout << "RBsortedInsert=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	sum = 0;//время
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			int element = i + j;
			if (i == tree_step - 1) {
				begin = steady_clock::now();
				rbtree.insert(element);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
				int mid_time = sum / i;
				cout << mid_time;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				begin = steady_clock::now();
				rbtree.insert(element);
				end = steady_clock::now();
				sum += duration_cast<nanoseconds>(end - begin).count();
			}
		}
	}
	cout << "];" << endl;

	avltree.destroy();
	rbtree.destroy();
}

void RandomDelResults() {
	srand(time(0));
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;
	int* numbers = new int[tree_max];

	for (int i = 0; i < tree_max; i++) { //массив случайных чисел для вставки в дерево
		numbers[i] = rand() % tree_max;
	}

	auto begin = steady_clock::now();
	auto end = steady_clock::now();

	cout << endl << "%Среднее время удаления случайного элемента из случайного дерева" << endl;
	cout << "AVLrandomDel=[";//вывод времени вставки возрастающего порядка
	int k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				float sum_del = 0;
				for (int del = 0; del < tree_step; del++)
				{
					int element = numbers[i + j];
					begin = steady_clock::now();
					avltree.deleteNode(element);
					end = steady_clock::now();
					sum_del += duration_cast<nanoseconds>(end - begin).count() / 10000.0;
					avltree.insert(element);
				}
				std::cout << (int)sum_del;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				avltree.insert(numbers[i + j]);
			}
		}
	}
	cout << "];" << endl;

	cout << "RBrandomDel=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				float sum_del = 0;
				for (int del = 0; del < tree_step; del++)
				{
					int element = numbers[i + j];
					begin = steady_clock::now();
					rbtree.deleteNode(element);
					end = steady_clock::now();
					sum_del += duration_cast<nanoseconds>(end - begin).count() / 10000.0;
					rbtree.insert(element);
				}
				std::cout << (int)sum_del;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				rbtree.insert(numbers[i + j]);
			}
		}
	}
	cout << "];" << endl;

	avltree.destroy();
	rbtree.destroy();
}

void SortedDelResults() {
	srand(time(0));
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;

	auto begin = steady_clock::now();
	auto end = steady_clock::now();

	cout << endl << "%Среднее время удаления случайного элемента из отсортированного по возрастанию дерева" << endl;
	cout << "AVLsortedDel=[";//вывод времени вставки возрастающего порядка
	int k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				float sum_del = 0;
				for (int del = 0; del < tree_step; del++) {
					int element = i + j;
					begin = steady_clock::now();
					avltree.deleteNode(element);
					end = steady_clock::now();
					sum_del += duration_cast<nanoseconds>(end - begin).count() / 10000.0;
					avltree.insert(element);
				}
				cout << (int)sum_del;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				avltree.insert(i + j);
			}
		}
	}
	cout << "];" << endl;

	cout << "RBsortedDel=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				float sum_del = 0;
				for (int del = 0; del < tree_step; del++) {
					int element = i + j;
					begin = steady_clock::now();
					rbtree.deleteNode(element);
					end = steady_clock::now();
					sum_del += duration_cast<nanoseconds>(end - begin).count() / 10000.0;
					rbtree.insert(element);
				}
				cout << (int)sum_del;
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				rbtree.insert(i + j);
			}
		}
	}
	cout << "];" << endl;

	avltree.destroy();
	rbtree.destroy();
}

void RandomHeightResults() {
	BinarySearchTreeClass bsttree;
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;

	int* numbers = new int[tree_max];

	for (int i = 0; i < tree_max; i++) { //массив случайных чисел для вставки в дерево
		numbers[i] = rand() % tree_max;
	}

	cout << endl << "%Высоты случайного дерева" << endl;
	cout << "AVLrandomHeight=[";//вывод времени вставки возрастающего порядка
	int k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				avltree.hight();
				avltree.insert(numbers[i + j]);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				avltree.insert(numbers[i + j]);
			}
		}
	}
	cout << "];" << endl;

	cout << "BSTrandomHeight=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				bsttree.hight();
				bsttree.insert(numbers[i + j]);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				bsttree.insert(numbers[i + j]);
			}
		}
	}
	cout << "];" << endl;

	cout << "RBrandomHeight=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				rbtree.hight();
				rbtree.insert(numbers[i + j]);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				rbtree.insert(numbers[i + j]);
			}
		}
	}
	cout << "];" << endl;



	avltree.destroy();
	bsttree.destroy();
	rbtree.destroy();
}

void SortedHeightResults() {
	BinarySearchTreeClass bsttree;
	AVLtreeClass avltree;
	RedBlackTreeClass rbtree;

	int tree_max = 100000;
	int tree_step = 10000;

	cout << endl << "%Высоты отсортированного по возрастанию дерева" << endl;
	cout << "AVLsortedHeight=[";//вывод времени вставки возрастающего порядка
	int k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				avltree.hight();
				avltree.insert(i + j);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				avltree.insert(i + j);
			}
		}
	}
	cout << "];" << endl;

	cout << "BSTsortedHeight=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				bsttree.hight();
				bsttree.insert(i + j);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				bsttree.insert(i + j);
			}
		}
	}
	cout << "];" << endl;

	cout << "RBsortedHeight=[";//вывод времени вставки возрастающего порядка
	k = 0; //счетчик запятых в выводе
	for (int j = 0; j < tree_max - 1; j += tree_step) {
		for (int i = 0; i < tree_step; i++) {
			if (i == tree_step - 1) {
				rbtree.hight();
				rbtree.insert(i + j);
				if (k < 9) {
					cout << ",";
					k++;
				}
			}
			else {
				rbtree.insert(i + j);
			}
		}
	}
	cout << "];" << endl;


	avltree.destroy();
	bsttree.destroy();
	rbtree.destroy();
}

void showResults() {
	RandomInsertResults();
	SortedInsertResults();
	RandomDelResults();
	SortedDelResults();
	RandomHeightResults();
	SortedHeightResults();
}

int main() {
	SetConsoleCP(LANGUAGE);
	SetConsoleOutputCP(LANGUAGE);
	int choiceTree;
	do {
		cout << "-------------------------------МЕНЮ-------------------------------";
		cout << endl << "Выберите дерево: " << endl;
		cout << "1. Двоичное дерево поиска" << endl;
		cout << "2. АВЛ дерево" << endl;
		cout << "3. Черно-красное дерево" << endl;
		cout << "4. ЗАМЕРЫ" << endl;
		cout << "0. Выйти из программы";
		cout << endl << "------------------------------------------------------------------" << endl;
		cout << endl << "Выберите действие: ";
		cin >> choiceTree;
		switch (choiceTree)
		{
		case 1:
			bstTree();
			break;

		case 2:
			avlTree();
			break;

		case 3:
			rbTree();
			break;

		case 4:
			showResults();
			break;
		}

	} while (choiceTree != 0);
	return 0;
}
