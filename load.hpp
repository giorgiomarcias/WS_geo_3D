#pragma once

#include <string>
#include <igl/readOBJ.h>

using namespace Eigen;

void loadAsIndexedTriangleMesh(std::string const &filename, MatrixXd &V, MatrixXi &F)
{
    igl::readOBJ(filename, V, F);
}

void loadAsTriangleSoup(std::string const &filename, MatrixXd &V, MatrixXi &F)
{
    MatrixXd VV;
    MatrixXi FF;
    igl::readOBJ(filename, VV, FF);

    V.resize(FF.rows() * 3, 3);
    F.resize(FF.rows(), 3);

    for (int i = 0; i < FF.rows(); ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int ind = i * 3 + j;
            V.row(ind) = VV.row(FF(i, j));
            F(i, j) = ind;
        }
    }
}