#include "Q3DScatterChart.h"


#include <QGridLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QtMath>
#include <stdexcept>

using namespace QtDataVisualization;


int const Q3DScatterChart::smRotationFrequency{ 30 };
QRandomGenerator Q3DScatterChart::smRandomGenerator;

//test git


Q3DScatterChart::Q3DScatterChart(QWidget * parent)
	: QWidget(parent)
	, m3DScatter{ new Q3DScatter }
	, m3DWidget{ QWidget::createWindowContainer(m3DScatter) }
	, mTitle{ new QLabel }
	, m3DScatterChartControl{ new Q3DScatterChartControl(*this) }
	, mTimer()
	, mXDegreesPerSec{}
{
	m3DScatter->setAspectRatio(1.0);
	m3DScatter->setHorizontalAspectRatio(1.0);

	QFont font(mTitle->font());
	font.setBold(true);
	mTitle->setFont(font);
	mTitle->setAlignment(Qt::AlignCenter);
	mTitle->setText("Title");
	m3DScatter->installEventFilter(this);

	QVBoxLayout* vLayout{ new QVBoxLayout };
	vLayout->addWidget(mTitle);
	vLayout->addWidget(m3DWidget);

	QHBoxLayout* layout{ new QHBoxLayout(this) };
	layout->addLayout(vLayout);
	layout->addWidget(m3DScatterChartControl);

	presentation().setAxisTitle("X", "Y", "Z");
	presentation().setAxisTitleVisibility(true, true, true);

	connect(&mTimer, &QTimer::timeout, [this]() {
			m3DScatter->scene()->activeCamera()->setXRotation(m3DScatter->scene()->activeCamera()->xRotation() + mXDegreesPerSec);
			//m3DScatter->scene()->activeCamera()->setYRotation(m3DScatter->scene()->activeCamera()->yRotation() + mYDegreesPerSec);
		});
	mTimer.setInterval(smRotationFrequency);

	
	mTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	m3DWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	
	setFocusPolicy(Qt::StrongFocus);
}

Q3DScatterChart::ChartPresentation Q3DScatterChart::presentation()
{
	return ChartPresentation(this);
}

int Q3DScatterChart::serieCount() const
{
	return m3DScatter->seriesList().size();
}

int Q3DScatterChart::serieIndex(QString const& name) const
{
	QList<QScatter3DSeries*> series(m3DScatter->seriesList());
	auto it(std::find_if(series.begin(), series.end(), [&name](QScatter3DSeries* serie)->bool { return serie->name() == name; }));

	if (it == series.end()) {
		return -1;
	}

	return std::distance(series.begin(), it);
}

int Q3DScatterChart::addSerie(SeriePresentation const & p, int size)
{
	if (serieIndex(p.name()) != -1) {
		return -1;
	}

	QScatterDataProxy* proxy{ new QScatterDataProxy };
	QScatter3DSeries* series{ new QScatter3DSeries(proxy) };
	m3DScatter->addSeries(series);

	QScatterDataArray* dataArray{ new QScatterDataArray };

	if (size > 0) {
		dataArray->resize(size);
		m3DScatter->seriesList().last()->dataProxy()->resetArray(dataArray);
	}
	
	m3DScatter->seriesList().last()->setName(p.name());
	m3DScatter->seriesList().last()->setMesh(static_cast<QAbstract3DSeries::Mesh>(static_cast<int>(p.shapeType())));
	m3DScatter->seriesList().last()->setItemSize(p.shapeSize());
	m3DScatter->seriesList().last()->setBaseColor(p.shapeColor());
	m3DScatter->seriesList().last()->setMeshSmooth(p.shapeSmooth());
	m3DScatter->seriesList().last()->setItemLabelFormat(("@seriesName [@xTitle, @yTitle, @zTitle] = (@xLabel, @yLabel, @zLabel)"));

	int serieIndex{ serieCount() - 1 };
	connect(series, &QScatter3DSeries::selectedItemChanged, [this, serieIndex](int pointIndex) { 
			if (pointIndex >= 0) { // when serie change, this signal is emmited with index to -1, then this signal is emmited again with the corresponding index
				emit selectedPointChanged(serieIndex, pointIndex);
			}
		});

	return serieIndex;
}

