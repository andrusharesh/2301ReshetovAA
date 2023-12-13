#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> 
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <chrono>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#define LEN 300

sf::Color OY_COLOR(0, 0, 0);//black
sf::Color QUAD_COLOR(139, 0, 0);//dark red
sf::Color QUICK_COLOR(0, 100, 0);//dark green
sf::Color SHELL_COLOR(25, 25, 112);//midnight blue
sf::Color STD_COLOR(184, 134, 11); //golden rod
sf::Color BLOCK_COLOR(169, 169, 169);//dark gray
sf::Color BACK_COLOR(220, 220, 220);//gainsboro
sf::Color VIS_COLOR(255, 140, 0);//dark orange

const int WIDTH = 1125;
const int ZERO = 60;
const int NUM_SIZE = 20;
const int THICKNESS = 3;
const int RADIUS = 5;

bool sorted = false;

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
    int k = 100;
    for (int i = 0; i < LEN; i++) {
        arr[i] = k;
        k +=10;
    }
}

void arrPartSorted(int arr[]) {//заполнение частично отсортированного массива
    for (int i = 0; i < LEN / 2; i++) {
        arr[i] = LEN/2 + i*60;
    }
    for (int i = 50; i < LEN; i++) {
        arr[i] = rand() % (LEN * 10);

    }
}

void arrBackSorted(int arr[]) {//заполнение обратно отсортированного массива
    int k = LEN * 11;
    for (int i = 0; i < LEN; i++) {
        arr[i] = k;
        k -= 10;
    }
}

void arrRandomSorted(int arr[]) {
    for (int i = 0; i < LEN; i++)
        arr[i] = rand() % (LEN * 10);
}

void QuickSort(int arr[], int left, int right)//быстрая сортировка, второй вариант, который не выдает ошибку при больших массивов
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
    if (left < l) QuickSort(arr, left, l);
    if (f < right) QuickSort(arr, f, right);
}

void ShellSort(int arr[], int arr_len) {//сортировка Шелла с шагом деления на 2
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

typedef int intfunc(const void* a, const void* b);

// Функция quad_swap производит "квадратный обмен" элементов массива с использованием заданной функции сравнения
template <intfunc difference>
void quad_swap(void* arr, void* swap, int arr_len)
{
    // Переменная для отслеживания текущей позиции в массиве
    int offset;

    // Приведение указателей на массивы к типу int для удобства работы с элементами
    int* array = (int*)arr;
    int* memory = (int*)swap;

    // Цикл обработки блоков по 4 элемента
    for (offset = 0; offset + 4 <= arr_len; offset += 4)
    {
        // Квадратные сравнения и обмен элементов в блоках
        // Сравнение и обмен первых двух элементов блока
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[1];
            memory[1] = array[0];
        }
        else
        {
            memory[0] = array[0];
            memory[1] = array[1];
        }

        // Сравнение и обмен следующих двух элементов блока
        if (difference(&array[2], &array[3]) > 0)
        {
            memory[2] = array[3];
            memory[3] = array[2];
        }
        else
        {
            memory[2] = array[2];
            memory[3] = array[3];
        }

        // Сравнение и обмен элементов внутри отсортированного блока
        if (difference(&memory[1], &memory[2]) <= 0)
        {
            *array++ = memory[0];
            *array++ = memory[1];
            *array++ = memory[2];
            *array++ = memory[3];
        }
        else if (difference(&memory[0], &memory[3]) > 0)
        {
            *array++ = memory[2];
            *array++ = memory[3];
            *array++ = memory[0];
            *array++ = memory[1];
        }
        else if (difference(&memory[0], &memory[2]) > 0)
        {
            *array++ = memory[2];
            *array++ = memory[0];

            // Внутренний блок сравнения и обмена
            if (difference(&memory[1], &memory[3]) > 0)
            {
                *array++ = memory[3];
                *array++ = memory[1];
            }
            else
            {
                *array++ = memory[1];
                *array++ = memory[3];
            }
        }
        else
        {
            *array++ = memory[0];
            *array++ = memory[2];

            // Внутренний блок сравнения и обмена
            if (difference(&memory[1], &memory[3]) > 0)
            {
                *array++ = memory[3];
                *array++ = memory[1];
            }
            else
            {
                *array++ = memory[1];
                *array++ = memory[3];
            }
        }
    }

    // Обработка оставшихся элементов (менее чем 4) в массиве
    switch (arr_len - offset)
    {
    case 0:
    case 1:
        // Если остался 1 или 0 элементов, завершаем функцию
        return;
    case 2:
        // Если осталось 2 элемента, сравниваем их и при необходимости меняем местами
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        return;
    case 3:
        // Если осталось 3 элемента, сравниваем и обмениваем, поддерживая порядок
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        if (difference(&array[1], &array[2]) > 0)
        {
            memory[0] = array[1];
            array[1] = array[2];
            array[2] = memory[0];
        }
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        return;
    default:
        // Если arr_len не соответствует ожидаемым значениям, вызывается утверждение (assert)
        assert(arr_len < 1 && arr_len > 3);
    }
}

// Функция quad_sort выполняет "квадратное слияние" массива с использованием заданной функции сравнения
template <intfunc difference>
void quad_merge(void* array, void* swap, int arr_len)
{
    // Переменные для отслеживания текущей позиции в массиве, размера блока и временных массивов
    int offset, block = 4;
    int* current_array, * current_swap, * first_block, * second_block, * merged, * merged_end, * array_end;

    // Инициализация указателя на конец массива
    array_end = (int*)array;
    array_end += arr_len;

    // Цикл увеличения размера блока для слияния
    while (block < arr_len)
    {
        offset = 0;

        // Цикл слияния блоков
        while (offset + block < arr_len)
        {
            // Приведение указателя к текущему массиву и установка начальной позиции
            current_array = (decltype(current_array))array;
            current_array += offset;

            // Установка конечной позиции для слияния блока
            merged_end = current_array + block;

            if (difference(merged_end - 1, merged_end) <= 0)
            {
                // Проверка для оптимизации слияния блоков
                if (offset + block * 3 < arr_len)
                {
                    merged_end = current_array + block * 3;

                    if (difference(merged_end - 1, merged_end) <= 0)
                    {
                        merged_end = current_array + block * 2;

                        if (difference(merged_end - 1, merged_end) <= 0)
                        {
                            // Пропуск слияния блока, если блок уже отсортирован
                            offset += block * 4;
                            continue;
                        }

                        // Подготовка к слиянию блоков требуемого размера
                        current_swap = (decltype(current_swap))swap;

                        first_block = current_array;
                        second_block = current_array + block * 2;
                        merged = second_block;
                        merged_end = offset + block * 4 <= arr_len ? merged + block * 2 : array_end;

                        // Копирование элементов во временный массив
                        while (first_block < second_block)
                            *current_swap++ = *first_block++;

                        while (merged < merged_end)
                            *current_swap++ = *merged++;

                        // Переход к следующему этапу слияния
                        goto step3;
                    }

                    current_swap = (int*)swap;

                    first_block = current_array;
                    second_block = current_array + block * 2;

                    // Копирование элементов во временный массив
                    while (first_block < second_block)
                        *current_swap++ = *first_block++;

                    // Переход к следующему этапу слияния
                    goto step2;
                }
                else if (offset + block * 2 < arr_len)
                {
                    // Пропуск слияния блока, если блок уже отсортирован
                    offset += block * 4;
                    continue;
                }
                else
                {
                    // Пропуск слияния блока, если блок уже отсортирован
                    offset += block * 4;
                    continue;
                }
            }

        //step1:

            // Подготовка к слиянию блоков
            current_swap = (int*)swap;

            first_block = current_array;
            second_block = current_array + block;

            merged = second_block;
            merged_end = offset + block * 2 <= arr_len ? merged + block : array_end;


            if (difference(second_block - 1, merged_end - 1) <= 0)
            {
                while (first_block < second_block)
                {
                    while (difference(first_block, merged) > 0)
                    {
                        *current_swap++ = *merged++;
                    }
                    *current_swap++ = *first_block++;
                }
                while (merged < merged_end)
                    *current_swap++ = *merged++;
            }
            else if (difference(first_block, merged_end - 1) > 0)
            {

                while (merged < merged_end)
                    *current_swap++ = *merged++;

                while (first_block < second_block)
                    *current_swap++ = *first_block++;
            }
            else
            {
                while (merged < merged_end)
                {
                    while (difference(first_block, merged) <= 0)
                    {
                        *current_swap++ = *first_block++;
                    }
                    *current_swap++ = *merged++;
                }

                while (first_block < second_block)
                {
                    *current_swap++ = *first_block++;
                }
            }

        // Этап 2 слияния:
        step2:

            // Проверка условия для слияния второго блока
            if (offset + block * 2 < arr_len)
            {
                first_block = current_array + block * 2;

                if (offset + block * 3 < arr_len)
                {
                    second_block = first_block + block;
                    merged = second_block;
                    merged_end = offset + block * 4 <= arr_len ? merged + block : array_end;

                    if (difference(second_block - 1, merged_end - 1) <= 0)
                    {

                        while (first_block < second_block)
                        {
                            while (difference(first_block, merged) > 0)
                            {
                                *current_swap++ = *merged++;
                            }
                            *current_swap++ = *first_block++;
                        }
                        while (merged < merged_end)
                            *current_swap++ = *merged++;
                    }
                    else if (difference(first_block, merged_end - 1) > 0)
                    {
                        while (merged < merged_end)
                            *current_swap++ = *merged++;
                        while (first_block < second_block)
                            *current_swap++ = *first_block++;
                    }
                    else
                    {
                        while (merged < merged_end)
                        {
                            while (difference(first_block, merged) <= 0)
                            {
                                *current_swap++ = *first_block++;
                            }
                            *current_swap++ = *merged++;
                        }
                        while (first_block < second_block)
                            *current_swap++ = *first_block++;
                    }
                }
                else
                {
                    while (first_block < array_end)
                        *current_swap++ = *first_block++;
                }
            }

        // Этап 3 слияния:
        step3:

            // Подготовка к слиянию блоков
            current_swap = (int*)swap;

            first_block = current_swap;

            // Проверка условия для слияния третьего блока
            if (offset + block * 2 < arr_len)
            {
                second_block = first_block + block * 2;

                merged = second_block;
                merged_end = offset + block * 4 <= arr_len ? merged + block * 2 : current_swap + arr_len - offset;

                if (difference(second_block - 1, merged_end - 1) <= 0)
                {
                    while (first_block < second_block)
                    {
                        while (difference(first_block, merged) > 0)
                        {
                            *current_array++ = *merged++;
                        }
                        *current_array++ = *first_block++;
                    }
                    while (merged < merged_end)
                        *current_array++ = *merged++;
                }
                else if (difference(first_block, merged_end - 1) > 0)
                {
                    while (merged < merged_end)
                        *current_array++ = *merged++;
                    while (first_block < second_block)
                        *current_array++ = *first_block++;
                }
                else
                {
                    while (merged < merged_end)
                    {
                        while (difference(merged, first_block) > 0)
                        {
                            *current_array++ = *first_block++;
                        }
                        *current_array++ = *merged++;
                    }
                    while (first_block < second_block)
                        *current_array++ = *first_block++;
                }
            }
            else
            {
                merged_end = current_swap + arr_len - offset;

                while (first_block < merged_end)
                    *current_array++ = *first_block++;
            }
            // Увеличение смещения для следующего блока
            offset += block * 4;
        }
        // Увеличение размера блока для следующего этапа сортировки
        block *= 4;
    }
}

