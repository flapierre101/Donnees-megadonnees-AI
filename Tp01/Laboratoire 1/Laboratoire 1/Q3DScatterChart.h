
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
#ifndef Q_3D_SCATTER_CHART_H
#define Q_3D_SCATTER_CHART_H


#include <QWidget>
#include <QTimer>
#include <QRandomGenerator>

#include <QtDataVisualization/Q3DScatter.h>

class QLabel;




/// \brief Contrôle graphique (<em>widget</em>) encapsulant un diagramme de dispersion 3d.
/// 
/// \details Cette classe est en fait un \a wrapper des classes équivalentes du module <em>Data Visualization</em> de Qt.
/// Plus spéciquement, Q3DScatterChart est une façade de la classe `QtDataVisualization::Q3DScatter` et de ses dépendances.
///		
/// Le diagramme de dispersion est constitué de \a m séries de \a n points 3d (x, y, z). Il est important de comprendre que 
/// chaque point fait partie d'une série et que seulement les séries sont paramétrables pour l'affichage. Il est ainsi impossible 
/// de déterminer l'apparence d'un point spécifiquement. Tous les points de la même série possède les mêmes caractéristiques visuelles.
/// 
/// Deux sous classes permettent la manipulation des séries et des points (insertion, suppression, édition et consultation) :
///		- Q3DScatterChart::Serie
///		- Q3DScatterChart::Point
/// 
/// Finalement, deux sous classes permettent la mise en forme du diagramme :
///		- Q3DScatterChart::ChartPresentation
///		- Q3DScatterChart::SeriePresentation
/// 
/// Voici une liste des facilités disponibles :
///		- intéraction avec l'interface usager :
///			- encapsulation du diagramme dans un QWidget
///			- rotation manuelle du point de vue par le bouton du centre de la souris
///			- zoom manuelle dans le graphique par la molette de la souris
///			- affichage ou non d'un petit panneau de contrôle facilitant l'intéraction avec le graphique 
///			  (cette interface n'est pas complète - elle ne se met pas à jour lorsque les paramètres changent par programmation)
///				- affichage ou non : titre des axes, la grille, l'arrière plan, l'ombrage
///				- détermination de la taille des formes dans le diagramme
///				- rotation automatique disponible
///				- il est possible de masquer ce contrôle en appuyant sur la barre d'espacement lorsque le \a widget possède le focus
///			- gestion simplifiée d'un point sélectionné dans le graphique (signal et connecteur)
///		- manipulation des données :
///			- séries (ajouts, suppression, modification)
///			- points (ajouts, suppression, modification)
///			- insertion de points par série incluant la création automatique de nouvelles séries
///			- détermination de valeurs 3d aléatoires
///		- présentation limité mais simplifié des éléments du graphiques :
///			- le graphique lui-même :
///				- définition du titre principal
///				- définition du titre des axes
///				- étendu des axes (manuel ou automatique)
///				- visibilité : titre principal, titre de chaque axe, la grille, l'arrière plan, l'ombrage
///			- chacune des séries :
///				- le nom
///				- le type de forme : point, sphere, cube, cube biseauté, cylindre, cône, pyramide 3 côtés et 4 côtés, flèche
///				- taille de la forme manuelle ou automatique
///				- couleur de la forme (la transparence n'est pas supporté)
///				- si la forme est lisse
/// 
/// Le lecteur intéressé devrait lire les éléments de documentation associés des classes de Qt. Principalement, ces deux articles :
///		- <a href="https://doc.qt.io/qt-5/qtdatavisualization-index.html" target="_blank">Qt Data Visualization</a>
///		- <a href="https://doc.qt.io/qt-5/q3dscatter.html#details" target="_blank">Q3DScatter</a>
/// D'ailleurs, l'utilisation des classes de Qt permet une plus grande flexibilité. 
/// 
/// 
/// 
/// L'exemple de code suivant donne ce résultat.
/// 
/// \image html Q3DScatterChartDemo.png 
/// 
/// \code{.cpp}
/// 
///	#include <QtMath>
///	#include <QtWidgets/QApplication>
///	#include <QMainWindow>
///	#include <QTimer>
///	#include "Q3DScatterChart.h"
///	
///	class Demo3DScatterChart : public QMainWindow
///	{
///	private:
///	    Q3DScatterChart* m3DScatterChart;
///	    QTimer mTimer;
///	
///	public:
///	    Demo3DScatterChart(QWidget* parent = nullptr)
///	        : QMainWindow(parent)
///	        , m3DScatterChart{ new Q3DScatterChart }
///	    {
///	        setCentralWidget(m3DScatterChart);
///	
///	        m3DScatterChart->presentation().setTitle("Q3DScatterChart demo");
///	        m3DScatterChart->presentation().setAxisTitle("x", "y", "z");
///	        m3DScatterChart->presentation().setAxisRange(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
///	
///	        // setup points : distribute each series radially 
///	        for (float radius{}; radius <= 1.0; radius += 0.1) {
///	            QString serieName(QString("rad-%1").arg(radius));
///	
///	            int nPoints{ static_cast<int>(radius * 35.0f + 1.0f) };
///	            float step{ static_cast<float>(M_PI * 2.0) / nPoints };
///	            for (float theta{}; theta < static_cast<float>(M_PI * 2.0); theta += step) {
///	                m3DScatterChart->addPoint(serieName, QVector3D(radius * qCos(theta), 0.0, radius* qSin(theta)));
///	            }
///	            (*m3DScatterChart)[serieName].presentation().setShapeType(Q3DScatterChart::SeriePresentation::ShapeType::Sphere);
///	            (*m3DScatterChart)[serieName].presentation().setShapeSize(0.25 - radius * 0.05);
///	            (*m3DScatterChart)[serieName].presentation().setShapeColor(QColor::fromHslF(radius, 1.0, 0.5, 1.0));
///	        }
///	
///	        connect(&mTimer, &QTimer::timeout, this, &Demo3DScatterChart::updateScatterChart);
///	        mTimer.start(30);
///	    }
///	
///	private:
///	    float plotFunction(float x, float z, float t) {
///	        float const twopi{ static_cast<float>(M_PI * 2.0) };
///	        float const frequency{ 2.0f };
///	
///	        float d2{ (x*x + z*z) };
///	        float d{ (float)qSqrt(d2) };
///	        float d2inv{ 1.0f - d2 };
///	
///	        return qCos(twopi / frequency * d + t) * d2inv;
///	    }
///	
///	private slots:
///	    void updateScatterChart() {
///	        static float t{1.0};
///	        t += 0.1;
///	
///	        for (int serieIndex{}; serieIndex < m3DScatterChart->serieCount(); ++serieIndex) {
///	            Q3DScatterChart::Serie serie((*m3DScatterChart)[serieIndex]);
///	
///	            for (int pointIndex{}; pointIndex < serie.size(); ++pointIndex) {
///	                Q3DScatterChart::Point point(serie[pointIndex]);
///	                point.setY(plotFunction(point.x(), point.z(), t));
///	            }
///	        }
///	    }
///	};
///  
/// 
/// int main(int argc, char *argv[])
/// {
///     QApplication a(argc, argv);
///     Demo3DScatterChart w;
///     w.show();
///     return a.exec();
/// }
/// 
/// \endcode
/// 

