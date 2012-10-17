#include "EstimateSphereFromPoints.h"
#include "SphereFunction.h"

#include <vnl/algo/vnl_levenberg_marquardt.h>
#include <iostream>
#include <math.h>
#include <QErrorMessage>


void EstimateSphereFromPoints::setPoints(vnl_matrix<double> points)
{
    this->points = points;
}

void EstimateSphereFromPoints::setCenter(vnl_matrix<double> centers)
{
    this->centers = centers;
}

vnl_vector<double> EstimateSphereFromPoints::getSphere()
{
    return sphere;
}

void EstimateSphereFromPoints::estimateSphere()
{

	double centerX = centers.get_column(0).mean();
	double centerY = centers.get_column(1).mean();
	double centerZ = centers.get_column(2).mean();
	
	std::cout<<std::endl;
	std::cout<<"Tracked center: "<<centerX<<", "<<centerY<<", "<<centerZ<<std::endl;

	double nPoints = points.rows();

	std::cout<<std::endl;
	std::cout<<"Number of surface points "<<nPoints<<std::endl;		

	double pCenterX = points.get_column(0).mean();
	double pCenterY = points.get_column(1).mean();
	double pCenterZ = points.get_column(2).mean();

	std::cout<<std::endl;
	std::cout<<"Initial values for center "<<pCenterX<<", "<<pCenterY<<", "<<pCenterZ<<std::endl;

	vnl_vector<double> dist;
	dist.set_size(points.rows());

	for(int i=0; i<points.rows(); i++)
	{
		dist[i] = sqrt(pow((pCenterX - points.get_column(0)[i]),2) + pow((pCenterY - points.get_column(1)[i]),2) +pow((pCenterZ - points.get_column(2)[i]),2));
	}

	double pRadius = dist.mean();

	std::cout<<std::endl;
	std::cout<<"Initial values for radius "<<pRadius<<std::endl;

	vnl_vector<double> x(4);
	x.put(0,pCenterX);
	x.put(1,pCenterY);
	x.put(2,61.55);
	x.put(3,6.51);

	SphereFunction sphereFunc(&points);

	vnl_levenberg_marquardt LM(sphereFunc);
    LM.set_verbose(true);
    
    LM.set_f_tolerance(10e-10);
    LM.set_x_tolerance(10e-10);
   
       // max iterations 5000
    LM.set_max_function_evals(5000);
    
    bool okOptimization = false;
    
    try
    {
         okOptimization = LM.minimize(x);
    }
    catch (std::exception& e)
    {
		std::cout<<"Excepcion"<<std::endl;
    }

  LM.diagnose_outcome(std::cout);
  std::cout << "x = " << x << std::endl;


}

