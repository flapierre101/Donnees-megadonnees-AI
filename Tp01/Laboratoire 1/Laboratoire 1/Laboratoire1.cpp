#include "Laboratoire1.h"
#include <QVBoxLayout>
#include <QDebug>
#include <cmath>
#include "FeatureExtraction.h"
#include <Qdir>
#include <qdiriterator>
#include <qfiledialog>
#include <qvector3d>
#include "KNN.h"



Laboratoire1::Laboratoire1(QWidget* parent)
    : QMainWindow(parent)
    , mChart{ new Q3DScatterChart }

{
    ui.setupUi(this);
    mChart->presentation().setAxisRange(0, 1, 0, 2, 0, 2);
    initLabo();
    
}

void Laboratoire1::doTest() {
 
  QImage imageDummy{};
  FeatureExtraction::scanImage(imageDummy);




}

void Laboratoire1::choixDossier() {
    
    // voir doc de QfileDialog pour choisir un fichier. 
    // getOpenFileName
    
    mDir = QFileDialog::getExistingDirectory( // fonction statique affichant boîte de dlg 
        this, // parent de la fenêtre modale
        "Select folder", // message affiché "
        "C:\\",         // dossier de depart (ici = working dir) 
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); // options 
                                                                       
        // la variable dir possède maintenant : 
        // - soit une chaîne de caractères vide si l’usager a fait cancel dans la boîte de dialogue 
        // - soit une chaîne de caractères valide correspondant à un dossier 
    
  

    if (!mDir.isNull()) {
        mLabelResPhase1->setText("Dossier ok!");
        int i = mChart->serieCount();

        for (i; i >= 0; --i) {
            mChart->removeSerie(i);
        }
        mKNN.clearData();
    }
    else
        mLabelResPhase1->setText("Dossier incompatible");

    

}

void Laboratoire1::debutPhase1()
{
    float m1Min, m1Max, m2Min, m2Max, m3Min, m3Max;

    m1Max = m2Max = m3Max = 0;
    m1Min = m1Min = m1Min = 250;
      
      if (!mDir.isNull()) {
          QDirIterator it(mDir, QStringList() << "*.png", QDir::Files, QDirIterator::NoIteratorFlags);
          while (it.hasNext()) {
              QFileInfo info(it.next());

              mTempoImg.load(info.absoluteFilePath()); // Chemin et fichier   

              if (mTempoImg.isNull())
                  return;

              if (mTempoImg.text("Author") != "KlustR" || mTempoImg.text("Tag").isNull())
                  return;

              mVectFeatures = FeatureExtraction::scanImage(mTempoImg);
              QString tempoTag = mTempoImg.text("Tag");
              qDebug() << tempoTag;

       

              if (mVectFeatures[0] > m1Max)
                  m1Max = mVectFeatures[0];
              if (mVectFeatures[0] < m1Min)
                  m1Min = mVectFeatures[0];
              if (mVectFeatures[1] > m2Max)
                  m2Max = mVectFeatures[1];
              if (mVectFeatures[1] < m2Min)
                  m2Min = mVectFeatures[1];
              if (mVectFeatures[2] > m3Max)
                  m3Max = mVectFeatures[2];
              if (mVectFeatures[2] < m3Min)
                  m3Min = mVectFeatures[2];
             
  

              KNN::sData dataTempo;

              dataTempo.mValues = mVectFeatures;
              dataTempo.mTag = tempoTag;

              mKNN.addData(dataTempo);

              mChart->addPoint(tempoTag, QVector3D(mVectFeatures[0], mVectFeatures[1], mVectFeatures[2]));

              
          }
          mLabelResPhase1->setText("Phase 1 ok!");

      }


      
}

void Laboratoire1::debutPhase2()
{

    if (!mDir.isNull() && !mImgPath.isNull()) {

        mTempoImg.load(mImgPath);

        if (mTempoImg.isNull())
            return;

        if (mTempoImg.text("Author") != "KlustR" || mTempoImg.text("Tag").isNull())
            return;

        mVectFeatures = FeatureExtraction::scanImage(mTempoImg);
        QString tempoTag = mTempoImg.text("Tag");
        qDebug() << tempoTag;

        KNN::sData dataTempo;

        dataTempo.mValues = mVectFeatures;
        dataTempo.mTag = tempoTag;

        QString rep = mKNN.analyseData(dataTempo, kValue);

        mChart->addPoint("Forme test", QVector3D(mVectFeatures[0], mVectFeatures[1], mVectFeatures[2]));

        qDebug() << rep;

        if(rep == tempoTag)
            mLabelResPhase2->setText("Bravo, bien un " + rep);
        else
            mLabelResPhase2->setText("Erreur! Trouve :" + rep + " Reponse etait : " + tempoTag);


    }


}

