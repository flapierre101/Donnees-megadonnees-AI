#include "Laboratoire2.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <string>

// Contient fonction d'initialisation du labo, fonctions de création des différents menus/boutons/connect, affichage des solutions générées par AlgoGen et ShapeOptimizer

Laboratoire2::Laboratoire2(QWidget* parent)
	: QMainWindow(parent),
	start{ false },
	reset{ true },
	mGen{ 0 },
	mTaille{500}
{
	ui.setupUi(this);
	initLabo();

	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(tick()));
	mTimer->start(mVitesse);
}

void Laboratoire2::initLabo()
{

	phase1 = new QGroupBox();
	phase2 = new QGroupBox();
	parametres = new QGroupBox();
	controles = new QGroupBox();

	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* phase1Layout{ new QVBoxLayout };
	QVBoxLayout* phase2Layout{ new QVBoxLayout };
	QHBoxLayout* phasesLayout{ new QHBoxLayout };           // contient sections phase 1 et phase 2 du bas
	QVBoxLayout* paramsLayout{ new QVBoxLayout };
	QVBoxLayout* controlsLayout{ new QVBoxLayout };
	QVBoxLayout* optControlsLayout{ new QVBoxLayout };      // contient sections parametres et controles
	QHBoxLayout* canvasControlsLayout{ new QHBoxLayout };   // contient le canvas et le optControlsLayout
	QVBoxLayout* mainLayout{ new QVBoxLayout };             // contient le canvasControlsLayout et le phasesLayout

    // Creation section Parametres

  QLabel* labelShape, * labelSize, * labelElite, * labelPopulation, * labelMutation, * labelObstacles;

  QStringList shapeList = { "Rectangle", "Triangle", "Square" };							// listes des choix de paramètres disponibles pour les différents menus (QComboBox)
  QStringList sizeList = { "500 x 500", "700 x 700", "800 x 800" };
  QStringList eliteList = { "0", "10", "25" };
  QStringList popList = { "100", "250", "500" };
  QStringList mutationList = { "10", "25", "50" };
  QStringList obstaclesList = { "10", "25", "50", "100" };

  paramsLayout->addLayout(createParams("Choisissez une forme: ", labelShape, mShape, mShapeMenu, shapeList));
  paramsLayout->addLayout(createParams("Choisissez la taille du canevas: ", labelSize, mCanvasSize, mSizeMenu, sizeList));
  paramsLayout->addLayout(createParams("Choisissez le nombre d'élites: ", labelElite, mElite, mEliteMenu, eliteList));
  paramsLayout->addLayout(createParams("Choisissez la taille de la population: ", labelPopulation, mPopulation, mPopulationMenu, popList));
  paramsLayout->addLayout(createParams("Choisissez le taux de mutation (%): ", labelMutation, mMutation, mMutationMenu, mutationList));
  paramsLayout->addLayout(createParams("Choisissez le nombre d'obstacles: ", labelObstacles, mObstacles, mObstaclesMenu, obstaclesList));

  parametres->setLayout(paramsLayout);
  parametres->setTitle("PARAMETRES");

  // Creation section Phase 1

	QLabel* labelCurrentShape, * labelCurrentSize, * labelCurrentElite, * labelCurrentPop, * labelCurrentMutation, * labelCurrentObstacles;

  phase1Layout->addLayout(createPhaseLayout("Forme choisie: ", labelCurrentShape, mShape, mShapeMenu));
  phase1Layout->addLayout(createPhaseLayout("Taille choisie: ", labelCurrentSize, mCanvasSize, mSizeMenu));
  phase1Layout->addLayout(createPhaseLayout("Nombre d'élites: ", labelCurrentElite, mElite, mEliteMenu));
  phase1Layout->addLayout(createPhaseLayout("Population: ", labelCurrentPop, mPopulation, mPopulationMenu));
  phase1Layout->addLayout(createPhaseLayout("Ratio mutation (%): ", labelCurrentMutation, mMutation, mMutationMenu));
  phase1Layout->addLayout(createPhaseLayout("Nombre d'obstacles: ", labelCurrentObstacles, mObstacles, mObstaclesMenu));

	phase1->setLayout(phase1Layout);
	phase1->setTitle("PHASE 1");


	// Creation section Phase 2
	QLabel* labelPhase2, * labelCurrentGen, * labelBestArea;
	mShowRejects = new QCheckBox();
	mShowRejects->setText("Afficher les solutions rejetées");

  phase2Layout->addLayout(createPhaseTwoLayout("No de generation: ", labelCurrentGen, mCurrentGen));
  phase2Layout->addLayout(createPhaseTwoLayout("Aire de la meilleure solution: ", labelBestArea, mBestArea));
  phase2Layout->addWidget(mShowRejects);
  phase2Layout->setAlignment(Qt::AlignHCenter);
  phase2->setLayout(phase2Layout);
  phase2->setTitle("PHASE 2");

	phasesLayout->addWidget(phase1);
	phasesLayout->addWidget(phase2);


  // Creation section Controles

	controlsLayout->addWidget(createButton("Start / Pause", mStart));
	controlsLayout->addWidget(createButton("Step", mStep));
	controlsLayout->addWidget(createButton("Reinitialiser", mReinitialize));
	controlsLayout->setAlignment(Qt::AlignHCenter);
	controlsLayout->setSpacing(25);
	controles->setLayout(controlsLayout);
	controles->setTitle("CONTROLES");


	optControlsLayout->addWidget(parametres);
	optControlsLayout->addWidget(controles);

	//Mise en boite

	mMainLabel = new QLabel();											// Label pour PixMap
	mMainLabel->setAlignment(Qt::AlignHCenter);
	mMainLabel->setFixedHeight(900);
	mMainLabel->setFixedWidth(900);

	canvasControlsLayout->addWidget(mMainLabel);
	canvasControlsLayout->addLayout(optControlsLayout);
	mainLayout->addLayout(canvasControlsLayout);
	mainLayout->addLayout(phasesLayout);
	centralWidget->setLayout(mainLayout);
	setCentralWidget(centralWidget);

	connect(mStep, &QPushButton::clicked, this, &Laboratoire2::prochaineGen);
	connect(mStart, &QPushButton::clicked, this, &Laboratoire2::startGen);
	connect(mReinitialize, &QPushButton::clicked, this, &Laboratoire2::resetSim);
}