template <intfunc difference>
void QuadSort(void* array, int arr_len)
{
    void* swap = malloc(arr_len * sizeof(int));
    // Выделение динамической памяти для временного массива swap

    quad_swap<difference>(array, swap, arr_len);
    quad_merge<difference>(array, swap, arr_len);

    free(swap);
    // Освобождение выделенной ранее динамической памяти
}

template <intfunc difference>
void visual(sf::RenderWindow& window, int* array, int arr_len)
{
    sf::Font font;//загрузка шрифта
    if (!font.loadFromFile("PakenhamBl Italic.ttf"))
    {
        // Обработка ошибки загрузки шрифта
        cout << endl << "Файл шрифта не загружен. Проверьте соответвие названия файла или его налиичие в файлах проекта." << endl;
    }

    window.clear(BACK_COLOR);

    sf::RectangleShape title_block(sf::Vector2f(1140, 200));
    title_block.setPosition(630, 50);
    title_block.setFillColor(BLOCK_COLOR);
    title_block.setOutlineThickness(3.f);
    title_block.setOutlineColor(sf::Color::Black);
    window.draw(title_block);

    sf::Text title_text("QUADSORT VISUALISATION", font, 55);
    title_text.setPosition(900, 80);
    title_text.setFillColor(OY_COLOR);
    title_text.setOutlineThickness(2.f);
    title_text.setOutlineColor(BACK_COLOR);
    window.draw(title_text);

    sf::Text is_sorted_text(!sorted ? "ARRAY IS SORTING" : "ARRAY IS SORTED", font, 50);
    is_sorted_text.setPosition(1010, 150);
    is_sorted_text.setFillColor(!sorted ? QUAD_COLOR : QUICK_COLOR);
    is_sorted_text.setOutlineThickness(2.f);
    is_sorted_text.setOutlineColor(BACK_COLOR);
    window.draw(is_sorted_text);

    // Отрисовка прямоугольников для каждого элемента массива
    for (int i = 0; i < arr_len; ++i)
    {
        sf::RectangleShape rect(sf::Vector2f(5, array[i] / 3));
        rect.setPosition(i * 8, window.getSize().y - rect.getSize().y);
        rect.setFillColor(!sorted ? VIS_COLOR : QUICK_COLOR);
        rect.setOutlineColor(OY_COLOR);
        rect.setOutlineThickness(2.f);

        window.draw(rect);
    }

    window.display();
}

template <intfunc difference>
void quad_swap_visial(void* arr, void* swap, int arr_len, sf::RenderWindow& window)
{
    int offset;

    int* array = (int*)arr;
    int* memory = (int*)swap;

    for (offset = 0; offset + 4 <= arr_len; offset += 4)
    {
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[1];
            memory[1] = array[0];
        }
        else
        {
            memory[0] = array[0];
            memory[1] = array[1];
        }

        if (difference(&array[2], &array[3]) > 0)
        {
            memory[2] = array[3];
            memory[3] = array[2];
        }
        else
        {
            memory[2] = array[2];
            memory[3] = array[3];
        }

        if (difference(&memory[1], &memory[2]) <= 0)
        {
            *array++ = memory[0];
            *array++ = memory[1];
            *array++ = memory[2];
            *array++ = memory[3];
        }
        else if (difference(&memory[0], &memory[3]) > 0)
        {
            *array++ = memory[2];
            *array++ = memory[3];
            *array++ = memory[0];
            *array++ = memory[1];
        }
        else if (difference(&memory[0], &memory[2]) > 0)
        {
            *array++ = memory[2];
            *array++ = memory[0];

            if (difference(&memory[1], &memory[3]) > 0)
            {
                *array++ = memory[3];
                *array++ = memory[1];
            }
            else
            {
                *array++ = memory[1];
                *array++ = memory[3];
            }
        }
        else
        {
            *array++ = memory[0];
            *array++ = memory[2];

            if (difference(&memory[1], &memory[3]) > 0)
            {
                *array++ = memory[3];
                *array++ = memory[1];
            }
            else
            {
                *array++ = memory[1];
                *array++ = memory[3];
            }
        }

        // Визуализация на каждой итерации
        visual<difference>(window, (int*)arr, arr_len);
        sf::sleep(sf::milliseconds(50));  // Задержка для визуализации
    }

    switch (arr_len - offset)
    {
    case 0:
    case 1:
        return;
    case 2:
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        return;
    case 3:
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        if (difference(&array[1], &array[2]) > 0)
        {
            memory[0] = array[1];
            array[1] = array[2];
            array[2] = memory[0];
        }
        if (difference(&array[0], &array[1]) > 0)
        {
            memory[0] = array[0];
            array[0] = array[1];
            array[1] = memory[0];
        }
        return;
    default:
        assert(arr_len < 1 && arr_len > 3);
    }
}

