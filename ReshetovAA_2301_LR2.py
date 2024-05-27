import os
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

# коэффициенты сжатия при даунсэмплинге
compression_ratio_height = 2
compression_ratio_width = 2

# Качество при квантовании
quality = 10

# --------ПРЕОБРАЗОВАНИЕ ИЗ RGB В YCBCR И НАОБОРОТ----------
def RGB_in_YCbCr(array):
    # Функция для преобразования изображения из RGB в YCbCr

    R = array[:, :, 0]
    G = array[:, :, 1]
    B = array[:, :, 2]

    # Преобразование из RGB в YCbCr
    Y = 0.299 * R + 0.587 * G + 0.114 * B
    Cb = -0.168736 * R - 0.331264 * G + 0.5 * B + 128
    Cr = 0.5 * R - 0.4187 * G - 0.081312 * B + 128

    # Объединение каналов яркости, cb и cr в один трехмерный массив
    ycbcr_image_array = np.dstack((Y, Cb, Cr))

    # Ограничение значений компонент в диапазоне [0, 255]
    ycbcr_image_array = np.clip(ycbcr_image_array, 0, 255)

    # Преобразование к типу данных np.uint8
    ycbcr_image_array = ycbcr_image_array.astype(np.uint8)

    return ycbcr_image_array
def YCbCr_in_RGB(Y, Cb, Cr):
    # Функция для преобразования изображения из YCbCr в RGB

    Y = Y.astype(np.int16) # меняем тип данных дл] возможности хранения отрицательных чисел
    Cb = Cb.astype(np.int16)
    Cr = Cr.astype(np.int16)

    # Преобразование из YCbCr в RGB
    R = Y + 1.402 * (Cr - 128)
    G = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
    B = Y + 1.772 * (Cb - 128)

    # Преобразование к типу данных np.uint8
    R = np.uint8(np.clip(R, 0, 255))
    G = np.uint8(np.clip(G, 0, 255))
    B = np.uint8(np.clip(B, 0, 255))

    # Объединение массивов R, G и B в один трехмерный массив
    rgb_image_array = np.dstack((R, G, B))

    return rgb_image_array
# ----------------------------------------------------------

