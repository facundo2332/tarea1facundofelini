# Sistema de Gestión Hospitalaria (Tarea)

Aplicación de consola en C para registrar pacientes (tickets), asignar prioridad y gestionar una lista de espera.

## Requisitos

- Compilador C (ej: `gcc`)
- Archivos del proyecto:
  - `main.c` (o el nombre que uses para el archivo con `main`)
  - `tdas/list.h` y su implementación correspondiente
  - `tdas/extra.h` y su implementación correspondiente

> Nota: este proyecto depende de las TDAs provistas por la cátedra (list/extra).

---

## Cómo compilar

Ejemplo (ajusta rutas/nombres según tu estructura real):

```bash
gcc -Wall -Wextra -Werror -o hospital main.c tdas/list.c tdas/extra.c
```

Si tus TDAs vienen como `.c` separados, inclúyelos en la compilación.  
Si vienen como biblioteca o se compilan distinto, adapta el comando.

---

## Cómo ejecutar

```bash
./hospital
```

---

## Opciones del menú: estado de funcionamiento

El menú presenta:

1) Registrar paciente  
2) Asignar prioridad a paciente  
3) Mostrar lista de espera  
4) Atender al siguiente paciente  
5) Mostrar pacientes por prioridad  
6) Salir  

### Funciona correctamente (con observaciones)

- **(1) Registrar paciente**
  - Registra un paciente con:
    - `id`
    - `descripcion`
    - prioridad inicial **3 (baja)** por defecto
    - fecha/hora de registro usando `time()` y `strftime()`
  - Valida que el `id` no exista mediante `existe_paciente`.

- **(2) Asignar prioridad a paciente**
  - Permite cambiar la prioridad a 1, 2 o 3.
  - Valida rango correcto de prioridad.
  - Si el ID no existe, muestra: `No se encontro el paciente.`

- **(3) Mostrar lista de espera**
  - Recorre e imprime los pacientes en el orden actual de la lista.

- **(5) Mostrar pacientes por prioridad**
  - Imprime pacientes agrupados por prioridad (1 a 3).
  - Ojo: **no reordena** la lista; solo filtra/imprime.

### NO funciona correctamente / problemas detectados

- **(4) Atender al siguiente paciente (problema de lógica de prioridad)**
  - Actualmente atiende **siempre al primer elemento de la lista** (`firstList(lista)`), no necesariamente al de mayor prioridad (prioridad 1).
  - Causa: no existe una estructura de cola por prioridad ni búsqueda del “mejor” paciente antes de atender.

- **Gestión de memoria / limpieza (bug importante)**
  - En el `main`, `list_clean(pacientes);` está dentro del `do { ... }` (según el código pegado).  
    Esto provoca que la lista se limpie **en cada iteración** del menú y se pierdan los pacientes registrados.
  - Lo correcto es llamar a `list_clean(pacientes);` **una sola vez al salir** (después del bucle), o implementar una limpieza final adecuada.

- **Registro/orden de la lista**
  - En `registrar_paciente` se usa `pushCurrent(lista, nuevo);`.
  - Dependiendo de la implementación de la TDA, esto podría insertar “en la posición actual” (que suele ser al inicio o donde quedó el cursor).
  - Si se busca comportamiento tipo “cola” (FIFO), normalmente se esperaría insertar al final (por ejemplo, `pushBack` si existiera). Si no existe, se debe simular.

---

## Ejemplo de interacción

A continuación un ejemplo típico de uso:

```
========================================
     Sistema de Gestión Hospitalaria
========================================
1) Registrar paciente
2) Asignar prioridad a paciente
3) Mostrar lista de espera
4) Atender al siguiente paciente
5) Mostrar pacientes por prioridad
6) Salir
Ingrese su opcion: 1
Ingrese ID del paciente: 10
Ingrese las especificaciones: Dolor abdominal intenso

(Se registra con prioridad 3)

Ingrese su opcion: 1
Ingrese ID del paciente: 22
Ingrese las especificaciones: Dificultad respiratoria

Ingrese su opcion: 2
Ingrese el ID del paciente: 22
Ingrese la nueva prioridad (1-Alta, 2-Media, 3-Baja): 1

Ingrese su opcion: 3
Tickets pendientes:

ID: 10
Descripcion: Dolor abdominal intenso
Prioridad: 3
Hora de registro: 06/09/2026 23:10:01

ID: 22
Descripcion: Dificultad respiratoria
Prioridad: 1
Hora de registro: 06/09/2026 23:12:15

Ingrese su opcion: 5
ID: 22
Descripcion: Dificultad respiratoria
Prioridad: 1
Hora de registro: 06/09/2026 23:12:15

ID: 10
Descripcion: Dolor abdominal intenso
Prioridad: 3
Hora de registro: 06/09/2026 23:10:01

Ingrese su opcion: 4
Paciente atendido:
ID: 10
Descripcion: Dolor abdominal intenso
Prioridad: 3
Hora de registro: 06/09/2026 23:10:01
```

> Observación del ejemplo: aunque el paciente 22 tiene prioridad 1, el sistema atiende al primero en la lista (ID 10). Esto corresponde a la limitación descrita en la sección de problemas.

---

## Notas finales

- El proyecto cumple el flujo básico de registro/consulta/modificación.
- Para cumplir estrictamente “atender por prioridad”, se debe cambiar la estrategia de almacenamiento (por ejemplo, 3 listas/colas: alta, media, baja) o buscar el paciente con menor valor de prioridad (1 es más urgente) antes de hacer `popCurrent`.