template <intfunc difference>
void quad_merge_visual(void* array, void* swap, int arr_len, sf::RenderWindow& window)
{
    int offset, block = 4;
    int* current_array, * current_swap, * first_block, * second_block, * merged, * merged_end, * array_end;

    array_end = (int*)array;
    array_end += arr_len;

    while (block < arr_len)
    {
        offset = 0;

        while (offset + block < arr_len)
        {
            current_array = (decltype(current_array))array;
            current_array += offset;

            merged_end = current_array + block;

            if (difference(merged_end - 1, merged_end) <= 0)
            {
                if (offset + block * 3 < arr_len)
                {
                    merged_end = current_array + block * 3;

                    if (difference(merged_end - 1, merged_end) <= 0)
                    {
                        merged_end = current_array + block * 2;

                        if (difference(merged_end - 1, merged_end) <= 0)
                        {
                            offset += block * 4;
                            continue;
                        }
                        current_swap = (decltype(current_swap))swap;

                        first_block = current_array;
                        second_block = current_array + block * 2;
                        merged = second_block;
                        merged_end = offset + block * 4 <= arr_len ? merged + block * 2 : array_end;

                        while (first_block < second_block)
                            *current_swap++ = *first_block++;

                        while (merged < merged_end)
                            *current_swap++ = *merged++;

                        goto step3;
                    }
                    current_swap = (int*)swap;

                    first_block = current_array;
                    second_block = current_array + block * 2;

                    while (first_block < second_block)
                        *current_swap++ = *first_block++;

                    goto step2;
                }
                else if (offset + block * 2 < arr_len)
                {
                    merged_end = current_array + block * 2;

                    if (difference(merged_end - 1, merged_end) <= 0)
                    {
                        offset += block * 4;
                        continue;
                    }
                    current_swap = (int*)swap;

                    first_block = current_array;
                    second_block = current_array + block * 2;

                    while (first_block < second_block)
                        *current_swap++ = *first_block++;

                    goto step2;
                }
                else
                {
                    offset += block * 4;
                    continue;
                }
            }

            // step1:

            current_swap = (int*)swap;

            first_block = current_array;
            second_block = current_array + block;

            merged = second_block;
            merged_end = offset + block * 2 <= arr_len ? merged + block : array_end;

            if (difference(second_block - 1, merged_end - 1) <= 0)
            {
                while (first_block < second_block)
                {
                    while (difference(first_block, merged) > 0)
                    {
                        *current_swap++ = *merged++;
                    }
                    *current_swap++ = *first_block++;
                }
                while (merged < merged_end)
                    *current_swap++ = *merged++;
            }
            else if (difference(first_block, merged_end - 1) > 0)
            {
                while (merged < merged_end)
                    *current_swap++ = *merged++;

                while (first_block < second_block)
                    *current_swap++ = *first_block++;
            }
            else
            {
                while (merged < merged_end)
                {
                    while (difference(first_block, merged) <= 0)
                    {
                        *current_swap++ = *first_block++;
                    }
                    *current_swap++ = *merged++;
                }

                while (first_block < second_block)
                {
                    *current_swap++ = *first_block++;
                }
            }

        step2:

            if (offset + block * 2 < arr_len)
            {
                first_block = current_array + block * 2;

                if (offset + block * 3 < arr_len)
                {
                    second_block = first_block + block;
                    merged = second_block;
                    merged_end = offset + block * 4 <= arr_len ? merged + block : array_end;

                    if (difference(second_block - 1, merged_end - 1) <= 0)
                    {
                        while (first_block < second_block)
                        {
                            while (difference(first_block, merged) > 0)
                            {
                                *current_swap++ = *merged++;
                            }
                            *current_swap++ = *first_block++;
                        }
                        while (merged < merged_end)
                            *current_swap++ = *merged++;
                    }
                    else if (difference(first_block, merged_end - 1) > 0)
                    {
                        while (merged < merged_end)
                            *current_swap++ = *merged++;
                        while (first_block < second_block)
                            *current_swap++ = *first_block++;
                    }
                    else
                    {
                        while (merged < merged_end)
                        {
                            while (difference(first_block, merged) <= 0)
                            {
                                *current_swap++ = *first_block++;
                            }
                            *current_swap++ = *merged++;
                        }
                        while (first_block < second_block)
                            *current_swap++ = *first_block++;
                    }
                }
                else
                {
                    while (first_block < array_end)
                        *current_swap++ = *first_block++;
                }
            }

        step3:

            current_swap = (int*)swap;

            first_block = current_swap;

            if (offset + block * 2 < arr_len)
            {
                second_block = first_block + block * 2;

                merged = second_block;
                merged_end = offset + block * 4 <= arr_len ? merged + block * 2 : current_swap + arr_len - offset;

                if (difference(second_block - 1, merged_end - 1) <= 0)
                {
                    while (first_block < second_block)
                    {
                        while (difference(first_block, merged) > 0)
                        {
                            *current_array++ = *merged++;
                        }
                        *current_array++ = *first_block++;
                    }
                    while (merged < merged_end)
                        *current_array++ = *merged++;
                }
                else if (difference(first_block, merged_end - 1) > 0)
                {
                    while (merged < merged_end)
                        *current_array++ = *merged++;
                    while (first_block < second_block)
                        *current_array++ = *first_block++;
                }
                else
                {
                    while (merged < merged_end)
                    {
                        while (difference(merged, first_block) > 0)
                        {
                            *current_array++ = *first_block++;
                        }
                        *current_array++ = *merged++;
                    }
                    while (first_block < second_block)
                        *current_array++ = *first_block++;
                }
            }
            else
            {
                merged_end = current_swap + arr_len - offset;

                while (first_block < merged_end)
                    *current_array++ = *first_block++;
            }
            offset += block * 4;

            // Визуализация на каждой итерации
            visual<difference>(window, (int*)array, arr_len);
            sf::sleep(sf::milliseconds(300));  // Задержка для визуализации
        }
        block *= 4;
        if (block >= arr_len) {
            sorted = true;
            visual<difference>(window, (int*)array, arr_len);

        }
    }
} 

template <intfunc difference>
void QuadSortVisualisation(void* array, int arr_len)
{
    sf::RenderWindow window(sf::VideoMode(arr_len * 8, 1400), "QuadSort Visualization", sf::Style::None);

    //Начальное положение окна относительно экрана
    window.setPosition(sf::Vector2i(200, 100));

    void* swap = malloc(arr_len * sizeof(int));

    // Визуализация начального состояния массива
    visual<difference>(window, (int*)array, arr_len);

    quad_swap_visial<difference>(array, swap, arr_len, window);
    quad_merge_visual<difference>(array, swap, arr_len, window);

    free(swap);

    // Задержка после завершения сортировки
    sf::sleep(sf::seconds(3));
}

