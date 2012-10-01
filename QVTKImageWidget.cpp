//
//  QVTKImageWidget.cpp
//  US_Probe_Calibration
//
//  Created by Zian Fanti on 01/12/11.
//  Copyright (c) 2011 __UNAM__. All rights reserved.
//

#include "QVTKImageWidget.h"
#include "QVTKImageWidgetCommand.h"

#include <QSize.h>
#include <QBoxLayout>
#include <QString>

#include <itkImage.h>
#include <itkImageFileReader.h>

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include <vtkCornerAnnotation.h>
#include <vtkPropPicker.h>
#include <vtkTextProperty.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>


#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkImageTracerWidget.h>
#include <vtkBMPReader.h>


QVTKImageWidget::QVTKImageWidget(QWidget *parent) : QWidget(parent)
{
    qvtkWidget = new QVTKWidget(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(qvtkWidget);
    this->setLayout(layout);

    // by default 
	this->probeFlag = false;
    this->isImageStackLoaded = false;
    this->imageDisplayedIndex = 0;


    // create the essentials vtk objects to display the images    
    this->imageViewer = vtkSmartPointer<vtkImageViewer2>::New();

    // to display data in the corners of the image viewer
    this->cornerAnnotation = vtkSmartPointer< vtkCornerAnnotation >::New();
}


QVTKImageWidget::~QVTKImageWidget()
{
    //	renderWindow->Finalize();
    this->itkImage = NULL;
    this->vtkImage = NULL;
    this->qvtkWidget = NULL;
    this->imageViewer = NULL;
}


void QVTKImageWidget::setAndDisplayImage(QString imageFilename)
{
    // reads a vtkImage for display purposes
    vtkSmartPointer <vtkImageReader2Factory> readerFactory =
            vtkSmartPointer <vtkImageReader2Factory>::New();

    vtkSmartPointer <vtkImageReader2> reader =
            readerFactory->CreateImageReader2(imageFilename.toAscii().data());

    reader->SetFileName(imageFilename.toAscii().data());
    reader->Update();

    this->setAndDisplayImage(reader->GetOutput());

    readerFactory = NULL;
    reader = NULL;

}


void QVTKImageWidget::setAndDisplayImage(vtkSmartPointer<vtkImageData> image)
{
    // if the image is alredy defined 
    itkImage = NULL;
    rgbItkImage = NULL;
    vtkImage = NULL;

    this->vtkImage = image;

    this->setImageProperties(true);

    this->displayImage(vtkImage);
}


void QVTKImageWidget::setAndDisplayMultipleImages(QStringList filenames)
{
    if (this->imageStack.size() > 0)
        {
            this->imageStack.clear();
            for (uint i = 0; i < imageStack.size(); i++)
                {
                    imageStack.at(i) = NULL;
                }
        }

    this->imageStack.reserve(filenames.size());

    for (int i = 0; i < filenames.size(); i++)
        {
            vtkSmartPointer<vtkImageReader2Factory> readerFactory =
                    vtkSmartPointer<vtkImageReader2Factory>::New();

            vtkSmartPointer<vtkImageReader2> reader =
                    readerFactory->CreateImageReader2(filenames.at(i).toAscii().data());
            reader->SetFileName(filenames.at(i).toAscii().data());
            reader->Update();

            vtkSmartPointer<vtkImageData> image = reader->GetOutput();
            this->imageStack.push_back(image);

            readerFactory = NULL;
            reader = NULL;

        }

    isImageStackLoaded = true;

    displayImage(imageStack.at(imageDisplayedIndex));

}


void QVTKImageWidget::
setAndDisplayMultipleImages(std::vector<vtkSmartPointer<vtkImageData> > imageStack)
{
    if (imageStack.size() > 0)
        {
            this->imageStack.clear();
            this->imageStack = imageStack;
            displayImage(imageStack.at(imageDisplayedIndex));
        }
    isImageStackLoaded = true;
}


void QVTKImageWidget::displayImage(vtkImageData *image)
{
    imageViewer->SetInput(image);
    imageViewer->GetRenderer()->ResetCamera();

    // make qt-vtk connection
    qvtkWidget->SetRenderWindow(imageViewer->GetRenderWindow());

    // disable interpolation, so we can see each pixel
    imageActor = imageViewer->GetImageActor();
    imageActor->InterpolateOff();

	if(probeFlag)
		initPicker();
	else
		startTracer();
}

void QVTKImageWidget::initPicker()
{
	// Picker to pick pixels
    vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
    propPicker->PickFromListOn();

    // Give the picker a prop to pick
    propPicker->AddPickList(imageActor);

    // Annotate the image with mouse over pixel information
    cornerAnnotation->SetLinearFontScaleFactor(2);
    cornerAnnotation->SetNonlinearFontScaleFactor(1);
    cornerAnnotation->SetMaximumFontSize(15);
    cornerAnnotation->GetTextProperty()->SetColor(1, 0, 0);
    imageViewer->GetRenderer()->AddViewProp(cornerAnnotation);

	    //listen to MouseMoveEvents invoked by the interactor's style
    vtkSmartPointer<QVTKImageWidgetCommand> callback =
            vtkSmartPointer<QVTKImageWidgetCommand>::New();
    callback->SetImageWidget(this);
    callback->SetAnnotation(cornerAnnotation);
    callback->SetPicker(propPicker);

	   vtkSmartPointer<vtkInteractorStyleImage> imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    imageViewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);

	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);


    // render image viewer
    imageViewer->Render();
}

