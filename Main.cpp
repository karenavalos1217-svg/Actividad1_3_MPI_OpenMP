#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <string>
#include <climits>

#include "OperacionesArreglos.h"

using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    int totalProcesos;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos);

    char nombreProcesador[MPI_MAX_PROCESSOR_NAME];
    int longitudNombre;

    MPI_Get_processor_name(
        nombreProcesador,
        &longitudNombre
    );

    string nombreEquipo(nombreProcesador);

    if (rank == 0)
    {
        cout << "Avalos Oliva Karen Lizeth | Chavez Torres Oliver Daniel | Ramirez Arvizu Fernando David" << endl;
    }

    OperacionesArreglos operaciones(
        rank,
        totalProcesos,
        nombreEquipo
    );

    int opcion = 0;
    int tamanoGlobal = 0;
    int inicioGlobal = 0;

    bool arreglosCreados = false;
    bool arreglosLlenados = false;

    if (rank == 0)
    {
        cout << endl;
        cout << "ACTIVIDAD 1.3 - REDUCE EN MPI CON OPENMP" << endl;
        cout << "Procesos MPI: " << totalProcesos << endl;
        cout << endl;
    }

    do
    {
        if (rank == 0)
        {
            cout << endl;
            cout << "MENU" << endl;
            cout << "1. Crear arreglos" << endl;
            cout << "2. Sumar arreglos" << endl;
            cout << "3. Restar arreglos" << endl;
            cout << "4. Multiplicar arreglos" << endl;
            cout << "5. Calcular el cuadrado de un arreglo" << endl;
            cout << "6. Llenar secuencial" << endl;
            cout << "7. Llenar aleatorio" << endl;
            cout << "8. Sumatoria" << endl;
            cout << "9. Promedio" << endl;
            cout << "10. Maximo" << endl;
            cout << "11. Minimo" << endl;
            cout << "12. Salir" << endl;
            cout << endl;
            cout << "Opcion: ";
            cin >> opcion;
        }

        MPI_Bcast(
            &opcion,
            1,
            MPI_INT,
            0,
            MPI_COMM_WORLD
        );

        if (opcion == 1)
        {
            if (rank == 0)
            {
                cout << endl;
                cout << "Tamano total del arreglo: ";
                cin >> tamanoGlobal;
            }

            MPI_Bcast(
                &tamanoGlobal,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            if (tamanoGlobal <= 0)
            {
                if (rank == 0)
                {
                    cout << "El tamano debe ser mayor que 0." << endl;
                }

                continue;
            }

            if (tamanoGlobal % totalProcesos != 0)
            {
                if (rank == 0)
                {
                    cout << "El tamano debe ser divisible entre "
                        << totalProcesos
                        << " procesos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.crearArregloMPI(tamanoGlobal);

            inicioGlobal =
                rank * operaciones.getTamLocal();

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            arreglosCreados = true;
            arreglosLlenados = false;

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Crear arreglos]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << endl;
                cout << "Arreglos creados correctamente." << endl;
                cout << "Cada proceso tiene "
                    << operaciones.getTamLocal()
                    << " elementos." << endl;
            }
        }

        else if (opcion == 2)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.sumarArreglos(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Suma de arreglos]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Suma terminada correctamente." << endl;
            }
        }

        else if (opcion == 3)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.restarArreglos(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Resta de arreglos]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Resta terminada correctamente." << endl;
            }
        }

        else if (opcion == 4)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.multiplicarArreglos(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Multiplicacion de arreglos]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Multiplicacion terminada correctamente." << endl;
            }
        }

        else if (opcion == 5)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.cuadradoArreglo(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Cuadrado]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Cuadrado terminado correctamente." << endl;
            }
        }

        else if (opcion == 6)
        {
            if (!arreglosCreados)
            {
                if (rank == 0)
                {
                    cout << "Primero debes crear los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.llenarSecuencial(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            arreglosLlenados = true;

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Llenado secuencial]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Llenado secuencial terminado." << endl;
            }
        }

        else if (opcion == 7)
        {
            if (!arreglosCreados)
            {
                if (rank == 0)
                {
                    cout << "Primero debes crear los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            operaciones.llenarAleatorio(inicioGlobal);

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            arreglosLlenados = true;

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Operacion: Llenado aleatorio]" +
                " [Tiempo: " + to_string(fin - inicio) +
                " segundos]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << "Llenado aleatorio terminado." << endl;
            }
        }

        else if (opcion == 8)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioReduce = MPI_Wtime();

            long long sumaLocal =
                operaciones.sumatoriaLocal();

            long long sumaGlobalReduce = 0;

            MPI_Reduce(
                &sumaLocal,
                &sumaGlobalReduce,
                1,
                MPI_LONG_LONG,
                MPI_SUM,
                0,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finReduce = MPI_Wtime();

            long long sumaGlobalAllreduce = 0;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioAllreduce = MPI_Wtime();

            MPI_Allreduce(
                &sumaLocal,
                &sumaGlobalAllreduce,
                1,
                MPI_LONG_LONG,
                MPI_SUM,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finAllreduce = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Sumatoria local: " +
                to_string(sumaLocal) +
                "] [MPI_Allreduce global: " +
                to_string(sumaGlobalAllreduce) + "]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << endl;
                cout << "SUMATORIA GLOBAL" << endl;

                cout << "MPI_Reduce: "
                    << sumaGlobalReduce << endl;

                cout << "Tiempo MPI_Reduce: "
                    << finReduce - inicioReduce
                    << " segundos" << endl;

                cout << "MPI_Allreduce: "
                    << sumaGlobalAllreduce << endl;

                cout << "Tiempo MPI_Allreduce: "
                    << finAllreduce - inicioAllreduce
                    << " segundos" << endl;
            }
        }

        else if (opcion == 9)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            long long sumaLocal =
                operaciones.sumatoriaLocal();

            long long sumaGlobalReduce = 0;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioReduce = MPI_Wtime();

            MPI_Reduce(
                &sumaLocal,
                &sumaGlobalReduce,
                1,
                MPI_LONG_LONG,
                MPI_SUM,
                0,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finReduce = MPI_Wtime();

            long long sumaGlobalAllreduce = 0;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioAllreduce = MPI_Wtime();

            MPI_Allreduce(
                &sumaLocal,
                &sumaGlobalAllreduce,
                1,
                MPI_LONG_LONG,
                MPI_SUM,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finAllreduce = MPI_Wtime();

            double promedioAllreduce =
                static_cast<double>(sumaGlobalAllreduce) /
                static_cast<double>(tamanoGlobal);

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Promedio global con MPI_Allreduce: " +
                to_string(promedioAllreduce) + "]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                double promedioReduce =
                    static_cast<double>(sumaGlobalReduce) /
                    static_cast<double>(tamanoGlobal);

                cout << endl;
                cout << "PROMEDIO GLOBAL" << endl;

                cout << "MPI_Reduce: "
                    << promedioReduce << endl;

                cout << "Tiempo MPI_Reduce: "
                    << finReduce - inicioReduce
                    << " segundos" << endl;

                cout << "MPI_Allreduce: "
                    << promedioAllreduce << endl;

                cout << "Tiempo MPI_Allreduce: "
                    << finAllreduce - inicioAllreduce
                    << " segundos" << endl;
            }
        }

        else if (opcion == 10)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            int maxLocal =
                operaciones.maximoLocal();

            int maxGlobalReduce = INT_MIN;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioReduce = MPI_Wtime();

            MPI_Reduce(
                &maxLocal,
                &maxGlobalReduce,
                1,
                MPI_INT,
                MPI_MAX,
                0,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finReduce = MPI_Wtime();

            int maxGlobalAllreduce = INT_MIN;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioAllreduce = MPI_Wtime();

            MPI_Allreduce(
                &maxLocal,
                &maxGlobalAllreduce,
                1,
                MPI_INT,
                MPI_MAX,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finAllreduce = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Maximo local: " +
                to_string(maxLocal) +
                "] [MPI_Allreduce global: " +
                to_string(maxGlobalAllreduce) + "]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << endl;
                cout << "MAXIMO GLOBAL" << endl;

                cout << "MPI_Reduce: "
                    << maxGlobalReduce << endl;

                cout << "Tiempo MPI_Reduce: "
                    << finReduce - inicioReduce
                    << " segundos" << endl;

                cout << "MPI_Allreduce: "
                    << maxGlobalAllreduce << endl;

                cout << "Tiempo MPI_Allreduce: "
                    << finAllreduce - inicioAllreduce
                    << " segundos" << endl;
            }
        }

        else if (opcion == 11)
        {
            if (!arreglosCreados || !arreglosLlenados)
            {
                if (rank == 0)
                {
                    cout << "Primero crea y llena los arreglos." << endl;
                }

                continue;
            }

            int minLocal =
                operaciones.minimoLocal();

            int minGlobalReduce = INT_MAX;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioReduce = MPI_Wtime();

            MPI_Reduce(
                &minLocal,
                &minGlobalReduce,
                1,
                MPI_INT,
                MPI_MIN,
                0,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finReduce = MPI_Wtime();

            int minGlobalAllreduce = INT_MAX;

            MPI_Barrier(MPI_COMM_WORLD);

            double inicioAllreduce = MPI_Wtime();

            MPI_Allreduce(
                &minLocal,
                &minGlobalAllreduce,
                1,
                MPI_INT,
                MPI_MIN,
                MPI_COMM_WORLD
            );

            MPI_Barrier(MPI_COMM_WORLD);

            double finAllreduce = MPI_Wtime();

            string mensaje =
                "[Equipo: " + nombreEquipo +
                "] [Proceso MPI: " + to_string(rank) +
                "] [Minimo local: " +
                to_string(minLocal) +
                "] [MPI_Allreduce global: " +
                to_string(minGlobalAllreduce) + "]";

            operaciones.escribirLog(mensaje);

            if (rank == 0)
            {
                cout << endl;
                cout << "MINIMO GLOBAL" << endl;

                cout << "MPI_Reduce: "
                    << minGlobalReduce << endl;

                cout << "Tiempo MPI_Reduce: "
                    << finReduce - inicioReduce
                    << " segundos" << endl;

                cout << "MPI_Allreduce: "
                    << minGlobalAllreduce << endl;

                cout << "Tiempo MPI_Allreduce: "
                    << finAllreduce - inicioAllreduce
                    << " segundos" << endl;
            }
        }

        else if (opcion == 12)
        {
            if (rank == 0)
            {
                cout << endl;
                cout << "Programa finalizado." << endl;
            }
        }

        else
        {
            if (rank == 0)
            {
                cout << "Opcion no valida." << endl;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

    } while (opcion != 12);

    if (rank == 0)
    {
        cout << "Avalos Oliva Karen Lizeth | Chavez Torres Oliver Daniel | Ramirez Arvizu Fernando David" << endl;
    }

    MPI_Finalize();

    return 0;
}