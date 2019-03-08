#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

/**
 * @brief Calcola la direzione normale di ogni triangolo nella mesh in input.
 *
 * Data una mesh di triangoli M = (V, F), dove V e' un array di posizioni 3D
 * (i vertici) e F e' un array di triple di indici ai vertici (ogni tripla
 * rappresenta una faccia triangolare), questa funzione calcola la direzione
 * normale di ogni triangolo.
 *
 * La direzione normale di un triangolo e' la direzione normale del piano su
 * cui giace. Questa viene calcolata come il prodotto vettoriale tra i vettori
 * che rappresentano due lati del triangolo centrati sul vertice a comune,
 * presi in senso antiorario, e normalizzato (cioe' reso unitario: lunghezza 1):
 *
 *             C
 *             *
 *           /  \                         (B - A) ^ (C - A)         ^
 *        /      \                  n = ---------------------       |
 *     /          \                     ||(B - A) ^ (C - A)||
 *  A *------------* B
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
 * @return MatrixXd La matrice restituita ha una riga per ogni faccia triangolare
 *                  (N.rows() == F.rows()) e 3 colonne. Ogni riga corrisponde
 *                  alla direzione normale del triangolo corrispondente, avente
 *                  le 3 coordinate x,y,z.
 */
MatrixXd perFaceNormals(MatrixXd const &V, MatrixXi const &F)
{
    MatrixXd N(F.rows(), 3);

    // per leggere/scrivere un elemento di una matrice X: X(i,j)
    // per leggere/scrivere una riga di una matrice X: X.row(i) (un vettore orizzontale)
    // il vettore da un punto p0 a un altro punto p1: Vector3d v = p1 - p0;
    // il prodotto vettoriale tra 2 vettori: Vector3d c = v1.cross(v2);
    // normalizzare un vettore, in-place: c.normalize();
    // normalizzare un vettore, in una nuova variabile: Vector3d cn = c.normalized();

    for (int f = 0; f < F.rows(); ++f)
    {
        Vector3d A = V.row(F(f, 0));
        Vector3d B = V.row(F(f, 1));
        Vector3d C = V.row(F(f, 2));
        Vector3d e0 = B - A;
        Vector3d e1 = C - A;
        N.row(f) = e0.cross(e1).normalized();
    }

    return N;
}