#include <chrono>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>

#include "viewer.hpp"

using namespace Eigen;

int main(int argc, char *argv[]) {
    MatrixXd V;
    MatrixXi F;
    Vector3f bc;
    Vector3d hitPoint;
    int fid;
    Vector3d red(1, 0, 0);

    Viewer viewer;

    igl::readOBJ("../meshes/teapot.obj", V, F);

    viewer.set_mesh(V, F);
    viewer.launch();
}