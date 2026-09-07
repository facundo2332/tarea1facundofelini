#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char descripcion[200];
    int prioridad;
    char horaRegistro[20];
} Paciente;


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


int existe_paciente(List *lista, int id) {
    Paciente *paciente = firstList(lista);

    while (paciente != NULL) {
        if (paciente->id == id) {
            return 1;
        }

        paciente = nextList(lista);
    }

    return 0;
}


void registrar_paciente(List *lista, int id, char descripcion[]) {
    Paciente *nuevo = malloc(sizeof(Paciente));
    time_t tiempo;
    struct tm *hora;

    if (nuevo == NULL) {
        printf("No se pudo reservar memoria.\n");
        return;
    }

    nuevo->id = id;
    strcpy(nuevo->descripcion, descripcion);
    nuevo->prioridad = 3;

    tiempo = time(NULL);
    hora = localtime(&tiempo);

    strftime(nuevo->horaRegistro, sizeof(nuevo->horaRegistro),
             "%d/%m/%Y %H:%M:%S", hora);

    pushCurrent(lista, nuevo);
}


void cambiar_prioridad(List *lista, int id, int nuevaPrioridad) {
    Paciente *paciente = firstList(lista);

    while (paciente != NULL) {
        if (paciente->id == id) {
            paciente->prioridad = nuevaPrioridad;
            return;
        }

        paciente = nextList(lista);
    }

    printf("No se encontro el paciente.\n");
}


void mostrar_lista_pacientes(List *lista) {
    Paciente *paciente;

    printf("Tickets pendientes:\n");

    paciente = firstList(lista);

    if (paciente == NULL) {
        printf("No hay tickets pendientes.\n");
        return;
    }

    while (paciente != NULL) {
        printf("\nID: %d", paciente->id);
        printf("\nDescripcion: %s", paciente->descripcion);
        printf("\nPrioridad: %d", paciente->prioridad);
        printf("\nHora de registro: %s\n", paciente->horaRegistro);

        paciente = nextList(lista);
    }
}


void atender_siguiente(List *lista) {
    Paciente *paciente = firstList(lista);

    if (paciente == NULL) {
        printf("No hay pacientes pendientes.\n");
        return;
    }

    printf("Paciente atendido:\n");
    printf("ID: %d\n", paciente->id);
    printf("Descripcion: %s\n", paciente->descripcion);
    printf("Prioridad: %d\n", paciente->prioridad);
    printf("Hora de registro: %s\n", paciente->horaRegistro);

    popCurrent(lista);
}


void mostrar_pacientes_por_prioridad(List *lista) {
    Paciente *paciente;

    paciente = firstList(lista);

    if (paciente == NULL) {
        printf("No hay tickets pendientes.\n");
        return;
    }

    for (int prioridad = 1; prioridad <= 3; prioridad++) {

        paciente = firstList(lista);

        while (paciente != NULL) {

            if (paciente->prioridad == prioridad) {
                printf("\nID: %d", paciente->id);
                printf("\nDescripcion: %s", paciente->descripcion);
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

    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion) {

    case '1': {
      int id;
      char descripcion[200];

      printf("Ingrese ID del paciente: ");
      scanf("%d", &id);

      if (existe_paciente(pacientes, id)) {
          printf("Ya existe un paciente con ese ID.\n");
          break;
      }

      getchar();

      printf("Ingrese las especificaciones: ");
      fgets(descripcion, sizeof(descripcion), stdin);

      descripcion[strcspn(descripcion, "\n")] = '\0';

      registrar_paciente(pacientes, id, descripcion);

      break;
    }

    case '2': {
      int id;
      int nuevaPrioridad;

      printf("Ingrese el ID del paciente: ");
      scanf("%d", &id);

      do {
          printf("Ingrese la nueva prioridad (1-Alta, 2-Media, 3-Baja): ");
          scanf("%d", &nuevaPrioridad);

          if (nuevaPrioridad < 1 || nuevaPrioridad > 3) {
              printf("Prioridad invalida. Debe ser entre 1 y 3.\n");
          }

      } while (nuevaPrioridad < 1 || nuevaPrioridad > 3);

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

    if (opcion != '6') {
      presioneTeclaParaContinuar();
    }

  } while (opcion != '6');

  list_clean(pacientes);

  return 0;
}