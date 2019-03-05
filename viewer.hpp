#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>

using namespace Eigen;

class Viewer
{
  public:
    Viewer(
        std::function<MatrixXd(MatrixXd const&, MatrixXi const&)> const& faceNormalFun
    ) : _viewer(), _faceNormalFun(faceNormalFun)
    {
        auto callback_mouse_down = [this](igl::opengl::glfw::Viewer &viewer, int button, int modifier) -> bool {
            if (button == GLFW_MOUSE_BUTTON_1 && modifier == 0)
            {
                _lastTimePoint = std::chrono::steady_clock::now();
            }
            return false;
        };

        auto callback_mouse_up = [this](igl::opengl::glfw::Viewer &viewer, int button, int modifier) -> bool {
            if (button == GLFW_MOUSE_BUTTON_1 && modifier == 0)
            {
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastTimePoint).count() <= 333)
                {
                    // Cast a ray in the view direction starting from the mouse position
                    float x = viewer.current_mouse_x;
                    float y = viewer.core.viewport(3) - viewer.current_mouse_y;
                    auto &V = _viewer.data().V;
                    auto &F = _viewer.data().F;
                    Vector3f bc;
                    int fid;
                    auto hit = igl::unproject_onto_mesh(
                        Vector2f(x, y),
                        viewer.core.view,
                        viewer.core.proj,
                        viewer.core.viewport,
                        V,
                        F,
                        fid,
                        bc);
                    if (hit)
                    {
                        auto hitPoint = V.row(F(fid, 0)) * bc(0) + V.row(F(fid, 1)) * bc(1) + V.row(F(fid, 2)) * bc(2);
                        MatrixXd P(1, 3);
                        P.row(0) = hitPoint;
                        MatrixXd C(1, 3);
                        C.row(0) << 1, 0, 0;
                        viewer.data().set_points(P, C);
                    }
                    else
                    {
                        viewer.data().set_points(MatrixXd(), MatrixXd());
                    }
                    return true;
                }
            }
            return false;
        };

        auto callback_key_down = [this](igl::opengl::glfw::Viewer &viewer, unsigned char key, int modifier) -> bool {
            auto &V = _viewer.data().V;
            auto &F = _viewer.data().F;

            MatrixXd N;
            switch (key)
            {
            case '1':
                // flat shading - face normals
                N = _faceNormalFun(V, F);
                break;
            case '2':
                // smooth normals - vertex normals
                break;
            case '3':
                // sharp/smooth shading - corner normals
                break;
            default:
                return false;
            }
            viewer.data().set_normals(N);
            return true;
        };

        _viewer.callback_mouse_down = callback_mouse_down;
        _viewer.callback_mouse_up = callback_mouse_up;
        _viewer.callback_key_down = callback_key_down;
    }

    void set_mesh(MatrixXd const &V, MatrixXi const &F)
    {
        _viewer.data().set_mesh(V, F);
        _viewer.data().set_normals(_faceNormalFun(V, F));
    }

    void launch()
    {
        _viewer.launch();
    }

  private:
    igl::opengl::glfw::Viewer _viewer;
    std::function<MatrixXd(MatrixXd const&, MatrixXi const&)> _faceNormalFun;
    std::chrono::steady_clock::time_point _lastTimePoint;
};