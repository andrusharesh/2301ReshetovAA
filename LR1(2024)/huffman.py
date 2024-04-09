class Heap:
    # Инициализация кучи
    def __init__(self, arr=[], isMin=True):
        self.heap = []  # внутреннее представление кучи
        self.isMin = isMin  # флаг минимальной или максимальной кучи
        for el in arr:  # инициализация кучи элементами из списка
            self.insert(el)

    # Получение индекса родителя
    def parent(self, index):
        return (index - 1) // 2

    # Получение индекса левого потомка
    def left(self, index):
        return 2 * index + 1

    # Получение индекса правого потомка
    def right(self, index):
        return 2 * index + 2

    # Восстановление свойства кучи после добавления элемента
    def sift_up(self, index):
        if index < 0 or index >= len(self.heap): return
        parent = self.parent(index)
        while index and not (self.heap[parent] < self.heap[index] and self.isMin or self.heap[parent] > self.heap[
            index] and not self.isMin):
            self.heap[parent], self.heap[index] = self.heap[index], self.heap[parent]
            index, parent = parent, self.parent(index)

    # Восстановление свойства кучи после удаления элемента
    def sift_down(self, index):
        if index < 0 or index >= len(self.heap): return
        mIndex = index
        while True:
            left, right = self.left(index), self.right(index)
            if right < len(self.heap) and (
                    self.heap[right] < self.heap[mIndex] and self.isMin or self.heap[right] > self.heap[
                mIndex] and not self.isMin): mIndex = right
            if left < len(self.heap) and (
                    self.heap[left] < self.heap[mIndex] and self.isMin or self.heap[left] > self.heap[
                mIndex] and not self.isMin): mIndex = left
            if mIndex == index:
                return
            else:
                self.heap[index], self.heap[mIndex] = self.heap[mIndex], self.heap[index]
                index = mIndex

    # Удаление и возвращение корня кучи
    def pop(self):
        if not self.heap: return
        mElement = self.heap[0]
        self.heap[0] = self.heap[-1]
        del self.heap[-1]
        self.sift_down(0)
        return mElement

    # Добавление элемента в кучу
    def insert(self, element):
        self.heap.append(element)
        self.sift_up(len(self.heap) - 1)

    # Получение размера кучи
    def size(self):
        return len(self.heap)


# Класс для представления символа в дереве Хаффмана
class Letter:
    def __init__(self, char, count, left=None, right=None):
        self.char = char  # символ
        self.count = count  # частота встречаемости
        self.code = ""  # код Хаффмана для символа
        self.left, self.right = left, right  # левый и правый потомки

    # Оператор сравнения для упорядочивания в куче
    def __lt__(self, other):
        if isinstance(other, Letter):
            if self.count != other.count:
                return self.count < other.count
            else:
                if other.char == "":
                    return True
                elif self.char == "":
                    return False
                else:
                    return self.char < other.char


# Подсчёт частоты каждого символа в строке
def countLetters(string):
    alphabet = list(set(string))  # уникальные символы в строке
    occurances = [0] * len(alphabet)  # частоты символов

    for char in string:
        occurances[alphabet.index(char)] += 1

    letters = []
    for ch in range(len(alphabet)):
        letters.append(Letter(alphabet[ch], occurances[ch]))
    return letters


# Построение дерева Хаффмана из таблицы частот
def buildHuffmanTree(table):
    heap = Heap(table, isMin=True)  # инициализация минимальной кучи
    while heap.size() > 1:  # пока в куче больше одного элемента
        left = heap.pop()  # извлечение двух минимальных элементов
        right = heap.pop()
        heap.insert(Letter('', left.count + right.count, left, right))  # вставка нового узла дерева
    return heap.pop()  # возвращение корня дерева


# Рекурсивное построение кодов Хаффмана
def buildCodes(top, chars, branch=''):
    current = top
    right, left = current.right, current.left

    current.code += branch
    if right:
        right.code = current.code
        buildCodes(right, chars, '0')
    if left:
        left.code = current.code
        buildCodes(left, chars, '1')
    if current.char:
        chars[current.char] = current.code


# Кодирование строки с использованием кодов Хаффмана
def HuffmanEncode(string, dictionary):
    top = buildHuffmanTree(countLetters(string))  # построение дерева Хаффмана
    buildCodes(top, dictionary)  # генерация кодов

    code = ''
    for ch in string: code += dictionary[ch]  # кодирование строки
    return code


# Декодирование строки
def decode(string, codes):
    outputString = ''
    codedChar = ''
    for char in string:
        codedChar += char
        if codedChar in codes.values():  # если текущая последовательность соответствует какому-либо коду
            outputString += list(codes.keys())[list(codes.values()).index(codedChar)]  # добавление символа в результат
            codedChar = ''  # сброс текущей последовательности
    return outputString

import os

def calculate_compression_ratio(original_file_path, compressed_file_path):
    original_size = os.path.getsize(original_file_path)  # Получаем размер исходного файла
    compressed_size = os.path.getsize(compressed_file_path)  # Получаем размер сжатого файла
    # Проверяем, чтобы избежать деления на ноль
    if compressed_size > 0:
        return compressed_size / original_size
    else:
        return 0

def main():
    input_file_name = 'op1.txt'
    output_file_name = "encoded_text.txt"
    decoded_file_name = "decoded_text.txt"

    # Открытие файлов для чтения и записи
    input_file_path = open(input_file_name, 'r', errors='ignore')
    output_file_path = open(output_file_name, "w")
    decoded_text_file_path = open(decoded_file_name, "w")

    string = input_file_path.read()  # чтение исходного текста
    codes = dict()  # словарь для кодов Хаффмана

    output = HuffmanEncode(string, codes)  # кодирование текста
    output_file_path.write(output)  # запись закодированного текста

    dec_text = decode(output, codes)  # декодирование текста
    decoded_text_file_path.write(dec_text)  # запись декодированного текста

    # Закрытие файлов
    input_file_path.close()
    output_file_path.close()
    decoded_text_file_path.close()

    # Передаем строки с путями к файлам в функцию
    compression_ratio = calculate_compression_ratio(input_file_name, output_file_name)
    print(f"Сжатие текста из файла '{input_file_name}' завершено и сохранено в '{output_file_name}'.")
    print(f"Декодированный текст сохранён в '{decoded_file_name}'.")
    print(f"Коэффициент сжатия: {compression_ratio:.2f}")

main()
