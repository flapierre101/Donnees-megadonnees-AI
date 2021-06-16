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

ShapeOptimizer reçoit les paramètres choisis dans la vue et applique les transformations sur les formes selon les gènes reçus de AlgoGen. Elle définit également l'étendue
des valeurs de mutation possibles selon chaque type de gène (scaling: 1x à 12x, translationX: de 0 à la taille du canevas, translationY: de 0 à la taille du canevas, rotate: 360 degrés, dans le sens
horaire ou contraire).

*/

#pragma once

#include <QVector>
#include "AlgoGen.h"
#include <string>
#include "Q2DGeometryUtilities.h"
#include <QLabel>

class ShapeOptimizer
{
private:
	// infos pour AlgoGen
	size_t mTaillePop;
	size_t mTauxMutation;
	size_t mNbElite;
	size_t mNbGene;
	size_t nbGenerationMax;
	size_t mMeilleurAire;

	size_t mNbPoints;
	size_t mTailleCanvas;
	size_t mNbCoins;
	std::string mForme;
	std::string mFormeVoulue;
	QVector<QPointF> mPoints;
	QRectF mCanvas;
	AlgoGen algoGen;
	QPolygonF laForme;
	AlgoGen::EtendueGenes mEtendue;
	std::string formeVoulue;

	QVector<QPolygonF> mFormesReussies, mFormesRejetees;

	double* scale, *transX, *transY, *rotation;
	void init();

	AlgoGen::DataType mFitness(AlgoGen::Solution);
	

public:
	ShapeOptimizer(std::string formeVoulue, size_t tailleCanvas, size_t nbObstacles, size_t nbElite, size_t taillePop, size_t tauxMutation);
	ShapeOptimizer();
	void setUp(std::string formeVoulue, size_t tailleCanvas, size_t nbObstacles, size_t nbElite, size_t taillePop, size_t tauxMutation);
	QVector<QPolygonF> getFormesReussies();
	QVector<QPolygonF> getFormesRejetees();
	QVector<QPointF> getObstacles();
	size_t getMeilleurAire();
	void next();
};
