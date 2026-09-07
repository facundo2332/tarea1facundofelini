#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Nodo {
    int id;
    char descripcion[200];
    int prioridad;
    char horaRegistro[20];

    struct Nodo *anterior;
    struct Nodo *siguiente;
} Nodo;


void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}


void registrar_paciente(List *lista, int id, char especificaciones[]) {
    Nodo *nuevo = malloc(sizeof(Nodo));
    time_t tiempo;
    struct tm *hora;

    nuevo->id = id;
    strcpy(nuevo->descripcion, especificaciones);
    nuevo->prioridad = 3;

    tiempo = time(NULL);
    hora = localtime(&tiempo);

    strftime(nuevo->horaRegistro, sizeof(nuevo->horaRegistro),
             "%d/%m/%Y %H:%M:%S", hora);

    pushCurrent(lista, nuevo);
}


void cambiar_prioridad(List *lista, int id, int nuevaPrioridad) {
    Nodo *paciente = firstList(lista);

    while (paciente != NULL) {
        if (paciente->id == id) {
            paciente->prioridad = nuevaPrioridad;
            printf("Prioridad cambiada correctamente.\n");
            return;
        }

        paciente = nextList(lista);
    }

    printf("No se encontro el paciente.\n");
}


void mostrar_lista_pacientes(List *lista) {
    Nodo *paciente;

    printf("Tickets pendientes:\n");

    paciente = firstList(lista);

    if (paciente == NULL) {
        printf("No hay tickets pendientes.\n");
        return;
    }

    while (paciente != NULL) {
        printf("\nID: %d", paciente->id);
        printf("\nEspecificaciones: %s", paciente->descripcion);
        printf("\nPrioridad: %d", paciente->prioridad);
        printf("\nHora de registro: %s\n", paciente->horaRegistro);

        paciente = nextList(lista);
    }
}


void atender_siguiente(List *lista) {
    Nodo *paciente = firstList(lista);

    if (paciente == NULL) {
        printf("No hay pacientes pendientes.\n");
        return;
    }

    printf("Paciente atendido:\n");
    printf("ID: %d\n", paciente->id);
    printf("Especificaciones: %s\n", paciente->descripcion);
    printf("Prioridad: %d\n", paciente->prioridad);
    printf("Hora de registro: %s\n", paciente->horaRegistro);

    popCurrent(lista);
}


void mostrar_pacientes_por_prioridad(List *lista) {
    Nodo *paciente;

    printf("Pacientes pendientes:\n");

    for (int prioridad = 1; prioridad <= 3; prioridad++) {
        paciente = firstList(lista);

        while (paciente != NULL) {
            if (paciente->prioridad == prioridad) {
                printf("\nID: %d", paciente->id);
                printf("\nEspecificaciones: %s", paciente->descripcion);
                printf("\nPrioridad: %d", paciente->prioridad);
                printf("\nHora de registro: %s\n", paciente->horaRegistro);
            }

            paciente = nextList(lista);
        }
    }
}


int main() {
  char opcion;
  List *pacientes = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {

    case '1': {
      int id;
      char especificaciones[200];

      printf("Ingrese ID del paciente: ");
      scanf("%d", &id);

      printf("Ingrese las especificaciones: ");
      scanf(" %[^\n]", especificaciones);

      registrar_paciente(pacientes, id, especificaciones);

      break;
    }

    case '2': {
      int id;
      int nuevaPrioridad;

      printf("Ingrese el ID del paciente: ");
      scanf("%d", &id);

      printf("Ingrese la nueva prioridad (1-Alta, 2-Media, 3-Baja): ");
      scanf("%d", &nuevaPrioridad);

      cambiar_prioridad(pacientes, id, nuevaPrioridad);

      break;
    }

    case '3':
      mostrar_lista_pacientes(pacientes);
      break;

    case '4':
      atender_siguiente(pacientes);
      break;

    case '5':
      mostrar_pacientes_por_prioridad(pacientes);
      break;

    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;

    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }

    presioneTeclaParaContinuar();

  } while (opcion != '6');

  list_clean(pacientes);

  return 0;
}