import os
# Функция для кодирования строки с использованием алгоритма RLE.
def rle_encode(data):
    encoding = ''  # Строка для хранения закодированных данных
    prev_char = ''  # Предыдущий символ
    count = 1  # Счетчик повторений символа

    if not data: return ''  # Если строка пустая, возвращаем пустую строку

    for char in data:
        if char != prev_char:
            if prev_char:
                encoding += str(count) + prev_char
            count = 1
            prev_char = char
        else:
            count += 1

    encoding += str(count) + prev_char
    return encoding

# Функция для декодирования строки, закодированной с использованием алгоритма RLE.
def decompress(data):
    result = ""  # Строка для хранения декодированных данных
    count = ""  # Счетчик для накопления числа повторений
    i = 0  # Индекс текущего символа в строке

    while i < len(data):
        if data[i].isdigit():
            count += data[i]
        else:
            result += data[i] * int(count)  # Повторяем символ int(count) раз
            count = ""  # Сбрасываем счетчик
        i += 1
    return result

# Функция для вычисления коэффициента сжатия.
def calculate_compression_ratio(original_file_path, compressed_file_path):
    original_size = os.path.getsize(original_file_path)  # Размер исходного файла
    compressed_size = os.path.getsize(compressed_file_path)  # Размер сжатого файла
    if compressed_size > 0:
        return compressed_size / original_size  # Возвращаем коэффициент сжатия
    else:
        return 0

# Главная функция, организующая процесс сжатия и декомпрессии.
def main():
    input_file_name = 'ru_test.txt'
    encoded_file_name = "encode_text.txt"
    decoded_file_name = "decod_text.txt"

    # Чтение исходных данных из файла
    with open(input_file_name, 'r', errors='ignore') as input_file:
        data = input_file.read()

    # Кодирование данных и запись в файл
    encoded_message = rle_encode(data)
    with open(encoded_file_name, "w", encoding='utf-8') as encoded_file:
        encoded_file.write(encoded_message)

    # Декодирование данных и запись в другой файл
    with open(encoded_file_name, "r", encoding='utf-8') as encoded_file:
        enc_data = encoded_file.read()
    dec_message = decompress(enc_data)
    with open(decoded_file_name, "w", encoding='utf-8') as decoded_file:
        decoded_file.write(dec_message)

    # Вычисление и вывод коэффициента сжатия
    compression_ratio = calculate_compression_ratio(input_file_name, encoded_file_name)
    print(f"Исходный текст сохранён в {input_file_name}")
    print(f"Сжатый текст сохранён в {encoded_file_name}")
    print(f"Декодированный текст сохранён в {decoded_file_name}")
    print(f"Коэффициент сжатия: {compression_ratio:.2f}")


if __name__ == "__main__":
    main()
