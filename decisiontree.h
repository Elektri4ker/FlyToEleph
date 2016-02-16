#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <QString>
#include "tree.hh"
#include "dictionary.h"
/*
 * Структура, инкапсулирующая слово в узел дерева.
 */
struct TreeNode {
    int score;      //Весовой коэффициент слова: +3 за каждую букву, совпадающую с
                    //буквой в конечном слове; +2 за каждую гласную букву, находящуюся
                    //на том же месте, что и гласная буква в конечном слове
    QString entry;  //Само слово
    bool deadEnd;   //Является ли тупиковой ветвью

    TreeNode(int sc, const QString ent):score(sc), entry(ent), deadEnd(false) {}
    TreeNode() {}
    TreeNode(const QString ent, const QString from):entry(ent), deadEnd(false) {
        score = calcScore(ent, from);
    }

private:
    QString vowels = "ёуеыаоэяию";
    bool isVowel (const QChar ch) {
        return vowels.contains(ch);
    }

    int calcScore(const QString ent, const QString from) {
        int sc = 0;
        for (int i=0; i<ent.size(); i++) {
            if (from[i]==ent[i]) sc+=3;
            if (isVowel(from[i]) && isVowel(ent[i])) sc+=2;
        }
        return sc;
    }
};

typedef tree<TreeNode>::leaf_iterator leafIterator;
typedef tree<TreeNode>::iterator baseIterator;

class DecisionTree {
    tree<TreeNode> mTree;           //Собственно, дерево
    QString mStartWord, mEndWord;   //Начальное и конечное слово
    Dictionary *mDict;              //Контекст словаря
    baseIterator itEndWord;         //Указывает на узел, в котором было найдено конечное слово
public:
    DecisionTree(QString startWord, QString endWord) :
        mTree(TreeNode(0, startWord)), mStartWord(startWord), mEndWord(endWord) {}

    void setDictionary(Dictionary *dict) {
        mDict = dict;
        mDict->removeEntry(mStartWord);
    }

    /*
     * Функция, генерирующая "новую волну" в дереве: для каждого из листьев
     * (узлов, не имеющих потомков), соответствующего критериям (максимальный коэффициент
     * score и не является тупиковой ветвью), подбираются из словаря те слова, которые
     * отличаются от слова в текущем узле на 1 букву, затем эти слова добавляются в качестве
     * потомков текущего узла и им присваивается коэффициент score. Если для текущего
     * узла похожих слов не обнаружено, он маркируется как тупиковая ветвь (deadEnd).
     * Возвращает:
     * 0 - если новая волна сгенерирована полностью
     * -1 - если остались только тупиковые ветви, т.е., решений нет
     * 1 - если найдено решение (итератор на конечное слово записывается тогда в itEndWord
     */
    int newWave();

    /*
     * Возвращает цепочку слов между начальным и конечным (сами начальное и конечное
     * сюда не входят)
     */
    QStringList resultChain();

    /*
     * Возвращает цепочку слов - результат
     */
    QStringList getResult();

#ifdef _TEST
#include <QStringList>
    //Возвращает список всех листьев дерева
    QStringList allLeaves() {
        QStringList res;
        leafIterator it = mTree.begin_leaf();
        while (it != mTree.end_leaf()) {
            res.append(it->entry);
            it++;
        }
        return res;
    }
#endif
private:
    /*
     * Возвращает максимальный коэффициент среди листьев дерева
     */
    int maxScore();

    /*
     * Возвращает список итераторов на листья дерева, соответствующие критериям
     * (максимальный коэффициент score и не является тупиковой ветвью)
     */
    QList<leafIterator> getLeafIterators(int max);

};

#endif // DECISIONTREE_H
