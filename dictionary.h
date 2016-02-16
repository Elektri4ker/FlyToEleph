#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <iostream>

/*
 * Содержит слова, с которыми мы будем работать
 * Все слова д.б. одинаковой длины
 */
class Dictionary {
    QStringList dict;   //Список слов

    /*
     * Возвращает true, если слова str1 и str2 отличаются не более, чем
     * на одну букву
     */
    bool oneDiff(QString str1, QString str2) {
        int num=0;
        for (int i=0; i<str1.size() && num<=1; i++)
            num += str1[i] != str2[i];
        return num<=1;
    }

public:
    /*
     * Загрузка из файла file слов длины len
     */
    bool getFromFile(QString file, int len) {
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly))
            return false;
        QTextStream in(&f);
        dict.clear();
        while (!in.atEnd()) {
            QString str = in.readLine();
            str.toLower();
            if (str.count()==len)
                dict.append(str);
        }
        dict.sort();
        dict.removeDuplicates();
        return true;
    }

    Dictionary() {}
    Dictionary(QString file, int len) {
        getFromFile(file, len);
    }

    bool isEmpty() { return dict.isEmpty(); }

    /*
     * Возвращает список слов, которые отличаются от слова original
     * не более чем на одну букву. Найденные слова удаляются из словаря
     */
    QStringList getDeleteSimilarList(QString original) {
        QStringList similar;
        for (QStringList::iterator it=dict.begin(); it!=dict.end(); ) {
            if (oneDiff(original, *it)) {
                similar.push_back(*it);
                it = dict.erase(it);
            } else
                it++;
        }
        return similar;
    }

    bool removeEntry(QString entry) {
        return dict.removeOne(entry);
    }
    bool contains (QString entry) {
        return dict.contains(entry);
    }

#ifdef _TEST
    QStringList *getVector() {
        return &dict; }
#endif
};


#endif // DICTIONARY_H
