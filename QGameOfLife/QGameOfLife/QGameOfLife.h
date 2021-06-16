#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QGameOfLife.h"
#include <qvector>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QRadioButton>
#include <QPushButton>


class QGameOfLife : public QMainWindow
{
    Q_OBJECT

public:
    QGameOfLife(QWidget *parent = Q_NULLPTR);
    
private:
    Ui::QGameOfLifeClass ui;
    size_t m_matDimension = 250;
    size_t m_vitesse = 10;
    QVector<QVector<bool>> m_simulation2d;
    QVector<QVector<bool>> m_resultSimulation2d;
   
    void updateImage();
    void resizeVector(size_t matDimension, QVector<QVector<bool>> &aModif);
    void nextGenVector(size_t matDimension);
    void initGame();
    QAbstractButton* createButton(QString const & txtBtn, QString const& type);


    QLabel* labelPrinc;
    QRadioButton* m_Radio50, *m_Radio250, *m_Radio500, *m_Radio1000, *m_RadioSurchaufe; // 100000000000
    QPushButton* m_btnStart, *m_btnStop, *m_btnReset;
    QImage m_gameImage;
    void updateImage(QImage* img, QVector<QVector<bool>> const & vectorModel);
    

    QTimer* mTimer;

    //int resultat = std::stoi(patate) + std::stoi(chose);
    
private slots:
    void tick();
};