# ---------------ДАУНСЭМПЛИНГ И АПСЭМПЛИНГ------------------
def downsampling(image, compression_ratio_height, compression_ratio_width, ds_form):
    img_height, img_width = image.shape
    # Проверка входных параметров

    # Удаление строк и столбцов
    if ds_form == 1:

        # Вычисляем новое количество строк и столбцов после уменьшения
        new_height, new_width = np.floor(img_height / compression_ratio_height), np.floor(img_width / compression_ratio_width)
        rows_to_delete, cols_to_delete = img_height - new_height, img_width - new_width
        # Начальный индекс для удаления строк
        start_index = 0
        # Удаляем строки до тех пор, пока их количество не уменьшится до нужного
        while rows_to_delete > 0:
            # Определяем индексы четных строк в текущей матрице
            even_row_indices = list(range(start_index, image.shape[0], 2))
            # Счетчик удаленных строк для корректировки индексов
            count_deleted_rows = 0
            for row_index in even_row_indices:
                if rows_to_delete > 0:
                    # Удаляем четную строку с учетом уже удаленных строк
                    image = np.delete(image, row_index - count_deleted_rows, axis=0)
                    count_deleted_rows += 1
                    rows_to_delete -= 1

            # Обновляем start_index для продолжения удаления с начала, если достигнут конец матрицы
            start_index = max(0, start_index - image.shape[0])

        start_index = 0
        # Удаляем столбцы до тех пор, пока их количество не уменьшится до нужного
        while cols_to_delete > 0:
            # Определяем индексы четных столбцов в текущей матрице
            even_col_indices = list(range(start_index, image.shape[1], 2))
            # Счетчик удаленных столбцов для корректировки индексов
            count_deleted_cols = 0
            for col_index in even_col_indices:
                if cols_to_delete > 0:
                    # Удаляем четный столбец с учетом уже удаленных столбцов
                    image = np.delete(image, col_index - count_deleted_cols, axis=1)
                    count_deleted_cols += 1
                    cols_to_delete -= 1

            # Обновляем start_index для продолжения удаления с начала, если достигнут конец матрицы
            start_index = max(0, start_index - image.shape[1])
        return image

    # Замена блока пикселей пикселем со средним значением цвета блока
    if ds_form == 2:

        # Вычисляем новые размеры уменьшенного изображения
        new_height, new_width = int((img_height / compression_ratio_height)), int((img_width / compression_ratio_width))
        # Определяем размеры блоков для вычисления среднего значения
        block_height, block_width = int(np.floor(img_height / new_height)), int(np.floor(img_width / new_width))
        # Создаем пустой массив для уменьшенного изображения
        downsampled_image = np.zeros((new_height, new_width), dtype=np.uint8)

        # Заполняем уменьшенное изображение средними значениями блоков
        for i in range(new_height):
            for j in range(new_width):
                # Извлекаем блок текущих пикселей
                block = image[i * block_height: (i + 1) * block_height, j * block_width: (j + 1) * block_width]
                # Вычисляем среднее значение блока
                average_value = np.mean(block)
                # Записываем среднее значение в уменьшенное изображение
                downsampled_image[i, j] = int(average_value)
        return downsampled_image

    # Замена блока пикселей на пиксель, значение которого ближе всего к среднему значению цвета блока
    if ds_form == 3:
        # Вычисляем новые размеры уменьшенного изображения
        new_height, new_width = int((img_height / compression_ratio_height)), int((img_width / compression_ratio_width))
        # Определяем размеры блоков для вычисления ближайшего значения к среднему
        block_height, block_width = int(np.floor(img_height / new_height)), int(np.floor(img_width / new_width))

        # Создаем пустой массив для уменьшенного изображения
        downsampled_image = np.zeros((new_height, new_width), dtype=np.uint8)

        # Заполняем уменьшенное изображение значениями пикселей, ближайшими к среднему значению блока
        for i in range(new_height):
            for j in range(new_width):
                # Извлекаем блок текущих пикселей
                block = image[i * block_height: (i + 1) * block_height, j * block_width: (j + 1) * block_width]
                # Вычисляем среднее значение блока
                average_value = np.mean(block)
                # Создаем матрицу разницы между каждым пикселем блока и средним значением
                diff_matrix = abs(block - average_value)
                # Находим индекс пикселя с минимальной разницей
                index_of_nearest_pixel = diff_matrix.argmin()
                # Извлекаем значение пикселя с минимальной разницей
                nearest_pixel = block.flat[index_of_nearest_pixel]
                # Записываем значение пикселя в уменьшенное изображение
                downsampled_image[i, j] = int(nearest_pixel)
        return downsampled_image
def upsampling(image, compression_ratio_height, compression_ratio_width):
    img_height, img_width = image.shape
    # Вычисляем новые размеры изображения
    new_height, new_width = int((img_height * compression_ratio_height)), int((img_width * compression_ratio_width))
    # Создание нового массива для увеличенного изображения
    upsampled_image = np.zeros((new_height, new_width), dtype=np.uint8)

    # Проход по  изображению
    for i in range(img_height):
        for j in range(img_width):
            # Выделяем блок
            block = image[i,j]
            # Копируем блок в изображение после апсэмплинга
            upsampled_image[i * compression_ratio_height: (i + 1) * compression_ratio_height, j * compression_ratio_width: (j + 1) * compression_ratio_width] = block
    return upsampled_image
# ----------------------------------------------------------

# -------РАЗДЕЛЕНИЕ ИЗОБРАЖЕНИЯ НА БЛОКИ 8х8 И ОБРАТНО------
def image_to_blocks(image, height, width):
    blocks = []
    for y in range(0, height, 8):
        for x in range(0, width, 8):
            blocks.append(image[y:y+8, x:x+8])
    return np.array(blocks)
def blocks_to_image(blocks, height, width):
    image = np.zeros((height, width), dtype=int)
    block_index = 0
    for y in range(0, height, 8):
        for x in range(0, width, 8):
            image[y:y+8, x:x+8] = blocks[block_index]
            block_index += 1
    return image
# ----------------------------------------------------------

