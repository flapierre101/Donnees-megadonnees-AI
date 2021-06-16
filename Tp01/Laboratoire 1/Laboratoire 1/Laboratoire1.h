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

#include <QtWidgets/QMainWindow>
#include "ui_Laboratoire1.h"
#include "Q3DScatterChart.h"
#include <QVector>
#include <QLabel>
#include <QImage>
#include <QColor>
#include <QPushButton>
#include "FeatureExtraction.h"
#include "KNN.h"
#include <array>

class Laboratoire1 : public QMainWindow
{
    Q_OBJECT

public:
    Laboratoire1(QWidget *parent = Q_NULLPTR);


private:
    Ui::Laboratoire1Class ui;

    Q3DScatterChart* mChart;
    QPushButton* mTest;
    KNN mKNN;
    QLabel* mLabelResKselect, *mLabelResPhase1, *mLabelResPhase2;

    int kValue;

    size_t const mDimension = 250;

    QImage mImage;
    std::array<float, 3> mVectFeatures;


    // Pour itérateur
    QImage mTempoImg;
    QString mDir, mImgPath;

  

    void initLabo();   



private slots:
    void doTest();
    void choixDossier();
    void debutPhase1();
    void debutPhase2();
    void choixFichier();
    void updateKValue();


};
