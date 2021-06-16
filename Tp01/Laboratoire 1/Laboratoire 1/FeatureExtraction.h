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
#include <QColor>
#include <QVector>


class FeatureExtraction
{

public:
	static std::array<float, 3> scanImage(QImage const& image);

	
	
private:
	static float calculDistance(float x1, float y1, int x2, int y2);


};
