#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#define LANGUAGE 1251

using namespace std;

struct Node //определение структуры узла для двусвязного списка
{
	int number;
	struct Node* next;
	struct Node* previous;
};

void printList(Node* start, int& length) //вывод списка
{
	cout << "Список: " << endl;
	Node* iterator = start->next;
	for (int i = 0; i < length; i++)
	{
		cout << iterator->number << " ";
		iterator = iterator->next;
	}
	cout << endl;
}

void addLastElem(Node* adder, Node* start, Node* end, int& length, int toAdd) //добавление элемента в конец списка
{
	adder = new Node;
	adder->number = toAdd;
	end->previous->next = adder;
	adder->next = end;
	adder->previous = end->previous;
	end->previous = adder;
	length++;

}

void addFirstElem(Node* adder, Node* start, int& length, int toAdd) //добавление элемента в начало списка
{
	adder = new Node;
	adder->number = toAdd;
	adder->next = start->next;
	start->next->previous = adder;
	start->next = adder;
	adder->previous = start;
	length++;

}

void deleteLastElem(Node* start, Node* end, int& length) //удаление последнего элемента
{
	Node* add = end->previous;
	end->previous = add->previous;
	add->previous->next = end;
	delete add;
	length--;

}

void deleteFirstElem(Node* start, int& length) //удаление первого элемента
{
	Node* add = start->next;
	start->next = add->next;
	add->next->previous = start;
	delete add;
	length--;

}

void addElem(Node* start, Node* end, int& length, int index) //добавление элемента по индексу
{
	if (index == 0) {
		Node* add = new Node;
		cout << "Введите значение нового элемента: ";
		int element;
		cin >> element;
		addFirstElem(add, start, length, element);
	}
	else if ((index < length) and (index > 0)) {
		int element;
		cout << "Введите значение нового элемента: ";
		cin >> element;
		if (index < ceil(length / 2)) {
			Node* add = start->next;
			for (int i = 1; i < index; i++) {
				add = add->next;
			}
			Node* addNew = new Node;
			addNew->number = element;
			add->next->previous = addNew;
			addNew->next = add->next;
			add->next = addNew;
			addNew->previous = add;
		}
		else {
			Node* add = end->previous;
			for (int i = 1; i < length - index; i++) {
				add = add->previous;
			}
			Node* addNew = new Node;
			addNew->number = element;
			add->previous->next = addNew;
			addNew->previous = add->previous;
			addNew->next = add;
			add->previous = addNew;
		}
		length++;
	}
	else {
		cout << "Ошибка, введен несуществующий индекс" << endl;
	}
}

Node* findElem(Node* start, Node* end, int& length, int index) { //нахождение элемента по индексу

	if ((index < length) and (index >= 0)) {

		if (index < ceil(length / 2)) {
			Node* add = start->next;
			for (int i = 0; i < index; i++) {
				add = add->next;
			}
			return add;
		}
		else {
			Node* add = end->previous;
			for (int i = 1; i < length - index; i++) {
				add = add->previous;
			}
			return add;
		}
	}
	else {
		cout << "Ошибка, введен несуществующий индекс" << endl;
	}
}

void swap(Node* start, Node* end, int& length, int indexFirst, int indexSecond) { //обмен элементов по индексам
	int temp;
	Node* addFirst = findElem(start, end, length, indexFirst);
	Node* addSecond = findElem(start, end, length, indexSecond);
	temp = addFirst->number;
	addFirst->number = addSecond->number;
	addSecond->number = temp;
}

void deleteElem(Node* start, Node* end, int& length, int index) { //удаление элемента по индексу

	if ((index < length) and (index >= 0)) {
		if (index < ceil(length / 2)) {
			Node* add = start->next;
			for (int i = 0; i < index; i++) {
				add = add->next;
			}
			Node* del = add;
			del->next->previous = del->previous;
			del->previous->next = del->next;
		}
		else {
			Node* add = end->previous;
			for (int i = 1; i < length - index; i++) {
				add = add->previous;
			}
			Node* del = add;
			del->next->previous = del->previous;
			del->previous->next = del->next;
		}
		length--;
	}
	else
	{
		cout << "Ошибка, введен несуществующий индекс" << endl;
	}
}

