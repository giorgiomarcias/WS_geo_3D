#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

/**
 * @brief Calcola l'insieme di facce adiancenti a ogni vertice.
 *
 * Una faccia e' adiacente a un vertice se la faccia punta ad esso,
 * ovvero se esso e' uno dei suoi corner.
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
 * @param VF Array di array di indici. Ogni elemento dell'array esterno
 *           corrisponde all'elenco di indici delle facce adiacenti al
 *           al vertice corrispondente. L'array esterno ha quindi V.rows() elementi.
 * @param VFi Array di array di indici. L'array esterno ha V.rows() elementi. VFi[i]
 *            ha lo stesso numero di elementi del corrispondente elemento in VF[i].
 *            VFi[i][j] corrisponde al corner di Vf[i][j] che incide sul vertice i.
 */
void vertex_face_adjacency(
    MatrixXd const &V,
    MatrixXi const &F,
    std::vector<std::vector<int>> &VF,
    std::vector<std::vector<int>> &VFi)
{
    VF.clear();
    VFi.clear();
    VF.resize(V.rows());
    VFi.resize(V.rows());
}

/**
 * @brief Calcola la lista di facce adiacenti ad ogni faccia.
 *
 * Una faccia e' adiacente a un'altra se queste condividono un lato, ovvero se
 * puntano agli stessi due vertici, ma in ordine inverso:
 * F.row(i) || F.row(j) <=> F(i,p) == F(j,q) &&
 *                          F(i,(p+1)%F.cols()) == F(j, (F.cols()+q-1)%F.cols())
 *
 * Questa funzione necessita dell'adiacenza vertice->facce, risultato della funzione
 * vertex_face_adjacency().
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
 * @param VF Array di array di indici. Ogni elemento dell'array esterno
 *           corrisponde all'elenco di indici delle facce adiacenti al
 *           al vertice corrispondente. L'array esterno ha quindi V.rows() elementi.
 * @param VFi Array di array di indici. L'array esterno ha V.rows() elementi. VFi[i]
 *            ha lo stesso numero di elementi del corrispondente elemento in VF[i].
 *            VFi[i][j] corrisponde al corner di Vf[i][j] che incide sul vertice i.
 * @param FF Lista di indici di facce adiacenti per ogni faccia. Ogni riga corrisponde
 *           alla lista di adiacenza di una faccia. Ogni faccia ha F.cols() lati,
 *           per cui avra' F.cols() facce adiacenti (alcune potrebbero essere
 *           nulle in presenza di buchi - indice -1).
 * @param FFi Lista di indici per ogni faccia. Stessa dimensionalita' di FF. Ogni
 *            riga corrisponde a indici per una faccia, un indice per ogni faccia
 *            adiacente. Alla riga i, colonna j, l'indice corrisponde al lato della
 *            j-esima faccia adiacente alla i-esima, condiviso con il lato j-esimo
 *            della faccia i-esima.
 */
void face_face_adjacency(
    MatrixXd const &V,
    MatrixXi const &F,
    std::vector<std::vector<int>> const& VF,
    std::vector<std::vector<int>> const& VFi,
    MatrixXi &FF,
    MatrixXi &FFi)
{
    FF.resize(F.rows(), F.cols());
    FFi.resize(F.rows(), F.cols());
    FF.setConstant(-1);
    FFi.setConstant(-1);
}