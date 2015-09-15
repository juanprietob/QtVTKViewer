#include "qtvolumerenderer.h"
#include "ui_qtvolumerenderer.h"

QtVolumeRenderer::QtVolumeRenderer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtVolumeRenderer)
{
    ui->setupUi(this);
}

QtVolumeRenderer::~QtVolumeRenderer()
{
    delete ui;
}
