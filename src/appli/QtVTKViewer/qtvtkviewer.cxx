
//----------------------------------------------------------------------
//		File:               qtvtkviewer.cxx
//		Programmer:         Prieto
//		Last modified:	25/08/11 (Release 0.1)
//		Description:	Sample program fro qtvtkviewer
//----------------------------------------------------------------------

#include "qtvtkviewerwidget.h"
#include <QMainWindow>
#include <QApplication>




#include "iostream"

#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkNIFTIImageReader.h"
#include "vtkNrrdReader.h"

using namespace std;

void help(char* exec){
    cerr<<"Usage: "<<exec<<" <imageFilename>"<<endl;
    cerr<<"Options: "<<endl;
    cerr<<"-f <filename>"<<endl;
}

int main(int argc, char **argv)
{

    vtkImageData* img = 0;
    std::string inputFilename = "";

    for(int i = 1; i < argc - 1; i++){
        if(string(argv[i]).compare("-f") == 0){
            inputFilename = argv[i + 1];
        }
    }

    if(argc==2){
        inputFilename = string(argv[1]);
    }

    if(inputFilename.compare("") == 0){
        help(argv[0]);
        return -1;
    }

    vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkImageReader2 * imageReader = readerFactory->CreateImageReader2(inputFilename.c_str());
    if(imageReader){
        imageReader->SetFileName(inputFilename.c_str());
        imageReader->Update();
        img = imageReader->GetOutput();
    }else{

        vtkImageReader* imagereader = 0;
        if(inputFilename.rfind(".nii") != string::npos || inputFilename.rfind(".nii.gz") != string::npos){
            imagereader = (vtkImageReader*)vtkNIFTIImageReader::New();
        }else if(inputFilename.rfind(".nrrd") != string::npos){
            imagereader = (vtkImageReader*)vtkNrrdReader::New();
        }

        if(imagereader){
            imagereader->SetFileName(inputFilename.c_str());
            imagereader->Update();
            img = imagereader->GetOutput();
        }else{
            cerr<<"Image type not supported."<<endl;
        }
    }


    QApplication app(argc, argv);

    QMainWindow* mainwindow = new QMainWindow();

    QtVTKViewerWidget *viewer = new QtVTKViewerWidget(mainwindow);

    mainwindow-> setCentralWidget(viewer);


    mainwindow->show();

    if(img)
        viewer->setImage(img);
    //viewer->SetLookupTable((vtkLookupTable*)colortransfer);

    return app.exec();
}

