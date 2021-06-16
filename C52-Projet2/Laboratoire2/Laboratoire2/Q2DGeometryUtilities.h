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


#include <QPolygonF>


class Q2DGeometryUtilities
{
public:
	Q2DGeometryUtilities() = delete;
	Q2DGeometryUtilities(Q2DGeometryUtilities const&) = delete;
	Q2DGeometryUtilities(Q2DGeometryUtilities&&) = delete;
	Q2DGeometryUtilities& operator=(Q2DGeometryUtilities const&) = delete;
	Q2DGeometryUtilities& operator=(Q2DGeometryUtilities&&) = delete;
	~Q2DGeometryUtilities() = delete;

	static qreal perimeter(QRect const & rectangle);
	static qreal perimeter(QRectF const & rectangle);
	static qreal perimeter(QPolygon const & polygon);
	static qreal perimeter(QPolygonF const & polygon);

	static qreal area(QRect const & rectangle);
	static qreal area(QRectF const & rectangle);
	static qreal area(QPolygon const & polygon); // polygon must be defined clockwise or counterclockwise without self intersection
	static qreal area(QPolygonF const& polygon); // polygon must be defined clockwise or counterclockwise without self intersection
};

