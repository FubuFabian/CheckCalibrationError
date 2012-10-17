#ifndef ESTIMATESPHEREFROMPOINTS_H
#define ESTIMATESPHEREFROMPOINTS_H

#endif // ESTIMATESPHEREFROMPOINTS_H

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>


class EstimateSphereFromPoints
{

public:

    static EstimateSphereFromPoints *New()
    {
        return new EstimateSphereFromPoints;
    }



    void setPoints(vnl_matrix<double>);

	void setCenter(vnl_matrix<double>);

    vnl_vector<double> getSphere();

    void estimateSphere();

private:

    vnl_matrix<double> points;

    vnl_vector<double> sphere;

	vnl_matrix<double> centers;

};
