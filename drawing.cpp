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
    painter->setPen(Qt::red);

    QPainterPath axes(QPointF(0,0));
    axes.lineTo(50, 0); axes.lineTo(48, -2);
    axes.lineTo(48, 2); axes.lineTo(50,0);
    axes.addText(QPointF(56,0),QFont("Arial",12,2),"X");

    axes.lineTo(0, 0); axes.lineTo(0, 50);
    axes.lineTo(-2, 48); axes.lineTo(0, 50);
    axes.lineTo(2, 48); axes.lineTo(0, 50);
    axes.addText(QPointF(56,0),QFont("Arial",12,2),"Y");

    painter->drawPath(axes);

    // Draw transform origin point.
    painter->drawEllipse(ORIGIN,12,12);
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
}

Drawing::~Drawing()
{}

void Drawing::setDrawingImage(Image img)
{
    QPainterPath path;
    switch (img) {
    // Circle case
    case Image::Circle:
        path.addEllipse(QPointF(0,0),100,100); return;
    // Square case
    case Image::Square:
        path.addRect(-50,-50,100,100); return;
    // Letter A case
    case Image::LetterA:
        path.addText(QPointF(-50,-50),QFont("Arial",100),"A");
        return;
    }
}

void Drawing::setPreviousTransform(QTransform t)
{ PREV_TRANSFORM = t; }

void Drawing::setTransformType(TT tt)
{ TRANSFORM_TYPE = tt; }

void Drawing::setValues(double x, double y, double z)
{ X = x; Y = y; Z = z; }

void Drawing::apply()
{ TRANSFORM = transform(TRANSFORM_TYPE,PREV_TRANSFORM,X,Y,X); update(); }

QRectF Drawing::boundingRect() const
{ return QRectF(-50,-50,100,100); }

void Drawing::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    drawPrimaries(painter);
    painter->drawPath(TRANSFORM.map(PATH));
}