void clearList(Node* start, int& length) //очищение списка
{
	Node* del = start->next;
	while (length != 0) {
		deleteFirstElem(start, length);
	}
}

void replaceElem(Node* start, Node* end, int& length, int index) { //заменить элемент по индексу
	if ((index < length) and (index >= 0)) {
		int newElem;
		cout << "Введите новое значение элемента: ";
		cin >> newElem;
		if (index < ceil(length / 2)) {
			Node* add = start->next;
			for (int i = 0; i < index; i++) {
				add = add->next;
			}
			add->number = newElem;
		}
		else {
			Node* add = end->previous;
			for (int i = 1; i < length - index; i++) {
				add = add->previous;
			}
			add->number = newElem;
		}
	}
	else {
		cout << "Ошибка, введен несуществующий индекс" << endl;
	}

}

void reverseList(Node*& start, Node*& end, int& length) //переворот списка
{
	Node* add = start->next;
	Node* temp = new Node;
	for (int i = 1; i <= length; i++) {
		temp->next = add->next;
		temp->previous = add->previous;
		add->next = temp->previous;
		add->previous = temp->next;
		add = temp->next;
	}
	temp = start;
	start = end;
	end = temp;
	start->next = start->previous;
	start->previous = NULL;
	end->previous = end->next;
	end->next = NULL;
}

void insertList(Node* start, Node* end, int& length, bool& inserted, Node* startSecondList, Node* endSecondList, int& lengthSecondList, int index) { //вставка другого списка по определенному индексу

	if ((index < length) and (index >= 0)) {
		cout << "Значение элемента по индексу: " << index << endl;

		if (index < ceil(length / 2)) {
			Node* add = start->next;
			for (int i = 0; i < index; i++) {
				add = add->next;
			}
			add->next->previous = endSecondList->previous;
			endSecondList->previous->next = add->next;
			startSecondList->next->previous = add;
			add->next = startSecondList->next;
		}
		else {
			Node* add = end->previous;
			for (int i = 1; i < length - index; i++) {
				add = add->previous;
			}
			add->next->previous = endSecondList->previous;
			endSecondList->previous->next = add->next;
			startSecondList->next->previous = add;
			add->next = startSecondList->next;
		}
		length += lengthSecondList;
		inserted = true;
		printList(start, length);
	}
	else {
		cout << "Ошибка, введен несуществующий индекс" << endl;
	}

}

void insertList_in_End(Node* start, Node* end, int& length, bool& inserted, Node* startSecondList, Node* endLastList, int& lengthSecondList) //вставка второго списка в конце
{
	startSecondList->next->previous = end->previous;
	end->previous->next = startSecondList->next;
	endLastList->previous->next = end;
	end->previous = endLastList->previous;
	length += lengthSecondList;
	inserted = true;
}

void insertList_in_Start(Node* start, Node* end, int& length, bool& inserted, Node* startSecondList, Node* endSecondList, int& lengthSecondList) //вставка второго списка в начале
{
	endSecondList->previous->next = start->next;
	start->next->previous = endSecondList->previous;
	start->next = startSecondList->next;
	startSecondList->next->previous = start;
	length += lengthSecondList;
	inserted = true;

}

int* is_Second_in_First(Node* start, Node* end, Node* startSecondList, Node* endSecondList, int length, int lengthSecondList) //проверка на содержание второго списка в основном 
{

	bool contains = false;
	Node* curFirst = start->next;
	Node* curSecond = startSecondList->next;
	int iterator = 0;
	int* indexes = new int[lengthSecondList];
	int index = 0;
	while (iterator < lengthSecondList && curFirst->next != end->next) {
		if (curFirst->number == curSecond->number) {
			indexes[iterator] = index;
			curSecond = curSecond->next;
			curFirst = curFirst->next;
			iterator++;
			index++;
		}
		else {
			if (iterator == 0) {
				curFirst = curFirst->next;
				index++;
			}
			iterator = 0;
			curSecond = startSecondList->next;
		}
	}

	if (iterator == lengthSecondList) {
		contains = true;
	}
	if (contains) {
		cout << "Вставной список содержится в начальном" << endl;
		return indexes;
	}
	else {
		cout << "Вставной список не содержится в начальном" << endl;
		return 0;
	}

}

