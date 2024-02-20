#include <iostream>

using namespace std;

void cleanSpaces(char* str); //функция для удаления пробелов
double Math(char* str); //функция, вычисляющая выражение
double MultDev(char* str, int& index); //функция для multiplication and division
double Number(char* str, int& index); //функция, распознающая число
char* Extract(char* str, int& index); //функция для извлечения подстроки

const int MAXSIZE = 128; //максимальный размер буфера

int main()
{
    setlocale( LC_ALL, "Russian" );
    char buffer[MAXSIZE] = { 0 }; //область хранения вычисляемого входного выражения
    unsigned int choose = 0;
    cout << endl << "Вас приветствует калькулятор!" << endl;
    
    while (true) {
        cout << "1. Начать ввод." << endl << "2. О программе." << endl << "0. Выйти из программы" << endl; 
        cin >> choose;
        switch (choose) {
        case 1:
            while (true)
            {
                cout << "Введите выражение или пустую строку для завершения." << endl;
                cin.ignore();
                cin.getline(buffer, sizeof buffer); //читаем то, что вписываем
                cleanSpaces(buffer); //удаляем все пробелы из строки
                if (!buffer[0]) return 0; //если пустая строка
                cout << "Результат: " << Math(buffer) << endl;

                break;
            }
        case 2:
            cout << "###################СПРАВКА###################" << endl;
            cout << "Пользователь вводит с клавиатуры некоторое арифметическое выражение. "
                << "Выражение может содержать : (), +, -, *, / .\n"
                << "Приложение рассчитывает результат выражения с учетом скобок, приоритетов. "
                << "Результат отображается на экране. \n"
                << "Например, если пользователь ввел : \n"
                << "5 * 2 + 1 \n"
                << "Результат : 11\n"
                << "Если пользователь ввел :\n"
                << "5 * (2 + 1) \n"
                << "Результат : 15" << endl 
                << "ЕСЛИ БУДЕТ ВВЕДЕНА ПУСТАЯ СТРОКА, ТО НУЖНО ПРОИЗОЙДЁТ ВЫХОД В ГЛАВНОЕ МЕНЮ!" << endl << "#############################################" << endl;
            break;
        case 0:
            cout << "Досвидания!" << endl;
            exit(1);
        default:
            cout << "Неправильный символ " << endl;
            break;
        }
    }
}

void cleanSpaces(char* array) //функция для удаления пробелов
{
    int i = 0;                          //индекс места в строке "куда копировать"
    int j = 0;                          //индекс места в строке "откуда копировать"
    while ((*(array + i) = *(array + j++)) != '\0') { //цикл, пока очередной символ не '\0'
        if (*(array + i) != ' ') i++;  //увеличиваем i, если символ не пробел
    }
    return;
}

double Math(char* array) //функция, вычисляющая выражение
{
    double value = 0.0;         //здесь сохраняем результат
    int index = 0;              //текущая позиция символа
    value = MultDev(array, index);   //получить первый элемент
    while(true)              //бесконечный цикл, выход внутри
    {
        switch (*(array + index++)) //выбрать действие на основе текущего символа
        {
        case '\0':          //конец строки, возвращаем значение
            return value;
        case '+':           //знак плюс, прибавляем элемент к value
            value += MultDev(array, index);
            break;
        case '-':           //знак минус, вычитаем элемент из value
            value -= MultDev(array, index);
            break;
        default:            //все остальное ошибка
            int i = index;
            while (--i > 0)
                cout << " ";
            cout << "^" << endl;
            cout << "Здесь ошибка. " << endl;
            return 0;
        }
    }
}

double MultDev(char* array, int& index) //функция анализа, которая смотрить где есть умножение или деление
{
    double value = 0.0;             //здесь накапливается значение результата
    value = Number(array, index);      //получить первое число элемента
    //выполняем цикл до тех пор, пока имеем допустимую операцию
    while ((*(array + index) == '*') || (*(array + index) == '/'))
    {
        if (*(array + index) == '*')
            value *= Number(array, ++index); //умножение на следующий элемент
        if (*(array + index) == '/')
            value /= Number(array, ++index);
    }
    return value;
}


double Number(char* array, int& index) 
{
    double value = 0.0;                 //хранит результирующее значение
    if (*(array + index) == '(') // Если встречаем скорбку, то переходим к функции распознование скобочек
    {
        char* p_subarray = 0;
        p_subarray = Extract(array, ++index);
        value = Math(p_subarray);
        delete[] p_subarray;
        return value;
    }
    //Цикл, превращает символы в число
    //Макрос isdigit() возвращает ненулевое значение, если аргумент ch является цифрой от 0 до 9, в противном случае возвращается 0. 
    while (isdigit(*(array + index)))       //цикл накапливает ведущие цифры 
        value = 10 * value + (*(array + index++) - '0');
    if (*(array + index) != '.')          //если не цифра, проверяем на десятичную точку
        return value;
    double factor = 1.0;                //множитель для десятичных разрядов
    //цикл, возвращает десятичную часть
    while (isdigit(*(array + (++index)))) //выполнять цикл, пока идут цифры 
    {
        factor *= 0.1;
        value = value + (*(array + index) - '0') * factor;
    }
    return value;
}

char* Extract(char* array, int& index) //функция для извлечения подстроки
{
    char buffer[MAXSIZE];       //временное пространство для подстроки
    char* ptr_array = 0;        //указатель на новую строку для возврата
    int numLeft = 0;           //счетчик найденных левых скобок
    int buf_index = index;  //сохранить начальное значение index
    do
    {
        buffer[index - buf_index] = *(array + index); //копируем символ текущей строки в подстроку
        switch (buffer[index - buf_index]) //смотрим, что это за символ
        {
        case ')':
            if (numLeft == 0)
            {
                buffer[index - buf_index] = '\0'; //если счетчик скобочек верный, ставим символ конца строки
                ++index;    //устанавливаем индекс на следующий за скобочкой элемент
                ptr_array = new char[index - buf_index];
                if (!ptr_array)
                {
                    cout << "Выделение памяти не удалось, программа прервана.";
                    exit(1);
                }
                strncpy(ptr_array, buffer, index - buf_index); //безопасное копирование строки при помощи функции strncpy
                return ptr_array;
            }
            
            else
                numLeft--;     //уменьшаем счетчик скобок
            break;
        case '(':
            numLeft++;         //соответственно увеличиваем
            break;
        }
    } while (*(array + index++) != '\0');     //устанавливаем индекс в следующий элемент
    cout << "Вывод за пределы выражения, возможно, плохой ввод." << endl;
    exit(1);
    return ptr_array;
}