#include "QGameOfLife.h"
#include <random>
#include <iostream>
#include <windows.h>

#include <QVBoxLayout>
#include <QtGui>

QGameOfLife::QGameOfLife(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initGame();  
   

    // Connexion entre le timer et la slot
    // 2e partir le timer mTimer.start(int) = int - fréquence en millisecondes
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(tick()));
    mTimer->start(m_vitesse);

    
    //connect()
}

void QGameOfLife::tick() {
    nextGenVector(m_matDimension);
    updateImage();
    //labelPrinc->setPixmap(QPixmap::fromImage(gameImage.scaled(250,250, Qt::AspectRatioMode::KeepAspectRatio, Qt::FastTransformation)));
    labelPrinc->setPixmap(QPixmap::fromImage(m_gameImage.scaled(labelPrinc->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::FastTransformation)));
    
}

void QGameOfLife::updateImage() {

    for (int i = 0; i < m_matDimension; i++)    {       

        for (int j = 0; j < m_matDimension; j++)
        {
            if(m_simulation2d[i][j])
                m_gameImage.setPixel(i, j, 0);
            else
                m_gameImage.setPixel(i, j, 1);
        }
    }


}

void QGameOfLife::resizeVector(size_t matDimension, QVector<QVector<bool>> &aModif)
{
    aModif.resize(matDimension);
    
    for (int i = 0; i < matDimension; i++)
    {
        aModif[i].resize(matDimension);
       
        for (int j = 0; j < matDimension; j++)
        {
            if (i == 0 || j == 0 || i == matDimension - 1 || j == matDimension - 1)
                aModif[i][j] = 0;
            else
                aModif[i][j] = rand() % 2;
        }
    }
    
}



void QGameOfLife::nextGenVector(size_t matDimension)
{
    int counterT;

    for (int i = 1; i < matDimension - 1; i++)
    {
        for (int j = 1; j < matDimension - 1 ; j++)
        {
            counterT = 0;
            
            if (m_simulation2d[i - 1][j - 1])
                counterT++;
            if (m_simulation2d[i][j - 1])
                counterT++;            
            if (m_simulation2d[i + 1][j - 1])
                counterT++;            
            if (m_simulation2d[i + 1][j])
                counterT++;            
            if (m_simulation2d[i - 1][j])
                counterT++;            
            if (m_simulation2d[i - 1][j + 1])
                counterT++;            
            if (m_simulation2d[i][j + 1])
                counterT++;            
            if (m_simulation2d[i + 1][j + 1])
                counterT++;
            
            if (m_simulation2d[i][j])
                m_resultSimulation2d[i][j] = counterT == 2 || counterT == 3;
            else
                m_resultSimulation2d[i][j] = counterT == 3;

        }
    }

    m_simulation2d = m_resultSimulation2d;
}

void QGameOfLife::initGame()
{
    //Qimage
    m_gameImage = QImage(m_matDimension, m_matDimension, QImage::Format_Indexed8);
    QRgb value;
    value = qRgb(0, 0, 0);
    m_gameImage.setColor(0, value);
    value = qRgb(255, 255, 255);
    m_gameImage.setColor(1, value);

    //Taille des vecteurs
    resizeVector(m_matDimension, m_simulation2d);
    resizeVector(m_matDimension, m_resultSimulation2d);

    /***************************************/


    //Layout
    QWidget* centralWidget{ new QWidget };
    QVBoxLayout* imageLayout{ new QVBoxLayout };
    QHBoxLayout* centralLayout{ new QHBoxLayout };

    QVBoxLayout* optionsLayout{ new QVBoxLayout };
    m_Radio50 = static_cast<QRadioButton*>(createButton("50", "radio"));
    optionsLayout->addWidget(m_Radio50);




    //Mettre en boite :
    labelPrinc = new QLabel();
    imageLayout->addWidget(labelPrinc);
    imageLayout->addLayout(optionsLayout);

    centralWidget->setLayout(imageLayout);
    
    setCentralWidget(centralWidget);
}

QAbstractButton* QGameOfLife::createButton(QString const& txtBtn, QString const& type)
{
    QAbstractButton* button {};

    if (type == "radio") {
        button = new QRadioButton(txtBtn);
    }
    else {
        button = new QPushButton(txtBtn);
    }

    return button;
}