bool Q3DScatterChart::removeSerie(int index)
{
	if (index < 0 || index >= serieCount()) {
		return false;
	}

	QScatter3DSeries* series{ m3DScatter->seriesList()[index] };
	//disconnection is automatic since the object is destroyed : disconnect(series, &QScatter3DSeries::selectedItemChanged);

	m3DScatter->removeSeries(series);
	return true;
}

bool Q3DScatterChart::removeSerie(QString const& name)
{
	return removeSerie(serieIndex(name));
}

Q3DScatterChart::Serie Q3DScatterChart::operator[](int index)
{
	if (index < 0 || index >= serieCount()) {
		throw std::invalid_argument("serie name do not exist in Q3DScatterChart::operator[]");
	}
	return Serie(m3DScatter->seriesList()[index]->dataProxy());
}

Q3DScatterChart::Serie Q3DScatterChart::operator[](QString const& name)
{
	return operator[](serieIndex(name));
}

QPair<int, int> Q3DScatterChart::addPoint(QString const& serieName, QVector3D const& point)
{
	int index{ serieIndex(serieName) };
	if (index == -1) {
		index = addSerie(SeriePresentation(serieName));
	}
	
	return QPair<int, int>(index, operator[](index).append(point));
}

QPair<int, int> Q3DScatterChart::addPoint(QVector3D const& point)
{
	return addPoint("", point);
}

void Q3DScatterChart::setSeedRandomizer(int value)
{
	if (value == 0) {
		smRandomGenerator.seed(std::seed_seq{ std::chrono::steady_clock::now().time_since_epoch().count() });
	} else {
		smRandomGenerator.seed(std::seed_seq{ value });
	}
}

bool Q3DScatterChart::autoRotationEnabled() const
{
	return autoRotationSpeed() != 0.0f;
}

float Q3DScatterChart::autoRotationSpeed() const
{
	return mXDegreesPerSec * static_cast<float>(smRotationFrequency);
}

void Q3DScatterChart::enableAutoRotation(float xDegreesPerSec)
{
	if (xDegreesPerSec == 0.0f) {
		disableAutoRotation();
	}

	mXDegreesPerSec = xDegreesPerSec / static_cast<float>(smRotationFrequency);
	//mYDegreesPerSec = yDegreesPerSec / static_cast<float>(smRotationFrequency);
	mTimer.start();

	m3DScatterChartControl->synchronizeGui();
}

void Q3DScatterChart::disableAutoRotation()
{
	mTimer.stop();
	m3DScatterChartControl->synchronizeGui();
}

void Q3DScatterChart::showControl(bool show)
{
	layout()->itemAt(1)->widget()->setVisible(show);
}

void Q3DScatterChart::setSelectedPoint(int serie, int point)
{
	if (serie < 0 || point < 0) return;

	QList<QScatter3DSeries*> series(m3DScatter->seriesList());
	if (serie < series.size()) {
		QScatter3DSeries* s{ series[serie] };
		if (point < s->dataProxy()->itemCount()) {
			s->setSelectedItem(point);
		}
	}
}

bool Q3DScatterChart::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m3DScatter) {
		if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Space) {
			toggleHumanInterface();
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void Q3DScatterChart::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space) {
		toggleHumanInterface();
	}
}

void Q3DScatterChart::toggleHumanInterface()
{
	layout()->itemAt(1)->widget()->setVisible(!layout()->itemAt(1)->widget()->isVisible());
}

float Q3DScatterChart::generateRandom(float min, float max)
{
	return smRandomGenerator.generateDouble() * (max - min) + min;
}

void Q3DScatterChart::randomize(float min, float max)
{
	randomize(min, max, min, max, min, max);
}

