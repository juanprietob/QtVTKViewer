#ifndef QTVTKVIEWER_H
#define QTVTKVIEWER_H


#include <QWidget>
#include <QMouseEvent>

#include "QVTKWidget.h"
#include "imageplanewidget.h"

class QtVTKViewer : public QVTKWidget, public ImagePlaneWidget
{

public:

    QtVTKViewer(QWidget* parent);

    typedef ImagePlaneWidget SuperImagePlaneWidget;

    void SetOutputFormatToRGBA();
private:

    void invariant();

    /**
    *   @brief double  _imageindex contains the current point on the image selected by the user, can be used to perform segmentation algorithms on the image i.e the seed for regiongrowing
    *
    */
    double _imageindex[3];
    /**
    *   @brief double  _rayorigin contains the origin position of the clicked performed by the user, can be used to perform ray intersection algorithms i.e
    *       finding an object in the scene
    *
    */
    double _rayorigin[4];        

protected:
    // overloaded mouse press handler
    virtual void mousePressEvent(QMouseEvent* event);
    // overloaded mouse move handler
    virtual void mouseMoveEvent(QMouseEvent* event);
    // overloaded mouse release handler
    //virtual void mouseReleaseEvent(QMouseEvent* event);

};

#endif // QTVTKVIEWER_H
