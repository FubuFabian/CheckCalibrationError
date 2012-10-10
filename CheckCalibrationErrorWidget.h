#ifndef CHECKCALIBRATIONERRORWIDGET_H
#define CHECKCALIBRATIONERRORWIDGET_H

#include "ui_CheckCalibrationErrorWidget.h"
#include "mainwindow.h"

#include <QWidget>
#include <vtkPoints.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>


class CheckCalibrationErrorWidget : public QWidget, private Ui::CheckCalibrationErrorWidget
{
    Q_OBJECT
    
public:
       /** Constructor */
    CheckCalibrationErrorWidget(QWidget* parent = 0);
    
    virtual ~CheckCalibrationErrorWidget();

	 /**
     * \brief Set this stack of vtkImageData 
     * \param[in] a std Vector of vtkImageData
     */
    void setImageStack(std::vector< vtkSmartPointer<vtkImageData> > imageStack);
    
    /**
     * \brief Set this vtkImageData 
     * \param[in] a smart Pointer of vtkImageData
     */
    void setImage(vtkSmartPointer<vtkImageData> image);
    
    
    /** Set the window to display the crop images */
    void setMainWindow(MainWindow* mainwindow);

	void setTracedPoints(vtkSmartPointer<vtkPoints> points);
    
private:
    Ui::CheckCalibrationErrorWidget *ui;

	bool workWithStack;

	 /** \brief an Array of vtkImageData to work */
    std::vector< vtkSmartPointer<vtkImageData> > imageStack;
        
	/** \brief a vnl_matrix to store the center pf the sphere */
    vnl_matrix<double> centers;
    
    /** \brief a vnl_matrix to store the translations of each image given by the tracker */
    vnl_matrix<double> translations;
    
    
    /** \brief a vnl_matrix to store the rotations of each image given by the tracker */
    vnl_matrix<double> rotations;
    float rotations_2[7][4];

	/** The angles and translation estimated */
	std::vector<double> calibrationData;
    
    /** the main window to call it */
    MainWindow* mainWindow;
    
    /** \brief the vtkImageData to work */
    vtkSmartPointer<vtkImageData> image;

	std::vector< vtkSmartPointer<vtkPoints> > pointsVector;

	//vtkSmartPointer<vtkPoints> points;

	virtual void closeEvent( QCloseEvent * event);

	vnl_matrix<double> transformPoints();

private slots:

    void loadCenter();

    void loadRotations();

    void loadTranslations();

    void loadCalibration();

    void checkError();

    void saveError();
};

#endif // CHECKCALIBRATIONERRORWIDGET_H