void Q3DScatterChart::randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	for (int i{}; i < serieCount(); ++i) {
		operator[](i).randomize(minX, maxX, minY, maxY, minZ, maxZ);
	}
}



////////////////////////////////////////////////////////////////////////////////
// Q3DScatterChart::Serie

Q3DScatterChart::SeriePresentation Q3DScatterChart::Serie::presentation()
{
	return SeriePresentation(mSerie->series());
}

int Q3DScatterChart::Serie::size() const
{
	return mSerie->itemCount();
}

void Q3DScatterChart::Serie::resize(int size)
{
	QScatterDataArray* dataArray{ new QScatterDataArray };
	dataArray->resize(qMax(1, size));
	mSerie->resetArray(dataArray);
}

Q3DScatterChart::Point Q3DScatterChart::Serie::operator[](int index) const
{
	return Point(mSerie, index);
}

Q3DScatterChart::Serie& Q3DScatterChart::Serie::operator<<(QVector3D const& point)
{
	append(point);
	return *this;
}

int Q3DScatterChart::Serie::append(float x, float y, float z)
{
	return append(QVector3D(x, y, z));
}

int Q3DScatterChart::Serie::append(QVector3D const& point)
{
	return mSerie->addItem(QScatterDataItem(point));
}

void Q3DScatterChart::Serie::remove(int index, int count)
{
	mSerie->removeItems(index, count);
}

void Q3DScatterChart::Serie::randomize(float min, float max)
{
	randomize(min, max, min, max, min, max);
}

void Q3DScatterChart::Serie::randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	for (int i{}; i < size(); ++i) {
		operator[](i).randomize(minX, maxX, minY, maxY, minZ, maxZ);
	}
}

bool Q3DScatterChart::Serie::operator==(Serie const& serie) const
{
	return mSerie == serie.mSerie;
}

Q3DScatterChart::Serie::Serie(QtDataVisualization::QScatterDataProxy * serie)
	: mSerie{ serie }
{
}


////////////////////////////////////////////////////////////////////////////////
// Q3DScatterChart::Point

void Q3DScatterChart::Point::setX(float x)
{
	QScatterDataItem const * item{ mSerie->itemAt(mIndex) };
	set(x, item->y(), item->z());
}

void Q3DScatterChart::Point::setY(float y)
{
	QScatterDataItem const* item{ mSerie->itemAt(mIndex) };
	set(item->x(), y, item->z());
}

void Q3DScatterChart::Point::setZ(float z)
{
	QScatterDataItem const* item{ mSerie->itemAt(mIndex) };
	set(item->x(), item->y(), z);
}

void Q3DScatterChart::Point::set(float x, float y, float z)
{
	set(QVector3D(x, y, z));
}

void Q3DScatterChart::Point::set(QVector3D const& point)
{
	mSerie->setItem(mIndex, QScatterDataItem(point));
}

float Q3DScatterChart::Point::x() const
{
	return mSerie->itemAt(mIndex)->x();
}

float Q3DScatterChart::Point::y() const
{
	return mSerie->itemAt(mIndex)->y();
}

float Q3DScatterChart::Point::z() const
{
	return mSerie->itemAt(mIndex)->z();
}

QVector3D Q3DScatterChart::Point::coord() const
{
	QScatterDataItem const * item{ mSerie->itemAt(mIndex) };
	return QVector3D(item->x(), item->y(), item->z());
}

void Q3DScatterChart::Point::randomize(float min, float max)
{
	randomize(min, max, min, max, min, max);
}

void Q3DScatterChart::Point::randomize(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	set(Q3DScatterChart::generateRandom(minX, maxX), Q3DScatterChart::generateRandom(minY, maxY), Q3DScatterChart::generateRandom(minZ, maxZ));
}

bool Q3DScatterChart::Point::operator==(Point const& point) const
{
	return mSerie == point.mSerie && mIndex == point.mIndex;
}

