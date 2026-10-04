#include <iostream>
using namespace std;
//Mainor Olivier Martinez Sanchez
// Gerald Andres Soto Esquivel

/*-------------------Para buscar por id estos serán los nombres de las funciones-----------

// ===== FIRMAS ACORDADAS =====
// A:
investigador* buscarInvestigador(int id);
universidad* buscarUniversidad(int id);
areaInvestigacion* buscarArea(int id);
coautor* buscarCoautor(investigador* inv, int id);
// B:
revista* buscarRevista(int id);
publicacion* buscarPublicacion(int id);
proyecto* buscarProyecto(int id);


*/


// ============================================================
//  DECLARACIONES ADELANTADAS
//  (se necesitan porque las estructuras se apuntan entre si)
// ============================================================
struct investigador;
struct revista;
struct areaInvestigacion;
struct publicacion;
struct coautor;
struct citacion;
struct proyecto;
struct universidad;
//dd

// ============================================================
//  LISTAS SIMPLES 
// ============================================================

// 1. Lista de Investigadores -> insercion: al final
struct investigador {
    int idInvestigador;
    string nombreCompleto;
    universidad* suUniversidad;      // enlace a la universidad
    string pais;
    areaInvestigacion* suArea;       // enlace al area de investigacion
    string correo;
    float indiceH;
    coautor* coautores;              // sublista (doble) de coautores
    pubInvestigador* publicaciones;

    investigador* sig;
}*primerInvestigador;

// 2. Lista de Revistas Cientificas -> insercion: ordenada por nombre
struct revista {
    int idRevista;
    string nombre;
    string editorial;
    string pais;
    float factorImpacto;
    char cuartil;                    // 'Q1','Q2','Q3','Q4' -> usar char o string
    pubRevista* publicaciones;

    revista* sig;
}*primeraRevista;

// 3. Lista de Areas de Investigacion -> insercion: al final
struct areaInvestigacion {
    int idArea;
    string nombreArea;
    string descripcion;

    areaInvestigacion* sig;
}*primeraArea;


// ============================================================
//  LISTA CIRCULAR 
// ============================================================

// 4. Lista de Publicaciones -> insercion: ordenada por anio
//    circular: el sig del ultimo nodo apunta de vuelta al primero
struct publicacion {
    int idPublicacion;
    string titulo;
    int anio;
    string tipo;                     // "Articulo", "Libro", "Conferencia"
    int cantidadCitas;
    string doi;
    investigador* investigadorPrincipal; // enlace al investigador principal
    revista* suRevista;               // enlace a revista
    proyecto* suProyecto;             // enlace a proyecto
    citacion* citas;                  // sublista (doble) de citaciones
    coautorPublicacion* coautores;

    publicacion* sig;
}*primeraPublicacion;


// ============================================================
//  LISTAS DOBLES 
// ============================================================

// 5. Lista de Coautores -> sublista de cada investigador, insercion: como guste
struct coautor {
    int idCoautor;
    string nombre;
    string universidad;               // nombre de la universidad (dato simple, no enlace)
    int cantidadPublicacionesConjuntas;

    coautor* sig;
    coautor* ant;
};
// No lleva "primero" global: cada investigador tiene su propio
// puntero investigador->coautores como cabeza de su sublista.

// 6. Lista de Citaciones -> sublista de cada publicacion, insercion: como guste
struct citacion {
    int idCita;
    int anio;
    publicacion* publicacionCitante;  // enlace a la publicacion que cita
    investigador* autorCitante;       // enlace al autor citante

    citacion* sig;
    citacion* ant;
};
// Tampoco lleva "primero" global, por la misma razon que coautor:
// cada publicacion tiene su propio puntero publicacion->citas.

// 7. Lista de Proyectos de Investigacion -> insercion: ordenada por anio de inicio
struct proyecto {
    int idProyecto;
    string nombre;
    float financiamiento;
    int anioInicio;
    int anioFinalizacion;
    investigador* investigadorResponsable; // enlace al investigador responsable

    proyecto* sig;
    proyecto* ant;
}*primerProyecto;

// 8. Lista de Universidades -> insercion: como guste
struct universidad {
    int idUniversidad;
    string nombre;
    string pais;
    int ranking;

    universidad* sig;
    universidad* ant;
}*primeraUniversidad;


int main() {
    // Al arrancar, todas las listas principales estan vacias
    primerInvestigador = NULL;
    primeraRevista = NULL;
    primeraArea = NULL;
    primeraPublicacion = NULL;
    primerProyecto = NULL;
    primeraUniversidad = NULL;

    cout << "|->->->->->->->->Sistema de Gestion de Produccion Cientifica<-<-<-<-<-<-<-<-<-" << endl;
    insertarUniversidad(1, "ITCR", "Costa Rica", 15);
    insertarUniversidad(2, "UCR", "Costa Rica", 10);
    insertarUniversidad(4, "Harvard", "Estados Unidos", 1);
    insertarUniversidad(1, "Repetida", "Peru", 3);   // debe dar error de ID repetido
    insertarUniversidad(3, "", "Mexico", 5);          // debe dar error de nombre vacio
    mostrarUniversidades();
    eliminarUniversidad(1);       // primero
    eliminarUniversidad(99);      // error: no existe
    mostrarUniversidades();


    cout << "Prueba de areas" << endl;
    insertarArea(1, "Ciberseguridad", "Proteccion de sistemas y datos");
    insertarArea(2, "Inteligencia Artificial", "Aprendizaje automatico y agentes");
    insertarArea(1, "Repetida", "Prueba de ID repetido");   // error: ID repetido
    insertarArea(3, "", "Prueba de nombre vacio");           // error: nombre vacio
    mostrarAreas();
    mostrarArea(buscarArea(2));
    mostrarArea(buscarArea(99));    
    
    return 0;
}