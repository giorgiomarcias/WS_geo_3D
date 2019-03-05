#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

MatrixXd perFaceNormals(MatrixXd const &V, MatrixXi const &F)
{
    MatrixXd N(F.rows(), 3);
    N.resize(F.rows(), F.cols());
    for (int i = 0; i < F.rows(); ++i)
    {
        auto p0 = V.row(F(i, 0));
        auto p1 = V.row(F(i, 1));
        auto p2 = V.row(F(i, 2));
        Vector3d e0 = p1 - p0;
        Vector3d e1 = p2 - p0;
        N.row(i) = e0.cross(e1).normalized();
    }
    return N;
}