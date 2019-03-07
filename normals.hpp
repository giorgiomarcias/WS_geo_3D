#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

#include "topology.hpp"

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
 * presi in senso antiorario, e normalizzato (lunghezza 1):
 *             C
 *             *                ^
 *           /  \                \        (B - A) ^ (C - A)
 *        /      \          N =   \   = ---------------------
 *     /          \                *    ||(B - A) ^ (C - A)||
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

    return N;
}

/**
 * @brief Calcola la direzione normale di ogni vertice nella mesh in input.
 *
 * Data una mesh di triangoli M = (V, F), dove V e' un array di posizioni 3D
 * (i vertici) e F e' un array di triple di indici ai vertici (ogni tripla
 * rappresenta una faccia triangolare), questa funzione calcola la direzione
 * normale di ogni vertice come una somma pesata delle normali dei triangoli.
 * Il peso è dato dall'area del triangolo per l'angolo incidente sul vertice.
 *
 * La direzione normale di un triangolo e' la direzione normale del piano su
 * cui giace. Questa viene calcolata come il prodotto vettoriale tra i vettori
 * che rappresentano due lati del triangolo centrati sul vertice a comune,
 * presi in senso antiorario, e normalizzato (lunghezza 1):
 *             C
 *             *                ^
 *           /  \                \        (B - A) ^ (C - A)
 *        /      \          N =   \   = ---------------------
 *     /          \                *    ||(B - A) ^ (C - A)||
 *  A *------------* B
 *
 * L'area di un triangolo viene calcolata come meta' della lunghezza (norma)
 * del prodotto vettoriale tra due lati. L'angolo incidente su un vertice
 * viene calcolato come l'arcocoseno del prodotto scalare tra i due lati
 * incidenti (intesi come vettori ventrati su di esso).
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
    // angolo tra due vettori non-normalizzati: std::atan2(v1.cross(c2).norm(), v1.dot(v2));

    return N;
}

/**
 * @brief Calcola la direzione normale di ogni corner di ogni triangolo nella
 * mesh in input.
 *
 * Data una mesh di triangoli M = (V, F), dove V e' un array di posizioni 3D
 * (i vertici) e F e' un array di triple di indici ai vertici (ogni tripla
 * rappresenta una faccia triangolare), questa funzione calcola la direzione
 * normale di ogni corner di ogni triangolo come una somma pesata delle normali
 * dei triangoli adiacenti nello stesso settore. Un settore e' costituito da
 * un insieme di triangoli incidenti su uno stesso vertice, uno adiacente al
 * successivo, che approssimano una superficie smooth: due triangoli adiacenti
 * tra loro (incidono sullo stesso lato) fanno parte dello stesso settore se
 * l'angolo fra le loro rispettive direzioni normali e' inferiore ad una data
 * soglia (il coseno - prodotto scalare - e' maggiore di una soglia).
 * Il peso è dato dall'area del triangolo per l'angolo incidente sul vertice.
 *
 * La direzione normale di un triangolo e' la direzione normale del piano su
 * cui giace. Questa viene calcolata come il prodotto vettoriale tra i vettori
 * che rappresentano due lati del triangolo centrati sul vertice a comune,
 * presi in senso antiorario, e normalizzato (lunghezza 1):
 *             C
 *             *                ^
 *           /  \                \        (B - A) ^ (C - A)
 *        /      \          N =   \   = ---------------------
 *     /          \                *    ||(B - A) ^ (C - A)||
 *  A *------------* B
 *
 * L'area di un triangolo viene calcolata come meta' della lunghezza (norma)
 * del prodotto vettoriale tra due lati. L'angolo incidente su un vertice
 * viene calcolato come l'arcocoseno del prodotto scalare tra i due lati
 * incidenti (intesi come vettori ventrati su di esso).
 *
 * Questa funzione richiama le funzioni vertex_face_adjacency() e
 * face_face_adjacency() per calcolare l'adiacenza tra le facce.
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
 * @return MatrixXd La matrice restituita ha tre righe per ogni triangolo
 *                  (N.rows() == F.rows() * 3) e 3 colonne. Ogni riga corrisponde
 *                  alla direzione normale del corner di un triangolo, avente
 *                  le 3 coordinate x,y,z.
 */
MatrixXd perCornerNormals(MatrixXd const &V, MatrixXi const &F)
{
    MatrixXd N = MatrixXd::Zero(F.rows() * 3, 3);

    // coseno di 30 gradi:
    const double cos_thr = std::sqrt(3) / 2;

    return N;
}