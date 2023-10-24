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
#define ARR_SIZE 100
#define minrun 32
using namespace std;
using namespace chrono;

void printArray(int arr[]) { //вывод массива
    for (int i = 0; i < ARR_SIZE; i++) {
        cout << arr[i] << " ";
    } 
    cout << endl;
}

void arrSorted(int arr[]) {//заполнение отсортированного массива
    int k = 0;
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = k;
        k += 1;
    }
}

void arrPartSorted(int arr[]) {//заполнение частично отсортированного массива
    int k = 99;
    for (int i = 0; i < ARR_SIZE / 2; i++) {
        arr[i] = i;
    }
    for (int i = 50; i < ARR_SIZE; i++) {
        arr[i] = k;
        k -= 1;
    }
}

void arrBackSorted(int arr[]) {//заполнение обратно отсортированного массива
    int k = ARR_SIZE;
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = k;
        k -= 1;
    }
}

void arrRandomSorted(int arr[]) {
    for (int i = 0; i < ARR_SIZE; i++)
        arr[i] = rand() % (ARR_SIZE);
}

void SelectionSort(int arr[], int arr_len) { //сортировка выбором
    int count, key;//count - сохраняет значение элемента, key - номер максимального элемента 
    for (int i = 0; i < arr_len - 1; i++)
    {
        count = arr[i]; 
        key = i;
        for (int j = i + 1; j < arr_len; j++)
            if (arr[j] < arr[key]) key = j;
        if (key != i)
        {
            arr[i] = arr[key];
            arr[key] = count;
        }
    }
}

void InsertSort(int* array, int start, int end) {
    int minElem;
    int j;
    for (int i = start + 1; i < end + 1; i++) {
        minElem = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > minElem) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = minElem;
    }
}

void BubbleSort(int arr[], int arr_len) { //сортировка пузырьком
    int count = 0; //переменная для временного хранения значения
    for (int i = 0; i < arr_len - 1; i++) {
        for (int j = arr_len - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                count = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = count;
            }
        }
    }
}

void merge(int* leftArray, int* rightArray, int* arr, int size) {
    int leftSize = size / 2;
    int rightSize = size - leftSize;

    int i = 0, l = 0, r = 0;
    while (l < leftSize && r < rightSize) {
        if (leftArray[l] < rightArray[r]) {
            arr[i] = leftArray[l];
            i++;
            l++;
        }
        else {
            arr[i] = rightArray[r];
            i++;
            r++;
        }
    }
    while (l < leftSize) {
        arr[i] = leftArray[l];
        i++;
        l++;
    }
    while (r < rightSize) {
        arr[i] = rightArray[r];
        i++;
        r++;
    }
}

void MergeSort(int* arr, int size) {//сортировка слиянием
    if (size <= 1) return;
    int middle = size / 2;
    int* leftArray = new int[middle];
    int midRight = size - middle;
    int* rightArray = new int[midRight];

    int j = 0;
    int i = 0;
    for (; i < size; i++) {
        if (i < middle) {
            leftArray[i] = arr[i];
        }
        else {
            rightArray[j] = arr[i];
            j++;
        }
    }
    MergeSort(leftArray, i - j);
    MergeSort(rightArray, j);
    merge(leftArray, rightArray, arr, size);
}

void QuickSort(int* arr, int left, int right)
{
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = arr[left];
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

void ShellSort(int arr[], int arr_len){//сортировка Шелла
    for (int gap = arr_len / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < arr_len; i += 1)
        {
            //сортировка подсписков, созданных с помощью gap 
            int key = arr[i];

            int j;
            for (j = i; j >= gap && arr[j - gap] > key; j -= gap)
                arr[j] = arr[j - gap];

            arr[j] = key;
        }
    }
}

void heapify(int arr[], int n, int root){// функция heapify для нагромождения дерева

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

void HeapSort(int arr[], int arr_len){//сортировка пирамидальная
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

int minRun(int arr_len) {
    int r = 0;
    while (arr_len >= minrun) {
        r |= arr_len & 1;
        arr_len >>= 1;
    }
    return arr_len + r;
}

void timMerge(int *arr, int start, int end, int mid) {
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

void TimSort(int *arr, int length) {//функция тимсорт

    int run = minRun(length);

    for (int left = 0; left < length; left += run) {
        int right = min(left + run - 1, length - 1);
        InsertSort(arr, left, right);
    }
    int size = run;
    while (size < length) {
        for (int left = 0; left < length; left += 2 * size) {
            int middle = min(length - 1, left + size - 1);
            int right = min(left + 2 * size - 1, length - 1);
            if (middle < right) {
                timMerge(arr, left, right, middle);
            }
        }
        size *= 2;
    }
}

int partition(int* arr, int start, int end) {
    int pivot = arr[end];

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

void IntroSort(int* array) {//функция интросорт
    int partSize = partition(array, 0, ARR_SIZE - 1);
    if (partSize < 16) {
        InsertSort(array, 0, ARR_SIZE - 1);
    }
    else if (partSize > (2 * log(ARR_SIZE))) {
        HeapSort(array, ARR_SIZE);
    }
    else {
        QuickSort(array, 0, ARR_SIZE - 1);
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
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
    cout << "6. Сортировка Шелла" << endl;
    cout << "7. Пирамидальная сортировка" << endl;
    cout << "8. TimSort" << endl;
    cout << "9. IntroSort" << endl;
    cout << "------------------------------------" << endl;
    cout << "Ваш выбор: ";
    cin >> usersChoice;
    cout << "------------------------------------" << endl;
    switch (usersChoice) { //выбор пользователем типа сортировки
    case 1:
        SelectionSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 2:
        InsertSort(arr, 0, ARR_SIZE - 1);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 3:
        BubbleSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 4:
        MergeSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 5:
        QuickSort(arr, 0, ARR_SIZE - 1);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 6:
        ShellSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 7:
        HeapSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 8:
        TimSort(arr, ARR_SIZE);
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    case 9:
        auto t1 = high_resolution_clock::now();
        IntroSort(arr);
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<microseconds>(t2 - t1);
        cout << "Время сортировки в микросекундах: " << ms_int.count() << endl;
        cout << "Массив после сортировки: " << endl;
        printArray(arr);
        break;
    }
    return 0;
    }
