#include "decisiontree.h"
#include <iostream>


int main(int argc, char *argv[])
{
    //Проверка правильного кол-ва аргументов
    QTextStream cout(stdout);
    cout.setCodec("IBM 866");
    if (argc!=3) {
        cout << "Illegal argument count\n";
        return -1;
    }
    //Проверка наличия файлов
    QFile fWords(argv[1]), fDict(argv[2]);
    if (!fWords.open(QIODevice::ReadOnly)) {
        cout << "could not open words file \"" << argv[1] <<"\"" << endl;
        return -1;
    }
    if (!fDict.open(QIODevice::ReadOnly)) {
        cout << "could not open dictionary file \"" << argv[2] <<"\"" << endl;
        return -1;
    }
    fDict.close();
    //Считывание начального/конечного слова из файла
    QTextStream sWords(&fWords);
    QString wordStart, wordEnd;
    wordStart = sWords.readLine().toLower();
    wordEnd = sWords.readLine().toLower();
    if (wordStart.size()!=wordEnd.size() || wordStart.isEmpty() || wordEnd.isEmpty()) {
        cout << "Words specified in the file have various length or absent at all\n";
        return -1;
    }

    //Загрузка словаря
    Dictionary dict(argv[2], wordStart.size());
    if (!dict.contains(wordStart) || !dict.contains(wordEnd)) {
        cout << "Dictionary contains no words mathing either start or end word" << endl;
        return -1;
    }
    //создание и инициализация дерева решений
    DecisionTree decisionTree(wordStart, wordEnd);
    decisionTree.setDictionary(&dict);

    //Получение цепочки решений
    QStringList chain = decisionTree.getResult();
    if (!chain.isEmpty()) {
        foreach (QString chainEntry, chain) {
            cout << chainEntry << endl;
        }
    } else {
        cout << "no chain found" << endl;
    }


    return 0;
}