class Q3DScatterChart : public QWidget
{
	Q_OBJECT

public:

	class Serie;
	class Point;

	class ChartPresentation;
	class SeriePresentation;


	Q3DScatterChart(QWidget* parent = nullptr);
	~Q3DScatterChart() override = default;

	ChartPresentation presentation();

	int serieCount() const; // étape 1 pour vider le graph
	int serieIndex(QString const & name) const; // retourne -1 si le nom de la série est inexistant
	int addSerie(SeriePresentation const & p, int size = 0); // retourne l'index de la série
	QPair<int, int> addPoint(QString const & serieName, QVector3D const & point); // retourne l'index de la série et de l'item --- ajoute le point dans le graph
	QPair<int, int> addPoint(QVector3D const & point); // ajoute dans la série sans nom : retourne l'index de la série et de l'item
	bool removeSerie(int index); // etape 2 pour vider le graph -> boucle while
	bool removeSerie(QString const & name);
	Serie operator[](int index);
	Serie operator[](QString const & name);
	
	void randomize(float min, float max);
	void randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);	
	static void setSeedRandomizer(int value = 0); // 0 => seed from clock now

	bool autoRotationEnabled() const;
	float autoRotationSpeed() const;

	bool eventFilter(QObject* watched, QEvent* event) override;

public slots:
	void showControl(bool show);
	void enableAutoRotation(float xDegreesPerSec);
	void disableAutoRotation();
	void setSelectedPoint(int serie, int point);

signals:
	void selectedPointChanged(int serie, int point);

protected:
	void keyPressEvent(QKeyEvent * event) override;

private:
	/// \internal
	class Q3DScatterChartControl;
	/// \endinternal

	QtDataVisualization::Q3DScatter * m3DScatter;
	QWidget * m3DWidget;
	QLabel * mTitle;
	Q3DScatterChartControl* m3DScatterChartControl;
	QTimer mTimer;
	float mXDegreesPerSec;

	void toggleHumanInterface();

	static QRandomGenerator smRandomGenerator;
	static int const smRotationFrequency;

	static float generateRandom(float min, float max);

};





/// \brief Classe représentant une série de Q3DScatterChart.
/// 
/// \details Permet la manipulation d'une série spécifique du diagramme.
class Q3DScatterChart::Serie
{
public:


	int size() const;
	void resize(int size);
	int append(float x, float y, float z);
	int append(QVector3D const& point);
	void remove(int index, int count = 1);

	Point operator[](int index) const;
	Serie& operator<<(QVector3D const& point);

	SeriePresentation presentation();

	void randomize(float min, float max);
	void randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

	bool operator==(Serie const & serie) const;

private:
	QtDataVisualization::QScatterDataProxy* mSerie;
	Serie(QtDataVisualization::QScatterDataProxy* serie);
	friend class Q3DScatterChart;
};