void findFirstEntry(Node* start, Node* end, Node* startSecondList, Node* endSecondList, int length, int lengthSecondList) { //индекс первого вхождения второго списка в начальный
	int* indexes = is_Second_in_First(start, end, startSecondList, endSecondList, length, lengthSecondList);

	if (indexes != 0) {
		cout << "Индекс первого вхождения второго списка в основной: " << indexes[0] << endl;
	}
	else {
		cout << "Второй список не входит в основной" << endl;
	}

}

void findLastEntry(Node* start, Node* end, Node* startSecondList, Node* endSecondList, int length, int lengthSecondList) //индекс последнего вхождения второго списка в начальный
{
	int* indexes = is_Second_in_First(start, end, startSecondList, endSecondList, length, lengthSecondList);

	if (indexes != 0) {
		cout << "Индекс последнего вхождения второго списка в основной: " << indexes[lengthSecondList - 1] << endl;
	}
	else {
		cout << "Второй список не входит в основной" << endl;
	}
}

void checkLength(Node* start, Node* end) //проверка на пустоту списка
{
	Node* cur = start->next;
	if (cur == end)
		cout << "Список пустой" << endl;
	else
		cout << "Список не пустой" << endl;
}

void findLength(Node* start, Node* end) //определение длины списка 
{

	int getLength = 1;
	Node* cur = start->next;
	while (cur->next != end) {
		getLength++;
		cur = cur->next;
	}
	cout << "Длина списка: " << getLength << endl;

}

