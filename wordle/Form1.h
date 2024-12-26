// 1. ����������� ��������� � ���������� ������������ ����
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Wordle {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    // 2. ����������� ������ Form1
    public ref class Form1 : public System::Windows::Forms::Form
    {
    private:
        static List<String^>^ wordsToGuess; // ������ ���� ��� ����������
        array<TextBox^>^ textBoxes; // ������ ��������� �����
        Panel^ menuPanel; // ������ ����
        Panel^ gamePanel; // ������� ������
        Button^ playButton; // ������ "������"
        System::Windows::Forms::Label^ label2; // ��������� ����
        int attemptCount = 0; // ������� �������
        String^ correctWord; // ���������� ��� �������� ����������� �����

    public:
        // 3. �����������
        Form1(void)
        {
            InitializeWords(); // ������������� ������ ����
            InitializeComponent(); // ������������� ����������� �����
            CreateMenu(); // �������� �������� ����
            CreateGamePanel(); // �������� ������� ������
            SubscribeToKeyDownEvents(); // �������� �� ������� ������� ������
        }

    protected:
        // 4. ����������
        ~Form1()
        {
            if (components)
            {
                delete components; // ������������ ��������
            }
        }

    private:
        System::ComponentModel::IContainer^ components; // ��������� �����������

        // 5. ������������� ����
        void InitializeWords()
        {
            wordsToGuess = gcnew List<String^>();
            wordsToGuess->Add("����");
            // �������� ������ ���� ��� �������������
            SelectRandomWord(); // ����� ���������� ����� ��� ����������
        }

        // 6. ����� ���������� �����
        void SelectRandomWord()
        {
            Random^ rand = gcnew Random();
            int index = rand->Next(wordsToGuess->Count);
            correctWord = wordsToGuess[index]; // ��������� ����������� �����
        }


        // 7. ������������� ����������� �����
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
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog; // ��������� ��������� ������� ����
            this->MaximizeBox = false; // ��������� ������ ������������
            this->MaximumSize = System::Drawing::Size(1140, 666); // ���������� ������������ ������ ����
            this->MinimumSize = System::Drawing::Size(1140, 666); // ���������� ����������� ������ ����
            this->Name = L"Form1";
            this->Text = L"Wordle";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->ResumeLayout(false);
        }

        // 8. �������� �������� ����
        void CreateMenu()
        {
            if (menuPanel != nullptr) {
                this->Controls->Remove(menuPanel); // �������� ������ ������ ����, ���� ����
            }

            menuPanel = gcnew Panel();
            menuPanel->Size = this->ClientSize; // ������ ������ ����� ������� �����
            menuPanel->BackColor = System::Drawing::Color::Gainsboro; // ���� ���� ������ ����

            playButton = gcnew Button();
            playButton->Text = "������"; // ����� ������
            playButton->Location = System::Drawing::Point(480, 300); // ������� ������
            playButton->Size = System::Drawing::Size(150, 50); // ������ ������
            playButton->Click += gcnew EventHandler(this, &Form1::PlayButton_Click); // �������� �� ������� �������

            // ��������� ������ �� ������ ����
            menuPanel->Controls->Add(playButton);
            this->Controls->Add(menuPanel); // ���������� ������ ���� �� �����
        }

        // 9. �������� ������� ������
        void CreateGamePanel()
        {
            gamePanel = gcnew Panel();
            gamePanel->Size = this->ClientSize; // ������ ������ ����� ������� �����
            gamePanel->BackColor = System::Drawing::Color::PowderBlue; // ���� ���� ������� ������
            CreateTextBoxes(); // �������� ��������� �����
            gamePanel->Controls->AddRange(textBoxes); // ���������� ��������� ����� �� ������� ������
            gamePanel->Visible = false; // ������ ������� ������ �� ���������
            this->Controls->Add(gamePanel); // ���������� ������� ������ �� �����
        }

        // 10. �������� ��������� �����
        void CreateTextBoxes()
        {
            textBoxes = gcnew array<TextBox^>(30); // ������ �� 30 ��������� �����
            int x = 350, y = 180; // ��������� ���������� ��� ��������� �����
            int width = 80, height = 70; // ������ � ������ ��������� �����
            int spacing = 1; // ���������� ����� ���������� ������
            

            for (int i = 0; i < 6; i++) // 6 ����� ��� �������
            {
                for (int j = 0; j < 5; j++) // ���� �� ��������
                {
                    int index = i * 5 + j; // ������ ���������� ����
                    textBoxes[index] = CreateTextBox(x + j * (width + spacing), y + i * (height + spacing)); // �������� ���������� ����
                }
            }
        }

        // 11. �������� ���������� ���������� ����
        TextBox^ CreateTextBox(int x, int y)
        {
            TextBox^ textBox = gcnew TextBox(); // �������� ���������� ����
            textBox->Location = System::Drawing::Point(x, y); // ������� ���������� ����
            textBox->Size = System::Drawing::Size(70, 40); // ������ ���������� ����
            textBox->Font = (gcnew System::Drawing::Font(L"Rubik Medium", 36)); // ����� ���������� ����
            textBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center; // ������������ ������
            textBox->MaxLength = 1; // ������������ ����� �����
            textBox->CharacterCasing = CharacterCasing::Upper; // ��������� ����� ������ � ������� ��������
            textBox->ReadOnly = false; // ���������� ����������� ����� ������
            textBox->Click += gcnew EventHandler(this, &Form1::TextBox_Click); // ������ ������ ���������� ����
            return textBox; // ������� ���������� ���������� ����
        }

        // 12. ��������� ������� ������ "������"
        void PlayButton_Click(Object^ sender, EventArgs^ e)
        {
            menuPanel->Visible = false; // ������ ������ ����
            gamePanel->Visible = true; // �������� ������� ������
            textBoxes[0]->Focus(); // ��������� ������ �� ������ ��������� ����
        }

         // 13. ��������� ������� �� ��������� ����
         void TextBox_Click(Object^ sender, EventArgs^ e) {
            TextBox^ clickedTextBox = (TextBox^)sender;
            int index = Array::IndexOf(textBoxes, clickedTextBox);
            if (index >= attemptCount * 5 && index < (attemptCount + 1) * 5) {
                textBoxes[attemptCount * 5]->Focus(); // ��������� ������ �� ������� ��������� ����
            }
        }

         // 14. �������� �� ������� ������� ������
         void SubscribeToKeyDownEvents()
         {
            for (int i = 0; i < 30; i++)
             {
                 textBoxes[i]->KeyDown += gcnew KeyEventHandler(this, &Form1::TextBox_KeyDown);
             }
         }
         // 15. ��������� ������� ������ � ��������� ����
         void TextBox_KeyDown(Object^ sender, KeyEventArgs^ e) {
             TextBox^ changedTextBox = (TextBox^)sender;
             int index = Array::IndexOf(textBoxes, changedTextBox);

             if (e->KeyCode == Keys::Enter) {
                 CheckGuessedWord(); // �������� ���������� �����
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

             // ��������� ����������� �� ��������� ����� ��� ������� ������
             if (index < attemptCount * 5 || index >= (attemptCount + 1) * 5) {
                 e->SuppressKeyPress = true;
                 textBoxes[attemptCount * 5]->Focus();
             }
         }

         // 16. �������� ���������� �����
         void CheckGuessedWord() {
             if (attemptCount < 6 && IsCompleteWord()) {
                 String^ guessedWord = "";
                 for (int i = attemptCount * 5; i < (attemptCount + 1) * 5; i++) {
                     guessedWord += textBoxes[i]->Text;
                     textBoxes[i]->ReadOnly = true; // ���������� ��������� ����� ����� ����� �����
                 }


                 if (guessedWord->ToUpper() == correctWord->ToUpper()) {
                     MessageBox::Show("�� ������� �����!", "�����������!", MessageBoxButtons::OK, MessageBoxIcon::Information);
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
                         MessageBox::Show("�� ��������� ��� �������! ���������� �����: " + correctWord, "���� ��������", MessageBoxButtons::OK, MessageBoxIcon::Information);
                         /*ClearGame();
                         ShowMenu();*/
                     }
                 }
             }
         }

         // 17. ��������� ������������ ����
         void HighlightIncorrectLetters(String^ guessedWord) {
             array<wchar_t>^ guessedChars = guessedWord->ToUpper()->ToCharArray();
             array<wchar_t>^ correctChars = correctWord->ToUpper()->ToCharArray();
             array<bool>^ matchedCorrect = gcnew array<bool>(5); // ������ ��� ������������ ���������� � ���������� �����

             for (int i = 0; i < 5; i++) {
                 wchar_t guessedChar = guessedChars[i];
                 wchar_t correctChar = correctChars[i];

                 if (guessedChar == correctChar) {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightGreen; // ���������� ����� �� ���������� �����
                     matchedCorrect[i] = true; // �������� ���������� �� ���� �����
                 }
                 else {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::White; // ��������� ������ ������ �����
                     matchedCorrect[i] = false; // ��� ����������
                 }
             }

             // ������ ������ ��� ������� ����, ������� ���� � �����, �� �� �� ���������� �������
             for (int i = 0; i < 5; i++) {
                 if (textBoxes[attemptCount * 5 + i]->BackColor == System::Drawing::Color::White) { // ������ ��� �� ������� �����
                     for (int j = 0; j < 5; j++) {
                         if (!matchedCorrect[j] && guessedChars[i] == correctChars[j]) {
                             textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightGoldenrodYellow; // ����� ���� � �����, �� �� �� �����
                             matchedCorrect[j] = true; // ��������, ��� ��� ����� ������������
                             break;
                         }
                     }
                 }
             }


             // ������ ������ ��� ������� ����, ������� ����������� � �����
             for (int i = 0; i < 5; i++) {
                 if (textBoxes[attemptCount * 5 + i]->BackColor == System::Drawing::Color::White) {
                     textBoxes[attemptCount * 5 + i]->BackColor = System::Drawing::Color::LightCoral; // ����� ����������� � �����
                 }
             }
         }

         // 18. ��������� ��������� ������ � ��������� ����
         void TextBox_TextChanged(Object^ sender, EventArgs^ e) {
             TextBox^ changedTextBox = (TextBox^)sender;

             if (changedTextBox->Text->Length == 1) {
                 int index = Array::IndexOf(textBoxes, changedTextBox);

                 if (index < (attemptCount + 1) * 5 - 1) {
                     textBoxes[index + 1]->Focus();
                 }
             }
         }

         // 19. ��������, ��������� �� �����
         bool IsCompleteWord()
         {
             for (int i = attemptCount * 5; i < (attemptCount + 1) * 5; i++)
             {
                 if (textBoxes[i]->Text->Length == 0)
                 {
                     return false; // ���� ���� �� ���� ������, ���������� false
                 }
             }
             return true; // ���� ��� ���������, ���������� true
         }

        private:
            // �������������� ��������� ��� �������� �����
            System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e)
            {
                // �������������� ��������� ��� �������� �����, ���� ����������
            }
    };
}