Q3DScatterChart::Point::Point(QtDataVisualization::QScatterDataProxy* serie, int index)
	: mSerie{ serie }, mIndex{ index }
{
}


////////////////////////////////////////////////////////////////////////////////
// Q3DScatterChart::ChartPresentation

void Q3DScatterChart::ChartPresentation::setTitle(QString const& title)
{
	mChart->mTitle->setText(title);
}

void Q3DScatterChart::ChartPresentation::setAxisTitle(QString const& xTitle, QString const& yTitle, QString const& zTitle)
{
	mChart->m3DScatter->axisX()->setTitle(xTitle);
	mChart->m3DScatter->axisY()->setTitle(yTitle);
	mChart->m3DScatter->axisZ()->setTitle(zTitle);
}

void Q3DScatterChart::ChartPresentation::setTitleVisibility(bool visible)
{
	mChart->mTitle->setVisible(visible);
}

void Q3DScatterChart::ChartPresentation::setAxisTitleVisibility(bool xVisible, bool yVisible, bool zVisible)
{
	mChart->m3DScatter->axisX()->setTitleVisible(xVisible);
	mChart->m3DScatter->axisY()->setTitleVisible(yVisible);
	mChart->m3DScatter->axisZ()->setTitleVisible(zVisible);
}

void Q3DScatterChart::ChartPresentation::setAxisAutoRanges()
{
	mChart->m3DScatter->axisX()->setAutoAdjustRange(true);
	mChart->m3DScatter->axisY()->setAutoAdjustRange(true);
	mChart->m3DScatter->axisZ()->setAutoAdjustRange(true);
}

void Q3DScatterChart::ChartPresentation::setAxisRange(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	mChart->m3DScatter->axisX()->setAutoAdjustRange(false);
	mChart->m3DScatter->axisY()->setAutoAdjustRange(false);
	mChart->m3DScatter->axisZ()->setAutoAdjustRange(false);
	mChart->m3DScatter->axisX()->setRange(xMin, xMax);
	mChart->m3DScatter->axisY()->setRange(yMin, yMax);
	mChart->m3DScatter->axisZ()->setRange(zMin, zMax);
}

void Q3DScatterChart::ChartPresentation::setGridVisibility(bool visible)
{
	mChart->m3DScatter->activeTheme()->setGridEnabled(visible);
}

void Q3DScatterChart::ChartPresentation::setBackgroundVisibility(bool visible)
{
	mChart->m3DScatter->activeTheme()->setBackgroundEnabled(visible);
}

void Q3DScatterChart::ChartPresentation::setShadow(ShadowType shadowType)
{
	mChart->m3DScatter->setShadowQuality(static_cast<QAbstract3DGraph::ShadowQuality>(static_cast<int>(shadowType)));
}

Q3DScatterChart::ChartPresentation::ChartPresentation(Q3DScatterChart * chart)
	: mChart{ chart }
{
}


////////////////////////////////////////////////////////////////////////////////
// Q3DScatterChart::SeriePresentation

Q3DScatterChart::SeriePresentation::SeriePresentation(QString const& title, ShapeType shapeType, float shapeSize, QColor const& shapeColor, bool shapeSmooth)
	: mLinkedSeries{}
	, mName(title)
	, mShapeType{ shapeType }
	, mShapeSize{ shapeSize }
	, mShapeColor(shapeColor)
	, mShapeSmooth{ shapeSmooth }
{
	if (!mShapeColor.isValid()) {
		// hue : 0 - 1/2 - 1/4 - 3/4 - 1/8 - 3/8 - 5/8 - 7/8 - 1/16 - 3/16 - ...
		static int generatedColorIndex{ 1 };
		int base{ qCeil(qLn((qreal)generatedColorIndex) / qLn(2.0)) };
		qreal hue{ (2.0 * (generatedColorIndex - qPow(2, base - 1.0)) - 1.0) / qPow(2.0, base) };
		mShapeColor = QColor::fromHsvF(hue, 1.0, 1.0, 1.0);
		++generatedColorIndex;
	}
}