/// \brief Classe représentant un points 3d de Q3DScatterChart.
/// 
/// \details Permet la manipulation d'un point spécifique du diagramme.
class Q3DScatterChart::Point
{
public:
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void set(float x, float y, float z);
	void set(QVector3D const& point);

	QVector3D coord() const;

	float x() const;
	float y() const;
	float z() const;

	void randomize(float min, float max);
	void randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

	bool operator==(Point const& point) const;

private:
	QtDataVisualization::QScatterDataProxy* mSerie;
	int mIndex;

	Point(QtDataVisualization::QScatterDataProxy* serie, int index);
	friend class Serie;
};


/// \brief Classe permettant la gestion des paramètres visuels de l'affichage du diagramme de dispersion.
/// 
/// \details Permet de consulter et modifier les caractéristiques visuelles du diagramme de dispersion.
class Q3DScatterChart::ChartPresentation
{
public:
	/// \brief Ombrage
	/// 
	enum class ShadowType {																		/// \hideinitializer
		None			= QtDataVisualization::QAbstract3DGraph::ShadowQualityNone,				/// \hideinitializer
		LowQuality		= QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftLow,			/// \hideinitializer
		MediumQuality	= QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftMedium,		/// \hideinitializer
		HighQuality		= QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftHigh,
	};

	//
	// to do
	//
	//QString title() const;
	//QString xAxisTitle() const;
	//QString yAxisTitle() const;
	//QString zAxisTitle() const;
	//bool isAxisAutoRanges() const;
	//float xAxisRangeMin() const;
	//float xAxisRangeMax() const;
	//float yAxisRangeMin() const;
	//float yAxisRangeMax() const;
	//float zAxisRangeMin() const;
	//float zAxisRangeMax() const;

	//bool titleVisibility() const;
	//bool xAxisTitleVisibility() const;
	//bool yAxisTitleVisibility() const;
	//bool zAxisTitleVisibility() const;
	//bool gridVisibility() const;
	//bool backgroundVisibility() const;
	//bool shadow() const;

	void setTitle(QString const& title);
	void setAxisTitle(QString const& xTitle, QString const& yTitle, QString const& zTitle = QString());
	void setAxisAutoRanges();
	void setAxisRange(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

	void setTitleVisibility(bool visible);
	void setAxisTitleVisibility(bool xVisible, bool yVisible, bool zVisible);
	void setGridVisibility(bool visible);
	void setBackgroundVisibility(bool visible);
	void setShadow(ShadowType shadowType);

private:
	ChartPresentation(Q3DScatterChart* chart);
	Q3DScatterChart * mChart;
	friend class Q3DScatterChart;
};


/// \brief Classe représentant la gestion des paramètres visuels de l'affichage d'une série de données.
/// 
/// \details Permet de consulter et modifier les caractéristiques visuelles d'une série de données.
class Q3DScatterChart::SeriePresentation
{
public:
	enum class ShapeType {
		Point			= QtDataVisualization::QAbstract3DSeries::MeshPoint,
		Sphere			= QtDataVisualization::QAbstract3DSeries::MeshSphere,
		Cube			= QtDataVisualization::QAbstract3DSeries::MeshCube,
		BevelCube		= QtDataVisualization::QAbstract3DSeries::MeshBevelCube,
		Cylinder		= QtDataVisualization::QAbstract3DSeries::MeshCylinder,
		Cone			= QtDataVisualization::QAbstract3DSeries::MeshCone,
		Pyramid3		= QtDataVisualization::QAbstract3DSeries::MeshMinimal,
		Pyramid4		= QtDataVisualization::QAbstract3DSeries::MeshPyramid,
		Arrow			= QtDataVisualization::QAbstract3DSeries::MeshArrow,
	};

	SeriePresentation(QString const& name, ShapeType shapeType = ShapeType::Sphere, float shapeSize = 0.0f, QColor const & shapeColor = QColor(), bool shapeSmooth = false);

	QString name() const;
	ShapeType shapeType() const;
	float shapeSize() const;
	QColor shapeColor() const;
	bool shapeSmooth() const;

	void setName(QString const& name);
	void setShapeType(ShapeType meshType);
	void setShapeSize(float size = 0.0f);				// 0 mean auto sizing
	void setShapeColor(QColor const& color);
	void setShapeSmooth(bool shapeSmooth);

private:
	SeriePresentation(QtDataVisualization::QScatter3DSeries * linkedSeries);

	QtDataVisualization::QScatter3DSeries * mLinkedSeries;
	QString mName;
	ShapeType mShapeType;
	float mShapeSize;
	QColor mShapeColor;
	bool mShapeSmooth;

	friend Q3DScatterChart;
};


#include <QScrollBar>

class Q3DScatterChart::Q3DScatterChartControl : public QWidget
{
	Q_OBJECT

private:
	Q3DScatterChartControl(Q3DScatterChart& parent);

	void synchronizeGui(); // to do... synchronization! (only auto rotation is synchronized).

	QScrollBar* mSetAutoRotate;

	friend class Q3DScatterChart;
};


#endif // Q_3D_SCATTER_CHART_H