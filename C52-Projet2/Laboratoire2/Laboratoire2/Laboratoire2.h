/*

420-C52-IN DONNÉES, MÉGADONNÉES ET INTELLIGENCE ARTIFICIELLE
Projet 2 - Algorithme génétique
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
#include "ui_Laboratoire2.h"
#include <qpixmap.h>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpainter.h>
#include <QHBoxLayout>
#include <qstring.h>
#include <QTimer> 
#include "ShapeOptimizer.h" 

class Laboratoire2 : public QMainWindow
{
    Q_OBJECT

public:
    Laboratoire2(QWidget *parent = Q_NULLPTR);

private:
    Ui::Laboratoire2Class ui;
    size_t const mVitesse{ 10 };
    size_t  mGen;
    bool start, reset, canvasCreated;
    size_t const mLabelWidth{ 175 };
    size_t const mParamWidth{ 150 };
    size_t const mPhaseWidth{ 100 };
    size_t const mButtonWidth{ 150 };
    size_t mTaille;
    QLabel* mMainLabel, *mCanvasSize, *mObstacles, *mCurrentGen, *mShape, *mBestArea, *mElite, *mMutation, *mPopulation;
    QGroupBox* phase1, * phase2, * parametres, * controles;
    QPushButton* mStart, *mReinitialize, *mStep;
    QVector<QPointF> mVectObstacles;
    QComboBox* mShapeMenu, *mSizeMenu, *mMutationMenu, *mPopulationMenu, *mEliteMenu, *mObstaclesMenu;
    QCheckBox* mShowRejects;
    QPainter mPainter;
    QTimer* mTimer;
    ShapeOptimizer so;

    void initLabo();
    QHBoxLayout* createParams(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected, QComboBox*& cb, QStringList const& items);
    QHBoxLayout* createPhaseLayout(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected, QComboBox*& paramMenu);
    QHBoxLayout* createPhaseTwoLayout(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected);
    QPushButton* createButton(QString const& buttonTxt, QPushButton*& button);

    void wipeCanvas();

    void dessiner();

    void dessinerPoints(QVector<QPointF> obstacles);

    //void dessinerCanvas();

    void updateLabels(size_t aire);



private slots:   
  void tick();
  void prochaineGen();
  void startGen();
  void resetSim();
};
