#ifndef DRAWING_H
#define DRAWING_H

#include <array>
#include <QGraphicsItem>

// Transformation Type
enum class TransformationType {SetOrigin, Translate, Scale, Shear, Project};

class Drawing;
// Preliminary Changes
typedef void (Drawing::*Transformation)(double x, double y, double z);
typedef QPainterPath (*Path)();

using std::array;

class Drawing : public QGraphicsItem
{    
    // Transformation Functions
    void moveOrigin(double x, double y, double);
    void translate(double x, double y, double);
    void scale(double x, double y, double);
    void shear(double x, double y, double);
    void project(double x, double y, double z);

    // Auxiliary Member Functions
    Transformation getTransformation(TransformationType tt);
    void setupAxes();
    void constructor(int image, QTransform prev=QTransform(), QPointF origin=QPointF());

    void updateVariables();

    // Variables
    array<QPainterPath,4> PRIMARIES;
    QPointF PRIMARY_ORIGIN, ORIGIN;
    QPainterPath PATH;
    double X, Y, Z;
    Transformation TRANSFORM_FUNC;
    QTransform PREVIOUS, TRANSFORM, FINAL;

public:
    enum class Image {Circle, Square, LetterA};
    Drawing();
    Drawing(int image, QTransform prev, QPointF origin=QPointF());
    ~Drawing();

    // Draw image based on enum input.
    void setDrawingImage(Image img);
    void setDrawingImage(int i);

    // Prepare the changes to be applied.
    int setTransformType(int t);
    void setPreviousTransform(QTransform t);
    void setValues(double x, double y, double z);

    // Details on the bounding rectangle and the image.
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    // Getters
    QPointF origin();
    QTransform previousTransform();
    QTransform currentTransform();
    QTransform finalTransform();
};

#endif // DRAWING_H
