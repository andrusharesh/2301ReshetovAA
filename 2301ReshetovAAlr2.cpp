#define _CRT_SECURE_NO_WARNINGS
#include <algorithm> 
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <chrono>
#define LEN 100
using namespace std;
using namespace chrono;

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
};

void printArray(int arr[]) { //вывод массива
    for (int i = 0; i < LEN; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void arrSorted(int arr[]) {//заполнение отсортированного массива
    int k = 0;
    for (int i = 0; i < LEN; i++) {
        arr[i] = k;
        k += 1;
    }
}

void arrPartSorted(int arr[]) {//заполнение частично отсортированного массива
    int k = 99;
    for (int i = 0; i < LEN / 2; i++) {
        arr[i] = i;
    }
    for (int i = 50; i < LEN; i++) {
        arr[i] = k;
        k -= 1;
    }
}

void arrBackSorted(int arr[]) {//заполнение обратно отсортированного массива
    int k = LEN;
    for (int i = 0; i < LEN; i++) {
        arr[i] = k;
        k -= 1;
    }
}

void arrRandomSorted(int arr[]) {
    for (int i = 0; i < LEN; i++)
        arr[i] = rand() % (LEN * 10);
}

void SelectionSort(int arr[], int arr_len) { //сортировка выбором
    int count, min;//count - сохраняет значение элемента, key - индекс минимального элемента 
    for (int i = 0; i < arr_len - 1; i++)
    {
        count = arr[i];
        min = i;
        for (int j = i + 1; j < arr_len; j++)
            if (arr[j] < arr[min]) min = j;
        if (min != i)
        {
            arr[i] = arr[min];
            arr[min] = count;
        }
    }
}

void InsertSort(int* array, int start, int end) {
    for (int i = start + 1; i < end + 1; i++) {
        int minElem = array[i];
        int j = i - 1;
        while (j >= 0 && array[j] > minElem) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = minElem;
    }
}

void BubbleSort(int arr[], int arr_len) { //сортировка пузырьком
    bool flag = true;
    for (int i = 0; i < arr_len - 1 && flag; i++) {
        flag = false;
        for (int j = arr_len - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                swap(arr[j - 1], arr[j]);
                flag = true;
            }
        }
    }
}

void merge(int arr[], int start, int end, int mid) {
    int lengthArray = end - start + 1;  //длина вспомогательного массива
    int right = mid + 1;    //индекс первого эл. правой стороны
    int left = start;     //индекс первого эл. левой стороны
    int indexArray = 0;   //индекс вспомогательного массива
    int* array = new int[lengthArray];


    while ((left <= mid) and (right <= end)) {   //идем по сторонам
        if (arr[left] < arr[right])     //меньший записываем в спомогательный
            array[indexArray++] = arr[left++];
        else array[indexArray++] = arr[right++];

    }
    while (left <= mid)     //записываем оставшийся хвостик массива
        array[indexArray++] = arr[left++];
    while (right <= end)
        array[indexArray++] = arr[right++];
    for (int i = start; i <= end; i++)      //переписываем массив
        arr[i] = array[i - start];

    delete[] array;     //удаляем массив
    array = nullptr;

}

void MergeSort(int arr[], int left, int right)
{
    if (left >= right) {    //пока не останется 1 элемент
        return;
    }
    int mid = left + (right - left) / 2; //находим середину
    MergeSort(arr, left, mid);     //разбиваем левую часть
    MergeSort(arr, mid + 1, right);//разбиваем правую часть
    merge(arr, left, right, mid);  //объединяем
}

void timmerge(int* array, int left, int right) {
    int i = left;
    int mid = left + (right - left) / 2;
    int j = mid + 1;
    int k = 0;
    int* sortedArr = new int[LEN];
    while ((i <= mid) && (j <= right)) {
        if (array[i] <= array[j]) {
            sortedArr[k] = array[i];
            i++;
        }
        else {
            sortedArr[k] = array[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        sortedArr[k] = array[i];
        i++;
        k++;
    }
    while (j <= right) {
        sortedArr[k] = array[j];
        j++;
        k++;
    }
    for (int x = 0; x < k; x++) {
        array[left + x] = sortedArr[x];
    }
}

int getMinRun(int size) {
    int n = 0;
    while (size >= 32) {
        n |= size & 1;
        size >>= 1;
    }
    return size + n;
}

void TimSort(int* array, int arr_len) {
    int minRun = getMinRun(arr_len);
    for (int start = 0; start < arr_len; start += minRun) {
        int end = min(start + minRun, arr_len - 1);
        InsertSort(array, start, end);
    }
    int size = minRun;
    while (size < arr_len) {
        for (int left = 0; left < arr_len; left += 2 * size) {
            int right = min(left + 2 * size - 1, arr_len - 1);
            if (left < right) {
                timmerge(array, left, right);
            }
        }
        size *= 2;
    }
}
/*
void QuickSort(int* arr, int left, int right)//быстрая сортировка первый вариант 
{
    int mid;
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    mid = arr[(l_hold + r_hold) / 2];
    pivot = mid;
    while (left < right) // пока границы не сомкнутся
    {
        while ((arr[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            arr[left] = arr[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо 
        }
        while ((arr[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            arr[right] = arr[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу влево 
        }
    }
    arr[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        QuickSort(arr, left, pivot - 1);
    if (right > pivot)
        QuickSort(arr, pivot + 1, right);
}
*/
void Quicksort(int* arr, int left, int right)//быстрая сортировка, второй вариант, который не выдает ошибку при больших массивов
{
    int mid, count;
    int f = left, l = right;
    mid = arr[(f + l) / 2]; //вычисление опорного элемента
    do
    {
        while (arr[f] < mid) f++;
        while (arr[l] > mid) l--;
        if (f <= l) //перестановка элементов
        {
            count = arr[f];
            arr[f] = arr[l];
            arr[l] = count;
            f++;
            l--;
        }
    } while (f < l);
    if (left < l) Quicksort(arr, left, l);
    if (f < right) Quicksort(arr, f, right);
}


void ShellSortDel(int arr[], int arr_len) {//сортировка Шелла с шагом деления на 2
    for (int gap = arr_len / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < arr_len; i += 1)
        {
            //сортировка подсписков, созданных с помощью gap 
            int key = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > key; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = key;
        }
    }
}

void ShellSortTsuira(int arr[], int arr_len) {
    int stepArr[9] = { 1750,701,301,132,57,23,10,4,1 };
    for (int istep = 0; istep < 9; istep++)//перебор всех шагов
    {
        int step = stepArr[istep];
        if (step < arr_len / 2 + 1)
        {

            for (int i = step; i < arr_len; i += 1)//перебор всех наборов с данным шагом
            {
                int key = arr[i];
                int j;
                for (j = i; j >= step && arr[j - step] > key; j -= step) {//проверка эллементов в наборе по алгоритму вставки
                    arr[j] = arr[j - step];
                }

                arr[j] = key;
            }
        }
    }
}

void ShellSortPow(int arr[], int arr_len)
{
    int firstStep = arr_len;

    for (int i = 8 * sizeof(int); i >= 0; i--)
    {
        int powN = pow(2, i);
        if ((arr_len & powN) != 0)
        {
            firstStep = powN;
            break;
        }
    }

    for (int step = firstStep; step > 1; step = (step + 1) / 2)//перебор всех шагов
    {

        for (int i = step; i < arr_len; i += 1)//перебор всех наборов с данным шагом
        {
            int temp = arr[i];
            int j;
            for (j = i; j >= step && arr[j - step] > temp; j -= step) {//проверка эллементов в наборе по алгоритму вставки
                arr[j] = arr[j - step];
            }

            arr[j] = temp;
        }
    }
    for (int i = 1; i < arr_len; i += 1)//перебор всех наборов с данным шагом
    {
        int temp = arr[i];
        int j;
        for (j = i; j >= 1 && arr[j - 1] > temp; j -= 1) {//проверка эллементов в наборе по алгоритму вставки
            arr[j] = arr[j - 1];
        }

        arr[j] = temp;
    }
}

void heapify(int arr[], int n, int root) {// функция heapify для нагромождения дерева

    int largest = root; // корень - самый большой элемент
    int l = 2 * root + 1; // слева = 2*корень + 1
    int r = 2 * root + 2; // справа = 2*корень + 2

    // Если левый дочерний элемент больше корневого
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // Если правый дочерний элемент больше, чем самый большой на данный момент
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // Если самый большой не является корневым
    if (largest != root)
    {
        //поменять местами корневой и самый большой
        swap(arr[root], arr[largest]);

        // Рекурсивное нагромождение поддерева
        heapify(arr, n, largest);
    }
}

void HeapSort(int arr[], int arr_len) {
    // создать кучу
    for (int i = arr_len / 2 - 1; i >= 0; i--)
        heapify(arr, arr_len, i);

    // извлечение элементов из кучи один за другим
    for (int i = arr_len - 1; i >= 0; i--)
    {
        // Переместить текущий корень в конец
        swap(arr[0], arr[i]);

        // снова вызовите max heapify для уменьшенной кучи
        heapify(arr, i, 0);
    }
}

int partition(int* arr, int start, int end) {
    int pivot = arr[start] + arr[end] + arr[(end - start) / 2] - max(arr[start], arr[end], arr[(end - start) / 2]) - min(arr[start], arr[end], arr[(end - start) / 2]);

    int index = start - 1;
    for (int j = start; j <= end - 1; j++) {
        if (arr[j] < pivot) {
            index++;
            swap(arr[index], arr[j]);
        }
    }
    index++;
    swap(arr[index], arr[end]);

    return index;
}

void IntroSort(int* array, int arr_len) {
    int partSize = partition(array, 0, arr_len - 1);
    if (partSize < 16) {
        InsertSort(array, 0, arr_len - 1);
    }
    else if (partSize > (2 * log(arr_len))) {
        HeapSort(array, arr_len);
    }
    else {
        QuickSort(array, 0, arr_len - 1);
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int* arr = new int[LEN]; // указатель для выделения памяти под массив
    int massChoice; //выбор пользователем типа массива
    cout << "----------------МЕНЮ----------------" << endl;
    cout << "Выберите тип массива: " << endl;
    cout << "1. Отсортированный массив" << endl;
    cout << "2. Частично отсортированный массив" << endl;
    cout << "3. Обратно отсортированный массив" << endl;
    cout << "4. Неотсортированный массив" << endl;
    cout << "------------------------------------" << endl;
    cout << "Ваш выбор: ";
    cin >> massChoice;
    cout << "------------------------------------" << endl;
    switch (massChoice) {
    case 1:
        arrSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        break;
    case 2:
        arrPartSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        break;
    case 3:
        arrBackSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        break;
    case 4:
        arrRandomSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        break;
    default:
        cout << "Выберите пункт из списка!" << endl;
        break;
    }
    cout << "------------------------------------" << endl;
    int usersChoice;
    cout << "Выберите способ сортировки: " << endl;
    cout << "1. Сортировка выбором" << endl;
    cout << "2. Сортировка вставками" << endl;
    cout << "3. Сортировка пузырьком" << endl;
    cout << "4. Сортировка слиянием" << endl;
    cout << "5. Быстрая сортировка" << endl;
    cout << "6. Сортировка Шелла с шагом деления на 2" << endl;
    cout << "7. Сортировка Шелла с последовательностью Марцина Цуира" << endl;
    cout << "8. Сортировка Шелла с шагом 2^i - 1" << endl;
    cout << "9. Пирамидальная сортировка" << endl;
    cout << "10. TimSort" << endl;
    cout << "11. IntroSort" << endl;
    cout << "------------------------------------" << endl;
    cout << "Ваш выбор: ";
    cin >> usersChoice;
    cout << "------------------------------------" << endl;
    switch (usersChoice) { //выбор пользователем типа сортировки
    case 1:
        SelectionSort(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 2:
        InsertSort(arr, 0, LEN - 1);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 3:
        BubbleSort(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 4:
        MergeSort(arr, 0, LEN - 1);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 5:
        QuickSort(arr, 0, LEN - 1);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 6:
        ShellSortDel(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 7:
        ShellSortTsuira(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 8:
        ShellSortPow(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 9:
        HeapSort(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 10:
        TimSort(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 11:
        IntroSort(arr, LEN);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    }
    return 0;
}
