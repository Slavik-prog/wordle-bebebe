// 1. Подключение библиотек и объявление пространства имен
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Wordle {

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
            InitializeComponent(); // Инициализация компонентов формы
            CreateMenu(); // Создание главного меню
            CreateGamePanel(); // Создание игровой панели
            SubscribeToKeyDownEvents(); // Подписка на события нажатия клавиш
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


        // 7. Инициализация компонентов формы
        void InitializeComponent(void)
        {
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // label2
            // 
            this->label2->BackColor = System::Drawing::Color::LightSeaGreen;
            this->label2->Font = (gcnew System::Drawing::Font(L"Rubik ExtraBold", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->label2->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
            this->label2->Location = System::Drawing::Point(278, 37);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(543, 107);
            this->label2->TabIndex = 0;
            this->label2->Text = L"GUESS THE WORD";
            this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            this->label2->UseWaitCursor = true;
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::Gainsboro;
            this->ClientSize = System::Drawing::Size(1140, 666);
            this->Controls->Add(this->label2);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog; // Отключить изменение размера окна
            this->MaximizeBox = false; // Отключить кнопку максимизации
            this->MaximumSize = System::Drawing::Size(1140, 666); // Установить максимальный размер окна
            this->MinimumSize = System::Drawing::Size(1140, 666); // Установить минимальный размер окна
            this->Name = L"Form1";
            this->Text = L"Wordle";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->ResumeLayout(false);
        }

        // 8. Создание главного меню
        void CreateMenu()
        {
            if (menuPanel != nullptr) {
                this->Controls->Remove(menuPanel); // Удаление старой панели меню, если есть
            }

            menuPanel = gcnew Panel();
            menuPanel->Size = this->ClientSize; // Размер панели равен размеру формы
            menuPanel->BackColor = System::Drawing::Color::Gainsboro; // Цвет фона панели меню

            playButton = gcnew Button();
            playButton->Text = "Играть"; // Текст кнопки
            playButton->Location = System::Drawing::Point(480, 300); // Позиция кнопки
            playButton->Size = System::Drawing::Size(150, 50); // Размер кнопки
            playButton->Click += gcnew EventHandler(this, &Form1::PlayButton_Click); // Подписка на событие нажатия

            // Добавляем кнопку на панель меню
            menuPanel->Controls->Add(playButton);
            this->Controls->Add(menuPanel); // Добавление панели меню на форму
        }

        // 9. Создание игровой панели
        void CreateGamePanel()
        {
            gamePanel = gcnew Panel();
            gamePanel->Size = this->ClientSize; // Размер панели равен размеру формы
            gamePanel->BackColor = System::Drawing::Color::PowderBlue; // Цвет фона игровой панели
            CreateTextBoxes(); // Создание текстовых полей
            gamePanel->Controls->AddRange(textBoxes); // Добавление текстовых полей на игровую панель
            gamePanel->Visible = false; // Скрыть игровую панель по умолчанию
            this->Controls->Add(gamePanel); // Добавление игровой панели на форму
        }

        // 10. Создание текстовых полей
        void CreateTextBoxes()
        {
            textBoxes = gcnew array<TextBox^>(30); // Массив из 30 текстовых полей
            int x = 350, y = 180; // Начальные координаты для текстовых полей
            int width = 80, height = 70; // Ширина и высота текстовых полей
            int spacing = 1; // Промежуток между текстовыми полями
            

            for (int i = 0; i < 6; i++) // 6 строк для попыток
            {
                for (int j = 0; j < 5; j++) // Цикл по столбцам
                {
                    int index = i * 5 + j; // Индекс текстового поля
                    textBoxes[index] = CreateTextBox(x + j * (width + spacing), y + i * (height + spacing)); // Создание текстового поля
                }
            }
        }

        // 11. Создание отдельного текстового поля
        TextBox^ CreateTextBox(int x, int y)
        {
            TextBox^ textBox = gcnew TextBox(); // Создание текстового поля
            textBox->Location = System::Drawing::Point(x, y); // Позиция текстового поля
            textBox->Size = System::Drawing::Size(70, 40); // Размер текстового поля
            textBox->Font = (gcnew System::Drawing::Font(L"Rubik Medium", 36)); // Шрифт текстового поля
            textBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center; // Выравнивание текста
            textBox->MaxLength = 1; // Максимальная длина ввода
            textBox->CharacterCasing = CharacterCasing::Upper; // Установка ввода только в верхнем регистре
            textBox->ReadOnly = false; // Установить возможность ввода текста
            textBox->Click += gcnew EventHandler(this, &Form1::TextBox_Click); // Запрет выбора текстового поля
            return textBox; // Возврат созданного текстового поля
        }

        // 12. Обработка нажатия кнопки "Играть"
        void PlayButton_Click(Object^ sender, EventArgs^ e)
        {
            menuPanel->Visible = false; // Скрыть панель меню
            gamePanel->Visible = true; // Показать игровую панель
            textBoxes[0]->Focus(); // Установка фокуса на первое текстовое поле
        }

         // 13. Обработка нажатия на текстовое поле
         void TextBox_Click(Object^ sender, EventArgs^ e) {
            TextBox^ clickedTextBox = (TextBox^)sender;
            int index = Array::IndexOf(textBoxes, clickedTextBox);
            if (index >= attemptCount * 5 && index < (attemptCount + 1) * 5) {
                textBoxes[attemptCount * 5]->Focus(); // Установка фокуса на текущее текстовое поле
            }
        }

         // 14. Подписка на события нажатия клавиш
         void SubscribeToKeyDownEvents()
         {
            for (int i = 0; i < 30; i++)
             {
                 textBoxes[i]->KeyDown += gcnew KeyEventHandler(this, &Form1::TextBox_KeyDown);
             }
         }
         // 15. Обработка нажатия клавиш в текстовом поле
         void TextBox_KeyDown(Object^ sender, KeyEventArgs^ e) {
             TextBox^ changedTextBox = (TextBox^)sender;
             int index = Array::IndexOf(textBoxes, changedTextBox);

             if (e->KeyCode == Keys::Enter) {
                 CheckGuessedWord(); // Проверка введенного слова
                 e->SuppressKeyPress = true;
             }
             else if (e->KeyCode == Keys::Back) {
                 if (changedTextBox->Text->Length == 0 && index > 0) {
                     textBoxes[index - 1]->Focus();
                 }
             }
             else if (e->KeyCode == Keys::Space) {
                 e->SuppressKeyPress = true;
             }

             // Запрещаем фокусировку на текстовых полях вне текущей строки
             if (index < attemptCount * 5 || index >= (attemptCount + 1) * 5) {
                 e->SuppressKeyPress = true;
                 textBoxes[attemptCount * 5]->Focus();
             }
         }

         // 16. Проверка угаданного слова
         void CheckGuessedWord() {
             if (attemptCount < 6 && IsCompleteWord()) {
                 String^ guessedWord = "";
                 for (int i = attemptCount * 5; i < (attemptCount + 1) * 5; i++) {
                     guessedWord += textBoxes[i]->Text;
                     textBoxes[i]->ReadOnly = true; // Блокировка текстовых полей после ввода слова
                 }


                 if (guessedWord->ToUpper() == correctWord->ToUpper()) {
                     MessageBox::Show("Вы угадали слово!", "Поздравляем!", MessageBoxButtons::OK, MessageBoxIcon::Information);
                     /*ClearGame();
                     ShowMenu();*/
                 }
                 else {
                     HighlightIncorrectLetters(guessedWord);
                     attemptCount++;

                     if (attemptCount < 6) {
                         textBoxes[attemptCount * 5]->Focus();
                     }
                     else {
                         MessageBox::Show("Вы исчерпали все попытки! Правильное слово: " + correctWord, "Игра окончена", MessageBoxButtons::OK, MessageBoxIcon::Information);
                         /*ClearGame();
                         ShowMenu();*/
                     }
                 }
             }
         }

         // 17. Выделение неправильных букв
         void HighlightIncorrectLetters(String^ guessedWord) {
             array<wchar_t>^ guessedChars = guessedWord->ToUpper()->ToCharArray();
             array<wchar_t>^ correctChars = correctWord->ToUpper()->ToCharArray();
             array<bool>^ matchedCorrect = gcnew array<bool>(5); // Массив для отслеживания совпадений в правильном слове

             for (int i = 0; i < 5; i++) {
                 wchar_t guessedChar = guessedChars[i];
                 wchar_t correctChar = correctChars[i];

                 if (guessedChar == correctChar) {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightGreen; // Правильная буква на правильном месте
                     matchedCorrect[i] = true; // Отмечаем совпадение на этом месте
                 }
                 else {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::White; // Остальные ячейки белого цвета
                     matchedCorrect[i] = false; // Нет совпадения
                 }
             }

             // Второй проход для отметки букв, которые есть в слове, но не на правильной позиции
             for (int i = 0; i < 5; i++) {
                 if (textBoxes[attemptCount * 5 + i]->BackColor == System::Drawing::Color::White) { // Только для не зеленых ячеек
                     for (int j = 0; j < 5; j++) {
                         if (!matchedCorrect[j] && guessedChars[i] == correctChars[j]) {
                             textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightGoldenrodYellow; // Буква есть в слове, но не на месте
                             matchedCorrect[j] = true; // Отмечаем, что эта буква использована
                             break;
                         }
                     }
                 }
             }


             // Третий проход для отметки букв, которые отсутствуют в слове
             for (int i = 0; i < 5; i++) {
                 if (textBoxes[attemptCount * 5 + i]->BackColor == System::Drawing::Color::White) {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightCoral; // Буква отсутствует в слове
                 }
             }
         }

         // 18. Обработка изменения текста в текстовом поле
         void TextBox_TextChanged(Object^ sender, EventArgs^ e) {
             TextBox^ changedTextBox = (TextBox^)sender;

             if (changedTextBox->Text->Length == 1) {
                 int index = Array::IndexOf(textBoxes, changedTextBox);

                 if (index < (attemptCount + 1) * 5 - 1) {
                     textBoxes[index + 1]->Focus();
                 }
             }
         }

         // 19. Проверка, заполнено ли слово
         bool IsCompleteWord()
         {
             for (int i = attemptCount * 5; i < (attemptCount + 1) * 5; i++)
             {
                 if (textBoxes[i]->Text->Length == 0)
                 {
                     return false; // Если хотя бы одно пустое, возвращаем false
                 }
             }
             return true; // Если все заполнены, возвращаем true
         }

        private:
            // Дополнительные настройки при загрузке формы
            System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e)
            {
                // Дополнительные настройки при загрузке формы, если необходимо
            }
    };
}
