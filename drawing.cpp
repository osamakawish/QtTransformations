#include "drawing.h"

#include <QPainter>
#include <QDebug>
#include <typeinfo>

void Drawing::setOrigin(double x, double y, double)
{ORIGIN = QPointF(x,y);}

void Drawing::translate(double x, double y, double)
{TRANSFORM = QTransform().translate(x,y); FINAL = PREVIOUS * TRANSFORM;}

void Drawing::scale(double x, double y, double)
{TRANSFORM = QTransform().scale(x,y); FINAL = PREVIOUS * TRANSFORM;}

void Drawing::shear(double x, double y, double)
{TRANSFORM = QTransform().shear(x,y); FINAL = PREVIOUS * TRANSFORM;}

void Drawing::project(double x, double y, double z)
{TRANSFORM = QTransform(1,0,x, 0,1,y, 0,0,z); FINAL = PREVIOUS * TRANSFORM;}

Transformation Drawing::getTransformation(TransformationType type)
{
    switch(type) {
    case TransformationType::SetOrigin: return &Drawing::setOrigin;
    case TransformationType::Translate: return &Drawing::translate;
    case TransformationType::Scale: return &Drawing::scale;
    case TransformationType::Shear: return &Drawing::shear;
    case TransformationType::Project: return &Drawing::project;
    }
}

void Drawing::setupAxes()
{
    // Draw axes.
    PRIMARIES[0].lineTo(46, 0); PRIMARIES[0].lineTo(44, -4); PRIMARIES[0].lineTo(50,0);
    PRIMARIES[0].lineTo(44, 4); PRIMARIES[0].lineTo(46,0);

    PRIMARIES[1].lineTo(0,46); PRIMARIES[1].lineTo(-4,44); PRIMARIES[1].lineTo(0,50);
    PRIMARIES[1].lineTo(4,44); PRIMARIES[1].lineTo(0,46);

    PRIMARIES[2].addText(QPointF(56,6),QFont("Arial",11,3),"X");
    PRIMARIES[3].addText(QPointF(-6,70),QFont("Arial",11,3),"Y");
}

void Drawing::constructor(int image, QTransform prev)
{
    setDrawingImage(image);
    X = 0; Y = 0; Z = 0;
    TRANSFORM_FUNC = &Drawing::translate;
    PREVIOUS = prev;
    FINAL = prev;
    setupAxes();
}

void Drawing::updateVariables()
{ (this->*TRANSFORM_FUNC)(X,Y,Z); }

Drawing::Drawing()
{ constructor(0); }

Drawing::Drawing(int image, QTransform prev)
{ constructor(image,prev); }

Drawing::~Drawing()
{}

void Drawing::setDrawingImage(Drawing::Image img)
{
    QPainterPath path;
    switch (img) {
    case Image::Circle:
        path.addEllipse(QPointF(0,0),50,50); break;
    case Image::Square:
        path.addRect(-50,-50,100,100); break;
    case Image::LetterA:
        path.addText(QPointF(-56,50),QFont("Arial",100),"A"); break;
    }
    PATH = path;
}

void Drawing::setDrawingImage(int i)
{ setDrawingImage(static_cast<Image>(i)); }

void Drawing::setPreviousTransform(QTransform t)
{ PREVIOUS = t; updateVariables(); }

//! Returns the amount input parameters based on enum index of TT (Circle, Square, LetterA).
//! Returns 0 for invalid inputs.
int Drawing::setTransformType(int tt)
{
    // Sets the transform type variable.
    TransformationType transformType = static_cast<TransformationType>(tt); // Never used except in this method.
    TRANSFORM_FUNC = getTransformation(transformType); updateVariables();

    // Return number of input parameters based on selected transform type.
    switch (transformType) {
    case TransformationType::SetOrigin: return 2;
    case TransformationType::Translate: return 2;
    case TransformationType::Scale: return 2;
    case TransformationType::Shear: return 2;
    case TransformationType::Project: return 3;
    }
    return 0;
}

void Drawing::setValues(double x, double y, double z)
{ X = x; Y = y; Z = z; updateVariables();}

QRectF Drawing::boundingRect() const
{ return QRectF(-50,-50,100,100); }

// Somehow, this is getting called repeatedly, without the support of any connections.
void Drawing::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    setTransformOriginPoint(ORIGIN);
    painter->drawEllipse(ORIGIN,9,9);

    for (int i=0; i<4; i++) { painter->drawPath(PRIMARIES[i]); }

    painter->setPen(Qt::red);
    painter->setBrush(QBrush(QColor(255,0,0,32)));
    painter->drawPath(FINAL.map(PATH));
}

QTransform Drawing::previousTransform() { return PREVIOUS; }
QTransform Drawing::currentTransform() { return TRANSFORM; }
QTransform Drawing::finalTransform() { return FINAL; }
