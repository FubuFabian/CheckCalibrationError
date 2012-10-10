#include "CheckCalibrationErrorWidget.h"

#include <vnl/vnl_quaternion.h>

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
	tableWidget->setRowCount(imageStack.size());
	pointsVector.reserve(imagestack.size());
	pointsVector.assign(imagestack.size(),0);

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
	int size = pointsVector.size();
 	int row = mainWindow->getDisplayWidget()->getImageDisplayedIndex();
	std::cout<<"traced "<<row<<" size"<<size<<std::endl;

	pointsVector[row] = points;

	QString str;

	tableWidget->setItem(row, 0, new QTableWidgetItem(str.setNum(row)));
    tableWidget->setItem(row, 1, new QTableWidgetItem("Ok"));
}

vnl_matrix<double> CheckCalibrationErrorWidget::transformPoints()
{
	double x = calibrationData[0];
	double y = calibrationData[1];
	double z = calibrationData[2];
	double a = calibrationData[3];
	double b = calibrationData[4];
	double c = calibrationData[5];

	vnl_vector<double> scale;
	scale.set_size(2);

	scale.put(0,calibrationData[6]);
	scale.put(1,calibrationData[7]);

	vnl_quaternion<double> rTpQuat(c, b, a);
	vnl_matrix<double> rTp = rTpQuat.rotation_matrix_transpose_4();
	rTp = rTp.transpose();
	rTp.put(0, 3, x);
	rTp.put(1, 3, y);
	rTp.put(2, 3, z);
 
	vnl_vector<double> point;
	point.set_size(4);

	point[2] = 0;
	point[3] = 1;

	vnl_vector<double> transformedPoint;
	transformedPoint.set_size(4);

	vnl_matrix<double> transformedPoints;
	transformedPoints.set_size(0,3);
	
	int p = 0;

	for (int i=0; i<imageStack.size(); i++){   
		
		vnl_vector<double> quaternion = rotations.get_row(i);
		vnl_vector<double> translation = translations.get_row(i);

		vnl_quaternion<double> tTrQuat(quaternion[1], quaternion[2], quaternion[3], quaternion[0]);
		vnl_matrix<double> tTr = tTrQuat.rotation_matrix_transpose_4();
		tTr = tTr.transpose();
		tTr.put(0, 3, translation[0]);
		tTr.put(1, 3, translation[1]);
		tTr.put(2, 3, translation[2]);

		vnl_matrix<double> tTp = tTr*rTp;

		vtkSmartPointer<vtkPoints> points = pointsVector.at(i);

		int oldSize = transformedPoints.size();
		int newSize = points->GetNumberOfPoints();

		transformedPoints.set_size(oldSize + newSize,3);
			
		for(int j=0; j<points->GetNumberOfPoints(); j++){

			 double * x = points->GetPoint(j);

			 point[0] = scale[0]*x[0];
			 point[1] = scale[1]*x[1];

			 transformedPoint = tTp*point;

			 transformedPoints.put(p,0,transformedPoint[0]);
			 transformedPoints.put(p,1,transformedPoint[1]);
			 transformedPoints.put(p,2,transformedPoint[2]);

			 p++;

		}
    }

	return transformedPoints;
}