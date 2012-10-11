#include "EstimateSphereFromPoints.h"

class SphereFunction : public vnl_least_squares_function {
	
public:
	
	SphereFunction()
		: vnl_least_squares_function(3, 4, no_gradient) {}

	~SphereFunction() {}

};


void EstimateSphereFromPoints::setPoints(vnl_matrix<double> points)
{
    this->points = points;
}

vnl_vector<double> EstimateSphereFromPoints::getSphere()
{
    return sphere;
}

void EstimateSphereFromPoints::estimateSphere()
{


}