void Laboratoire1::choixFichier()
{

    mImgPath = QFileDialog::getOpenFileName( // fonction statique affichant boîte de dlg 
        this,                   // parent de la fenêtre modale
        tr("Open File"),        // message affiché "
        "C:\\",                // dossier de depart (ici = working dir) 
        tr("Images (*.png)"));  // options 

        // la variable dir possède maintenant : 
        // - soit une chaîne de caractères vide si l’usager a fait cancel dans la boîte de dialogue 
        // - soit une chaîne de caractères valide correspondant à un dossier 

    qDebug() << mImgPath;



}

void Laboratoire1::updateKValue()
{
    kValue = mLabelResKselect->text().toInt();
    qDebug() << kValue;
}




void Laboratoire1::initLabo()
{

  mImage = QImage(mDimension, mDimension, QImage::Format_Indexed8);

  QLabel* labelPhase1 = new QLabel;
  labelPhase1->setText("Phase 1");
  QPushButton* buttonChoixDossierPhase1 = new QPushButton("Choisir le dossier source");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);
  QPushButton* buttonLancerPhase1 = new QPushButton("Commencer l'entrainement");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);

  mLabelResPhase1 = new QLabel;
  mLabelResPhase1->setText("Resultats Phase 1 affiches ici");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);

  QLabel* labelSpace = new QLabel;
  labelSpace->setText("");

  QVBoxLayout* phase1Layout{ new QVBoxLayout };
  phase1Layout->addWidget(labelPhase1);
  phase1Layout->addWidget(buttonChoixDossierPhase1);
  phase1Layout->addWidget(labelSpace);
  phase1Layout->addWidget(buttonLancerPhase1);
  phase1Layout->addWidget(mLabelResPhase1);

  QLabel* labelPhase2 = new QLabel;
  labelPhase2->setText("Phase 2");

  QPushButton* buttonChoixImagePhase2 = new QPushButton("Choisir l'image a classifier");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);
  QPushButton* buttonLancerPhase2 = new QPushButton("Classifier");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);

  mLabelResPhase2 = new QLabel;
  mLabelResPhase2->setText("Resultats Phase 2 affiches ici");
  //connect(mTest, &QPushButton::clicked, this, &Laboratoire1::doTest);

  mLabelResKselect = new QLabel;
  mLabelResKselect->setText("1");

  QScrollBar* scrollBar_Kselect_phase2 = new QScrollBar;
  scrollBar_Kselect_phase2->setOrientation(Qt::Horizontal);
  scrollBar_Kselect_phase2->setRange(1, 15);
  scrollBar_Kselect_phase2->setValue(1);
  connect(scrollBar_Kselect_phase2, &QScrollBar::valueChanged, mLabelResKselect, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));

  kValue = mLabelResKselect->text().toInt();


  QHBoxLayout* kSelectLayout{ new QHBoxLayout };
  kSelectLayout->addWidget(labelSpace);
  kSelectLayout->addWidget(mLabelResKselect);
  kSelectLayout->addWidget(scrollBar_Kselect_phase2);

  QVBoxLayout* phase2Layout{ new QVBoxLayout };
  phase2Layout->addWidget(labelPhase2);
  phase2Layout->addWidget(buttonChoixImagePhase2);
  phase2Layout->addLayout(kSelectLayout);
  phase2Layout->addWidget(buttonLancerPhase2);
  phase2Layout->addWidget(mLabelResPhase2);

  QHBoxLayout* phasesLayout{ new QHBoxLayout };
  phasesLayout->addLayout(phase1Layout);
  phasesLayout->addLayout(phase2Layout);

  QVBoxLayout* mainLayout{ new QVBoxLayout };
  mainLayout->addWidget(mChart);
  mainLayout->addLayout(phasesLayout);

  QWidget* mainWidget{ new QWidget };
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  QImage imageDummy{};

  connect(buttonChoixDossierPhase1, &QPushButton::clicked, this, &Laboratoire1::choixDossier);
  connect(buttonLancerPhase1, &QPushButton::clicked, this, &Laboratoire1::debutPhase1);
  connect(buttonChoixImagePhase2, &QPushButton::clicked, this, &Laboratoire1::choixFichier);
  connect(buttonLancerPhase2, &QPushButton::clicked, this, &Laboratoire1::debutPhase2);
  connect(scrollBar_Kselect_phase2, &QScrollBar::valueChanged, this, &Laboratoire1::updateKValue);
}