QString Q3DScatterChart::SeriePresentation::name() const
{
	return mLinkedSeries ? mLinkedSeries->name() : mName;
}

Q3DScatterChart::SeriePresentation::ShapeType Q3DScatterChart::SeriePresentation::shapeType() const
{
	return mLinkedSeries ? static_cast<ShapeType>(static_cast<int>(mLinkedSeries->mesh())) : mShapeType;
}

float Q3DScatterChart::SeriePresentation::shapeSize() const
{
	return mLinkedSeries ? mLinkedSeries->itemSize() : mShapeSize;
}

QColor Q3DScatterChart::SeriePresentation::shapeColor() const
{
	return mLinkedSeries ? mLinkedSeries->baseColor() : mShapeColor;
}

bool Q3DScatterChart::SeriePresentation::shapeSmooth() const
{
	return mLinkedSeries ? mLinkedSeries->isMeshSmooth() : mShapeSmooth;
}

void Q3DScatterChart::SeriePresentation::setName(QString const& name)
{
	if (mLinkedSeries) {
		mLinkedSeries->setName(name);
	} else {
		mName = name;
	}
}

void Q3DScatterChart::SeriePresentation::setShapeType(ShapeType meshType)
{
	if (mLinkedSeries) {
		mLinkedSeries->setMesh(static_cast<QAbstract3DSeries::Mesh>(static_cast<int>(meshType)));
	} else {
		mShapeType = meshType;
	}
}

void Q3DScatterChart::SeriePresentation::setShapeSize(float size)
{
	if (mLinkedSeries) {
		mLinkedSeries->setItemSize(size);
	} else {
		mShapeSize = size;
	}
}

void Q3DScatterChart::SeriePresentation::setShapeColor(QColor const& color)
{
	if (mLinkedSeries) {
		mLinkedSeries->setBaseColor(color);
	} else {
		mShapeColor = color;
	}
}

void Q3DScatterChart::SeriePresentation::setShapeSmooth(bool shapeSmooth)
{
	if (mLinkedSeries) {
		mLinkedSeries->setMeshSmooth(shapeSmooth);
	} else {
		mShapeSmooth = shapeSmooth;
	}
}

Q3DScatterChart::SeriePresentation::SeriePresentation(QtDataVisualization::QScatter3DSeries* linkedSeries)
	: mLinkedSeries{ linkedSeries }
	, mName("")
	, mShapeType{ ShapeType::Sphere }
	, mShapeSize{ 0.0f }
	, mShapeColor(Qt::blue)
	, mShapeSmooth{ false}
{
}



#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QScrollBar>
#include <QFormLayout>


////////////////////////////////////////////////////////////////////////////////
// Q3DScatterChart::Q3DScatterChartControl

