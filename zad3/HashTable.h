#ifndef HASHTABLE_H
#define HASHTABLE_H

using namespace std;

#include <iostream>
#include <string> 
#include <fstream>
#include <stdexcept>

// Константа для определения размера таблицы
const int TABLE_SIZE = 10;

// Структура для представления узла в хеш-таблице
struct NodeH {
    string key;     // Ключ узла
    string value;   // Значение узла
    NodeH* next;    // Указатель на следующий узел (для цепочечного разрешения коллизий)

    // Конструктор для инициализации узла с заданным ключом и значением
    NodeH(const string &k, const string &v) : key(k), value(v), next(nullptr) {}
};

// Класс для реализации хеш-таблицы
class HashTable {
public:
    // Конструктор хеш-таблицы
    HashTable();
    
    // Деструктор хеш-таблицы для очистки ресурсов
    ~HashTable();
    
    void Hinsert(const string &key, const string &value);   // Метод для добавления или обновления элемента в хеш-таблице
    bool Hget(const string &key, string &value) const;    // Метод для получения значения по ключу
    bool Hdel(const string &key);   // Метод для удаления элемента по ключ
    void Hprint() const;    // Метод для отображения всех элементов в хеш-таблице
    void HreadFromFile(const std::string &filename);
    void HwriteToFile(const std::string &filename) const;

    // Новые методы для операций над множествами
    void Hunion(const HashTable& other);    // обьединение
    void Hintersection(const HashTable& other); // пересечение
    void Hdifference(const HashTable& other);   // разность

private:
    // Хеш-функция для получения индекса по ключу
    size_t hash(const string &key) const;

    // Массив указателей на узлы для представления хеш-таблицы
    NodeH* table[TABLE_SIZE];
};

// Конструктор хеш-таблицы
HashTable::HashTable() {
    // Инициализируем каждый элемент массива таблицы указателем NULL
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

// Деструктор хеш-таблицы
HashTable::~HashTable() {
    // Освобождаем память, выделенную для каждого узла в таблице
    for (int i = 0; i < TABLE_SIZE; ++i) { 
        NodeH* current = table[i]; // Указатель на текущий узел
        while (current) {
            NodeH* temp = current; // Временный указатель на удаляемый узел
            current = current->next; // Переходим на следующий узел
            delete temp; // Удаляем текущий узел
        }
    }
}

// Хеш-функция для получения индекса в таблице по ключу
size_t HashTable::hash(const string &key) const {
    size_t hashValue = 0; // Начальное значение хеша
    for (char ch : key) {
        // Применяем простой алгоритм хеширования
        hashValue = (hashValue * 31 + ch) % TABLE_SIZE;
    }
    return hashValue; // Возвращаем рассчитанный индекс
}

// Функция для добавления или обновления элемента в хеш-таблице
void HashTable::Hinsert(const string &key, const string &value) {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* newNode = new NodeH(key, value); // Создаем новый узел

    // Если в таблице нет узла по этому индексу, добавляем новый узел
    if (!table[index]) {
        table[index] = newNode;
    } else {
        NodeH* current = table[index]; // Начинаем с первого узла в цепочке
        while (current->next) {
            // Проверяем, существует ли уже ключ
            if (current->key == key) {
                current->value = value; // Обновляем значение
                delete newNode; // Избегаем утечки памяти
                return; // Завершаем выполнение функции
            }
            current = current->next; // Переходим к следующему узлу
        }
        // Проверяем последний узел (если у нас несколько узлов в цепочке)
        if (current->key == key) {
            current->value = value; // Обновляем значение
            delete newNode;         // Избегаем утечки памяти
        } else {
            current->next = newNode;  // Добавляем новый узел в конец цепочки
        }
    }
}

// Функция для получения значения по ключу
bool HashTable::Hget(const string &key, string &value) const {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* current = table[index]; // Начинаем с первого узла в цепочке
    while (current) {
        // Проверяем, совпадает ли ключ
        if (current->key == key) {
            value = current->value; // Возвращаем значение
            return true; // Успешно найдено
        }
        current = current->next; // Переходим к следующему узлу
    }
    return false; // Ключ не найден
}

// Функция для удаления элемента по ключу
bool HashTable::Hdel(const string &key) {
    size_t index = hash(key); // Вычисляем индекс для данного ключа
    NodeH* current = table[index]; // Начинаем с первого узла
    NodeH* previous = nullptr; // Предыдущий узел для отслеживания ссылок

    while (current) {
        // Проверяем, совпадает ли ключ
        if (current->key == key) {
            if (previous) {
                previous->next = current->next; // Удаляем узел, если он не первый
            } else {
                table[index] = current->next; // Удаляем первый узел (голову)
            }
            delete current; // Освобождаем память
            return true; // Успешно удалено
        }
        previous = current; // Обновляем предыдущий узел
        current = current->next; // Переходим к следующему узлу
    }
    return false; // Ключ не найден
}

// Функция для отображения всех элементов таблицы
void HashTable::Hprint() const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cout << i << ": "; // Печатаем индекс
        NodeH* current = table[i]; // Указатель на текущий узел
        while (current) {
            cout << "{" << current->key << ": " << current->value << "} "; // Печатаем ключ-значение
            current = current->next; // Переходим к следующему узлу
        }
        cout << endl; // Переходим на новую строку
    }
}

// Функция для считывания хеш-таблицы из файла
void HashTable::HreadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для чтения.");
    }

    string key;
    while (file >> key) {
        Hinsert(key, ""); // Вставляем каждый ключ в хеш-таблицу с пустым значением
    }

    file.close(); // Закрываем файл
}

// Функция для записи хеш-таблицы в файл
void HashTable::HwriteToFile(const string &filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для записи.");
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        NodeH* current = table[i];
        while (current) {
            file << current->key << endl; // Записываем только ключи
            current = current->next; // Переходим к следующему узлу
        }
    }

    file.close(); // Закрываем файл
}

// Метод для объединения множеств
void HashTable::Hunion(const HashTable& other) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        NodeH* current = other.table[i];
        while (current) {
            Hinsert(current->key, ""); // Добавляем элементы из другого множества
            current = current->next;
        }
    }
}

// Метод для пересечения множеств
void HashTable::Hintersection(const HashTable& other) {
    HashTable temp; // Временная хеш-таблица для хранения результата
    for (int i = 0; i < TABLE_SIZE; ++i) {
        NodeH* current = table[i];
        while (current) {
            string value;
            if (other.Hget(current->key, value)) {
                temp.Hinsert(current->key, ""); // Добавляем элементы, которые есть в обоих множествах
            }
            current = current->next;
        }
    }
    // Копируем результат во временную таблицу
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = temp.table[i];
        temp.table[i] = nullptr; // Очищаем временную таблицу
    }
}

// Метод для разности множеств
void HashTable::Hdifference(const HashTable& other) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        NodeH* current = table[i];
        NodeH* previous = nullptr;
        while (current) {
            string value;
            if (other.Hget(current->key, value)) {
                if (previous) {
                    previous->next = current->next; // Удаляем элемент, если он есть в другом множестве
                } else {
                    table[i] = current->next; // Удаляем первый узел (голову)
                }
                NodeH* temp = current;
                current = current->next;
                delete temp; // Освобождаем память
            } else {
                previous = current;
                current = current->next;
            }
        }
    }
}

#endif