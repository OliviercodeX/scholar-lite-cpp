#include <iostream>
#include <string> 
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
    string cuartil;                    // 'Q1','Q2','Q3','Q4' -> usar char o string
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
//____________________________________________________________________________________________________________________
//|                                                                                                                   |
//|--------------------------------------------PARTE DE MAINOR------------------------------------------------------ |
//|__________________________________________________________________________________________________________________|
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
//

//---------------------------------------------Areas de investigacion---------------------------------------------

// Busca un area por ID. Devuelve NULL si no existe.
areaInvestigacion* buscarArea(int id) {
    areaInvestigacion* aux = primeraArea;
    while (aux != NULL) {
        if (aux->idArea == id)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

// Inserta al final de la lista simple. Valida y comprueba que el ID sea unico.
// Devuelve true si se inserto, false si fallo una validacion.
bool insertarArea(int id, string nombre, string descripcion) {
    // Validaciones
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }
    if (buscarArea(id) != NULL) {
        cout << "Error: ya existe un area con ese ID." << endl;
        return false;
    }
    if (nombre == "" || descripcion == "") {
        cout << "Error: nombre y descripcion no pueden estar vacios." << endl;
        return false;
    }

    // Crear nodo
    areaInvestigacion* nueva = new areaInvestigacion;
    nueva->idArea = id;
    nueva->nombreArea = nombre;
    nueva->descripcion = descripcion;
    nueva->sig = NULL;

    // Insertar al final
    if (primeraArea == NULL) {
        primeraArea = nueva;
    } else {
        areaInvestigacion* aux = primeraArea;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nueva;
    }
    return true;
}

// Muestra los datos de una sola area
void mostrarArea(areaInvestigacion* area) {
    if (area == NULL) {
        cout << "Area no encontrada." << endl;
        return;
    }
    cout << "ID: " << area->idArea
         << " | Area: " << area->nombreArea
         << " | Descripcion: " << area->descripcion << endl;
}

// Muestra todas las areas
void mostrarAreas() {
    if (primeraArea == NULL) {
        cout << "No hay areas de investigacion registradas." << endl;
        return;
    }
    areaInvestigacion* aux = primeraArea;
    while (aux != NULL) {
        mostrarArea(aux);
        aux = aux->sig;
    }
}

//---------------------------------------------Investigadores---------------------------------------------

// Busca un investigador por ID. Devuelve NULL si no existe.
investigador* buscarInvestigador(int id) {
    investigador* aux = primerInvestigador;
    while (aux != NULL) {
        if (aux->idInvestigador == id)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

// Inserta al final de la lista simple. Valida y comprueba que el ID sea unico.
// Devuelve true si se inserto, false si fallo una validacion.
bool insertarInvestigador(int id, string nombre, universidad* uni, string pais,
                          areaInvestigacion* area, string correo, float indiceH) {
    // Validaciones
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }
    if (buscarInvestigador(id) != NULL) {
        cout << "Error: ya existe un investigador con ese ID." << endl;
        return false;
    }
    if (nombre == "" || pais == "") {
        cout << "Error: nombre y pais no pueden estar vacios." << endl;
        return false;
    }
    if (correo.find('@') == string::npos || correo.find('.') == string::npos) {
        cout << "Error: correo invalido." << endl;
        return false;
    }
    if (uni == NULL) {
        cout << "Error: la universidad no existe." << endl;
        return false;
    }
    if (area == NULL) {
        cout << "Error: el area de investigacion no existe." << endl;
        return false;
    }
    if (indiceH < 0) {
        cout << "Error: el indice H no puede ser negativo." << endl;
        return false;
    }

    // Crear nodo
    investigador* nuevo = new investigador;
    nuevo->idInvestigador = id;
    nuevo->nombreCompleto = nombre;
    nuevo->suUniversidad = uni;
    nuevo->pais = pais;
    nuevo->suArea = area;
    nuevo->correo = correo;
    nuevo->indiceH = indiceH;
    nuevo->coautores = NULL;
    nuevo->publicaciones = NULL;
    nuevo->sig = NULL;

    // Insertar al final
    if (primerInvestigador == NULL) {
        primerInvestigador = nuevo;
    } else {
        investigador* aux = primerInvestigador;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nuevo;
    }
    return true;
}

// Muestra los datos de un solo investigador
void mostrarInvestigador(investigador* inv) {
    if (inv == NULL) {
        cout << "Investigador no encontrado." << endl;
        return;
    }
    cout << "ID: " << inv->idInvestigador
         << " | Nombre: " << inv->nombreCompleto
         << " | Pais: " << inv->pais
         << " | Correo: " << inv->correo
         << " | Indice H: " << inv->indiceH << endl;
    cout << "   Universidad: ";
    if (inv->suUniversidad != NULL) cout << inv->suUniversidad->nombre;
    else cout << "(sin universidad)";
    cout << " | Area: ";
    if (inv->suArea != NULL) cout << inv->suArea->nombreArea;
    else cout << "(sin area)";
    cout << endl;
}

// Muestra todos los investigadores
void mostrarInvestigadores() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    investigador* aux = primerInvestigador;
    while (aux != NULL) {
        mostrarInvestigador(aux);
        aux = aux->sig;
    }
}
//---------------------------------------------Coautores---------------------------------------------

// Busca un coautor por ID dentro de la sublista de un investigador.
// Devuelve NULL si no existe (o si el investigador es NULL).
coautor* buscarCoautor(investigador* inv, int id) {
    if (inv == NULL)
        return NULL;
    coautor* aux = inv->coautores;
    while (aux != NULL) {
        if (aux->idCoautor == id)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

// Revisa si un ID de coautor ya esta usado en la sublista de cualquier investigador
bool existeIdCoautor(int id) {
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        if (buscarCoautor(inv, id) != NULL)
            return true;
        inv = inv->sig;
    }
    return false;
}

// Inserta al final de la sublista doble de un investigador.
// Devuelve true si se inserto, false si fallo una validacion.
bool insertarCoautor(investigador* inv, int id, string nombre, string nombreUni,
                     int publicacionesConjuntas) {
    // Validaciones
    if (inv == NULL) {
        cout << "Error: el investigador no existe." << endl;
        return false;
    }
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }
    if (existeIdCoautor(id)) {
        cout << "Error: ya existe un coautor con ese ID." << endl;
        return false;
    }
    if (nombre == "" || nombreUni == "") {
        cout << "Error: nombre y universidad no pueden estar vacios." << endl;
        return false;
    }
    if (publicacionesConjuntas < 0) {
        cout << "Error: las publicaciones conjuntas no pueden ser negativas." << endl;
        return false;
    }

    // Crear nodo
    coautor* nuevo = new coautor;
    nuevo->idCoautor = id;
    nuevo->nombre = nombre;
    nuevo->universidad = nombreUni;
    nuevo->cantidadPublicacionesConjuntas = publicacionesConjuntas;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    // Insertar al final de la sublista
    if (inv->coautores == NULL) {
        inv->coautores = nuevo;
    } else {
        coautor* aux = inv->coautores;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nuevo;
        nuevo->ant = aux;
    }
    return true;
}

// Muestra los datos de un solo coautor
void mostrarCoautor(coautor* c) {
    if (c == NULL) {
        cout << "Coautor no encontrado." << endl;
        return;
    }
    cout << "ID: " << c->idCoautor
         << " | Nombre: " << c->nombre
         << " | Universidad: " << c->universidad
         << " | Publicaciones conjuntas: " << c->cantidadPublicacionesConjuntas << endl;
}

// Muestra todos los coautores de un investigador
void mostrarCoautores(investigador* inv) {
    if (inv == NULL) {
        cout << "Investigador no encontrado." << endl;
        return;
    }
    if (inv->coautores == NULL) {
        cout << inv->nombreCompleto << " no tiene coautores registrados." << endl;
        return;
    }
    cout << "Coautores de " << inv->nombreCompleto << ":" << endl;
    coautor* aux = inv->coautores;
    while (aux != NULL) {
        mostrarCoautor(aux);
        aux = aux->sig;
    }
}
//
// -----------------------------------------------------------------------------------------------------------------
//---------------------------------------------Parte de Gerald - Revistas---------------------------------------------

revista* buscarRevista(int id) {
    revista* aux = primeraRevista;

    while (aux != NULL) {
        if (aux->idRevista == id) {
            return aux;
        }

        aux = aux->sig;
    }

    return NULL;
}


bool insertarRevista(int id, string nombre, string editorial,
                     string pais, float factorImpacto, string cuartil) {

    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }

    if (buscarRevista(id) != NULL) {
        cout << "Error: ya existe una revista con ese ID." << endl;
        return false;
    }

    if (nombre == "" || editorial == "" || pais == "") {
        cout << "Error: nombre, editorial y pais no pueden estar vacios." << endl;
        return false;
    }

    if (factorImpacto < 0) {
        cout << "Error: el factor de impacto no puede ser negativo." << endl;
        return false;
    }

    if (cuartil != "Q1" &&
        cuartil != "Q2" &&
        cuartil != "Q3" &&
        cuartil != "Q4") {

        cout << "Error: el cuartil debe ser Q1, Q2, Q3 o Q4." << endl;
        return false;
    }

    revista* nueva = new revista;

    nueva->idRevista = id;
    nueva->nombre = nombre;
    nueva->editorial = editorial;
    nueva->pais = pais;
    nueva->factorImpacto = factorImpacto;
    nueva->cuartil = cuartil;
    nueva->publicaciones = NULL;
    nueva->sig = NULL;

    if (primeraRevista == NULL) {
        primeraRevista = nueva;
        return true;
    }

    if (nombre < primeraRevista->nombre) {
        nueva->sig = primeraRevista;
        primeraRevista = nueva;
        return true;
    }

    revista* aux = primeraRevista;

    while (aux->sig != NULL &&
           aux->sig->nombre < nombre) {

        aux = aux->sig;
    }

    nueva->sig = aux->sig;
    aux->sig = nueva;

    return true;
}


