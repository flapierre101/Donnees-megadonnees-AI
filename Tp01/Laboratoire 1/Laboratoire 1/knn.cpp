#include "KNN.h"
#include <QDirIterator>
#include <QFileInfo>
#include "Laboratoire1.h"
#include <QFileDialog>
#include <QVector>
#include <QTMath>
#include <QtAlgorithms>
#include <QMap>
#include <algorithm>


const QVector <KNN::sData>& KNN::getData()const
{
    return mData;
}

void KNN::addData(sData const& data)
{
    mData.append(data);
}

void KNN::clearData()
{
    mData.clear();
}

QString KNN::analyseData(sData& data, int k)
{
     
    for (size_t i = 0; i < mData.size(); i++)
    {
        mData[i].mDistance = 0.0f;
        for (size_t j = 0; j < data.mValues.size(); j++)
        {
            mData[i].mDistance += qPow(data.mValues[j] - mData[i].mValues[j], 2.0f);
        }
        mData[i].mDistance = qSqrt(mData[i].mDistance);
    }

    //voir QSort
    qSort(mData.begin(), mData.end(), compare);
    
    QMap<QString, int> tempoMap{};
    for (int i = 0; i < k; i++)
    {   
        if (tempoMap.contains(mData[i].mTag)) {
            tempoMap[mData[i].mTag]++;
        }
        else {
            tempoMap.insert(mData[i].mTag, 1);
        }
            

    }

    QString maxTag("");
    int maxCount{};

    for (auto it(tempoMap.begin()); it != tempoMap.end(); ++it)
    {
        if (it.value() > maxCount) {
            maxCount = it.value();
            maxTag = it.key();
        }
    }
    
    return maxTag;
}


bool KNN::compare(const KNN::sData& a, const KNN::sData& b)
{
    return a.mDistance < b.mDistance;
}
