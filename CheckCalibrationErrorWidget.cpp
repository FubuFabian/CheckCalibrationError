#include "CheckCalibrationErrorWidget.h"
#include "EstimateSphereFromPoints.h"

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

	QString centerFilename = QFileDialog::getOpenFileName(this, tr("Load Center File"), 
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
        
        QTextStream strSize(&file);
        QString line;
        
        // declare a translation matrix 
 
		int fileSize = 0;

        while (!strSize.atEnd())
		{
			strSize.readLine();
			fileSize++;
		}

		this->centers.set_size(fileSize, 3);

		file.close();
		file.open(QIODevice::ReadOnly);

		QTextStream stream(&file);

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
        this->translations.set_size(imageStack.size(), 3); 
        
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

	EstimateSphereFromPoints * estimator = EstimateSphereFromPoints::New();
	
	estimator->setCenter(centers);
	estimator->setPoints(transformPoints());
	estimator->estimateSphere();

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

	pointsVector[row] = points;

	QString str;

	tableWidget->setItem(row, 0, new QTableWidgetItem(str.setNum(row)));
    tableWidget->setItem(row, 1, new QTableWidgetItem("Ok"));
}

vnl_matrix<double> CheckCalibrationErrorWidget::transformPoints()
{

	std::cout<<std::endl;
	std::cout<<"Transforming points"<<std::endl;	

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

	std::vector<vnl_vector< double > > transX;
	std::vector<vnl_vector< double > > transY;
	std::vector<vnl_vector< double > > transZ;

	transX.reserve(imageStack.size());
	transY.reserve(imageStack.size());
	transZ.reserve(imageStack.size());

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

		int newSize = points->GetNumberOfPoints();
			
		vnl_vector<double> transformedPointsX;
		vnl_vector<double> transformedPointsY;
		vnl_vector<double> transformedPointsZ;
		
		transformedPointsX.set_size(newSize);
		transformedPointsY.set_size(newSize);
		transformedPointsZ.set_size(newSize);

		for(int j=0; j<newSize; j++){

			 double * x = points->GetPoint(j);

			 point[0] = scale[0]*x[0];
			 point[1] = scale[1]*x[1];

			 transformedPoint = tTp*point;

			 transformedPointsX.put(j,transformedPoint[0]);
			 transformedPointsY.put(j,transformedPoint[1]);
			 transformedPointsZ.put(j,transformedPoint[2]);

			 p++;

		}

		transX.push_back(transformedPointsX);
		transY.push_back(transformedPointsY);
		transZ.push_back(transformedPointsZ);
    }


	vnl_matrix<double> transformedPoints;
	transformedPoints.set_size(p,3);

	int pp = 0;
	for(int i=0; i<imageStack.size(); i++)
	{
		vnl_vector<double> X = transX[i];
		vnl_vector<double> Y = transY[i];
		vnl_vector<double> Z = transZ[i];

		for(int j=0; j<X.size(); j++)
		{
			transformedPoints.put(pp,0,X[j]);
			transformedPoints.put(pp,1,Y[j]);
			transformedPoints.put(pp,2,Z[j]);

			pp++;

		}
	}

	return transformedPoints;
}