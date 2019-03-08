#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

#include "topology.hpp"

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

    std::vector<std::vector<int>> VF, VFi;
    MatrixXi FF, FFi;

    vertex_face_adjacency(V, F, VF, VFi);
    face_face_adjacency(V, F, VF, VFi, FF, FFi);

    MatrixXd FN(F.rows(), 3);
    VectorXd Fareas(F.rows());
    MatrixXd Fangles(F.rows(), 3);

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

        Fareas(f) = c0.norm() / 2.0;

        FN.row(f) = c0.normalized();

        Fangles(f, 0) = std::atan2(c0.norm(), e0.dot(-e2));
        Fangles(f, 1) = std::atan2(c1.norm(), e1.dot(-e0));
        Fangles(f, 2) = std::atan2(c2.norm(), e2.dot(-e1));
    }

    MatrixXd FF_cosines(F.rows(), 3);
    for (int f = 0; f < F.rows(); ++f) {
        auto const& fn = FN.row(f);
        for (int p = 0; p < 3; ++p) {
            auto const& ffn = FN.row(FF(f, p));
            FF_cosines(f, p) = fn.dot(ffn);
        }
    }

    for (int f = 0; f < F.rows(); ++f) {
        auto const& fn = FN.row(f);
        for (int p = 0; p < 3; ++p) {
            auto n = Fareas(f) * Fangles(f, p) * fn;

            N.row(3 * f + p) += n;

            // contribusci alle normali di tutte le facce adiacenti intorno
            // al corner p, il cui angolo diedrale non supera la soglia
            int nf = FF(f, (3 + p - 1) % 3);
            if (nf >= 0) {
                int nfi = FFi(f, (3 + p - 1) % 3);
                // prima le facce in senso antiorario
                do {
                    if (FF_cosines(nf, nfi) >= cos_thr) {
                        N.row(3 * nf + nfi) += n;
                        int nnf = FF(nf, (3 + nfi - 1) % 3);
                        nfi = FFi(nf, (3 + nfi - 1) % 3);
                        nf = nnf;
                    } else {
                        break;
                    }
                } while (nf >= 0 && nf != f);

                // poi le facce in senso orario, eventualmente
                if (nf != f) {
                    nf = f;
                    nfi = p;
                    while (nf >= 0 && FF_cosines(nf, nfi) >= cos_thr) {
                        int nnf = FF(nf, nfi);
                        nfi = (FFi(nf, nfi) + 1) % 3;
                        nf = nnf;
                        N.row(3 * nf + nfi) += n;
                    }
                }
            }
        }
    }

    for (int n = 0; n < N.rows(); ++n) {
        N.row(n).normalize();
    }

    return N;
}