// Статическая инлайн-функция diff_func для сравнения элементов массива
static inline int diff_func(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int height_grafik_window(int a, int b, int c) {
    int max_num = std::max({ a, b, c });
    if (max_num <= 500) {
        return 600;
    }
    else {
        return ((max_num + 999) / 1000) * 1000;
    }
}

void grafik(sf::RenderWindow& window, int yCoords1[], int yCoords2[], int yCoords3[], int yCoords4[], int HEIGHT) {
    // Массив с координатами x  для каждого графика
    int xCoords[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

    //Начальное положение окна относительно экрана
    window.setPosition(sf::Vector2i(1600, 50));

    // Создаем вершинные массивы для осей x и y
    sf::VertexArray xAxis(sf::Lines, 2), yAxis(sf::Lines, 2);

    // Устанавливаем позиции для осей
    xAxis[0].position = sf::Vector2f(0, HEIGHT - ZERO);
    xAxis[1].position = sf::Vector2f(WIDTH, HEIGHT - ZERO);
    yAxis[0].position = sf::Vector2f(ZERO, 0);
    yAxis[1].position = sf::Vector2f(ZERO, HEIGHT);

    // Устанавливаем цвет для осей
    xAxis[0].color = yAxis[0].color = OY_COLOR;
    xAxis[1].color = yAxis[1].color = OY_COLOR;

    std::vector<sf::Vector2f> points1, points2, points3, points4;

    // Заполняем векторы точек для каждого графика
    for (int i = 0; i < 10; i++)
    {
        points1.push_back(sf::Vector2f(xCoords[i] + ZERO, HEIGHT - (yCoords1[i]) - ZERO));
        points2.push_back(sf::Vector2f(xCoords[i] + ZERO, HEIGHT - (yCoords2[i]) - ZERO));
        points3.push_back(sf::Vector2f(xCoords[i] + ZERO, HEIGHT - (yCoords3[i]) - ZERO));
        points4.push_back(sf::Vector2f(xCoords[i] + ZERO, HEIGHT - (yCoords4[i]) - ZERO));
    }

    // Линии, соединаяющие точки на графике
    sf::VertexArray lines1(sf::Quads, points1.size() * 4);
    sf::VertexArray lines2(sf::Quads, points2.size() * 4);
    sf::VertexArray lines3(sf::Quads, points3.size() * 4);
    sf::VertexArray lines4(sf::Quads, points4.size() * 4);

    // Так как в сфмл есть некоторые сложности с типами линий, из-за которых нельзя менять толщину линии,
    //  мы представляем каждую линию как "прямоугольник", параметры которого мы уже модем менять
    for (std::size_t i = 0; i < points1.size() - 1; ++i)
    {
        sf::Vector2f direction = points1[i + 1] - points1[i];
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (THICKNESS / 2.f) * unitPerpendicular;

        lines1[i * 4].position = points1[i] + offset;
        lines1[i * 4 + 1].position = points1[i + 1] + offset;
        lines1[i * 4 + 2].position = points1[i + 1] - offset;
        lines1[i * 4 + 3].position = points1[i] - offset;

        lines1[i * 4].color = QUAD_COLOR;
        lines1[i * 4 + 1].color = QUAD_COLOR;
        lines1[i * 4 + 2].color = QUAD_COLOR;
        lines1[i * 4 + 3].color = QUAD_COLOR;
    }

    for (std::size_t i = 0; i < points2.size() - 1; ++i)
    {
        sf::Vector2f direction = points2[i + 1] - points2[i];
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (THICKNESS / 2.f) * unitPerpendicular;

        lines2[i * 4].position = points2[i] + offset;
        lines2[i * 4 + 1].position = points2[i + 1] + offset;
        lines2[i * 4 + 2].position = points2[i + 1] - offset;
        lines2[i * 4 + 3].position = points2[i] - offset;

        lines2[i * 4].color = QUICK_COLOR;
        lines2[i * 4 + 1].color = QUICK_COLOR;
        lines2[i * 4 + 2].color = QUICK_COLOR;
        lines2[i * 4 + 3].color = QUICK_COLOR;
    }

    for (std::size_t i = 0; i < points3.size() - 1; ++i)
    {
        sf::Vector2f direction = points3[i + 1] - points3[i];
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (THICKNESS / 2.f) * unitPerpendicular;

        lines3[i * 4].position = points3[i] + offset;
        lines3[i * 4 + 1].position = points3[i + 1] + offset;
        lines3[i * 4 + 2].position = points3[i + 1] - offset;
        lines3[i * 4 + 3].position = points3[i] - offset;

        lines3[i * 4].color = SHELL_COLOR;
        lines3[i * 4 + 1].color = SHELL_COLOR;
        lines3[i * 4 + 2].color = SHELL_COLOR;
        lines3[i * 4 + 3].color = SHELL_COLOR;
    }

    for (std::size_t i = 0; i < points4.size() - 1; ++i)
    {
        sf::Vector2f direction = points4[i + 1] - points3[i];
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (THICKNESS / 2.f) * unitPerpendicular;

        lines4[i * 4].position = points4[i] + offset;
        lines4[i * 4 + 1].position = points4[i + 1] + offset;
        lines4[i * 4 + 2].position = points4[i + 1] - offset;
        lines4[i * 4 + 3].position = points4[i] - offset;

        lines4[i * 4].color = STD_COLOR;
        lines4[i * 4 + 1].color = STD_COLOR;
        lines4[i * 4 + 2].color = STD_COLOR;
        lines4[i * 4 + 3].color = STD_COLOR;
    }


    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("PakenhamBl Italic.ttf"))
    {
        // Обработка ошибки загрузки шрифта
        cout << endl << "Файл шрифта не загружен. Проверьте соответвие названия файла или его налиичие в файлах проекта." << endl;
    }

    // Создаем текст для осей
    sf::Text xAxisText("X", font, NUM_SIZE);
    sf::Text yAxisText("Y", font, NUM_SIZE);
    sf::Text zeroText("0", font, NUM_SIZE);

    // Устанавливаем позиции для текста
    xAxisText.setPosition(WIDTH - 30, HEIGHT - ZERO - 30);
    yAxisText.setPosition(ZERO + 10, 10);
    zeroText.setPosition(ZERO - 15, HEIGHT - ZERO);

    xAxisText.setStyle(sf::Text::Bold);
    yAxisText.setStyle(sf::Text::Bold);

    // Устанавливаем цвет для текста
    xAxisText.setFillColor(OY_COLOR);
    yAxisText.setFillColor(OY_COLOR);
    zeroText.setFillColor(OY_COLOR);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(BACK_COLOR);

        // Рисуем оси
        window.draw(xAxis);
        window.draw(yAxis);

        // Рисуем палочки на оси OX
        for (int i = ZERO; i <= WIDTH; i += 100) {
            sf::VertexArray tick(sf::Lines, 2);
            tick[0].position = sf::Vector2f(i, HEIGHT - ZERO - 5);
            tick[1].position = sf::Vector2f(i, HEIGHT - ZERO + 5);
            tick[0].color = OY_COLOR;
            tick[1].color = OY_COLOR;
            window.draw(tick);
        }

        // Рисуем палочки на оси OY
        for (int i = ZERO; i < HEIGHT; i += 50) {
            sf::VertexArray tick(sf::Lines, 2);
            tick[0].position = sf::Vector2f(ZERO - 5, HEIGHT - i);
            tick[1].position = sf::Vector2f(ZERO + 5, HEIGHT - i);
            tick[0].color = OY_COLOR;
            tick[1].color = OY_COLOR;
            window.draw(tick);
        }

        // Создаем текст для подписей на оси OX
        std::vector<sf::Text> xLabels;
        int pix_x = 100;
        for (int i = ZERO; i < WIDTH; i += pix_x) {
            sf::Text label(std::to_string((i - ZERO) * 100), font, NUM_SIZE);
            label.setPosition(i - 28, HEIGHT - ZERO + 10);
            label.setFillColor(OY_COLOR);
            if (i != ZERO) {
                xLabels.push_back(label);
            }
        }

        // Рисуем подписи на оси OX
        for (const auto& label : xLabels) {
            window.draw(label);
        }

        // Создаем текст для подписей на оси OY
        std::vector<sf::Text> yLabels;
        int pix_y = 50;
        for (int i = HEIGHT - pix_y; i > ZERO; i -= pix_y) {
            sf::Text label(std::to_string((i - pix_y) * 10), font, NUM_SIZE);
            label.setPosition(ZERO - pix_y - 8, HEIGHT - i - 20);
            label.setFillColor(OY_COLOR);
            yLabels.push_back(label);
        }

        // Рисуем подписи на оси OY
        for (const auto& label : yLabels) {
            window.draw(label);
        }

        // Рисуем текст для осей
        window.draw(xAxisText);
        window.draw(yAxisText);
        window.draw(zeroText);

        // Рисуем графики
        window.draw(lines1);
        window.draw(lines2);
        window.draw(lines3);
        window.draw(lines4);

        // Создаем стрелочки на концах осей
        sf::ConvexShape arrowX(3), arrowX2(3), arrowY(3), arrowY2(3);

        // Устанавливаем позиции для стрелочек
        arrowX.setPoint(0, sf::Vector2f(WIDTH, HEIGHT - ZERO));
        arrowX.setPoint(1, sf::Vector2f(WIDTH - 10, HEIGHT - ZERO + 10));
        arrowX.setPoint(2, sf::Vector2f(WIDTH - 10, HEIGHT - ZERO - 10));

        arrowY.setPoint(0, sf::Vector2f(ZERO, 0));
        arrowY.setPoint(1, sf::Vector2f(ZERO + 10, 10));
        arrowY.setPoint(2, sf::Vector2f(ZERO - 10, 10));


        // Устанавливаем цвет для стрелочек
        arrowX.setFillColor(sf::Color::Black);
        arrowY.setFillColor(sf::Color::Black);

        // Рисуем стрелочки
        window.draw(arrowX);
        window.draw(arrowX2);
        window.draw(arrowY);
        window.draw(arrowY2);

        for (const auto& point : points1)
        {
            sf::CircleShape circle(RADIUS / 1.f); // Создаем круг с радиусом r
            circle.setPosition(point.x - RADIUS, point.y - RADIUS); // Смещаем круг на половину его ширины и высоты
            circle.setFillColor(QUAD_COLOR); // Устанавливаем цвет круга
            window.draw(circle); // Рисуем круг
        }

        for (const auto& point : points2)
        {
            sf::CircleShape circle(RADIUS / 1.f); // Создаем круг с радиусом r
            circle.setPosition(point.x - RADIUS, point.y - RADIUS); // Смещаем круг на половину его ширины и высоты
            circle.setFillColor(QUICK_COLOR); // Устанавливаем цвет круга
            window.draw(circle); // Рисуем круг
        }

        for (const auto& point : points3)
        {
            sf::CircleShape circle(RADIUS / 1.f); // Создаем круг с радиусом r
            circle.setPosition(point.x - RADIUS, point.y - RADIUS); // Смещаем круг на половину его ширины и высоты
            circle.setFillColor(SHELL_COLOR); // Устанавливаем цвет круга
            window.draw(circle); // Рисуем круг
        }

        for (const auto& point : points4)
        {
            sf::CircleShape circle(RADIUS / 1.f); // Создаем круг с радиусом r
            circle.setPosition(point.x - RADIUS, point.y - RADIUS); // Смещаем круг на половину его ширины и высоты
            circle.setFillColor(STD_COLOR); // Устанавливаем цвет круга
            window.draw(circle); // Рисуем круг
        }

        // Создаем квадратик
        sf::RectangleShape rectangle(sf::Vector2f(200.f, 150.f));
        rectangle.setFillColor(BLOCK_COLOR);
        rectangle.setPosition((2 * ZERO) / 1.f, (0.5 * ZERO) / 1.f);
        rectangle.setOutlineThickness(3.f);
        rectangle.setOutlineColor(sf::Color::Black);
        window.draw(rectangle);

        // Создаем текст в блоке
        sf::Text block_quad_text("Quadsort", font, 24);
        block_quad_text.setFillColor(OY_COLOR);
        block_quad_text.setPosition((2 * ZERO + 15) / 1.f, (0.5 * ZERO + 10) / 1.f);
        window.draw(block_quad_text);
        // Создаем горизонтальные полоски
        sf::RectangleShape block_quad_line;
        block_quad_line.setSize(sf::Vector2f(50.f, THICKNESS / 1.f));
        block_quad_line.setFillColor(QUAD_COLOR);
        block_quad_line.setPosition((2 * ZERO + 120) / 1.f, (0.5 * ZERO + 26) / 1.f);
        window.draw(block_quad_line);

        // Создаем текст в блоке
        sf::Text block_quick_text("Quicksort", font, 24);
        block_quick_text.setFillColor(OY_COLOR);
        block_quick_text.setPosition((2 * ZERO + 15) / 1.f, (0.5 * ZERO + 20 + 24) / 1.f);
        window.draw(block_quick_text);
        // Создаем горизонтальные полоски
        sf::RectangleShape block_quick_line;
        block_quick_line.setSize(sf::Vector2f(50.f, THICKNESS / 1.f));
        block_quick_line.setFillColor(QUICK_COLOR);
        block_quick_line.setPosition((2 * ZERO + 120) / 1.f, (0.5 * ZERO + 36 + 24) / 1.f);
        window.draw(block_quick_line);

        // Создаем текст в блоке
        sf::Text block_shell_text("Shellsort", font, 24);
        block_shell_text.setFillColor(OY_COLOR);
        block_shell_text.setPosition((2 * ZERO + 15) / 1.f, (0.5 * ZERO + 30 + 48) / 1.f);
        window.draw(block_shell_text);
        // Создаем горизонтальные полоски
        sf::RectangleShape block_shell_line;
        block_shell_line.setSize(sf::Vector2f(50.f, THICKNESS / 1.f));
        block_shell_line.setFillColor(SHELL_COLOR);
        block_shell_line.setPosition((2 * ZERO + 120) / 1.f, (0.5 * ZERO + 46 + 48) / 1.f);
        window.draw(block_shell_line);

        // Создаем текст в блоке
        sf::Text block_std_text(" std::sort", font, 24);
        block_std_text.setFillColor(OY_COLOR);
        block_std_text.setPosition((2 * ZERO + 15) / 1.f, (0.5 * ZERO + 40 + 72) / 1.f);
        window.draw(block_std_text);
        // Создаем горизонтальные полоски
        sf::RectangleShape block_std_line;
        block_std_line.setSize(sf::Vector2f(50.f, THICKNESS / 1.f));
        block_std_line.setFillColor(STD_COLOR);
        block_std_line.setPosition((2 * ZERO + 120) / 1.f, (0.5 * ZERO + 56 + 72) / 1.f);
        window.draw(block_std_line);


        window.display();
    }
}

