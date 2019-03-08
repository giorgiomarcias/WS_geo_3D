#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


/**
 * @brief Calcola la direzione normale di ogni vertice nella mesh in input.
 *
 * Data una mesh di triangoli M = (V, F), dove V e' un array di posizioni 3D
 * (i vertici) e F e' un array di triple di indici ai vertici (ogni tripla
 * rappresenta una faccia triangolare), questa funzione calcola la direzione
 * normale di ogni vertice come una somma pesata delle normali dei triangoli
 * intorno (incidenti) a tale vertice.
 * Il peso è dato dall'area del triangolo moltiplicato l'angolo incidente
 * sul vertice.
 *
 * L'area di un triangolo viene calcolata come meta' della lunghezza (norma)
 * del prodotto vettoriale tra due lati.
 * L'angolo incidente su un vertice viene calcolato come l'arcocoseno del
 * prodotto scalare tra i due lati incidenti (intesi come vettori unitari
 * centrati su di esso), oppure come l'arcotangente del seno e coseno:
 * - il seno e' proporzionale alla normal del prodotto vettoriale
 * - il cose e' proporzionale al prodotto scalare.
 *
 * @param V I vertici della mesh. Per ogni riga della matrice V, la posizione
 *          del vertice e' costituita dalle coordinate x,y,z memorizzate nelle
 *          3 colonne della riga.
 * @param F I triangoli della mesh. Per ogni riga della matrice F, il triangolo
 *          e' descritto dagli indici i,j,k memorizzati nelle 3 colonne della
 *          riga. Gli indici i,j,k si riferiscono ai 3 vertici A, B, C del
 *          triangolo, memorizzati in V.row(i), V.row(j) e V.row(k),
 *          rispettivamente. NOTA: per ogni triangolo, i 3 vertici sono da
 *          considerarsi indicati in senso antiorario.
 * @return MatrixXd La matrice restituita ha una riga per ogni vertice
 *                  (N.rows() == V.rows()) e 3 colonne. Ogni riga corrisponde
 *                  alla direzione normale del vertice corrispondente, avente
 *                  le 3 coordinate x,y,z.
 */
MatrixXd perVertexNormals(MatrixXd const &V, MatrixXi const &F)
{
    MatrixXd N = MatrixXd::Zero(V.rows(), 3);

    // per leggere/scrivere un elemento di una matrice X: X(i,j)
    // per leggere/scrivere una riga di una matrice X: X.row(i) (un vettore orizzontale)
    // il vettore da un punto p0 a un altro punto p1: Vector3d v = p1 - p0;
    // il prodotto vettoriale tra 2 vettori: Vector3d c = v1.cross(v2);
    // normalizzare un vettore, in-place: c.normalize();
    // normalizzare un vettore, in una nuova variabile: Vector3d cn = c.normalized();
    // lunghezza (norma) di un vettore: v.norm();
    // angolo tra due vettori normalizzati: std::acos(n1.dot(n2));
    // angolo tra due vettori non-normalizzati: std::atan2(v1.cross(v2).norm(), v1.dot(v2));

    for (int f = 0; f < F.rows(); ++f)
    {
        int i = F(f, 0);
        int j = F(f, 1);
        int k = F(f, 2);

        Vector3d A = V.row(i);
        Vector3d B = V.row(j);
        Vector3d C = V.row(k);

        Vector3d e0 = B - A;
        Vector3d e1 = C - B;
        Vector3d e2 = A - C;

        Vector3d c0 = e0.cross(-e2);
        Vector3d c1 = e1.cross(-e0);
        Vector3d c2 = e2.cross(-e1);

        double area = c0.norm() / 2.0;

        Vector3d n = c0.normalized();

        double a_i = std::atan2(c0.norm(), e0.dot(-e2));
        double a_j = std::atan2(c1.norm(), e1.dot(-e0));
        double a_k = std::atan2(c2.norm(), e2.dot(-e1));

        N.row(i) += area * a_i * n;
        N.row(j) += area * a_j * n;
        N.row(k) += area * a_k * n;
    }

    for (int v = 0; v < V.rows(); ++v) {
        N.row(v).normalize();
    }

    return N;
}