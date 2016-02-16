#include "decisiontree.h"

int DecisionTree::maxScore() {
    int max = 0;
    leafIterator it = mTree.begin_leaf();
    while (it != mTree.end_leaf()) {
        if (it->score>max && !it->deadEnd)
            max = it->score;
        it++;
    }
    return max;
}
int DecisionTree::newWave() {
    int max = maxScore();
    //Сначала выбираем итераторы на нужные нам листья
    QList<leafIterator> leaves = getLeafIterators(max);
    if (leaves.isEmpty())
        return -1; //Больше не осталось листьев, которые можно продолжить

    foreach(leafIterator it, leaves) {
        //ищутся все слова, отличающиеся на 1 букву. То что найдено, удаляется из словаря
        QStringList similars = mDict->getDeleteSimilarList(it->entry);
        if (similars.isEmpty()) {
            it->deadEnd = true;
        } else {
            //Каждое найденное слово пакуется в узел-потомок текущего узла it
            foreach(QString newEntry, similars) {
                if (newEntry == mEndWord) { // Целевое слово найдено
                    itEndWord = it;
                    return 1;
                }
                TreeNode newNode(newEntry, it->entry);
                mTree.append_child(it, newNode);
            }
        }
    }
    return 0;
}
QStringList DecisionTree::resultChain() {
    QStringList res;
    baseIterator it = itEndWord;
    while (it != mTree.begin()) {
        res.prepend(it->entry);
        it = mTree.parent(it);
    }
    return res;
}
QList<leafIterator> DecisionTree::getLeafIterators(int max) {
    QList<leafIterator> res;
    leafIterator it = mTree.begin_leaf();
    while (it != mTree.end_leaf()) {
        if (it->score>=max && !it->deadEnd)
            res.append(it);
        it++;
    }
    return res;
}

QStringList DecisionTree::getResult() {
    //Поиск решения
    int res;
    while (0 == (res = newWave()));

    //Если решение есть, то извлекаем искомую цепочку
    if (res == 1) {
        QStringList chain = resultChain();
        chain.prepend(mStartWord);
        chain.append(mEndWord);
        return chain;
    }
    //if (res == -1)
        return QStringList();
}
