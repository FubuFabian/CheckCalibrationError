#include "EstimateSphereFromPoints.h"

class SphereFunction : public vnl_least_squares_function {
	
public:
	
	SphereFunction()
		: vnl_least_squares_function(3, 4, no_gradient) {}

	~SphereFunction() {}

};


double f(const vnl_vector<double>& x, vnl_vector<double>& fx) {
    fx[0] = 10 * (x[1] - vnl_math_sqr(x[0]));
    fx[1] = 1 - x[0];
}
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

	SphereFunction sphereFunc;
    //vnl_levenberg_marquardt lm(f);


    //vnl_vector<double> x;
	//x.set_size(2);

    //lm.minimize(x);

}

