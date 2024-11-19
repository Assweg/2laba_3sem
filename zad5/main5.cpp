#include "CompleteBinaryTree.h"
#include <iostream>
#include <string>

using namespace std;

// Функция для удаления элемента из дерева
template <typename T>
void Tremove(CompleteBinaryTree<T>& tree, const T& value) {
    int index = -1;
    for (int i = 0; i < tree.size; i++) {
        if (tree.tree[i] == value) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Элемент не найден в дереве.\n";
        return;
    }

    // Заменяем удаляемый элемент последним элементом в дереве
    tree.tree[index] = tree.tree[tree.size - 1];
    tree.size--;

    // Восстанавливаем свойства бинарного дерева поиска
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < tree.size && tree.tree[leftChild] < tree.tree[smallest]) {
            smallest = leftChild;
        }
        if (rightChild < tree.size && tree.tree[rightChild] < tree.tree[smallest]) {
            smallest = rightChild;
        }

        if (smallest != index) {
            swap(tree.tree[index], tree.tree[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

int main() {
    int capacity;
    cout << "Введите максимальный размер дерева: ";
    cin >> capacity;

    CompleteBinaryTree<int> tree(capacity);

    while (true) {
        cout << "\nВыберите действие:\n";
        cout << "1. Вставить элемент\n";
        cout << "2. Удалить элемент\n";
        cout << "3. Найти элемент\n";
        cout << "4. Вывести дерево\n";
        cout << "5. Загрузить дерево из файла\n";
        cout << "6. Сохранить дерево в файл\n";
        cout << "7. Выход\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int value;
            cout << "Введите значение для вставки: ";
            cin >> value;
            tree.Tinsert(value);
        } else if (choice == 2) {
            int value;
            cout << "Введите значение для удаления: ";
            cin >> value;
            Tremove(tree, value);
        } else if (choice == 3) {
            int value;
            cout << "Введите значение для поиска: ";
            cin >> value;
            if (tree.Tsearch(value)) {
                cout << "Элемент найден в дереве.\n";
            } else {
                cout << "Элемент не найден в дереве.\n";
            }
        } else if (choice == 4) {
            tree.Tprint();
        } else if (choice == 5) {
            string filename;
            cout << "Введите имя файла для загрузки: ";
            cin >> filename;
            tree.TreadFromFile(filename);
        } else if (choice == 6) {
            string filename;
            cout << "Введите имя файла для сохранения: ";
            cin >> filename;
            tree.TwriteToFile(filename);
        } else if (choice == 7) {
            break;
        } else {
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}