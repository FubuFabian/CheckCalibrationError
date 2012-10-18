#ifndef SPHEREFUNCTION_H
#define SPHEREFUNCTION_H

#endif // SPHEREFUNCTION_H

#include <vnl/vnl_least_squares_function.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>
#include <vector>

class SphereFunction : public vnl_least_squares_function
{

public:

    SphereFunction
       (vnl_matrix<double> * points);

       virtual ~SphereFunction();

       virtual void f(vnl_vector<double> const &p, vnl_vector<double> &fx);

private:

    vnl_matrix<double> * _data;

};