int main()
{
	SetConsoleCP(LANGUAGE);
	SetConsoleOutputCP(LANGUAGE);

	int lengthMain = 0;
	int lengthSecond = 0;
	Node* headMain = new Node;
	Node* headSecond = new Node;
	Node* tailMain = new Node;
	Node* tailSecond = new Node;
	Node* add = NULL;
	int firstElem;
	int countAdded = 0;
	int choice;
	int choiceBegin;
	bool inserted = false;

	cout << "=========================== Заполните второй список (для дальнейшей работы с двумя списками) ===========================" << endl << endl;
	if (lengthSecond == 0)
	{
		cout << "Введите первый элемент для добавления в список: ";
		cin >> firstElem;
		add = new Node;
		add->number = firstElem;
		add->next = tailSecond;
		add->previous = headSecond;
		headSecond->next = add;
		tailSecond->previous = add;
		lengthSecond++;
		printList(headSecond, lengthSecond);
	}

	do {
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "Выберите: " << endl;
		cout << "1. Добавить элемент в конец" << endl;
		cout << "2. Добавить элемент в начало" << endl;
		cout << "3. Добавить элемент по индексу" << endl;
		cout << "0. Закончить ввод и перейти к основному списку" << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "Ваш выбор: ";
		cin >> choiceBegin;
		switch (choiceBegin)
		{
		case 1:
			int lastElem;
			cout << "Введите элемент для добавления: ";
			cin >> lastElem;
			addLastElem(add, headSecond, tailSecond, lengthSecond, lastElem);
			break;
		case 2:
			int newElem;
			cout << "Введите элемент для добавления: ";
			cin >> newElem;
			addFirstElem(add, headSecond, lengthSecond, newElem);
			break;
		case 3:
			int getIndex;
			cout << "Введите индекс для добавления: ";
			cin >> getIndex;
			addElem(headSecond, tailSecond, lengthSecond, getIndex);
			break;
		}
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		printList(headSecond, lengthSecond);
	} while (choiceBegin != 0);
	cout << endl << "========================== Заполните основной список (для дальнейшей работы с двумя списками) ==========================" << endl << endl;
	do
	{
		if (lengthMain == 0)
		{
			cout << "Введите первый элемент для добавления в список: ";
			cin >> firstElem;
			add = new Node;
			add->number = firstElem;
			add->next = tailMain;
			add->previous = headMain;
			headMain->next = add;
			tailMain->previous = add;
			lengthMain++;
			printList(headMain, lengthMain);
			cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		}

		cout << "========================================================= МЕНЮ =========================================================" << endl;
		cout << "Выберите действие: " << endl;
		cout << "1. Добавить элемент в конец списка" << endl;
		cout << "2. Добавить элемент в начало списка" << endl;
		cout << "3. Удалить последний элемент списка" << endl;
		cout << "4. Удалить первый элемент списка" << endl;
		cout << "5. Добавить элемент по индексу" << endl;
		cout << "6. Получить элемент по индексу" << endl;
		cout << "7. Удалить элемент по индексу" << endl;
		cout << "8. Получить размер списка" << endl;
		cout << "9. Удалить все элементы списка" << endl;
		cout << "10. Заменить элемент по индексу" << endl;
		cout << "11. Проверка на пустоту списка" << endl;
		cout << "12. Поменять порядок элементов на обратный" << endl;
		cout << "13. Вставить список в список по индексу" << endl;
		cout << "14. Вставить другой список в конец" << endl;
		cout << "15. Вставить другой список в начало" << endl;
		cout << "16. Проверить на содержание одного списка в другом" << endl;
		cout << "17. Поиск первого вхождения списка в список" << endl;
		cout << "18. Поиск последнего вхождения списка в список" << endl;
		cout << "19. Обмен двух элементов списка по индексам" << endl;
		cout << "0. Завершить работу" << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "Ваш выбор: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			int lastElem;
			cout << "Введите элемент для добавления: ";
			cin >> lastElem;
			addLastElem(add, headMain, tailMain, lengthMain, lastElem);
			break;
		case 2:
			int firstElem;
			cout << "Введите элемент для добавления: ";
			cin >> firstElem;
			addFirstElem(add, headMain, lengthMain, firstElem);
			break;
		case 3:
			deleteLastElem(headMain, tailMain, lengthMain);
			break;
		case 4:
			deleteFirstElem(headMain, lengthMain);
			break;
		case 5:
			cout << "Введите индекс для добавления: ";
			int indexAdd;
			cin >> indexAdd;
			addElem(headMain, tailMain, lengthMain, indexAdd);
			break;
		case 6:
			cout << "Введите индекс элемента для поиска: ";
			int findIndex;
			cin >> findIndex;
			add = findElem(headMain, tailMain, lengthMain, findIndex);
			cout << "Значение элемента по индексу " << findIndex << ": " << add->number << endl;
			break;
		case 7:
			cout << "Введите индекс для удаления: ";
			int delIndex;
			cin >> delIndex;
			deleteElem(headMain, tailMain, lengthMain, delIndex);
			break;
		case 8:
			findLength(headMain, tailMain);
			break;
		case 9:
			clearList(headMain, lengthMain);
			break;
		case 10:
			cout << "Введите индекс элемента: ";
			cin >> findIndex;
			replaceElem(headMain, tailMain, lengthMain, findIndex);
			break;
		case 11:
			checkLength(headMain, tailMain);
			break;
		case 12:
			reverseList(headMain, tailMain, lengthMain);
			break;
		case 13:
			cout << "Введите индекс элемента, после которого вставить список: ";
			int insertIndex;
			cin >> insertIndex;
			insertList(headMain, tailMain, lengthMain, inserted, headSecond, tailSecond, lengthSecond, insertIndex);
			break;
		case 14:
			insertList_in_End(headMain, tailMain, lengthMain, inserted, headSecond, tailSecond, lengthSecond);
			break;
		case 15:
			insertList_in_Start(headMain, tailMain, lengthMain, inserted, headSecond, tailSecond, lengthSecond);
			break;
		case 16:
			is_Second_in_First(headMain, tailMain, headSecond, tailSecond, lengthMain, lengthSecond);
			break;
		case 17:
			findFirstEntry(headMain, tailMain, headSecond, tailSecond, lengthMain, lengthSecond);
			break;
		case 18:
			findLastEntry(headMain, tailMain, headSecond, tailSecond, lengthMain, lengthSecond);
			break;
		case 19:
			cout << "Введите индексы элементов для перестановки: ";
			int index1;
			int index2;
			cin >> index1;
			cin >> index2;
			swap(headMain, tailMain, lengthMain, index1, index2);
			break;
		}
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		printList(headMain, lengthMain);
	} while (choice != 0);


	return 0;
}