QHBoxLayout* Laboratoire2::createParams(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected, QComboBox*& cb, QStringList const& items)
{
  labelTitle = new QLabel(txtTitle);
  paramSelected = new QLabel();
  cb = new QComboBox(this);
  cb->addItems(items);								// ajout de la liste des choix disponibles
  cb->setFixedWidth(mParamWidth);

	QHBoxLayout* layout{ new QHBoxLayout };
	labelTitle->setFixedWidth(mLabelWidth);
	layout->addWidget(labelTitle);
	layout->addWidget(cb);

	connect(cb, &QComboBox::currentTextChanged, paramSelected, &QLabel::setText);

	return layout;
}

QHBoxLayout* Laboratoire2::createPhaseLayout(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected, QComboBox*& paramMenu)
{
  labelTitle = new QLabel(txtTitle);
  paramSelected->setText(paramMenu->itemText(0));
  labelTitle->setFixedWidth(mLabelWidth);
  paramSelected->setFixedWidth(mPhaseWidth);

	QHBoxLayout* layout{ new QHBoxLayout };
	layout->addWidget(labelTitle);
	layout->addWidget(paramSelected);

	return layout;
}

QPushButton* Laboratoire2::createButton(QString const& buttonTxt, QPushButton*& button)
{

	button = new QPushButton(buttonTxt);
	button->setFixedWidth(mButtonWidth);

	return button;
}

QHBoxLayout* Laboratoire2::createPhaseTwoLayout(QString const& txtTitle, QLabel*& labelTitle, QLabel*& paramSelected)
{
  labelTitle = new QLabel(txtTitle);
  paramSelected = new QLabel("0");
  labelTitle->setFixedWidth(mLabelWidth);
  paramSelected->setFixedWidth(mPhaseWidth);

  QHBoxLayout* layout{ new QHBoxLayout };
  layout->addWidget(labelTitle);
  layout->addWidget(paramSelected);

  return layout;
}

void Laboratoire2::dessiner() {

	QSize size(mTaille, mTaille);
	QPixmap pixmap(size);
	pixmap.fill(Qt::white);

	QPainter painter(&pixmap);

	QVector<QPolygonF> tempoForme;

	if (mShowRejects->isChecked()) {
		tempoForme = so.getFormesRejetees();
		painter.setPen(QPen(Qt::gray, 3.0));
		
		for (int i{ 0 }; i < tempoForme.size(); ++i) {

				painter.drawPolygon(tempoForme[i]);
		}
	}

	tempoForme = so.getFormesReussies();
	
	for (int i{ tempoForme.size() - 1 }; i >= 0; --i) {							// ordre décroissant afin de dessiner la meilleure solution en dernier, donc "par-dessus" les autres formes pour plus de visibilité
		if (i == 0) {

			painter.setPen(QPen(Qt::red, 3.0));													// meilleure solution en rouge
			painter.drawPolygon(tempoForme[i]);
		}
		else {

			painter.setPen(QPen(Qt::blue, 3.0));
			painter.drawPolygon(tempoForme[i]);

		}
	}

	painter.setPen(QPen(Qt::black, 5.0));
	painter.setBrush(QBrush(QColor(0, 0, 0)));


	for (QPointF point : so.getObstacles()) {											// points/obstacles
		painter.drawPoint(point);
	}

	mMainLabel->setPixmap(pixmap);

}

void Laboratoire2::updateLabels(size_t aire) {


	mBestArea->setText(QString::number(aire));

	mCurrentGen->setText(QString::number(mGen));

}

void Laboratoire2::prochaineGen() {


	if (reset) {																// vérification si l'utilisateur a demandé une réinitialisation de la simulation en cours
		
		if (mCanvasSize->text() == "500 x 500")
			mTaille = 500;
		else if (mCanvasSize->text() == "700 x 700")
			mTaille = 700;
		else if (mCanvasSize->text() == "800 x 800")
			mTaille = 800;

		so.setUp(mShape->text().toStdString(), mTaille, mObstacles->text().toInt(), mElite->text().toInt(), mPopulation->text().toInt(), mMutation->text().toInt());
		reset = false;
		
	}

	dessiner();

	mGen++;

	updateLabels(so.getMeilleurAire());

	so.next();

}


void Laboratoire2::tick() {
	if (start) {

		prochaineGen();								// défilement en continu des différentes solutions/générations (ou une seule génération à la fois si l'utilisateur appuie sur bouton Step)
	}

}

void Laboratoire2::startGen() {
	start = !start;									// lié au bouton start / pause
}

void Laboratoire2::resetSim() {
	reset = !reset;
	mGen = 0;
	mBestArea->setText("0");
	mCurrentGen->setText(QString::number(mGen));
	
}