# ------ДИСКРЕТНО-КОСИНУСНОЕ ПРЕОБРАЗОВАНИЕ И ОБРАТНОЕ------
def dct_transform_matrix(M):
    # функция, которая возвращает ДКТ матрицу М на М
    dct_matrix = np.zeros((M, M))
    for p in range(M):
        for q in range(M):
            if p == 0:
                dct_matrix[p, q] = 1 / np.sqrt(M)
            else:
                dct_matrix[p, q] = np.sqrt(2 / M) * np.cos(np.pi * (2 * q + 1) * p / (2 * M))
    return dct_matrix
def discrete_cosine_transform(input_block):
    # функция, которая выполняет дкт

    # получаем размеры изображения
    height, width = input_block.shape

    # Создаем матрицу дкт для строк и столбцов
    dct_matrix_height = dct_transform_matrix(height)
    dct_matrix_width = dct_transform_matrix(width)

    # Выполняем преобразование
    dct_output = np.dot(np.dot(dct_matrix_height, input_block), dct_matrix_width.T)

    return dct_output.astype(np.int16)
def reverse_discrete_cosine_transform(input_block):
    # функция, которая выполняет обратное дкт

    # получаем размеры коэффициентов
    height, width = input_block.shape

    # Создаем матрицу для строк и столбцов
    dct_matrix_height = dct_transform_matrix(height)
    dct_matrix_width = dct_transform_matrix(width)

    # Выполняем обратное преобразование
    idct_output = np.dot(np.dot(dct_matrix_height.T, input_block), dct_matrix_width)

    return idct_output.astype(np.int16)
# ----------------------------------------------------------

# ---------------------КВАНТОВАНИЕ--------------------------
def quantization_matrix(quality):
    quantization_matrix = np.array([
        [16, 11, 10, 16, 24, 40, 51, 61],
        [12, 12, 14, 19, 26, 58, 60, 55],
        [14, 13, 16, 24, 40, 57, 69, 56],
        [14, 17, 22, 29, 51, 87, 80, 62],
        [18, 22, 37, 56, 68, 109, 103, 77],
        [24, 35, 55, 64, 81, 104, 113, 92],
        [49, 64, 78, 87, 103, 121, 120, 101],
        [72, 92, 95, 98, 112, 100, 103, 99]
    ])

    # Масштабирование оригинальной матрицы квантования в зависимости от уровня качества
    if quality <= 50:
        scaling_ratio = 5000 / quality
    else:
        scaling_ratio = 200 - 2 * quality

    # Обновляем значения матрицы квантования
    new_quantization_matrix = np.floor((quantization_matrix * scaling_ratio + 50) / 100)
    # Ограничим значения в полученном массиве от 1 до 255
    new_quantization_matrix = np.clip(new_quantization_matrix, 1, 255)

    return new_quantization_matrix.astype(np.uint8)
def quantization(dct_matrix, q_matrix):
    quantized_matrix = np.round(dct_matrix / q_matrix)
    return quantized_matrix.astype(int)
def dequantization(dct_matrix, q_matrix):
    quantized_matrix = np.round(dct_matrix * q_matrix)
    return quantized_matrix.astype(int)
# ----------------------------------------------------------

# ---------ОБХОД ИЗОБРАЖЕНИЯ ЗИГ-ЗАГОМ И ОБРАТНЫЙ-----------
def straight_zigzag(array):
    zigzag_result = []  # Список для хранения результата
    row, col = len(array), len(array[0])  # Количество строк и столбцов в матрице
    total = row + col - 1  # Общее количество диагоналей

    for current_sum in range(total):
        if current_sum % 2 == 0:
            for i in range(min(current_sum, row-1), max(-1, current_sum-col), -1):
                zigzag_result.append(array[i][current_sum - i])
        else:
            for i in range(max(0, current_sum-col+1), min(current_sum+1, row)):
                zigzag_result.append(array[i][current_sum - i])

    return zigzag_result
def back_zigzag__(array, rows, cols):
    matrix = np.zeros((rows, cols), dtype=int)  # Создаем пустую матрицу нужного размера
    current_index = 0  # Индекс для элементов входного массива
    total = rows + cols - 1  # Общее количество диагоналей

    for current_sum in range(total):
        if current_sum % 2 == 0:
            for i in range(min(current_sum, rows-1), max(-1, current_sum-cols), -1):
                matrix[i][current_sum - i] = array[current_index]
                current_index += 1
        else:
            for i in range(max(0, current_sum-cols+1), min(current_sum+1, rows)):
                matrix[i][current_sum - i] = array[current_index]
                current_index += 1

    return matrix
