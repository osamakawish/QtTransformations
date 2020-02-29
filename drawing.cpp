#include "drawing.h"

#include <QPainter>

void Drawing::setOrigin(QTransform &, double x, double y, double)
{ORIGIN = QPointF(x,y);}

void Drawing::translate(QTransform &transform, double x, double y, double)
{transform.translate(x,y);}

void Drawing::scale(QTransform &transform, double x, double y, double)
{transform.scale(x,y);}

void Drawing::shear(QTransform &transform, double x, double y, double)
{transform.shear(x,y);}

void Drawing::project(QTransform &transform, double x, double y, double z)
{transform *= QTransform(1,0,x, 0,1,y, 0,0,z);}

Transformation Drawing::getTransformation(TT type)
{
    switch(type) {
    case TT::SetOrigin: return &Drawing::setOrigin;
    case TT::Translate: return &Drawing::translate;
    case TT::Scale: return &Drawing::scale;
    case TT::Shear: return &Drawing::shear;
    case TT::Project: return &Drawing::project;
    }
}

QTransform Drawing::transform(TT tt, QTransform prev, qreal x, qreal y, qreal z)
{
    Transformation tr = getTransformation(tt);
    (this->*tr)(prev,x,y,z); return prev;
}

void Drawing::drawPrimaries(QPainter *painter)
{
    // Draw axes.
    painter->setPen(Qt::blue);
    painter->setBrush(QBrush(QColor(0,0,255,32)));

    QPainterPath xAxis;
    xAxis.lineTo(46, 0); xAxis.lineTo(44, -4); xAxis.lineTo(50,0);
    xAxis.lineTo(44, 4); xAxis.lineTo(46,0);

    QPainterPath yAxis;
    yAxis.lineTo(0,46); yAxis.lineTo(-4,44); yAxis.lineTo(0,50);
    yAxis.lineTo(4,44); yAxis.lineTo(0,46);

    QPainterPath xLabel; xLabel.addText(QPointF(56,6),QFont("Arial",11,3),"X");
    QPainterPath yLabel; yLabel.addText(QPointF(-6,70),QFont("Arial",11,3),"Y");

    painter->drawPath(xAxis); painter->drawPath(yAxis);
    painter->drawPath(xLabel); painter->drawPath(yLabel);

    // Draw transform origin point.
    painter->drawEllipse(ORIGIN,11,11);
}

Drawing::Drawing()
{
    PATH = QPainterPath();
    TRANSFORM_TYPE = TT::Translate;
    X = 0; Y = 0; Z = 0;
    TRANSFORM_FUNC = &Drawing::translate;
    TRANSFORM = QTransform();

    typedef Drawing D;
    TT_FUNC = { {TT::SetOrigin, &D::setOrigin}, {TT::Translate, &D::translate}, {TT::Scale, &D::scale},
          {TT::Shear, &D::shear}, {TT::Project, &D::project} };
    setDrawingImage(Image::Circle);
}

Drawing::~Drawing()
{}

void Drawing::setDrawingImage(Image img)
{
    QPainterPath path;
    switch (img) {
    // Circle case
    case Image::Circle:
        path.addEllipse(QPointF(0,0),100,100); break;
    // Square case
    case Image::Square:
        path.addRect(-50,-50,100,100); break;
    // Letter A case
    case Image::LetterA:
        path.addText(QPointF(-50,-50),QFont("Arial",100),"A");
        break;
    }
    PATH = path;
}

void Drawing::setPreviousTransform(QTransform t)
{ PREV_TRANSFORM = t; }

void Drawing::setTransform(QTransform t)
{ TRANSFORM = t; }

//! Returns the amount input parameters
int Drawing::setTransformType(int tt)
{
    TRANSFORM_TYPE = static_cast<TT>(tt);
    switch (TRANSFORM_TYPE) {
    case TT::SetOrigin: return 2;
    case TT::Translate: return 2;
    case TT::Scale: return 2;
    case TT::Shear: return 2;
    case TT::Project: return  3;
    }
}

void Drawing::setValues(double x, double y, double z)
{ X = x; Y = y; Z = z; }

void Drawing::apply()
{ TRANSFORM = transform(TRANSFORM_TYPE,PREV_TRANSFORM,X,Y,X); update(); }

QRectF Drawing::boundingRect() const
{ return QRectF(-50,-50,100,100); }

void Drawing::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    drawPrimaries(painter);
    painter->setPen(Qt::red);
    painter->setBrush(QBrush(QColor(255,0,0,32)));
    painter->drawPath(TRANSFORM.map(PATH));
}
