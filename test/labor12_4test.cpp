#include <gtest/gtest.h>
#include <fstream>
#include <cstring>
#include <string>

// Структура Note
struct Note {
    char lastName[50];
    char firstName[50];
    char phoneNumber[15];
    int birthday[3]; // [0] - день, [1] - місяць, [2] - рік
};

// Функція для перевірки файлу після запису
void inputToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary | std::ios::app);
    Note note1 = {"Smith", "John", "1234567890", {15, 4, 1990}};
    Note note2 = {"Doe", "Jane", "0987654321", {23, 12, 1985}};
    outFile.write(reinterpret_cast<char*>(&note1), sizeof(Note));
    outFile.write(reinterpret_cast<char*>(&note2), sizeof(Note));
    outFile.close();
}

// Функція для перевірки пошуку записів у файлі
bool searchByPhoneNumber(const std::string& filename, const char* phoneNumber, Note& foundNote) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return false;

    Note note;
    while (inFile.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        if (strcmp(note.phoneNumber, phoneNumber) == 0) {
            foundNote = note;
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false;
}

// Тест перевірки запису у файл
TEST(FileTest, InputToFileTest) {
    std::string testFilename = "test_file.dat";
    std::remove(testFilename.c_str()); // Очистити файл перед тестом

    inputToFile(testFilename);

    // Перевірка записів у файлі
    std::ifstream inFile(testFilename, std::ios::binary);
    ASSERT_TRUE(inFile.is_open());

    Note note;
    inFile.read(reinterpret_cast<char*>(&note), sizeof(Note));
    EXPECT_STREQ(note.lastName, "Smith");
    EXPECT_STREQ(note.firstName, "John");
    EXPECT_STREQ(note.phoneNumber, "1234567890");
    EXPECT_EQ(note.birthday[0], 15);
    EXPECT_EQ(note.birthday[1], 4);
    EXPECT_EQ(note.birthday[2], 1990);

    inFile.read(reinterpret_cast<char*>(&note), sizeof(Note));
    EXPECT_STREQ(note.lastName, "Doe");
    EXPECT_STREQ(note.firstName, "Jane");
    EXPECT_STREQ(note.phoneNumber, "0987654321");
    EXPECT_EQ(note.birthday[0], 23);
    EXPECT_EQ(note.birthday[1], 12);
    EXPECT_EQ(note.birthday[2], 1985);

    inFile.close();
    std::remove(testFilename.c_str()); // Очистити файл після тесту
}

// Тест пошуку запису за номером телефону
TEST(FileTest, SearchByPhoneNumberTest) {
    std::string testFilename = "test_file.dat";
    std::remove(testFilename.c_str()); // Очистити файл перед тестом

    inputToFile(testFilename);

    Note foundNote;
    ASSERT_TRUE(searchByPhoneNumber(testFilename, "1234567890", foundNote));
    EXPECT_STREQ(foundNote.lastName, "Smith");
    EXPECT_STREQ(foundNote.firstName, "John");
    EXPECT_EQ(foundNote.birthday[0], 15);
    EXPECT_EQ(foundNote.birthday[1], 4);
    EXPECT_EQ(foundNote.birthday[2], 1990);

    ASSERT_TRUE(searchByPhoneNumber(testFilename, "0987654321", foundNote));
    EXPECT_STREQ(foundNote.lastName, "Doe");
    EXPECT_STREQ(foundNote.firstName, "Jane");
    EXPECT_EQ(foundNote.birthday[0], 23);
    EXPECT_EQ(foundNote.birthday[1], 12);
    EXPECT_EQ(foundNote.birthday[2], 1985);

    ASSERT_FALSE(searchByPhoneNumber(testFilename, "0000000000", foundNote));

    std::remove(testFilename.c_str()); // Очистити файл після тесту
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}