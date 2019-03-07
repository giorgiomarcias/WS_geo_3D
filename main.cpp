#include <chrono>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>

#include "viewer.hpp"
#include "load.hpp"
#include "normals.hpp"

using namespace Eigen;

int main(int argc, char *argv[]) {
    MatrixXd V;
    MatrixXi F;

    Viewer viewer(&perFaceNormals, &perVertexNormals, &perCornerNormals);

    // loadAsTriangleSoup("../meshes/vase.obj", V, F);
    loadAsIndexedTriangleMesh("../meshes/vase.obj", V, F);

    viewer.set_mesh(V, F);
    viewer.launch();
}