import os

class MTF:
    def encode(self, in_str: str, alphabet):
        out_lst = []
        alph = list(alphabet)
        for char in in_str:
            index = alph.index(char)
            out_lst.append(index)
            alph.pop(index)
            alph.insert(0, char)
        return out_lst

    def decode(self, in_lst, alphabet):
        decoded_string = ""
        alph = list(alphabet)
        for index in in_lst:
            char = alph[index]
            decoded_string += char
            alph.pop(index)
            alph.insert(0, char)
        return decoded_string

def find_alphabet(string):
    return sorted(set(string))

def calculate_compression_ratio(original_size, compressed_size):
    return compressed_size / original_size if compressed_size > 0 else 0

def main():
    input_file_name = 'enwik7.txt'
    encoded_file_name = "encoded_data.txt"
    decoded_file_name = "decoded.txt"

    with open(input_file_name, 'r', encoding='utf-8') as input_file:
        data = input_file.read()

    # Создаем алфавит на основе входного текста
    alphabet = find_alphabet(data)
    mtf = MTF()

    # Кодируем текст
    encoded_data = mtf.encode(data, alphabet)

    # Сохраняем закодированные данные
    with open(encoded_file_name, "w", encoding='utf-8') as encoded_file:
        encoded_file.write(' '.join(map(str, encoded_data)))

    # Читаем закодированные данные для декодирования
    with open(encoded_file_name, "r", encoding='utf-8') as encoded_file:
        enc_data = list(map(int, encoded_file.read().split()))

    # Декодируем данные обратно в текст
    decoded_data = mtf.decode(enc_data, alphabet)
    with open(decoded_file_name, "w", encoding='utf-8') as decoded_file:
        decoded_file.write(decoded_data)

    # Расчет коэффициента сжатия
    original_size = os.path.getsize(input_file_name)
    compressed_size = os.path.getsize(encoded_file_name)
    compression_ratio = calculate_compression_ratio(original_size, compressed_size)
    print(f"Исходный текст сохранён в '{input_file_name}'")
    print(f"Закодированный текст сохранён в '{encoded_file_name}'")
    print(f"Декодированный текст сохранён в '{decoded_file_name}'")
    print(f"Коэффициент сжатия: {compression_ratio:.2f}")

if __name__ == "__main__":
    main()