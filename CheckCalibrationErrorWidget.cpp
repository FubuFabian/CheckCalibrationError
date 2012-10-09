#include "CheckCalibrationErrorWidget.h"

CheckCalibrationErrorWidget::CheckCalibrationErrorWidget(QWidget* parent) : QWidget(parent)
{
    this->setupUi(this);

}


CheckCalibrationErrorWidget::~CheckCalibrationErrorWidget()
{
    delete this;
}

void CheckCalibrationErrorWidget::loadCenter()
{

	QString centerFilename = QFileDialog::getOpenFileName(this, tr("Load Translations File"), 
		QDir::currentPath(),tr("Txt (*.txt *.doc)"));
    
	std::cout<<std::endl;
	std::cout<<"Loading Center File"<<std::endl;

    if (!centerFilename.isEmpty())
    {
        
        QFile file(centerFilename);
        if (!file.open(QIODevice::ReadOnly)){
           std::cout<<"Could not open center File"<<std::endl;
            return;
        }
        
        QTextStream stream(&file);
        QString line;
        
        // declare a translation matrix 
        this->translations.set_size(imageStack.size(), 4); 
        
        int idx = 0;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList lineList = line.split(" ");
            
            for (int j = 0; j < lineList.size(); j++)
            {
                centers.put(idx, j, lineList.at(j).toDouble());        
            }
            idx++;   
            //std::cout << line.toAscii().data() << std::endl;
        }
        file.close(); // when your done.        

    }
}

void CheckCalibrationErrorWidget::loadRotations()
{
	QString rotationFilename = QFileDialog::getOpenFileName(this, tr("Load Rotations File"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));

	std::cout<<std::endl;
    std::cout<<"Loading Rotations File"<<std::endl;

    if (!rotationFilename.isEmpty())
    {

        QFile file(rotationFilename);
        if (!file.open(QIODevice::ReadOnly)){
            std::cout<<"Could not open rotations File"<<std::endl;
             return;
         }
        
        QTextStream stream(&file);
        QString line;
        
        // declare a rotation matrix 
        this->rotations.set_size(imageStack.size(), 4);
        
        int idx = 0;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList lineList = line.split(" ");
            
            for (int j = 0; j < lineList.size(); j++)
            {
                rotations.put(idx, j, lineList.at(j).toDouble());                 
            }
            
            idx++;            
            //std::cout << line.toAscii().data() << std::endl;
        }
        file.close();        

    }
}

void CheckCalibrationErrorWidget::loadTranslations()
{
	QString translationFilename = QFileDialog::getOpenFileName(this, tr("Load Translations File"), 
		QDir::currentPath(),tr("Txt (*.txt *.doc)"));
    
	std::cout<<std::endl;
	std::cout<<"Loading Translations File"<<std::endl;

    if (!translationFilename.isEmpty())
    {
        
        QFile file(translationFilename);
        if (!file.open(QIODevice::ReadOnly)){
           std::cout<<"Could not open translations File"<<std::endl;
            return;
        }
        
        QTextStream stream(&file);
        QString line;
        
        // declare a translation matrix 
        this->translations.set_size(imageStack.size(), 4); 
        
        int idx = 0;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList lineList = line.split(" ");
            
            for (int j = 0; j < lineList.size(); j++)
            {
                translations.put(idx, j, lineList.at(j).toDouble());        
            }
            idx++;   
            //std::cout << line.toAscii().data() << std::endl;
        }
        file.close(); // when your done.        

    }
}

void CheckCalibrationErrorWidget::loadCalibration()
{
	QString calibrationFilename = QFileDialog::getOpenFileName(this, tr("Load Calibration File"), 
		QDir::currentPath(),tr("Txt (*.txt *.doc)"));
    
	std::cout<<std::endl;
	std::cout<<"Loading Calibration File"<<std::endl;

    if (!calibrationFilename.isEmpty())
    {
		
	QFile file(calibrationFilename);
         if (!file.open(QIODevice::ReadOnly))
            return;

         QTextStream stream(&file);
         QString line;

         // declare a translation matrix
         this->calibrationData.reserve(8);


         while (!stream.atEnd())
         {
               line = stream.readLine();           
			   calibrationData.push_back(line.toDouble());
          }
          
		  file.close(); // when your done.
        }  

}

void CheckCalibrationErrorWidget::checkError(){

}

void CheckCalibrationErrorWidget::saveError()
{

}

void CheckCalibrationErrorWidget::setImage(vtkSmartPointer<vtkImageData> image)
{
    this->workWithStack = false;
    this->image = image;
}


void CheckCalibrationErrorWidget::
setImageStack(std::vector<vtkSmartPointer<vtkImageData> > imagestack)
{
    this->workWithStack = true;
    this->imageStack = imagestack;
}

void CheckCalibrationErrorWidget::setMainWindow(MainWindow* mainwindow)
{
    this->mainWindow = mainwindow;
}

void CheckCalibrationErrorWidget::closeEvent( QCloseEvent * event)
{
	mainWindow->getDisplayWidget()->tracerStyle->clearTracer();
	mainWindow->getDisplayWidget()->probeFlag = true;
	mainWindow->getDisplayWidget()->initPicker();
	event->accept();
}

void CheckCalibrationErrorWidget::setTracedPoints(vtkSmartPointer<vtkPoints> points)
{
 	int row = mainWindow->getDisplayWidget()->getImageDisplayedIndex();
	std::cout<<"traced "<<row<<std::endl;
	pointsVector[row] = points;
	
	QString str;

	tableWidget->setItem(row, 0, new QTableWidgetItem(str.setNum(row)));
    tableWidget->setItem(row, 1, new QTableWidgetItem("Ok"));
}

void CheckCalibrationErrorWidget::setImageStackSize(int imageStackSize)
{
 	pointsVector.reserve(imageStackSize);
}