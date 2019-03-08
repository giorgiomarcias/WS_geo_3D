#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

/**
 * @brief Calcola la direzione normale di ogni corner di ogni triangolo nella
 * mesh in input.
 *
 * Data una mesh di triangoli M = (V, F), dove V e' un array di posizioni 3D
 * (i vertici) e F e' un array di triple di indici ai vertici (ogni tripla
 * rappresenta una faccia triangolare), questa funzione calcola la direzione
 * normale di ogni corner di ogni triangolo come una somma pesata delle normali
 * dei triangoli adiacenti nello stesso settore.
 * Ogni triangolo ha 3 corner.
 * Un settore e' costituito da un insieme di triangoli incidenti su uno stesso
 * vertice, uno adiacente al successivo, che approssimano una superficie smooth:
 * due triangoli adiacenti tra loro (incidono sullo stesso lato) fanno parte
 * dello stesso settore se l'angolo fra le loro rispettive direzioni normali e'
 * inferiore ad una data soglia (il coseno, o prodotto scalare, - e' maggiore
 * di una soglia).
 * Il peso è dato dall'area del triangolo moltiplicato l'angolo incidente sul
 * vertice.
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
 * @return MatrixXd La matrice restituita ha tre righe per ogni triangolo
 *                  (N.rows() == F.rows() * 3) e 3 colonne. Ogni riga corrisponde
 *                  alla direzione normale di un corner di un triangolo, avente
 *                  le 3 coordinate x,y,z.
 */
MatrixXd perCornerNormals(MatrixXd const &V, MatrixXi const &F)
{
    MatrixXd N = MatrixXd::Zero(F.rows() * 3, 3);

    // coseno di 30 gradi:
    const double cos_thr = std::sqrt(3) / 2;

    // suggerimento: e' necessario trovare le facce adiacenti intorno ad un vertice
    // se non vuoi scrivere il codice che fa questo, puoi includere il file
    // #include "topology.hpp"
    // e chiamare le funzioni `vertex_face_adjacency()` e `face_face_adjacency()`

    return N;
}