void diagram(sf::RenderWindow& window, int quad_result_arrSorted, int quick_result_arrSorted, int shell_result_arrSorted, int std_result_arrSorted, int quad_result_arrPartSorted, int quick_result_arrPartSorted, int shell_result_arrPartSorted, int std_result_arrPartSorted, int quad_result_arrBackSorted, int quick_result_arrBackSorted, int shell_result_arrBackSorted, int std_result_arrBackSorted, int quad_result_arrRandomSorted, int quick_result_arrRandomSorted, int shell_result_arrRandomSorted, int std_result_arrRandomSorted)
{
    int barWidth = 50;
    int barSpacing = 50;
    int del = 1;
    int space = 30;

    int bar_max = max({ quad_result_arrSorted, quick_result_arrSorted, shell_result_arrSorted, std_result_arrSorted, quad_result_arrPartSorted, quick_result_arrPartSorted, shell_result_arrPartSorted, std_result_arrPartSorted, quad_result_arrBackSorted, quick_result_arrBackSorted, shell_result_arrBackSorted, std_result_arrBackSorted, quad_result_arrRandomSorted, quick_result_arrRandomSorted, shell_result_arrRandomSorted, std_result_arrRandomSorted }) * 10;
    int bar_max_memory = bar_max;

    while (bar_max >= 1150) {
        bar_max = bar_max_memory;
        del += 1;
        bar_max /= del;
    }

    int quadBarHeight1 = quad_result_arrSorted * 10 / del;
    int quickBarHeight1 = quick_result_arrSorted * 10 / del;
    int shellBarHeight1 = shell_result_arrSorted * 10 / del;
    int stdBarHeight1 = std_result_arrSorted * 10 / del;

    int quadBarHeight2 = quad_result_arrPartSorted * 10 / del;
    int quickBarHeight2 = quick_result_arrPartSorted * 10 / del;
    int shellBarHeight2 = shell_result_arrPartSorted * 10 / del;
    int stdBarHeight2 = std_result_arrPartSorted * 10 / del;

    int quadBarHeight3 = quad_result_arrBackSorted * 10 / del;
    int quickBarHeight3 = quick_result_arrBackSorted * 10 / del;
    int shellBarHeight3 = shell_result_arrBackSorted * 10 / del;
    int stdBarHeight3 = std_result_arrBackSorted * 10 / del;

    int quadBarHeight4 = quad_result_arrRandomSorted * 10 / del;
    int quickBarHeight4 = quick_result_arrRandomSorted * 10 / del;
    int shellBarHeight4 = shell_result_arrRandomSorted * 10 / del;
    int stdBarHeight4 = std_result_arrRandomSorted * 10 / del;

    //Начальное положение окна относительно экрана
    window.setPosition(sf::Vector2i(1000, 50));

    sf::Font font;
    if (!font.loadFromFile("PakenhamBl Italic.ttf"))
    {
        // Обработка ошибки загрузки шрифта
        cout << endl << "Файл шрифта не загружен. Проверьте соответвие названия файла или его налиичие в файлах проекта." << endl;
    }

    // Создаем квадратик с обозначением диаграм
    sf::RectangleShape block_legend(sf::Vector2f(200.f, 150.f));
    block_legend.setFillColor(BLOCK_COLOR);
    block_legend.setPosition(space / 1.f, space / 1.f);
    block_legend.setOutlineThickness(3.f);
    block_legend.setOutlineColor(sf::Color::Black);

    sf::RectangleShape big_block(sf::Vector2f((1600 - 3 * space) / 1.f, (1400 - 2 * space) / 1.f));
    big_block.setFillColor(BLOCK_COLOR);
    big_block.setPosition((2 * space + 200) / 1.f, space / 1.f);
    big_block.setOutlineThickness(3.f);
    big_block.setOutlineColor(sf::Color::Black);

    sf::RectangleShape lil_block(sf::Vector2f((1600 - 13 * space) / 1.f, (1400 - 6 * space) / 1.f));
    lil_block.setFillColor(BACK_COLOR);
    lil_block.setPosition((11 * space + 200) / 1.f, 2 * space / 1.f);
    lil_block.setOutlineThickness(3.f);
    lil_block.setOutlineColor(sf::Color::Black);

    sf::Text time_text("TIME", font, 40);
    time_text.setFillColor(OY_COLOR);
    time_text.setPosition((2 * (11 * space + 200) + space) / 1.f, (1400 - 4 * space) / 1.f);

    // Создаем текст в блоке и горизонтальные полоски
    sf::Text block_quad_text("Quadsort", font, 24);
    block_quad_text.setFillColor(OY_COLOR);
    block_quad_text.setPosition((space + 18) / 1.f, (space + 10) / 1.f);

    sf::RectangleShape block_quad_line;
    block_quad_line.setSize(sf::Vector2f(50.f, 12 / 1.f));
    block_quad_line.setFillColor(QUAD_COLOR);
    block_quad_line.setPosition((space + 120) / 1.f, (space + 21) / 1.f);
    block_quad_line.setOutlineThickness(1.f);
    block_quad_line.setOutlineColor(sf::Color::Black);

    sf::Text block_quick_text("Quicksort", font, 24);
    block_quick_text.setFillColor(OY_COLOR);
    block_quick_text.setPosition((space + 15) / 1.f, (space + 20 + 24) / 1.f);

    sf::RectangleShape block_quick_line;
    block_quick_line.setSize(sf::Vector2f(50.f, 12 / 1.f));
    block_quick_line.setFillColor(QUICK_COLOR);
    block_quick_line.setPosition((space + 120) / 1.f, (space + 31 + 24) / 1.f);
    block_quick_line.setOutlineThickness(1.f);
    block_quick_line.setOutlineColor(sf::Color::Black);

    sf::Text block_shell_text("Shellsort", font, 24);
    block_shell_text.setFillColor(OY_COLOR);
    block_shell_text.setPosition((space + 15) / 1.f, (space + 30 + 48) / 1.f);

    sf::RectangleShape block_shell_line;
    block_shell_line.setSize(sf::Vector2f(50.f, 12 / 1.f));
    block_shell_line.setFillColor(SHELL_COLOR);
    block_shell_line.setPosition((space + 120) / 1.f, (space + 41 + 48) / 1.f);
    block_shell_line.setOutlineThickness(1.f);
    block_shell_line.setOutlineColor(sf::Color::Black);

    sf::Text block_std_text(" std::sort", font, 24);
    block_std_text.setFillColor(OY_COLOR);
    block_std_text.setPosition((space + 15) / 1.f, (space + 40 + 72) / 1.f);

    sf::RectangleShape block_std_line;
    block_std_line.setSize(sf::Vector2f(50.f, 12 / 1.f));
    block_std_line.setFillColor(STD_COLOR);
    block_std_line.setPosition((space + 120) / 1.f, (space + 51 + 72) / 1.f);
    block_std_line.setOutlineThickness(1.f);
    block_std_line.setOutlineColor(sf::Color::Black);


    //диаграммы
    sf::RectangleShape quadbar1(sf::Vector2f(quadBarHeight1, barWidth));
    quadbar1.setPosition((11 * space + 200) / 1.f, (2 * space + 70) / 1.f);
    quadbar1.setFillColor(QUAD_COLOR);
    quadbar1.setOutlineThickness(2.f);
    quadbar1.setOutlineColor(sf::Color::Black);
    sf::Text quadbar1_num(to_string(quad_result_arrSorted / 100000.0) + " sec", font, 24);
    quadbar1_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75) / 1.f);
    quadbar1_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quickbar1(sf::Vector2f(quickBarHeight1, barWidth));
    quickbar1.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + barSpacing) / 1.f);
    quickbar1.setFillColor(QUICK_COLOR);
    quickbar1.setOutlineThickness(2.f);
    quickbar1.setOutlineColor(sf::Color::Black);
    sf::Text quickbar1_num(to_string(quick_result_arrSorted / 100000.0) + " sec", font, 24);
    quickbar1_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + barSpacing) / 1.f);
    quickbar1_num.setFillColor(BACK_COLOR);

    sf::RectangleShape shellbar1(sf::Vector2f(shellBarHeight1, barWidth));
    shellbar1.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 2 * barSpacing) / 1.f);
    shellbar1.setFillColor(SHELL_COLOR);
    shellbar1.setOutlineThickness(2.f);
    shellbar1.setOutlineColor(sf::Color::Black);
    sf::Text shellbar1_num(to_string(shell_result_arrSorted / 100000.0) + " sec", font, 24);
    shellbar1_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 2 * barSpacing) / 1.f);
    shellbar1_num.setFillColor(BACK_COLOR);

    sf::RectangleShape stdbar1(sf::Vector2f(stdBarHeight1, barWidth));
    stdbar1.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 3 * barSpacing + 2) / 1.f);
    stdbar1.setFillColor(STD_COLOR);
    stdbar1.setOutlineThickness(2.f);
    stdbar1.setOutlineColor(sf::Color::Black);
    sf::Text stdbar1_num(to_string(std_result_arrSorted / 100000.0) + " sec", font, 24);
    stdbar1_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 3 * barSpacing + 2) / 1.f);
    stdbar1_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quadbar2(sf::Vector2f(quadBarHeight2, barWidth));
    quadbar2.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 300) / 1.f);
    quadbar2.setFillColor(QUAD_COLOR);
    quadbar2.setOutlineThickness(2.f);
    quadbar2.setOutlineColor(sf::Color::Black);
    sf::Text quadbar2_num(to_string(quad_result_arrPartSorted / 100000.0) + " sec", font, 24);
    quadbar2_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 300) / 1.f);
    quadbar2_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quickbar2(sf::Vector2f(quickBarHeight2, barWidth));
    quickbar2.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + barSpacing + 300) / 1.f);
    quickbar2.setFillColor(QUICK_COLOR);
    quickbar2.setOutlineThickness(2.f);
    quickbar2.setOutlineColor(sf::Color::Black);
    sf::Text quickbar2_num(to_string(quick_result_arrPartSorted / 100000.0) + " sec", font, 24);
    quickbar2_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + barSpacing + 300) / 1.f);
    quickbar2_num.setFillColor(BACK_COLOR);

    sf::RectangleShape shellbar2(sf::Vector2f(shellBarHeight2, barWidth));
    shellbar2.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 2 * barSpacing + 300) / 1.f);
    shellbar2.setFillColor(SHELL_COLOR);
    shellbar2.setOutlineThickness(2.f);
    shellbar2.setOutlineColor(sf::Color::Black);
    sf::Text shellbar2_num(to_string(shell_result_arrPartSorted / 100000.0) + " sec", font, 24);
    shellbar2_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 2 * barSpacing + 300) / 1.f);
    shellbar2_num.setFillColor(BACK_COLOR);

    sf::RectangleShape stdbar2(sf::Vector2f(stdBarHeight2, barWidth));
    stdbar2.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 3 * barSpacing + 302) / 1.f);
    stdbar2.setFillColor(STD_COLOR);
    stdbar2.setOutlineThickness(2.f);
    stdbar2.setOutlineColor(sf::Color::Black);
    sf::Text stdbar2_num(to_string(std_result_arrPartSorted / 100000.0) + " sec", font, 24);
    stdbar2_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 3 * barSpacing + 302) / 1.f);
    stdbar2_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quadbar3(sf::Vector2f(quadBarHeight3, barWidth));
    quadbar3.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 600) / 1.f);
    quadbar3.setFillColor(QUAD_COLOR);
    quadbar3.setOutlineThickness(2.f);
    quadbar3.setOutlineColor(sf::Color::Black);
    sf::Text quadbar3_num(to_string(quad_result_arrBackSorted / 100000.0) + " sec", font, 24);
    quadbar3_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 600) / 1.f);
    quadbar3_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quickbar3(sf::Vector2f(quickBarHeight3, barWidth));
    quickbar3.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + barSpacing + 600) / 1.f);
    quickbar3.setFillColor(QUICK_COLOR);
    quickbar3.setOutlineThickness(2.f);
    quickbar3.setOutlineColor(sf::Color::Black);
    sf::Text quickbar3_num(to_string(quick_result_arrBackSorted / 100000.0) + " sec", font, 24);
    quickbar3_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + barSpacing + 600) / 1.f);
    quickbar3_num.setFillColor(BACK_COLOR);

    sf::RectangleShape shellbar3(sf::Vector2f(shellBarHeight3, barWidth));
    shellbar3.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 2 * barSpacing + 600) / 1.f);
    shellbar3.setFillColor(SHELL_COLOR);
    shellbar3.setOutlineThickness(2.f);
    shellbar3.setOutlineColor(sf::Color::Black);
    sf::Text shellbar3_num(to_string(shell_result_arrBackSorted / 100000.0) + " sec", font, 24);
    shellbar3_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 2 * barSpacing + 600) / 1.f);
    shellbar3_num.setFillColor(BACK_COLOR);

    sf::RectangleShape stdbar3(sf::Vector2f(stdBarHeight3, barWidth));
    stdbar3.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 3 * barSpacing + 602) / 1.f);
    stdbar3.setFillColor(STD_COLOR);
    stdbar3.setOutlineThickness(2.f);
    stdbar3.setOutlineColor(sf::Color::Black);
    sf::Text stdbar3_num(to_string(std_result_arrBackSorted / 100000.0) + " sec", font, 24);
    stdbar3_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 3 * barSpacing + 602) / 1.f);
    stdbar3_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quadbar4(sf::Vector2f(quadBarHeight4, barWidth));
    quadbar4.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 900) / 1.f);
    quadbar4.setFillColor(QUAD_COLOR);
    quadbar4.setOutlineThickness(2.f);
    quadbar4.setOutlineColor(sf::Color::Black);
    sf::Text quadbar4_num(to_string(quad_result_arrRandomSorted / 100000.0) + " sec", font, 24);
    quadbar4_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 900) / 1.f);
    quadbar4_num.setFillColor(BACK_COLOR);

    sf::RectangleShape quickbar4(sf::Vector2f(quickBarHeight4, barWidth));
    quickbar4.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + barSpacing + 900) / 1.f);
    quickbar4.setFillColor(QUICK_COLOR);
    quickbar4.setOutlineThickness(2.f);
    quickbar4.setOutlineColor(sf::Color::Black);
    sf::Text quickbar4_num(to_string(quick_result_arrRandomSorted / 100000.0) + " sec", font, 24);
    quickbar4_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + barSpacing + 900) / 1.f);
    quickbar4_num.setFillColor(BACK_COLOR);

    sf::RectangleShape shellbar4(sf::Vector2f(shellBarHeight4, barWidth));
    shellbar4.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 2 * barSpacing + 900) / 1.f);
    shellbar4.setFillColor(SHELL_COLOR);
    shellbar4.setOutlineThickness(2.f);
    shellbar4.setOutlineColor(sf::Color::Black);
    sf::Text shellbar4_num(to_string(shell_result_arrRandomSorted / 100000.0) + " sec", font, 24);
    shellbar4_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 2 * barSpacing + 900) / 1.f);
    shellbar4_num.setFillColor(BACK_COLOR);

    sf::RectangleShape stdbar4(sf::Vector2f(stdBarHeight4, barWidth));
    stdbar4.setPosition((11 * space + 200) / 1.f, (2 * space + 70 + 3 * barSpacing + 902) / 1.f);
    stdbar4.setFillColor(STD_COLOR);
    stdbar4.setOutlineThickness(2.f);
    stdbar4.setOutlineColor(sf::Color::Black);
    sf::Text stdbar4_num(to_string(std_result_arrRandomSorted / 100000.0) + " sec", font, 24);
    stdbar4_num.setPosition((11 * space + 205) / 1.f, (2 * space + 75 + 3 * barSpacing + 902) / 1.f);
    stdbar4_num.setFillColor(BACK_COLOR);

    //подписи типов диаграм
    sf::Text sorted_array_text("sorted array", font, 30);
    sorted_array_text.setFillColor(OY_COLOR);
    sorted_array_text.setPosition((3 * space + 220) / 1.f, (2 * space + 100 + barSpacing) / 1.f);

    sf::Text part_sorted_array_text("part sorted array", font, 30);
    part_sorted_array_text.setFillColor(OY_COLOR);
    part_sorted_array_text.setPosition((3 * space + 220) / 1.f, (2 * space + 100 + barSpacing + 300) / 1.f);

    sf::Text back_sorted_array_text("back sorted array", font, 30);
    back_sorted_array_text.setFillColor(OY_COLOR);
    back_sorted_array_text.setPosition((3 * space + 220) / 1.f, (2 * space + 100 + barSpacing + 600) / 1.f);

    sf::Text random_array_text("random array", font, 30);
    random_array_text.setFillColor(OY_COLOR);
    random_array_text.setPosition((3 * space + 220) / 1.f, (2 * space + 100 + barSpacing + 900) / 1.f);

    //вывод окна
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(BACK_COLOR);

        window.draw(block_legend);
        window.draw(big_block);
        window.draw(lil_block);

        window.draw(time_text);
        window.draw(block_quad_text);
        window.draw(block_quad_line);
        window.draw(block_quick_text);
        window.draw(block_quick_line);
        window.draw(block_shell_text);
        window.draw(block_shell_line);
        window.draw(block_std_text);
        window.draw(block_std_line);

        window.draw(quadbar1);
        window.draw(quickbar1);
        window.draw(shellbar1);
        window.draw(stdbar1);
        window.draw(quadbar2);
        window.draw(quickbar2);
        window.draw(stdbar2);
        window.draw(shellbar2);
        window.draw(quadbar3);
        window.draw(stdbar3);
        window.draw(quickbar3);
        window.draw(shellbar3);
        window.draw(quadbar4);
        window.draw(quickbar4);
        window.draw(shellbar4);
        window.draw(stdbar4);

        window.draw(quadbar1_num);
        window.draw(quickbar1_num);
        window.draw(shellbar1_num);
        window.draw(stdbar1_num);
        window.draw(quadbar2_num);
        window.draw(quickbar2_num);
        window.draw(shellbar2_num);
        window.draw(stdbar2_num);
        window.draw(quadbar3_num);
        window.draw(quickbar3_num);
        window.draw(shellbar3_num);
        window.draw(stdbar3_num);
        window.draw(quadbar4_num);
        window.draw(quickbar4_num);
        window.draw(shellbar4_num);
        window.draw(stdbar4_num);

        window.draw(sorted_array_text);
        window.draw(part_sorted_array_text);
        window.draw(back_sorted_array_text);
        window.draw(random_array_text);

        window.display();
    }
}

