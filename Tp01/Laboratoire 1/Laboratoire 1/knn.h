/*
420-C52-IN DONNÉES, MÉGADONNÉES ET INTELLIGENCE ARTIFICIELLE
Projet 1 - Algorithme KNN
17 décembre 2020

Équipe:
Viens, Dany
Lapierre, François
Gaudreault,  Vincent
Emond-Serret, Caroline
Genest, Carl


*/

#pragma once
#include <QImage>
#include <QString>
#include <array>

class KNN {
public:
    struct sData {
        std::array<float, 3> mValues;
        QString mTag;
        float mDistance; //comparaison
    };
    const QVector <sData>& getData()const;
    void addData(sData const& data);
    void clearData();
    QString analyseData(sData& data, int k);


private:

    QVector<sData> mData;

    sData test;
    
   
   
    static bool compare(const sData& a, const sData& b);

};


