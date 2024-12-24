// 1. Подключение библиотек и объявление пространства имен
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace CppCLRWinFormsProject {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    // 2. Определение класса Form1
    public ref class Form1 : public System::Windows::Forms::Form
    {
    private:
        static List<String^>^ wordsToGuess; // Список слов для угадывания
        array<TextBox^>^ textBoxes; // Массив текстовых полей
        Panel^ menuPanel; // Панель меню
        Panel^ gamePanel; // Игровая панель
        Button^ playButton; // Кнопка "Играть"
        System::Windows::Forms::Label^ label2; // Заголовок игры
        int attemptCount = 0; // Счётчик попыток
        String^ correctWord; // Переменная для хранения правильного слова

    public:
        // 3. Конструктор
        Form1(void)
        {
            InitializeWords(); // Инициализация списка слов
        }

    protected:
        // 4. Деструктор
        ~Form1()
        {
            if (components)
            {
                delete components; // Освобождение ресурсов
            }
        }

    private:
        System::ComponentModel::IContainer^ components; // Контейнер компонентов

        // 5. Инициализация слов
        void InitializeWords()
        {
            wordsToGuess = gcnew List<String^>();
            wordsToGuess->Add("Актёр");
            // Добавьте больше слов при необходимости
            SelectRandomWord(); // Выбор случайного слова для угадывания
        }

        // 6. Выбор случайного слова
        void SelectRandomWord()
        {
            Random^ rand = gcnew Random();
            int index = rand->Next(wordsToGuess->Count);
            correctWord = wordsToGuess[index]; // Установка правильного слова
        }
    };
}