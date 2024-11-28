#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

using namespace std;

// Структура Note
struct Note {
    char lastName[50];
    char firstName[50];
    char phoneNumber[15];
    int birthday[3]; // [0] - день, [1] - місяць, [2] - рік
};

// Функція введення даних у файл
void inputToFile(const string& filename) {
    ofstream outFile(filename, ios::binary | ios::app);
    if (!outFile) {
        cerr << "Не вдалося відкрити файл для запису." << endl;
        return;
    }

    char choice;
    do {
        Note note;

        cout << "Введіть прізвище: ";
        cin.ignore();
        cin.getline(note.lastName, 50);

        cout << "Введіть ім'я: ";
        cin.getline(note.firstName, 50);

        cout << "Введіть номер телефону: ";
        cin.getline(note.phoneNumber, 15);

        cout << "Введіть дату народження (день, місяць, рік): ";
        cin >> note.birthday[0] >> note.birthday[1] >> note.birthday[2];

        outFile.write(reinterpret_cast<char*>(&note), sizeof(Note));

        cout << "Додати ще один запис? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    outFile.close();
}

// Функція сортування записів за датами народження
void sortFileByBirthday(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Не вдалося відкрити файл для сортування." << endl;
        return;
    }

    file.seekg(0, ios::end);
    int recordCount = file.tellg() / sizeof(Note);

    for (int i = 0; i < recordCount - 1; ++i) {
        for (int j = 0; j < recordCount - i - 1; ++j) {
            Note current, next;

            file.seekg(j * sizeof(Note), ios::beg);
            file.read(reinterpret_cast<char*>(&current), sizeof(Note));

            file.seekg((j + 1) * sizeof(Note), ios::beg);
            file.read(reinterpret_cast<char*>(&next), sizeof(Note));

            bool needSwap = false;
            if (current.birthday[2] > next.birthday[2] ||
                (current.birthday[2] == next.birthday[2] && current.birthday[1] > next.birthday[1]) ||
                (current.birthday[2] == next.birthday[2] && current.birthday[1] == next.birthday[1] &&
                 current.birthday[0] > next.birthday[0])) {
                needSwap = true;
            }

            if (needSwap) {
                file.seekp(j * sizeof(Note), ios::beg);
                file.write(reinterpret_cast<char*>(&next), sizeof(Note));

                file.seekp((j + 1) * sizeof(Note), ios::beg);
                file.write(reinterpret_cast<char*>(&current), sizeof(Note));
            }
        }
    }

    file.close();
}

// Функція пошуку запису за номером телефону
void searchByPhoneNumber(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не вдалося відкрити файл для пошуку." << endl;
        return;
    }

    char searchPhone[15];
    cout << "Введіть номер телефону для пошуку: ";
    cin.ignore();
    cin.getline(searchPhone, 15);

    Note note;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        if (strcmp(note.phoneNumber, searchPhone) == 0) {
            cout << "Запис знайдено:\n";
            cout << "Прізвище: " << note.lastName << "\n";
            cout << "Ім'я: " << note.firstName << "\n";
            cout << "Номер телефону: " << note.phoneNumber << "\n";
            cout << "Дата народження: " << setw(2) << setfill('0') << note.birthday[0] << "."
                 << setw(2) << setfill('0') << note.birthday[1] << "." << note.birthday[2] << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Запис із вказаним номером телефону не знайдено." << endl;
    }

    inFile.close();
}
// Меню програми
void displayMenu() {
    cout << "Меню:\n";
    cout << "1. Додати записи у файл\n";
    cout << "2. Сортувати записи за датою народження\n";
    cout << "3. Шукати запис за номером телефону\n";
    cout << "4. Вийти\n";
    cout << "Виберіть опцію: ";
}

int main() {
    string filename;

    cout << "Введіть ім'я файлу: ";
    cin >> filename;

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                inputToFile(filename);
                break;
            case 2:
                sortFileByBirthday(filename);
                break;
            case 3:
                searchByPhoneNumber(filename);
                break;
            case 4:
                cout << "Вихід із програми." << endl;
                break;
            default:
                cout << "Некоректний вибір. Спробуйте ще раз." << endl;
        }
    } while (choice != 4);
    return 0;
}