void CompareResults(int startSizeArr, int finishSizeArr, int kolvo, int choice) {
    srand(time(0));
    auto begin = steady_clock::now();
    auto end = steady_clock::now();

    int QuadSort_arrSorted[10] = { 0 };
    int QuickSort_arrSorted[10] = { 0 };
    int ShellSort_arrSorted[10] = { 0 };
    int stdSort_arrSorted[10] = { 0 };

    int QuadSort_arrPartSorted[10] = { 0 };
    int QuickSort_arrPartSorted[10] = { 0 };
    int ShellSort_arrPartSorted[10] = { 0 };
    int stdSort_arrPartSorted[10] = { 0 };

    int QuadSort_arrBackSorted[10] = { 0 };
    int QuickSort_arrBackSorted[10] = { 0 };
    int ShellSort_arrBackSorted[10] = { 0 };
    int stdSort_arrBackSorted[10] = { 0 };

    int QuadSort_arrRandomSorted[10] = { 0 };
    int QuickSort_arrRandomSorted[10] = { 0 };
    int ShellSort_arrRandomSorted[10] = { 0 };
    int stdSort_arrRandomSorted[10] = { 0 };

    cout << "Размеры массивов = [";
    for (int i = startSizeArr; i <= finishSizeArr; i += startSizeArr) {
        cout << i;
        if (i < finishSizeArr) cout << ",";
    }
    cout << "];" << endl << "Количество сортировок для каждого результата: " << kolvo << endl << endl;
    cout << "Среднее время сортировки отсортированного массива: " << endl;
    cout << "\033[31m" << "QuadSort_arrSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = (int*)malloc(ARR_SIZE * sizeof(int)); // указатель для выделения памяти под массив
            arrSorted(arr);
            begin = steady_clock::now();
            QuadSort<diff_func>(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
            free(arr);
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuadSort_arrSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[32m" << "QuickSort_arrSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrSorted(arr);
            begin = steady_clock::now();
            QuickSort(arr, 0, ARR_SIZE - 1);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuickSort_arrSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[34m" << "ShellSort_arrSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrSorted(arr);
            begin = steady_clock::now();
            ShellSort(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        ShellSort_arrSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;
    cout << "\033[33m" << "stdSort_arrSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrSorted(arr);
            begin = steady_clock::now();
            std::sort(arr, arr + ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        stdSort_arrSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl << endl << "\033[0m";

    cout << "Среднее время сортировки частично отсортированного массива: " << endl;
    cout << "\033[31m" << "QuadSort_arrPartSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = (int*)malloc(ARR_SIZE * sizeof(int)); // указатель для выделения памяти под массив
            arrPartSorted(arr);
            begin = steady_clock::now();
            QuadSort<diff_func>(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
            free(arr);
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuadSort_arrPartSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[32m" << "QuickSort_arrPartSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrPartSorted(arr);
            begin = steady_clock::now();
            QuickSort(arr, 0, ARR_SIZE - 1);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuickSort_arrPartSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[34m" << "ShellSort_arrPartSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrPartSorted(arr);
            begin = steady_clock::now();
            ShellSort(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        ShellSort_arrPartSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;
    cout << "\033[33m" << "stdSort_arrPartSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrPartSorted(arr);
            begin = steady_clock::now();
            std::sort(arr, arr + ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        stdSort_arrPartSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl << endl << "\033[0m";

    cout << "Среднее время сортировки обратно отсортированного массива: " << endl;
    cout << "\033[31m" << "QuadSort_arrBackSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = (int*)malloc(ARR_SIZE * sizeof(int)); // указатель для выделения памяти под массив
            arrBackSorted(arr);
            begin = steady_clock::now();
            QuadSort<diff_func>(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
            free(arr);
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuadSort_arrBackSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[32m" << "QuickSort_arrBackSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrBackSorted(arr);
            begin = steady_clock::now();
            QuickSort(arr, 0, ARR_SIZE - 1);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuickSort_arrBackSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;
    cout << "\033[34m" << "ShellSort_arrBackSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrBackSorted(arr);
            begin = steady_clock::now();
            ShellSort(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        ShellSort_arrBackSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;
    cout << "\033[33m" << "stdSort_arrBackSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrBackSorted(arr);
            begin = steady_clock::now();
            std::sort(arr, arr + ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        stdSort_arrBackSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl << endl << "\033[0m";

    cout << "Среднее время сортировки случайного массива: " << endl;
    cout << "\033[31m" << "QuadSort_arrRandomSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = (int*)malloc(ARR_SIZE * sizeof(int)); // указатель для выделения памяти под массив
            arrRandomSorted(arr);
            begin = steady_clock::now();
            QuadSort<diff_func>(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
            free(arr);
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuadSort_arrRandomSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[32m" << "QuickSort_arrRandomSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrRandomSorted(arr);
            begin = steady_clock::now();
            QuickSort(arr, 0, ARR_SIZE - 1);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        QuickSort_arrRandomSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;

    cout << "\033[34m" << "ShellSort_arrRandomSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrRandomSorted(arr);
            begin = steady_clock::now();
            ShellSort(arr, ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        ShellSort_arrRandomSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl;
    cout << "\033[33m" << "stdSort_arrRandomSorted=[";
    for (int ARR_SIZE = startSizeArr; ARR_SIZE <= finishSizeArr; ARR_SIZE += startSizeArr) {
        int sumtime = 0;//переменная для хранения среднего времени сортировки
        for (int i = 0; i < kolvo; i += 1) {
            int* arr = new int[ARR_SIZE]; // указатель для выделения памяти под массив
            arrRandomSorted(arr);
            begin = steady_clock::now();
            std::sort(arr, arr + ARR_SIZE);
            end = steady_clock::now();
            sumtime += duration_cast<microseconds>(end - begin).count() / kolvo;
        }
        cout << sumtime;
        int k = (ARR_SIZE / startSizeArr) - 1;
        stdSort_arrRandomSorted[k] = int(sumtime / 10);
        if (ARR_SIZE < finishSizeArr) {
            cout << ",";
        }
    }
    cout << "];" << endl << endl << "\033[0m";

    if (choice == 8)
    {
        int HEIGHT_arrSorted = height_grafik_window(QuadSort_arrSorted[9], QuickSort_arrSorted[9], ShellSort_arrSorted[9]);//высота окна
        int HEIGHT_arrPartSorted = height_grafik_window(QuadSort_arrPartSorted[9], QuickSort_arrPartSorted[9], ShellSort_arrPartSorted[9]);//высота окна
        int HEIGHT_arrBackSorted = height_grafik_window(QuadSort_arrBackSorted[9], QuickSort_arrBackSorted[9], ShellSort_arrBackSorted[9]);//высота окна
        int HEIGHT_arrRandomSorted = height_grafik_window(QuadSort_arrRandomSorted[9], QuickSort_arrRandomSorted[9], ShellSort_arrRandomSorted[9]);//высота окна
        int HEIGHT;
        HEIGHT = max({ HEIGHT_arrSorted, HEIGHT_arrPartSorted, HEIGHT_arrBackSorted, HEIGHT_arrRandomSorted });

        sf::RenderWindow window_arrSorted(sf::VideoMode(WIDTH, HEIGHT), "Avarage time of sorting arrSorted");//создание окна
        grafik(window_arrSorted, QuadSort_arrSorted, QuickSort_arrSorted, ShellSort_arrSorted, stdSort_arrSorted, HEIGHT);//вывод графика с помощью функции

        sf::RenderWindow window_arrPartSorted(sf::VideoMode(WIDTH, HEIGHT), "Avarage time of sorting arrPartSorted");//создание окна
        grafik(window_arrPartSorted, QuadSort_arrPartSorted, QuickSort_arrPartSorted, ShellSort_arrPartSorted, stdSort_arrPartSorted, HEIGHT);//вывод графика с помощью функции

        sf::RenderWindow window_arrBackSorted(sf::VideoMode(WIDTH, HEIGHT), "Avarage time of sorting arrBackSorted");//создание окна
        grafik(window_arrBackSorted, QuadSort_arrBackSorted, QuickSort_arrBackSorted, ShellSort_arrBackSorted, stdSort_arrBackSorted, HEIGHT);//вывод графика с помощью функции

        sf::RenderWindow window_arrRandomSorted(sf::VideoMode(WIDTH, HEIGHT), "Avarage time of sorting arrRandomSorted ");//создание окна
        grafik(window_arrRandomSorted, QuadSort_arrRandomSorted, QuickSort_arrRandomSorted, ShellSort_arrRandomSorted,stdSort_arrRandomSorted, HEIGHT);//вывод графика с помощью функции
    }
    else if (choice == 9) {
        sf::RenderWindow window(sf::VideoMode(1800, 1400), "Horizontal bar chart");
        diagram(window, QuadSort_arrSorted[0], QuickSort_arrSorted[0], ShellSort_arrSorted[0], stdSort_arrSorted[0], QuadSort_arrPartSorted[0], QuickSort_arrPartSorted[0], ShellSort_arrPartSorted[0], stdSort_arrPartSorted[0], QuadSort_arrBackSorted[0], QuickSort_arrBackSorted[0], ShellSort_arrBackSorted[0], stdSort_arrBackSorted[0],  QuadSort_arrRandomSorted[0], QuickSort_arrRandomSorted[0], ShellSort_arrRandomSorted[0], stdSort_arrRandomSorted[0]);
    }

}

int main()
{
    setlocale(LC_ALL, "Rus");
    int* arr = new int[LEN]; // указатель для выделения памяти под массив
    int menuChoice;
    cout << "----------------------------------------------------------МЕНЮ----------------------------------------------------------" << endl;
    cout << "Выберите тип массива, для демонстрации работоспособности Quadsort: " << endl;
    cout << "1. Отсортированный массив" << endl;
    cout << "2. Частично отсортированный массив" << endl;
    cout << "3. Обратно отсортированный массив" << endl;
    cout << "4. Случайный массив" << endl;
    cout << "----------------------------------------------------------ИЛИ-----------------------------------------------------------" << endl;
    cout << "Выберите тип массива, для демонстрации визуализации Quadsort: " << endl;
    cout << "5. Частично отсортированный массив" << endl;
    cout << "6. Обратно отсортированный массив" << endl;
    cout << "7. Случайный массив" << endl;
    cout << "----------------------------------------------------------ИЛИ-----------------------------------------------------------" << endl;
    cout << "Выберите, какие результаты хотите получить: " << endl;
    cout << "8. Сравнение среднего времени работы Quadsort c другими типами сортировок(QuickSort, ShellSort)" << endl;
    cout << "   [массивы от 10.000 до 100.000 элементов с шагом в 10.000]" << endl;
    cout << "9. Сравнение времени работы Quadsort c другими типами сортировок(QuickSort, ShellSort)" << endl;
    cout << "   [массив 100.000.000 элементов]" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Ваш выбор: ";
    cin >> menuChoice;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    switch (menuChoice) {
    case 1:
        arrSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSort<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 2:
        arrPartSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSort<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 3:
        arrBackSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSort<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 4:
        arrRandomSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSort<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;

    case 5:
        arrPartSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSortVisualisation<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 6:
        arrBackSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSortVisualisation<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 7:
        arrRandomSorted(arr);
        cout << "Массив до сортировки: " << endl;
        printArray(arr);
        QuadSortVisualisation<diff_func>(arr, LEN);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Массив после сортировки Quadsort: " << endl;
        printArray(arr);
        break;
    case 8:
        CompareResults(10000, 100000, 50, menuChoice);
        break;
    case 9:
        CompareResults(100000000,100000000, 1, menuChoice);
        break;
    default:
        cout << "Выберите пункт из списка!" << endl;
        break;
    }
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    return 0;
}
