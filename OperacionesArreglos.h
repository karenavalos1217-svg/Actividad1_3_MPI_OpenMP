#pragma once

#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <climits>

class OperacionesArreglos
{
private:

    // Arreglos dinamicos locales de cada proceso MPI
    int* arregloA;
    int* arregloB;
    long long* arregloC;

    // Informacion del proceso
    int tamLocal;
    int rank;
    int totalProcesos;

    std::string nombreEquipo;

    // Archivo de registro local
    std::ofstream archivoLog;

    // Indica si se debe mostrar informacion detallada
    bool mostrarDetalle;

public:

    // Constructor
    OperacionesArreglos(
        int rankMPI,
        int numeroProcesos,
        std::string equipo
    );

    // Destructor
    ~OperacionesArreglos();


    // ==========================================
    // LOG
    // ==========================================

    void iniciarLog();

    void escribirLog(std::string mensaje);


    // ==========================================
    // CREACION DE ARREGLOS
    // ==========================================

    void crearArregloMPI(int tamanoGlobal);


    // ==========================================
    // LLENADO
    // ==========================================

    void llenarSecuencial(int inicioGlobal);

    void llenarAleatorio(int inicioGlobal);


    // ==========================================
    // OPERACIONES ARITMETICAS
    // ==========================================

    void sumarArreglos(int inicioGlobal);

    void restarArreglos(int inicioGlobal);

    void multiplicarArreglos(int inicioGlobal);

    void cuadradoArreglo(int inicioGlobal);


    // ==========================================
    // REDUCCIONES LOCALES CON OPENMP
    // ==========================================

    long long sumatoriaLocal();

    int maximoLocal();

    int minimoLocal();


    // ==========================================
    // GETTERS
    // ==========================================

    int getTamLocal();

    int* getArregloA();

    int* getArregloB();

    long long* getArregloC();
};