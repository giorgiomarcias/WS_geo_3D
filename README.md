# WS_geo_3D
Workshop: Geometria 3D per programmatori

Base di codice per gli esercizi.

# Requisiti
Pacchetti necessari:
* cmake
* make
* git
* un compilatore C++ (gcc, clang, ...)

# Istruzioni
1. Scaricare il repository in una directory dedicata
```sh
git clone --recurse-submodules https://github.com/giorgiomarcias/WS_geo_3D.git
cd WS_geo_3D
```
2. preparare una directory per la compilazione
```sh
mkdir build
cd build
```
3. lanciare cmake per generare gli script di compilazione (la prima volta assicurarsi di avere connessione internet attiva, potrebbe impiegare diverso tempo perchè ulteriori librerie necessarie saranno scaricate)
```sh
cmake ..
```
4. compilare
```sh
make
```