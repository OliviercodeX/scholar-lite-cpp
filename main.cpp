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
struct pubInvestigador;
struct pubRevista;
struct coautorPublicacion;
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
struct pubInvestigador {
    publicacion* laPublicacion;
    pubInvestigador* sig;
    pubInvestigador* ant;
};

struct pubRevista {
    publicacion* laPublicacion;
    pubRevista* sig;
    pubRevista* ant;
};

struct coautorPublicacion {
    coautor* elCoautor;
    coautorPublicacion* sig;
    coautorPublicacion* ant;
};


//---------------------------------------------Parte de Mainor------------------------------------------------------
//insertar universidad
// Busca una universidad por ID. Devuelve NULL si no existe.
universidad* buscarUniversidad(int id) {
    universidad* aux = primeraUniversidad;
    while (aux != NULL) {
        if (aux->idUniversidad == id)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

// Inserta al final de la lista doble. Valida y comprueba que el ID sea unico.
// Devuelve true si se inserto, false si fallo una validacion.
bool insertarUniversidad(int id, string nombre, string pais, int ranking) {
    // Validaciones
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }
    if (buscarUniversidad(id) != NULL) {
        cout << "Error: ya existe una universidad con ese ID." << endl;
        return false;
    }
    if (nombre == "" || pais == "") {
        cout << "Error: nombre y pais no pueden estar vacios." << endl;
        return false;
    }
    if (ranking <= 0) {
        cout << "Error: el ranking debe ser mayor que 0." << endl;
        return false;
    }

    // Crear nodo
    universidad* nueva = new universidad;
    nueva->idUniversidad = id;
    nueva->nombre = nombre;
    nueva->pais = pais;
    nueva->ranking = ranking;
    nueva->sig = NULL;
    nueva->ant = NULL;

    // Insertar al final
    if (primeraUniversidad == NULL) {
        primeraUniversidad = nueva;
    } else {
        universidad* aux = primeraUniversidad;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nueva;
        nueva->ant = aux;    // enlace hacia atras, lo que la hace doble
    }
    return true;
}



// Muestra una sola (útil para reportes)
// Muestra los datos de una sola universidad
void mostrarUniversidad(universidad* uni) {
    if (uni == NULL) {
        cout << "Universidad no encontrada." << endl;
        return;
    }
    cout << "ID: " << uni->idUniversidad
         << " | Nombre: " << uni->nombre
         << " | Pais: " << uni->pais
         << " | Ranking: " << uni->ranking << endl;
}
// Muestra todas las universidades (para probar)
void mostrarUniversidades() {
    if (primeraUniversidad == NULL) {
        cout << "No hay universidades registradas." << endl;
        return;
    }
    universidad* aux = primeraUniversidad;
    while (aux != NULL) {
        mostrarUniversidad(aux);
        aux = aux->sig;
    }
}
// Valida los datos de una universidad (se reutiliza al insertar y modificar)
bool validarDatosUniversidad(string nombre, string pais, int ranking) {
    if (nombre == "" || pais == "") {
        cout << "Error: nombre y pais no pueden estar vacios." << endl;
        return false;
    }
    if (ranking <= 0) {
        cout << "Error: el ranking debe ser mayor que 0." << endl;
        return false;
    }
    return true;
}

// Modifica nombre, pais y ranking de una universidad
bool modificarUniversidad(int id, string nombre, string pais, int ranking) {
    universidad* uni = buscarUniversidad(id);
    if (uni == NULL) {
        cout << "Error: no existe una universidad con ese ID." << endl;
        return false;
    }
    if (!validarDatosUniversidad(nombre, pais, ranking))
        return false;

    uni->nombre = nombre;
    uni->pais = pais;
    uni->ranking = ranking;
    return true;
}

// Elimina una universidad por ID (lista doble)
bool eliminarUniversidad(int id) {
    universidad* uni = buscarUniversidad(id);
    if (uni == NULL) {
        cout << "Error: no existe una universidad con ese ID." << endl;
        return false;
    }

    // No eliminar si algun investigador pertenece a ella (evita punteros colgantes)
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        if (inv->suUniversidad == uni) {
            cout << "Error: hay investigadores asociados a esta universidad." << endl;
            return false;
        }
        inv = inv->sig;
    }

    // Reconectar vecinos
    if (uni->ant == NULL)                 // es el primero
        primeraUniversidad = uni->sig;
    else
        uni->ant->sig = uni->sig;

    if (uni->sig != NULL)                 // no es el ultimo
        uni->sig->ant = uni->ant;

    delete uni;
    return true;
}

// -----------------------------------------------------------------------------------------------------------------


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
    
    return 0;
}