#include "SphereFunction.h"

#include <vnl/vnl_math.h>


SphereFunction::SphereFunction(vnl_matrix<double> * points)
    : vnl_least_squares_function(4, points->rows(),
                                vnl_least_squares_function::no_gradient)
{
    this->_data = points;
}

SphereFunction::~SphereFunction(){}

void SphereFunction::f(const vnl_vector<double> &x, vnl_vector<double> &fx)
{
    double xh = x[0];
    double yh = x[1];
    double zh = x[2];
    double r = x[3];

    for(int i=0; i< _data->rows(); i++){
		
		vnl_vector<double> point = _data->get_row(i);

		fx[i] = (point[0] - xh)
    }
}


