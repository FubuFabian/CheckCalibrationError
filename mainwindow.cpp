#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "ProbeCalibrationWidget.h"
#include "CheckCalibrationErrorWidget.h"

#include <QVBoxLayout>
#include <vtkEventQtSlotConnect.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);

  this->displayWidget = new QVTKImageWidget();

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->QLayout::addWidget(displayWidget);
  ui->imageWidget->setLayout(layout);

  // hide the slider andonly show if an image stack is load
  ui->imageSlider->hide();

  // create the connections 
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();

}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::addImages()
{

	std::cout<<"Loading 2D Images"<<std::endl;
  this->imagesFilenames = QFileDialog::getOpenFileNames(this, tr("Open Images"),
	  QDir::currentPath(),tr("Image Files (*.png *.jpg *.bmp)"));
  if (!imagesFilenames.isEmpty())
    {
      if (imagesFilenames.size() == 1)
        {
          ui->imageSlider->hide();

          addLogText("Loading: <b>" + imagesFilenames.first() + "</b>");
          displayWidget->setAndDisplayImage(imagesFilenames.first());

          addLogText("Image Type: <b>" + displayWidget->getImageType() + "</b>");
          addLogText("Pixel Type: <b>" + displayWidget->getPixelType() + "</b>");
          addLogText("Num of Dimensions: <b>" + displayWidget->getNumOfDimesions() + "</b>");
        }
      else
        {
          // if selected multiple files
          ui->imageSlider->show();
          ui->imageSlider->setTickInterval(1);
          ui->imageSlider->setRange(0, imagesFilenames.size() - 1);

          this->displayWidget->setAndDisplayMultipleImages(imagesFilenames);
          for (int i = 0; i < imagesFilenames.size(); i++)
            {
              addLogText("Loading: <b>" + imagesFilenames.at(i) + "</b>");
            }
        }
    }
  else
    {
      return;
    }
}


void MainWindow::probeCalibration()
{
  if (!displayWidget->getImageStack().empty())
    {
      ProbeCalibrationWidget* probeCalibration = new ProbeCalibrationWidget();

      if (displayWidget->isImageStackLoaded)
        probeCalibration->setImageStack(displayWidget->getImageStack());
      else
        probeCalibration->setImage(displayWidget->getImageViewer()->GetInput());

	  displayWidget->setProbeFlag(true);
      // get left mouse pressed with high priority
      Connections->Connect(displayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           probeCalibration,
                           SLOT(getCoordinates()));

      probeCalibration->setMainWindow(this);
      probeCalibration->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load an images before calibrate the probe");
      errorMessage.exec();
    }
}


void MainWindow::checkCalibrationError()
{

	std::cout<<"Check Calibration Error"<<std::endl<<std::endl;

	if (!displayWidget->getImageStack().empty())
    {
      CheckCalibrationErrorWidget* checkCalibrationWidget = new CheckCalibrationErrorWidget();

      if (displayWidget->isImageStackLoaded)
        checkCalibrationWidget->setImageStack(displayWidget->getImageStack());
      else
        checkCalibrationWidget->setImage(displayWidget->getImageViewer()->GetInput());

	    displayWidget->setCalibrationErrorWidget(checkCalibrationWidget);
	  	displayWidget->setProbeFlag(false);
		displayWidget->startTracer();
	  
      checkCalibrationWidget->setMainWindow(this); 
      checkCalibrationWidget->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load an images before checking the calibration");
      errorMessage.exec();
    }
}

void MainWindow::displaySelectedImage(int idx)
{
  this->displayWidget->displaySelectedImage(idx);
}


QVTKImageWidget* MainWindow::getDisplayWidget()
{
  return this->displayWidget;
}


void MainWindow::addLogText(QString str)
{
  ui->textEdit->append(str);

  // move the cursor to the end of the last line
  QTextCursor cursor = ui->textEdit->textCursor();
  cursor.movePosition(QTextCursor::End);
  ui->textEdit->setTextCursor(cursor);
}


void MainWindow::print()
{
  this->addLogText("estoy cachando el evento");
}