void QVTKImageWidget::startTracer()
{


 vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
   vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  renderWindowInteractor->SetInteractorStyle(style);
  imageViewer->GetRenderWindow()->SetInteractor(renderWindowInteractor);
  imageViewer->Render();
  imageViewer->GetRenderer()->ResetCamera();
  imageViewer->Render();
  vtkSmartPointer<vtkImageTracerWidget> tracer =
    vtkSmartPointer<vtkImageTracerWidget>::New();
  /*tracer->GetLineProperty()->SetLineWidth(1);
  tracer->GetLineProperty()->SetColor(255,255,255);
  tracer->SetInteractor(imageViewer->GetRenderWindow()->GetInteractor());
  tracer->SetViewProp(imageActor);
  tracer->AutoCloseOff();*/

  tracer->SetInteractor(renderWindowInteractor);
  tracer->SetViewProp(imageActor);
  tracer->ProjectToPlaneOn();
  //tracer->SetInputData(imageActor->GetInput());
  tracer->SnapToImageOn();
 

 
  vtkSmartPointer<vtkCallbackCommand> callback =
  vtkSmartPointer<vtkCallbackCommand>::New();
  callback->SetCallback(CallbackFunction);
  tracer->AddObserver(vtkCommand::EndInteractionEvent, callback);
 
  tracer->On();


}

void QVTKImageWidget::displaySelectedImage(int idx)
{
    if (!imageStack.empty())
        {
            if (idx >= 0 && idx < (int) imageStack.size())
                {
                    imageDisplayedIndex = idx;
                    displayImage(imageStack[imageDisplayedIndex]);
                }
        }
}


void QVTKImageWidget::setImageProperties(bool verbose)
{
    this->numDimensions = this->vtkImage->GetDataDimension();
    this->pixelType = this->vtkImage->GetScalarTypeAsString();
    this->imageType = this->vtkImage->GetNumberOfScalarComponents();

    int* dim = vtkImage->GetDimensions();
    this->imageWidth = dim[0];
    this->imageHeight = dim[1];
    std::cout << imageWidth << "," << imageHeight << "," << dim[2] << std::endl;

    if (verbose)
        {
            std::cout << "Pixels type: " << pixelType << std::endl;
            std::cout << "Image type: " << imageType << std::endl;
            std::cout << "Num of Dimensions: " << numDimensions << std::endl;
        }
}



QVTKWidget* QVTKImageWidget::getQVTKWidget()
{
    return this->qvtkWidget;
}


vtkSmartPointer<vtkImageViewer2> QVTKImageWidget::getImageViewer()
{
    return this->imageViewer;
}


std::vector< vtkSmartPointer<vtkImageData> > QVTKImageWidget::getImageStack()
{
    return this->imageStack;
}


int QVTKImageWidget::getXPicked()
{
    return this->xPicked;
}


int QVTKImageWidget::getYPicked()
{
    return this->yPicked;
}

void QVTKImageWidget::setProbeFlag(bool probeFlag)
{
    this->probeFlag = probeFlag;
}
void QVTKImageWidget::setXPicked(int xPosition)
{
    this->xPicked = xPosition;
}


void QVTKImageWidget::setYPicked(int yPosition)
{
    this->yPicked = yPosition;
}


QString QVTKImageWidget::getPixelType()
{
    return QString(pixelType.c_str());
}


QString QVTKImageWidget::getImageType()
{
    if (imageType == 1)
        {
            return QString("grayscale");
        }
    else if (imageType == 3)
        {
            return QString("rgb");
        }
    else
        {
            return QString("-");
        }
}


QString QVTKImageWidget::getNumOfDimesions()
{
    QString out;
    out.setNum(numDimensions);
    return out;
}


int QVTKImageWidget::getImageDisplayedIndex()
{
    return imageDisplayedIndex;
}