Q3DScatterChart::Q3DScatterChartControl::Q3DScatterChartControl(Q3DScatterChart & parent)
	: QWidget(&parent)
	, mSetAutoRotate{ new QScrollBar }
{
	int const widgetWidth{ 125 };

	QCheckBox* axisVisibility{ new QCheckBox };
	QCheckBox* gridVisibility{ new QCheckBox };
	QCheckBox* backgroundVisibility{ new QCheckBox };
	QCheckBox* shadowVisibility{ new QCheckBox };
	axisVisibility->setChecked(true);
	gridVisibility->setChecked(true);
	backgroundVisibility->setChecked(true);
	shadowVisibility->setChecked(true);

	QPushButton* stopAutoRotate{ new QPushButton("Stop") };
	stopAutoRotate->setEnabled(false);
	
	mSetAutoRotate->setOrientation(Qt::Horizontal);
	mSetAutoRotate->setRange(-50, 50);
	mSetAutoRotate->setValue(0);
	mSetAutoRotate->setMinimumWidth(widgetWidth);

	QScrollBar* shapeSize{ new QScrollBar };
	shapeSize->setOrientation(Qt::Horizontal);
	shapeSize->setRange(0, 100);
	shapeSize->setValue(0);
	shapeSize->setMinimumWidth(widgetWidth);
	QLabel* shapeSizeLabel{ new QLabel("Automatic") };
	shapeSizeLabel->setAlignment(Qt::AlignCenter);
	connect(shapeSize, &QScrollBar::valueChanged, [this, shapeSizeLabel](int value) { 
			float v{ value / 100.0f };
			if (value == 0) {
				shapeSizeLabel->setText("Automatic");
			} else {
				shapeSizeLabel->setNum(v);
			}
			for (int i{}; i < static_cast<Q3DScatterChart*>(this->parent())->serieCount(); ++i) {
				(*static_cast<Q3DScatterChart*>(this->parent()))[i].presentation().setShapeSize(v);
			}
		});

	QGroupBox* visibilityGroup{ new QGroupBox("Visibility") };
	QFormLayout* visibilityLayout{ new QFormLayout(visibilityGroup) };
	visibilityLayout->addRow("Axis title", axisVisibility);
	visibilityLayout->addRow("Grid", gridVisibility);
	visibilityLayout->addRow("Background", backgroundVisibility);
	visibilityLayout->addRow("Shadow", shadowVisibility);
	

	QGroupBox* autoRotateGroup{ new QGroupBox("Auto-rotation") };
	QVBoxLayout* autoRotateLayout{ new QVBoxLayout(autoRotateGroup) };
	autoRotateLayout->addWidget(stopAutoRotate);
	autoRotateLayout->addWidget(mSetAutoRotate);

	QGroupBox* shapeSizeGroup{ new QGroupBox("Override shape size") };
	QVBoxLayout* shapeSizeGroupLayout{ new QVBoxLayout(shapeSizeGroup) };
	shapeSizeGroupLayout->addWidget(shapeSize);
	shapeSizeGroupLayout->addWidget(shapeSizeLabel);

	QVBoxLayout* layout{ new QVBoxLayout(this) };
	layout->addStretch();
	layout->addWidget(visibilityGroup);
	layout->addWidget(shapeSizeGroup);
	layout->addWidget(autoRotateGroup);
	layout->setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

	connect(axisVisibility, &QCheckBox::clicked, [this](bool checked) { static_cast<Q3DScatterChart*>(this->parent())->presentation().setAxisTitleVisibility(checked, checked, checked); });
	connect(gridVisibility, &QCheckBox::clicked, [this](bool checked) { static_cast<Q3DScatterChart*>(this->parent())->presentation().setGridVisibility(checked); });
	connect(backgroundVisibility, &QCheckBox::clicked, [this](bool checked) { static_cast<Q3DScatterChart*>(this->parent())->presentation().setBackgroundVisibility(checked); });
	connect(shadowVisibility, &QCheckBox::clicked, [this](bool checked) { checked ? 
			static_cast<Q3DScatterChart*>(this->parent())->presentation().setShadow(Q3DScatterChart::ChartPresentation::ShadowType::HighQuality) : 
			static_cast<Q3DScatterChart*>(this->parent())->presentation().setShadow(Q3DScatterChart::ChartPresentation::ShadowType::None); });

	connect(stopAutoRotate, &QPushButton::clicked, [this](bool checked) { mSetAutoRotate->setValue(0); });
	connect(mSetAutoRotate, &QScrollBar::valueChanged, [this, stopAutoRotate](int value) { 
			const QSignalBlocker blocker(mSetAutoRotate);
			static_cast<Q3DScatterChart*>(this->parent())->enableAutoRotation(value); 
			stopAutoRotate->setEnabled(value != 0);
		});
}

void Q3DScatterChart::Q3DScatterChartControl::synchronizeGui()
{
	Q3DScatterChart* chart{ static_cast<Q3DScatterChart*>(this->parent()) };
	
	mSetAutoRotate->setValue(chart->autoRotationSpeed());
}