void mostrarRevista(revista* rev) {
    if (rev == NULL) {
        cout << "Revista no encontrada." << endl;
        return;
    }

    cout << "ID: " << rev->idRevista
         << " | Nombre: " << rev->nombre
         << " | Editorial: " << rev->editorial
         << " | Pais: " << rev->pais
         << " | Factor de impacto: " << rev->factorImpacto
         << " | Cuartil: " << rev->cuartil
         << endl;
}


void mostrarRevistas() {
    if (primeraRevista == NULL) {
        cout << "No hay revistas registradas." << endl;
        return;
    }

    revista* aux = primeraRevista;

    while (aux != NULL) {
        mostrarRevista(aux);
        aux = aux->sig;
    }

}
//----------------------------------------------------------------------------------------------------------------------


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------------------------------------------------

// Funciones auxiliares para ordenar la salida de las pruebas
void titulo(string texto) {
    cout << endl << "========== " << texto << " ==========" << endl;
}

void prueba(string texto) {
    cout << "-> " << texto << endl;}

int main() {
    // Al arrancar, todas las listas principales estan vacias
    primerInvestigador = NULL;
    primeraRevista = NULL;
    primeraArea = NULL;
    primeraPublicacion = NULL;
    primerProyecto = NULL;
    primeraUniversidad = NULL;

    cout << "|->->->->->->->->Sistema de Gestion de Produccion Cientifica<-<-<-<-<-<-<-<-<-" << endl;

    // ---------------- UNIVERSIDADES ----------------
    titulo("PRUEBA DE UNIVERSIDADES");
    prueba("Insertar 4 validas");
    insertarUniversidad(1, "ITCR", "Costa Rica", 15);
    insertarUniversidad(2, "UCR", "Costa Rica", 10);
    insertarUniversidad(3, "UNAM", "Mexico", 5);
    insertarUniversidad(4, "Harvard", "Estados Unidos", 1);
    prueba("ID repetido (debe dar error)");
    insertarUniversidad(1, "Repetida", "Peru", 3);
    prueba("Nombre vacio (debe dar error)");
    insertarUniversidad(5, "", "Mexico", 5);
    prueba("Ranking invalido (debe dar error)");
    insertarUniversidad(5, "UNA", "Costa Rica", 0);
    prueba("Lista actual");
    mostrarUniversidades();
    prueba("Buscar ID 3 y buscar ID 99 (no existe)");
    mostrarUniversidad(buscarUniversidad(3));
    mostrarUniversidad(buscarUniversidad(99));
    prueba("Modificar ID 2");
    modificarUniversidad(2, "UCR Sede Central", "Costa Rica", 9);
    mostrarUniversidad(buscarUniversidad(2));
    prueba("Modificar ID 99 (debe dar error)");
    modificarUniversidad(99, "X", "Y", 1);
    prueba("Eliminar el primero (ID 1)");
    eliminarUniversidad(1);
    prueba("Eliminar ID 99 (debe dar error)");
    eliminarUniversidad(99);
    mostrarUniversidades();

    // ---------------- AREAS ----------------
    titulo("PRUEBA DE AREAS");
    prueba("Insertar 2 validas");
    insertarArea(1, "Ciberseguridad", "Proteccion de sistemas y datos");
    insertarArea(2, "Inteligencia Artificial", "Aprendizaje automatico y agentes");
    prueba("ID repetido (debe dar error)");
    insertarArea(1, "Repetida", "Prueba de ID repetido");
    prueba("Nombre vacio (debe dar error)");
    insertarArea(3, "", "Prueba de nombre vacio");
    prueba("Lista actual");
    mostrarAreas();
    prueba("Buscar ID 2 y buscar ID 99 (no existe)");
    mostrarArea(buscarArea(2));
    mostrarArea(buscarArea(99));

    // ---------------- INVESTIGADORES ----------------
    titulo("PRUEBA DE INVESTIGADORES");
    prueba("Insertar 2 validos");
    insertarInvestigador(1, "Ana Mora", buscarUniversidad(2), "Costa Rica", buscarArea(1), "ana@tec.ac.cr", 0);
    insertarInvestigador(2, "Luis Rojas", buscarUniversidad(4), "Mexico", buscarArea(2), "luis@unam.mx", 0);
    prueba("ID repetido (debe dar error)");
    insertarInvestigador(1, "Repetido", buscarUniversidad(2), "Peru", buscarArea(1), "r@x.com", 0);
    prueba("Universidad que no existe (debe dar error)");
    insertarInvestigador(3, "Sin Uni", buscarUniversidad(99), "Peru", buscarArea(1), "s@x.com", 0);
    prueba("Area que no existe (debe dar error)");
    insertarInvestigador(3, "Sin Area", buscarUniversidad(2), "Peru", buscarArea(99), "s@x.com", 0);
    prueba("Correo invalido (debe dar error)");
    insertarInvestigador(3, "Mal Correo", buscarUniversidad(2), "Peru", buscarArea(1), "correo.com", 0);
    prueba("Lista actual");
    mostrarInvestigadores();
    prueba("Buscar ID 99 (no existe)");
    mostrarInvestigador(buscarInvestigador(99));

    // ---------------- COAUTORES ----------------
    titulo("PRUEBA DE COAUTORES");
    prueba("Insertar 2 validos al investigador 1 y 1 al investigador 2");
    insertarCoautor(buscarInvestigador(1), 1, "Pedro Soto", "UCR", 3);
    insertarCoautor(buscarInvestigador(1), 2, "Marta Diaz", "UNAM", 5);
    insertarCoautor(buscarInvestigador(2), 3, "Raul Vega", "MIT", 1);
    prueba("ID repetido en el mismo investigador (debe dar error)");
    insertarCoautor(buscarInvestigador(1), 2, "Repetido", "UCR", 1);
    prueba("ID repetido en otro investigador (debe dar error)");
    insertarCoautor(buscarInvestigador(2), 1, "Repetido otro", "UCR", 1);
    prueba("Investigador que no existe (debe dar error)");
    insertarCoautor(buscarInvestigador(99), 4, "X", "UCR", 1);
    prueba("Coautores del investigador 1 y del 2");
    mostrarCoautores(buscarInvestigador(1));
    mostrarCoautores(buscarInvestigador(2));
    prueba("Buscar coautor 2 del investigador 1, y coautor 3 en el investigador 1 (no esta ahi)");
    mostrarCoautor(buscarCoautor(buscarInvestigador(1), 2));
    mostrarCoautor(buscarCoautor(buscarInvestigador(1), 3));

    // ---------------- ELIMINAR CON DEPENDENCIAS ----------------
    titulo("PRUEBA DE ELIMINAR CON INVESTIGADORES ASOCIADOS");
    prueba("Eliminar UCR (ID 2), a la que pertenece Ana (debe dar error)");
    eliminarUniversidad(2);
    mostrarUniversidades();
    
      mostrarArea(buscarArea(99));   
    
    cout << endl;
    cout << "Prueba de revistas" << endl;

    insertarRevista(1, "Nature", "Springer Nature",
                    "Reino Unido", 64.8, "Q1");

    insertarRevista(2, "ACM Computing Surveys", "ACM",
                    "Estados Unidos", 16.6, "Q1");

    insertarRevista(3, "IEEE Access", "IEEE",
                    "Estados Unidos", 3.9, "Q1");

    mostrarRevistas();

    return 0;
}