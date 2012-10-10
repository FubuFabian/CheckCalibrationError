#ifndef ESTIMATESPHEREFROMPOINTS_H
#define ESTIMATESPHEREFROMPOINTS_H

#endif // ESTIMATESPHEREFROMPOINTS_H

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>
#include <vnl/algo/vnl_levenberg_marquardt.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_least_squares_function.h>
	
class SphereFunction : public vnl_least_squares_function {
	
public:
	
	SphereFunction()
		: vnl_least_squares_function(3, 4, no_gradient) {}

	~SphereFunction() {}

};

class EstimateSphereFromPoints
{

public:

    static EstimateSphereFromPoints *New()
    {
        return new EstimateSphereFromPoints;
    }



    void setPoints(vnl_matrix<double>);

    vnl_vector<double> getSphere();

    void estimateSphere();

private:

    vnl_matrix<double> points;

    vnl_vector<double> sphere;

};
