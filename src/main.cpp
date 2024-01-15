#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

enum class State
{
    START,
    INIT,
    CONDITION,
    LOOP,
    BODY
};

struct Data
{
    string variable;
    string condition;
    string body;
    string loop;
};

string convertDataToDoWhile(const Data& data)
{
    string convertible;
    convertible += data.variable + ";\n";
    convertible += "do {";
    convertible += data.body;
    convertible += data.loop + ";\n";
    convertible += "} while (" + data.condition + " )\n";
    return convertible;
}

string convertForToDoWhile(const string& input)
{
    string ans;
    Data data;
    State state = State::START;
    int index = 0;
    while (index < (int)input.size())
    {
        if (state == State::START)
        {
            if (input[index] == 'f')
            {
                if (input.substr(index, 3) == "for")
                {
                    // Когда мы убедились что это начало цикла for, ищем круглую скобку
                    auto findIndex = input.find('(', index + 3);

                    // Если не находим выходим из цикла
                    if (findIndex == string::npos)
                    {
                        break;
                    }

                    // На этом этапе мы значем что findIndex это индекс круглой скобки
                    // Поэтому прибавляем нашему текущему индексу + 1 и переводим состояние в
                    // INIT
                    index = findIndex + 1;
                    state = State::INIT;
                    continue;
                }
            }
        }
        else if (state == State::INIT)
        {
            // Ищем точку с запятой с позиций текущего индекса
            auto findIndex = input.find(';', index);

            // Если не находим выходим из цикла
            if (findIndex == string::npos)
            {
                break;
            }

            // На этом этапе мы просто берем и записываем все что нашли от текущей позиций индекса
            // до точки с запятой, в строку для записи переменной
            data.variable = input.substr(index, findIndex - index);

            // На данном этапе findIndex это индекс точки с запятой поэтому прибавляем к текущему
            // индексу + 1  и переводим в состояние CONDITION
            index = findIndex + 1;
            state = State::CONDITION;
            continue;
        }
        else if (state == State::CONDITION)
        {
            // Ищем точку с запятой с позиций текущего индекса
            auto findIndex = input.find(';', index);

            // Если не находим выходим из цикла
            if (findIndex == string::npos)
            {
                break;
            }
            // На этом этапе мы просто берем и записываем все что нашли от текущей позиций индекса
            // до точки с запятой, в строку для записи условия
            data.condition = input.substr(index, findIndex - index);

            // На данном этапе findIndex это индекс точки с запятой поэтому прибавляем к текущему
            // индексу + 1  и переводим в состояние LOOP
            index = findIndex + 1;
            state = State::LOOP;
            continue;
        }
        else if (state == State::LOOP)
        {
            // Ищем закрывающую круглую скобку с позиций текущего индекса
            auto findIndex = input.find(')', index);

            // Если не находим выходим из цикла
            if (findIndex == string::npos)
            {
                break;
            }
            // На этом этапе мы просто берем и записываем все что нашли от текущей позиций индекса
            // до закрывающей круглой скобки, в строку для записи циклического выражения
            data.loop = input.substr(index, findIndex - index);

            // На данном этапе findIndex это индекс закрывающей круглой скобки поэтому прибавляем к
            // текущему индексу + 1  и переводим в состояние BODY
            index = findIndex + 1;
            state = State::BODY;
            continue;
        }
        else if (state == State::BODY)
        {
            // Ищем открывающую фигурную скобку с позиций текущего индекса
            auto findIndexOpen = input.find('{', index);
            if (findIndexOpen == string::npos)
            {
                // Если не находим тут два варианта либо такого не может быть, либо просто после
                // цикла идет одна строчка без фигурных скобок, её мы и запишем в тело цикла для do
                // while Для того чтобы проверить просто ищем с позиций текущего индекса точку с
                // запятой если мы её не найдем то выходим
                auto findIndex = input.find(';', index);
                if (findIndex == string::npos)
                {
                    break;
                }

                // Далее просто бёрем и записываем в тело цикла строку от текущего индекса до
                // найденой позиций точки с запятой
                data.body = input.substr(index, findIndex - index);
                ans += convertDataToDoWhile(data) + "\n";

                // На данном этапе findIndex это индекс точки с запятой поэтому
                // прибавляем к текущему индексу + 1  и переводим в состояние START и повторяем все
                // заново
                index = findIndex + 1;
                state = State::START;
            }
            else
            {
                // Ищем закрывающую фигурную скобку с позиций найденного индекса открывающей
                // фигурной скобки
                auto findIndexClose = input.find('}', findIndexOpen);

                // Если не находим выходим из цикла
                if (findIndexClose == string::npos)
                {
                    break;
                }

                // На этом этапе мы просто берем и записываем все что нашли от позиций открывающей
                // фигурной скобки + 1 до закрывающей круглой скобки, в строку для записи
                // тела цикла
                data.body = input.substr(findIndexOpen + 1, findIndexClose - findIndexOpen - 1);

                // Далее записываем в результат функций полученный цикл do while
                ans += convertDataToDoWhile(data) + "\n";

                // На данном этапе findIndexClose это индекс закрывающей фигурной скобки поэтому
                // прибавляем к текущему индексу + 1  и переводим в состояние START и повторяем все
                // заново
                index = findIndexClose + 1;
                state = State::START;
            }
            continue;
        }
        ++index;
    }
    return ans;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Недостаточное количество аргументов командной строки." << endl;
        cerr << "Требуется указать 2 аргумента: <аргумент 1 - путь до считываемого файла>, <2 "
                "аргумент - путь до файла записи>."
             << endl;
        return 1;
    }
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if (!in.is_open())
    {
        cerr << "Не удалось открыть файл для чтения: " << argv[1];
        return 1;
    }
    if (!out.is_open())
    {
        cerr << "Не удалось открыть файл для записи: " << argv[2];
        return 1;
    }

    string text;
    string line;
    while (std::getline(in, line))
    {
        text += line + "\n";
    }
    out << convertForToDoWhile(text);
    in.close();
    out.close();
    return 0;
}
