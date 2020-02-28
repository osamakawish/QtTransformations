#ifndef DRAWING_H
#define DRAWING_H

#include <QMap>
#include <QGraphicsItem>

// Transformation Type
enum class TT {SetOrigin, Translate, Scale, Shear, Project};

class Drawing;
// Preliminary Changes
typedef void (Drawing::*Transformation)(QTransform &transform, double x, double y, double z);
typedef QPainterPath (*Path)();

class Drawing : public QGraphicsItem
{
    // Transformation Functions
    void setOrigin(QTransform &transform, double x, double y, double);
    void translate(QTransform &transform, double x, double y, double);
    void scale(QTransform &transform, double x, double y, double);
    void shear(QTransform &transform, double x, double y, double);
    void project(QTransform &transform, double x, double y, double z);

    // Auxiliary Member Functions
    Transformation getTransformation(TT tt);
    QTransform transform(TT tt, QTransform prev, qreal x, qreal y, qreal z);
    void drawPrimaries(QPainter *painter);

    // Variables
    QPointF ORIGIN;
    QPainterPath PATH;
    TT TRANSFORM_TYPE;
    double X, Y, Z;
    Transformation TRANSFORM_FUNC;
    QTransform PREV_TRANSFORM;
    QTransform TRANSFORM;
    QMap<TT,Transformation> TT_FUNC;

public:
    Drawing();
    ~Drawing();

    enum class Image {Circle, Square, LetterA};
    void setDrawingImage(Image img);

    void setPreviousTransform(QTransform t);
    void setTransformType(TT tt);
    void setValues(double x, double y, double z);
    void apply();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

#endif // DRAWING_H