def back_zigzag(array, rows, cols):
    matrix = np.zeros((rows, cols), dtype=int)  # Создаем пустую матрицу нужного размера
    current_index = 0  # Индекс для элементов входного массива
    total = rows + cols - 1  # Общее количество диагоналей

    for current_sum in range(total):
        if current_sum % 2 == 0:
            for i in range(min(current_sum, rows-1), max(-1, current_sum-cols), -1):
                matrix[i][current_sum - i] = array[current_index]
                current_index += 1
        else:
            for i in range(max(0, current_sum-cols+1), min(current_sum+1, rows)):
                matrix[i][current_sum - i] = array[current_index]
                current_index += 1

    return matrix
# ----------------------------------------------------------

# ------------------RLE КОДЕР И ДЕКОДЕР---------------------
def rle_coder(input_array):
    compressed_line = []  # Создаем пустой список для хранения закодированной строки
    run_length = 1  # Инициализируем счетчик повторений текущего символа

    # Проходим по всем элементам массива
    for i in range(len(input_array)):
        # Если текущий элемент массива отрицателен, добавляем специальный символ перед символом
        if input_array[i] < 0:
            current_value = '减' + chr(-input_array[i])
        else:
            current_value = chr(input_array[i])

        # Если следующий символ равен текущему, увеличиваем счетчик повторений
        if i + 1 < len(input_array) and input_array[i + 1] == input_array[i]:
            run_length += 1
        else:
            # Если следующий символ не равен текущему, добавляем текущий символ и его количество повторений в результат
            if run_length > 1:
                compressed_line.append('重')  # Добавляем специальный символ для обозначения повторений
                compressed_line.append(chr(run_length))  # Добавляем количество повторений
            compressed_line.append(current_value)  # Добавляем текущий символ
            run_length = 1  # Сбрасываем счетчик повторений для нового символа

    # Преобразуем список в строку и возвращаем закодированную строку
    return ''.join(compressed_line)
def rle_decoder(input_string):
    decoded_array = []  # Создаем пустой массив для раскодированных чисел
    index_string = 0  # Инициализируем индекс для прохода по закодированной строке
    length_of_string = len(input_string)  # Получаем длину закодированной строки

    # Проходим по всей закодированной строке
    while index_string < length_of_string:
        # Если встречаем символ 'ཋ', значит перед нами повторяющиеся символы
        if input_string[index_string] == '重':
            # Читаем количество повторений из следующего символа
            count = ord(input_string[index_string + 1])
            index_string += 2
        else:
            # Если символ не 'ཋ', значит повторений нет
            count = 1

        # Если встречаем символ 'ᇗ', значит перед нами отрицательное число
        if input_string[index_string] == '减':
            # Читаем отрицательное число из следующего символа
            number = -ord(input_string[index_string + 1])
            index_string += 2
        else:
            # Если символ не '减', значит перед нами положительное число
            number = ord(input_string[index_string])
            index_string += 1

        # Добавляем число в раскодированный массив нужное количество раз
        decoded_array.extend([number] * count)

    return np.array(decoded_array)
# ----------------------------------------------------------

def txt_file_save(array, file_name):
    # Открываем файл
    with open(file_name, 'w') as file:
        # Перебираем все пиксели в изображении
        for row in array:
            for pixel in row:
                # Записываем значения каналов YCbCr в файл, разделяя их пробелами
                file.write(f"{pixel[0]} {pixel[1]} {pixel[2]} ")
            # Добавляем перенос строки после каждой строки пикселей
            file.write("\n")

