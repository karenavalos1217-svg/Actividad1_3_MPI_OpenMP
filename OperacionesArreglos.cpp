#include "OperacionesArreglos.h"

using namespace std;

// Constructor
OperacionesArreglos::OperacionesArreglos(
    int rankMPI,
    int numeroProcesos,
    string equipo)
{
    arregloA = nullptr;
    arregloB = nullptr;
    arregloC = nullptr;

    tamLocal = 0;
    rank = rankMPI;
    totalProcesos = numeroProcesos;
    nombreEquipo = equipo;

    mostrarDetalle = false;

    iniciarLog();
}

// Destructor
OperacionesArreglos::~OperacionesArreglos()
{
    if (arregloA != nullptr)
        delete[] arregloA;

    if (arregloB != nullptr)
        delete[] arregloB;

    if (arregloC != nullptr)
        delete[] arregloC;

    if (archivoLog.is_open())
        archivoLog.close();
}

// Inicia el archivo de log de cada proceso
void OperacionesArreglos::iniciarLog()
{
    string nombreArchivo =
        "log_equipo_" +
        nombreEquipo +
        "_nodo_" +
        to_string(rank) +
        ".txt";

    archivoLog.open(nombreArchivo, ios::out);

    if (archivoLog.is_open())
    {
        archivoLog << "LOG DE EJECUCION MPI + OPENMP" << endl;
        archivoLog << "Equipo: " << nombreEquipo << endl;
        archivoLog << "Proceso MPI: " << rank << endl;
        archivoLog << endl;

        archivoLog.flush();
    }
}

// Escribe el mensaje en consola y en el archivo log
void OperacionesArreglos::escribirLog(string mensaje)
{
    cout << mensaje << endl;

    if (archivoLog.is_open())
    {
        archivoLog << mensaje << endl;
        archivoLog.flush();
    }
}

// Crea los arreglos dinamicos de cada proceso MPI
void OperacionesArreglos::crearArregloMPI(int tamanoGlobal)
{
    tamLocal = tamanoGlobal / totalProcesos;

    if (arregloA != nullptr)
        delete[] arregloA;

    if (arregloB != nullptr)
        delete[] arregloB;

    if (arregloC != nullptr)
        delete[] arregloC;

    arregloA = new int[tamLocal];
    arregloB = new int[tamLocal];
    arregloC = new long long[tamLocal];

    mostrarDetalle = (tamanoGlobal <= 42);

    string mensaje =
        "[Equipo: " + nombreEquipo +
        "] [Proceso MPI: " + to_string(rank) +
        "] Arreglos creados. Elementos locales: " +
        to_string(tamLocal);

    escribirLog(mensaje);
}

// Llena los arreglos con valores consecutivos
void OperacionesArreglos::llenarSecuencial(int inicioGlobal)
{
#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        arregloA[i] = posicionGlobal + 1;
        arregloB[i] = posicionGlobal + 1;

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Llenado secuencial]" +
                " [Valor A: " + to_string(arregloA[i]) +
                "] [Valor B: " + to_string(arregloB[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Llena los arreglos con numeros aleatorios entre 1 y 1,000,000
void OperacionesArreglos::llenarAleatorio(int inicioGlobal)
{
    unsigned int semilla =
        static_cast<unsigned int>(time(nullptr)) +
        static_cast<unsigned int>(rank * 1000);

    srand(semilla);

    for (int i = 0; i < tamLocal; i++)
    {
        int aleatorioA =
            static_cast<int>(
                (rand() * 32768LL + rand()) % 1000000
                ) + 1;

        int aleatorioB =
            static_cast<int>(
                (rand() * 32768LL + rand()) % 1000000
                ) + 1;

        arregloA[i] = aleatorioA;
        arregloB[i] = aleatorioB;
    }

#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Llenado aleatorio]" +
                " [Valor A: " + to_string(arregloA[i]) +
                "] [Valor B: " + to_string(arregloB[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Suma los elementos de A y B
void OperacionesArreglos::sumarArreglos(int inicioGlobal)
{
#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        arregloC[i] =
            static_cast<long long>(arregloA[i]) +
            arregloB[i];

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Suma]" +
                " [Resultado: " + to_string(arregloC[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Resta los elementos de A y B
void OperacionesArreglos::restarArreglos(int inicioGlobal)
{
#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        arregloC[i] =
            static_cast<long long>(arregloA[i]) -
            arregloB[i];

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Resta]" +
                " [Resultado: " + to_string(arregloC[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Multiplica los elementos de A y B
void OperacionesArreglos::multiplicarArreglos(int inicioGlobal)
{
#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        arregloC[i] =
            static_cast<long long>(arregloA[i]) *
            arregloB[i];

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Multiplicacion]" +
                " [Resultado: " + to_string(arregloC[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Calcula el cuadrado de cada elemento de A
void OperacionesArreglos::cuadradoArreglo(int inicioGlobal)
{
#pragma omp parallel for
    for (int i = 0; i < tamLocal; i++)
    {
        int hilo = omp_get_thread_num();
        int posicionGlobal = inicioGlobal + i;

        arregloC[i] =
            static_cast<long long>(arregloA[i]) *
            arregloA[i];

        if (mostrarDetalle)
        {
            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Hilo OpenMP: " + to_string(hilo) +
                "] [Posicion: " + to_string(posicionGlobal) +
                "] [Operacion: Cuadrado]" +
                " [Resultado: " + to_string(arregloC[i]) + "]";

#pragma omp critical
            {
                escribirLog(mensaje);
            }
        }
    }
}

// Calcula la suma local usando reduccion de OpenMP
long long OperacionesArreglos::sumatoriaLocal()
{
    long long sumaLocal = 0;

#pragma omp parallel for reduction(+:sumaLocal)
    for (int i = 0; i < tamLocal; i++)
    {
        sumaLocal += arregloA[i];
    }

    return sumaLocal;
}

// Busca el valor maximo local usando reduccion de OpenMP
int OperacionesArreglos::maximoLocal()
{
    int maxLocal = INT_MIN;

#pragma omp parallel for reduction(max:maxLocal)
    for (int i = 0; i < tamLocal; i++)
    {
        if (arregloA[i] > maxLocal)
        {
            maxLocal = arregloA[i];
        }
    }

    return maxLocal;
}

// Busca el valor minimo local usando reduccion de OpenMP
int OperacionesArreglos::minimoLocal()
{
    int minLocal = INT_MAX;

#pragma omp parallel for reduction(min:minLocal)
    for (int i = 0; i < tamLocal; i++)
    {
        if (arregloA[i] < minLocal)
        {
            minLocal = arregloA[i];
        }
    }

    return minLocal;
}

// Regresa el tamano de la seccion local
int OperacionesArreglos::getTamLocal()
{
    return tamLocal;
}

// Regresa el arreglo A
int* OperacionesArreglos::getArregloA()
{
    return arregloA;
}

// Regresa el arreglo B
int* OperacionesArreglos::getArregloB()
{
    return arregloB;
}

// Regresa el arreglo de resultados C
long long* OperacionesArreglos::getArregloC()
{
    return arregloC;
}