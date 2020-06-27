#include <Windows.h>
#include "Wstring.h"

/*
Тесты кастомного класса стринг wchar_t
*/

int main()
{

   //Создание массива строк 
   Wstring s[10];

   s[0] = L" Тестовая Строка. ";
   s[1] = L"Это тестовая строка";
   s[2] = L"Тест склейки строк:";
   s[3] = L"ТЕСТОВАЯ СТРОКА КАПСОМ";
   s[4] = L"Тестовая строка строчный текст";
   s[5] = L"\n";
   s[6] = L"";
   s[7] = L"  split  split  split  ";
   s[8] = L"                          ";
   s[9] = L"testString";

  //Склеивание строк
   Wstring test1 = s[2] + s[0] + s[1];
   MessageBoxW(0, test1.c_wstr(), 0, MB_OK);

   //Сравнение строк
   Wstring test2 = s[0] + s[1];
   Wstring test3 = s[0] + s[1];

   if (test2 == test3) {
       MessageBoxW(0, L"Тест сравнение строк пройден", L"OK", MB_OK);
   }
   else {
       MessageBoxW(0, L"Тест сравнение строк провален, где-то касяк, хе-хе", L"OK", MB_OK);
   }

   if (s[1] > s[0]) {
       MessageBoxW(0, L"Тест сравнение строк пройден: s[1] > s[0]", L"OK", MB_OK);
   }
   else {
       MessageBoxW(0, L"Тест сравнение строк провален, где-то касяк, хе-хе", 0, MB_OK);
   }

   if (s[0] < s[1]) {
       MessageBoxW(0, L"Тест сравнение строк пройден: s[0] < s[0]", L"OK", MB_OK);
   }
   else {
       MessageBoxW(0, L"Тест сравнение строк провален, где-то касяк, хе-хе", 0, MB_OK);
   }

  //Разбиение строки на лексемы
   Wstring test_split[10];

  //Для безопасности, заполнить созданный массив строк, пустой строкой
   for (int i = 0; i < 10; i++) {
       test_split[i] = L"\0";
   }

   Wstring tested_str = L"Test1|Test2|Test3|Test4|Test5";

   size_t count_splited = 0;
   tested_str.split((wchar_t*)L"|", test_split, 10, &count_splited);

   //Вывод итоговой строки
   Wstring split_str_finall = L"Итоговая разбитая строка:"; 

   for (int i = 0; i < count_splited; i++) {
       split_str_finall = split_str_finall + test_split[i];
   }

   MessageBoxW(0, split_str_finall.c_wstr(),L"OK", MB_OK);

   //Тест поиска строки
   Wstring test_find = L"This is an awesome fun project. Let's do this again.";
   size_t npos = test_find.Find(L"awesome");

   //Тест конвертации числа в строку
   Wstring Position_str = L"\0"; 
   Position_str = Position_str.NumericToWstr(npos);

   Wstring Str_to_send = L"Позициая искомой строки:";
   Str_to_send = Str_to_send + Position_str;
   
   MessageBoxW(0, Str_to_send.c_wstr(), L"OK", MB_OK);

   //Проверка добовления символов в строку
   Wstring Str_test_array = L"\0";

   Str_test_array = Str_test_array + L"К";
   Str_test_array = Str_test_array + L"О";
   Str_test_array = Str_test_array + L"Н";
   Str_test_array = Str_test_array + L"Е";
   Str_test_array = Str_test_array + L"Ц";
   Str_test_array = Str_test_array + L" ";
   Str_test_array = Str_test_array + L"Т";
   Str_test_array = Str_test_array + L"Е";
   Str_test_array = Str_test_array + L"С";
   Str_test_array = Str_test_array + L"Т";
   Str_test_array = Str_test_array + L"О";
   Str_test_array = Str_test_array + L"В";
   Str_test_array = Str_test_array + L"!";

   MessageBoxW(0, Str_test_array.c_wstr(), L"OK", MB_OK);

   return 0;
}