def whole_algorithm__(image_filepath):
    # Чтение изображения и сохранение массива РГБ в текстовый файл
    image = Image.open(image_filepath)
    image_in_rgb = image.convert("RGB")
    image_in_rgb_array = np.array(image_in_rgb)
    width, height = image.size
    txt_file_save(image_in_rgb_array, 'step_0_RGB_image.txt')
    print(f"=========================================СЖАТИЕ==============================================")
    print(f"Массив RGB записан в файл 'step_0_RGB_image.txt'.")
    print(f"Размер файла = {os.path.getsize('step_0_RGB_image.txt')/1024} килобайт.")
    print(f"  ")

    # Преобразование изображения из формата РГБ в YCbCr и сохранение в текстовый файл
    image_in_ycbcr_array = RGB_in_YCbCr(image_in_rgb_array)
    txt_file_save(image_in_ycbcr_array, 'step_1_YCbCr_image.txt')
    print(f"Массив YCbCr записан в файл 'step_1_YCbCr_image.txt'.")
    print(f"Размер файла = {os.path.getsize('step_1_YCbCr_image.txt')/1024} килобайт.")
    print(f"  ")

    # Записываем Y, Cb, Cr значения по разным массивам
    Y, Cb, Cr = image_in_ycbcr_array[:, :, 0], image_in_ycbcr_array[:, :, 1], image_in_ycbcr_array[:, :, 2]

    # Даунсэмплинг массивов Cb Cr методом замены блоков на среднее значение
    downsampling_of_Cb = downsampling(Cb, compression_ratio_height, compression_ratio_width, 2)
    downsampling_of_Cr = downsampling(Cr, compression_ratio_height, compression_ratio_width, 2)
    with open('step_2_downsampling.txt', 'w') as file:
        file.write(' '.join(map(str, Y)) + '\n')
        file.write(' '.join(map(str, downsampling_of_Cb)) + '\n')
        file.write(' '.join(map(str, downsampling_of_Cr)) + '\n')
        file.write("\n")
    print(f"Массивы YCbCr после даунсэмплинга записаны в файл 'step_2_downsampling.txt'.")
    print(f"Размер файла = {os.path.getsize('step_2_downsampling.txt')/1024} килобайт.")
    print(f"  ")

    # Дискретно-косинусное преобразование
    Y_blocks = image_to_blocks(Y, width, height)
    Cb_blocks = image_to_blocks(downsampling_of_Cb, int(width / compression_ratio_width),
                                int(height / compression_ratio_height))
    Cr_blocks = image_to_blocks(downsampling_of_Cr, int(width / compression_ratio_width),
                                int(height / compression_ratio_height))
    dct_Y_blocks = np.array([discrete_cosine_transform(block) for block in Y_blocks])
    dct_Cb_blocks = np.array([discrete_cosine_transform(block) for block in Cb_blocks])
    dct_Cr_blocks = np.array([discrete_cosine_transform(block) for block in Cr_blocks])
    # Запись в файл
    Y_after_dct = blocks_to_image(dct_Y_blocks, width, height)
    Cb_after_dct = blocks_to_image(dct_Cb_blocks, int(width/compression_ratio_width),
                                   int(height/compression_ratio_height))
    Cr_after_dct = blocks_to_image(dct_Cr_blocks, int(width/compression_ratio_width),
                                   int(height/compression_ratio_height))
    with open('step_3_dct.txt', 'w') as file:
        file.write(' '.join(map(str, Y_after_dct)) + '\n')
        file.write(' '.join(map(str, Cb_after_dct)) + '\n')
        file.write(' '.join(map(str, Cr_after_dct)) + '\n')
        file.write("\n")
    print(f"Массивы YCbCr после ДКП записаны в файл 'step_3_dct.txt'.")
    print(f"Размер файла = {os.path.getsize('step_3_dct.txt')/1024} килобайт.")
    print(f"  ")

    # Квантование
    quantization_Y = np.array([quantization(block, quantization_matrix(quality)) for block in dct_Y_blocks])
    quantization_Cb = np.array([quantization(block, quantization_matrix(quality)) for block in dct_Cb_blocks])
    quantization_Cr = np.array([quantization(block, quantization_matrix(quality)) for block in dct_Cr_blocks])

    Y_merged = blocks_to_image(quantization_Y, width, height)
    Cb_merged = blocks_to_image(quantization_Cb, int(width/compression_ratio_width),
                                    int(height/compression_ratio_height))
    Cr_merged = blocks_to_image(quantization_Cr, int(width/compression_ratio_width),
                                    int(height/compression_ratio_height))
    with open('step_4_quantization.txt', 'w') as file:
        file.write(' '.join(map(str, Y_merged)) + '\n')
        file.write(' '.join(map(str, Cb_merged)) + '\n')
        file.write(' '.join(map(str, Cr_merged)) + '\n')
        file.write("\n")
    print(f"Массивы YCbCr после квантования (качество {quality}) записаны в файл 'step_4_quantization.txt'.")
    print(f"Размер файла = {os.path.getsize('step_4_quantization.txt')/1024} килобайт.")
    print(f"  ")

    # Преобразование матриц в строку зиг-заг обходом
    Y_after_zigzag = straight_zigzag(Y_merged)
    Cb_after_zigzag = straight_zigzag(Cb_merged)
    Cr_after_zigzag = straight_zigzag(Cr_merged)

    # Сжатие полученных строк алгоритмом RLE
    Y_after_rle = rle_coder(Y_after_zigzag)
    Cb_after_rle = rle_coder(Cb_after_zigzag)
    Cr_after_rle = rle_coder(Cr_after_zigzag)
    rle_YCbCr = Y_after_rle + '$' + Cb_after_rle + '$' + Cr_after_rle
    with open('step_5_rle.txt', 'w', encoding='utf-8') as file:
        file.write(rle_YCbCr)
    print(f"Массивы YCbCr после сжатия RLE записаны в файл 'step_5_rle.txt'.")
    print(f"Размер файла = {os.path.getsize('step_5_rle.txt')/1024} килобайт.")
    print(f"  ")
    print(f"=============================================================================================")
    print(f"Сжатие изображения завершено. Файлы сохранены в директории проекта. Начинается декодирование.")
    print(f"=============================================================================================")

    # Декодирование RLE
    Y_decode_rle = rle_decoder(Y_after_rle)
    Cb_decode_rle = rle_decoder(Cb_after_rle)
    Cr_decode_rle = rle_decoder(Cr_after_rle)
    print(f"...")

    # Обратный проход зиг-загом
    Y_after_back_zigzag = back_zigzag(Y_decode_rle, height, width)
    Cb_after_back_zigzag = back_zigzag(Cb_decode_rle, int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    Cr_after_back_zigzag = back_zigzag(Cr_decode_rle, int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    print(f"...")

    # Обратное квантование
    Y_decode_blocks = image_to_blocks(Y_after_back_zigzag, width, height)
    Cb_decode_blocks = image_to_blocks(Cb_after_back_zigzag,int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    Cr_decode_blocks = image_to_blocks(Cr_after_back_zigzag,int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    dequantization_Y = dequantization(Y_decode_blocks, quantization_matrix(quality))
    dequantization_Cb = dequantization(Cb_decode_blocks, quantization_matrix(quality))
    dequantization_Cr = dequantization(Cr_decode_blocks, quantization_matrix(quality))
    print(f"...")

    # Обратное ДКП
    Y_decode_dct = np.array([reverse_discrete_cosine_transform(block) for block in dequantization_Y])
    Cb_decode_dct = np.array([reverse_discrete_cosine_transform(block) for block in dequantization_Cb])
    Cr_decode_dct = np.array([reverse_discrete_cosine_transform(block) for block in dequantization_Cr])
    print(f"...")

    # Апсэмплинг
    Y_decode_merged = blocks_to_image(Y_decode_dct, width, height)
    Cb_decode_merged = blocks_to_image(Cb_decode_dct, int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    Cr_decode_merged = blocks_to_image(Cr_decode_dct, int(width/compression_ratio_width),
                                       int(height/compression_ratio_height))
    Cb_upsampled = upsampling(Cb_decode_merged, compression_ratio_height, compression_ratio_width)
    Cr_upsampled = upsampling(Cr_decode_merged, compression_ratio_height, compression_ratio_width)
    print(f"...")

    # Преобразование в RGB
    rgb_decode = YCbCr_in_RGB(Y_decode_merged, Cb_upsampled, Cr_upsampled)
    print(f"...")

    image_decoded = Image.fromarray(rgb_decode, 'RGB')
    image_decoded.save('decoded_image.jpg')
    # проверка изображений на кодирование
    fig, axs = plt.subplots(1, 2)
    # Отображение изображений на Axes
    axs[0].imshow(image)
    axs[0].set_title("ДО СЖАТИЯ")
    axs[1].imshow(image_decoded)
    axs[1].set_title("ПОСЛЕ СЖАТИЯ")
    # Установка расстояния между изображениями
    plt.tight_layout()
    # Отображение Figure
    fig.suptitle("СРАВНЕНИЕ ИЗОБРАЖЕНИй")
    plt.show()

if __name__ == "__main__":
    whole_algorithm__("random_image.jpg")