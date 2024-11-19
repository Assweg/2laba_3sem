#include "hashtable.h"

bool areIsomorphic(const string &str1, const string &str2) {
    // Если длины строк не совпадают, они не могут быть изоморфными
    if (str1.length() != str2.length()) {
        return false;
    }

    HashTable mapping1, mapping2; // Хеш-таблицы для сопоставления символов

    for (size_t i = 0; i < str1.length(); ++i) {
        char char1 = str1[i];
        char char2 = str2[i];

        string value;
        if (mapping1.Hget(string(1, char1), value)) {
            // Если символ из str1 уже сопоставлен, проверяем, совпадает ли сопоставление
            if (value != string(1, char2)) {
                return false;
            }
        } else {
            // Если символ из str1 еще не сопоставлен, добавляем сопоставление
            mapping1.Hinsert(string(1, char1), string(1, char2));
        }

        if (mapping2.Hget(string(1, char2), value)) {
            // Если символ из str2 уже сопоставлен, проверяем, совпадает ли сопоставление
            if (value != string(1, char1)) {
                return false;
            }
        } else {
            // Если символ из str2 еще не сопоставлен, добавляем сопоставление
            mapping2.Hinsert(string(1, char2), string(1, char1));
        }
    }

    return true; // Если все проверки пройдены, строки изоморфны
}

int main() {
    string str1, str2;

    // Запрашиваем у пользователя ввод двух строк
    cout << "Введите первую строку: ";
    cin >> str1;
    cout << "Введите вторую строку: ";
    cin >> str2;

    // Проверяем, являются ли строки изоморфными
    if (areIsomorphic(str1, str2)) {
        cout << "Строки изоморфны." << endl;
    } else {
        cout << "Строки не изоморфны." << endl;
    }

    return 0;
}