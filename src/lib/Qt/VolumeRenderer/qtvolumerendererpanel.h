#ifndef QTVOLUMERENDERERPANEL_H
#define QTVOLUMERENDERERPANEL_H

#include <QWidget>

#include "volumerendererdata.h"

namespace Ui {
    class QtVolumeRendererPanel;
}

class QtVolumeRendererPanel : public QWidget, public VolumeRendererData
{
    Q_OBJECT

public:
    explicit QtVolumeRendererPanel(QWidget *parent = 0);
    ~QtVolumeRendererPanel();   

private slots:
    void on_checkBoxShowVolume_clicked(bool checked);

    void on_opacitySlider_valueChanged(int value);    

    void on_checkBoxBoundingBox_clicked(bool checked);

    void on_radioButtonMIP_clicked(bool checked);

    void on_radioButtonComposite_clicked(bool checked);

    void on_radioButtonLinear_clicked(bool checked);

    void on_radioButtonNearest_clicked(bool checked);

    void on_radioButtonMinIP_clicked(bool checked);

private:
    Ui::QtVolumeRendererPanel *ui;
    void invariant();

};

#endif // QTVOLUMERENDERERPANEL_H
