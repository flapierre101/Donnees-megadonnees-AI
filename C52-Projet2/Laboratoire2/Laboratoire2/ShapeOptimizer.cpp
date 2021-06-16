#include "ShapeOptimizer.h"
#include <QPainter>
#include <algorithm>

//Selon paramètres recus, positionne une forme de base dans le coin supérieur gauche du canevas pour un maximum d'espace d'exploration, applique les transformations et détermine validité des solutions


ShapeOptimizer::ShapeOptimizer()
	: ShapeOptimizer("Triangle", 0,0,0,0,0)
{
}

void ShapeOptimizer::setUp(std::string formeVoulue, size_t tailleCanvas, size_t nbObstacles, size_t nbElite, size_t taillePop, size_t tauxMutation)	
{
	mFormeVoulue = formeVoulue;
	mTailleCanvas = tailleCanvas;
	mNbPoints = nbObstacles;
	mNbElite = nbElite;
	mTaillePop = taillePop;
	mTauxMutation = tauxMutation;
	init();
}

ShapeOptimizer::ShapeOptimizer(std::string formeVoulue, size_t tailleCanvas, size_t nbObstacles, size_t nbElite, size_t taillePop, size_t tauxMutation)
	:mTaillePop{ taillePop },
	mTauxMutation{ tauxMutation },
	mNbElite{ nbElite },
	mNbGene{ 4 },
	mFormeVoulue {formeVoulue},
	mNbPoints{ nbObstacles },
	mTailleCanvas{ tailleCanvas },
	mNbCoins{},
	mForme(),
	mPoints(),
	mFormesReussies{},
	mFormesRejetees{},
	mMeilleurAire{},
	mCanvas{}
{
	init();
}

void ShapeOptimizer::init()
{
	mEtendue.resize(4);
	// Scaling
	mEtendue[0][0] = 1.0;
	mEtendue[0][1] = 12.0;
	// Trans X
	mEtendue[1][0] = 0.0;
	mEtendue[1][1] = (mTailleCanvas);
	// Trans Y
	mEtendue[2][0] = 0.0;
	mEtendue[2][1] = (mTailleCanvas);
	// Rotation
	mEtendue[3][0] = -360.00;
	mEtendue[3][1] = 360.00;


	mPoints.resize(mNbPoints);

	// Remplir Vecteur de points
	for (int i{ 0 }; i < mNbPoints; i++) {
		size_t x{ rand() % mTailleCanvas + 1 };
		x = std::clamp(x, 25ull, mTailleCanvas - 25);
		size_t y{ rand() % mTailleCanvas + 1 };
		y = std::clamp(y, 25ull, mTailleCanvas - 25 );
		QPointF point(x, y);
		mPoints[i] = point;
	}

	//Creation Canvas
	mCanvas = QRectF(0, 0, mTailleCanvas, mTailleCanvas);

	laForme.clear();

	if (mFormeVoulue == "Triangle") {					
	
		laForme
			<< QPointF(0, 0) // Triangle Equi
			<< QPointF(0, 40)
			<< QPointF(40, 20);
	}
	else if (mFormeVoulue == "Square") {
		laForme
			<< QPointF(0, 0)
			<< QPointF(0, 30)
			<< QPointF(30, 30)
			<< QPointF(30, 0);
	}

	else { // Rectangle
		laForme
			<< QPointF(0,0 )
			<< QPointF(0, 30)
			<< QPointF(60, 30 )
			<< QPointF(60, 0);
	}

	
	//reset
	algoGen = AlgoGen(mNbGene, mEtendue, mTaillePop, mTauxMutation, mNbElite, std::bind(&ShapeOptimizer::mFitness, this, std::placeholders::_1));
}

QVector<QPointF> ShapeOptimizer::getObstacles() {
	return mPoints;
}

size_t ShapeOptimizer::getMeilleurAire() {
	return mMeilleurAire;
}


AlgoGen::DataType ShapeOptimizer::mFitness(AlgoGen::Solution s)
{
	QPolygonF formeTemp = laForme;

	QTransform transform; 
	transform.translate(s[1], s[2]); 
	transform.scale(s[0], s[0]); 
	transform.rotate(s[3]);		
							    
	formeTemp = transform.map(formeTemp); 
	

	// calcul aire du polygone 
	double area{ Q2DGeometryUtilities::area(formeTemp) }; 


	// determine si le point entre en collision avec le rectangle 
	AlgoGen::DataType reponse = area;
	
	 for (QPointF point : mPoints) {
		 if (formeTemp.containsPoint(point, Qt::OddEvenFill)) {
			 reponse=0;
		 }
	 }
	 QRectF boundingRect = formeTemp.boundingRect();

	 for (QPointF point : formeTemp) {
		 if (!mCanvas.contains(point))
			 reponse = 0;
	 }
	 
	 if (!mCanvas.contains(formeTemp.boundingRect()))
		 reponse = 0;
	 	 
	 return reponse;
}

QVector<QPolygonF> ShapeOptimizer::getFormesReussies()
{
	mFormesReussies = {};
	mFormesRejetees = {};
	AlgoGen::Population genCourante = algoGen.getGenerationCourante();

	for (auto& individu : genCourante) {
			QPolygonF formeTemp = laForme;

			QTransform transform;
			transform.translate(individu.solution[1], individu.solution[2]); 
			transform.scale(individu.solution[0], individu.solution[0]); 
			transform.rotate(individu.solution[3]);		
										
			formeTemp = transform.map(formeTemp); 

			if (individu.fitnessValue > 1.0) 
				mFormesReussies.append(formeTemp);					
			else
				mFormesRejetees.append(formeTemp);

	}

	mMeilleurAire = genCourante[0].fitnessValue;

	return mFormesReussies;
}

QVector<QPolygonF> ShapeOptimizer::getFormesRejetees()
{

	return mFormesRejetees;
}

void ShapeOptimizer::next()
{
	algoGen.nextGen();
}



