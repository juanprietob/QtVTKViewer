#ifndef QTVOLUMERENDERER_H
#define QTVOLUMERENDERER_H

#include <QWidget>

namespace Ui {
    class QtVolumeRenderer;
}

class QtVolumeRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit QtVolumeRenderer(QWidget *parent = 0);
    ~QtVolumeRenderer();

private:
    Ui::QtVolumeRenderer *ui;
};

#endif // QTVOLUMERENDERER_H
