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

// Valida los datos de un investigador (se reutiliza al modificar)
bool validarDatosInvestigador(string nombre, string pais, string correo,
                              universidad* uni, areaInvestigacion* area) {
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
    return true;
}

// Modifica nombre, universidad, pais, area y correo de un investigador.
// Devuelve true si se modifico, false si fallo una validacion.
bool modificarInvestigador(int id, string nombre, universidad* uni, string pais,
                           areaInvestigacion* area, string correo) {
    investigador* inv = buscarInvestigador(id);
    if (inv == NULL) {
        cout << "Error: no existe un investigador con ese ID." << endl;
        return false;
    }
    if (!validarDatosInvestigador(nombre, pais, correo, uni, area))
        return false;

    inv->nombreCompleto = nombre;
    inv->suUniversidad = uni;
    inv->pais = pais;
    inv->suArea = area;
    inv->correo = correo;
    return true;
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

//---------------------------------------------Eliminar investigador---------------------------------------------

// Revisa si otra estructura todavia apunta al investigador.
// Devuelve true (y avisa por pantalla) si lo encuentra.
bool investigadorTieneReferencias(investigador* inv) {
    // Proyectos que lo tienen como responsable
    proyecto* proy = primerProyecto;
    while (proy != NULL) {
        if (proy->investigadorResponsable == inv) {
            cout << "Error: el investigador es responsable de un proyecto." << endl;
            return true;
        }
        proy = proy->sig;
    }

    // Publicaciones (lista circular), sus citaciones y sus coautores
    if (primeraPublicacion != NULL) {
        publicacion* pub = primeraPublicacion;
        do {
            if (pub->investigadorPrincipal == inv) {
                cout << "Error: el investigador es el principal de una publicacion." << endl;
                return true;
            }
            citacion* cit = pub->citas;
            while (cit != NULL) {
                if (cit->autorCitante == inv) {
                    cout << "Error: el investigador es autor citante en una citacion." << endl;
                    return true;
                }
                cit = cit->sig;
            }
            coautorPublicacion* cp = pub->coautores;
            while (cp != NULL) {
                if (cp->elCoautor != NULL &&
                    buscarCoautor(inv, cp->elCoautor->idCoautor) == cp->elCoautor) {
                    cout << "Error: una publicacion usa coautores de este investigador." << endl;
                    return true;
                }
                cp = cp->sig;
            }
            pub = pub->sig;
        } while (pub != NULL && pub != primeraPublicacion);
    }
    return false;
}

// Elimina un investigador por ID (lista simple).
// Devuelve true si se elimino, false si no existe o si todavia esta referenciado.
bool eliminarInvestigador(int id) {
    investigador* inv = buscarInvestigador(id);
    if (inv == NULL) {
        cout << "Error: no existe un investigador con ese ID." << endl;
        return false;
    }

    // No eliminar si otra estructura todavia apunta a el (evita punteros colgantes)
    if (investigadorTieneReferencias(inv))
        return false;

    // Desenlazar de la lista simple
    if (primerInvestigador == inv) {          // es el primero
        primerInvestigador = inv->sig;
    } else {
        investigador* ant = primerInvestigador;
        while (ant->sig != inv)
            ant = ant->sig;
        ant->sig = inv->sig;
    }

    // Liberar su sublista de coautores
    coautor* c = inv->coautores;
    while (c != NULL) {
        coautor* siguiente = c->sig;
        delete c;
        c = siguiente;
    }

    // Liberar los nodos de enlace de su sublista de publicaciones
    // (solo los nodos de enlace, las publicaciones no se borran)
    pubInvestigador* p = inv->publicaciones;
    while (p != NULL) {
        pubInvestigador* siguiente = p->sig;
        delete p;
        p = siguiente;
    }

    delete inv;
    return true;
}

//---------------------------------------------Sublista pubInvestigador---------------------------------------------

// Busca una publicacion por ID dentro de la sublista de un investigador.
// Devuelve el puntero a la publicacion, o NULL si no la tiene (o si inv es NULL).
publicacion* buscarPubInvestigador(investigador* inv, int idPublicacion) {
    if (inv == NULL)
        return NULL;
    pubInvestigador* aux = inv->publicaciones;
    while (aux != NULL) {
        if (aux->laPublicacion->idPublicacion == idPublicacion)
            return aux->laPublicacion;
        aux = aux->sig;
    }
    return NULL;
}

// Agrega una publicacion a la sublista doble de un investigador (al final).
// Es la funcion que llama B desde insertarPublicacion.
// Devuelve true si se agrego, false si fallo una validacion.
bool agregarPublicacionAInvestigador(investigador* inv, publicacion* pub) {
    // Validaciones
    if (inv == NULL) {
        cout << "Error: el investigador no existe." << endl;
        return false;
    }
    if (pub == NULL) {
        cout << "Error: la publicacion no existe." << endl;
        return false;
    }
    if (buscarPubInvestigador(inv, pub->idPublicacion) != NULL) {
        cout << "Error: el investigador ya tiene esa publicacion." << endl;
        return false;
    }

    // Crear nodo de enlace (solo guarda el puntero, no copia datos)
    pubInvestigador* nuevo = new pubInvestigador;
    nuevo->laPublicacion = pub;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    // Insertar al final de la sublista
    if (inv->publicaciones == NULL) {
        inv->publicaciones = nuevo;
    } else {
        pubInvestigador* aux = inv->publicaciones;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nuevo;
        nuevo->ant = aux;
    }
    return true;
}

// Muestra todas las publicaciones de un investigador
void mostrarPublicacionesInvestigador(investigador* inv) {
    if (inv == NULL) {
        cout << "Investigador no encontrado." << endl;
        return;
    }
    if (inv->publicaciones == NULL) {
        cout << inv->nombreCompleto << " no tiene publicaciones registradas." << endl;
        return;
    }
    cout << "Publicaciones de " << inv->nombreCompleto << ":" << endl;
    pubInvestigador* aux = inv->publicaciones;
    while (aux != NULL) {
        publicacion* p = aux->laPublicacion;
        cout << "ID: " << p->idPublicacion
             << " | Titulo: " << p->titulo
             << " | Anio: " << p->anio
             << " | Tipo: " << p->tipo
             << " | Citas: " << p->cantidadCitas << endl;
        aux = aux->sig;
    }
}

//---------------------------------------------Metricas academicas---------------------------------------------

// Cantidad de publicaciones de un investigador (produccion cientifica)
int contarPublicaciones(investigador* inv) {
    int total = 0;
    pubInvestigador* p = inv->publicaciones;
    while (p != NULL) {
        total++;
        p = p->sig;
    }
    return total;
}

// Suma de las citas de todas sus publicaciones
int calcularTotalCitas(investigador* inv) {
    int total = 0;
    pubInvestigador* p = inv->publicaciones;
    while (p != NULL) {
        total += p->laPublicacion->cantidadCitas;
        p = p->sig;
    }
    return total;
}

// Cantidad de coautores de su sublista (colaboracion cientifica)
int contarCoautores(investigador* inv) {
    int total = 0;
    coautor* c = inv->coautores;
    while (c != NULL) {
        total++;
        c = c->sig;
    }
    return total;
}

// Indice H: el mayor k tal que hay k publicaciones con al menos k citas
int calcularIndiceH(investigador* inv) {
    int totalPub = contarPublicaciones(inv);
    int h = 0;
    for (int k = 1; k <= totalPub; k++) {
        int cuantas = 0;
        pubInvestigador* p = inv->publicaciones;
        while (p != NULL) {
            if (p->laPublicacion->cantidadCitas >= k)
                cuantas++;
            p = p->sig;
        }
        if (cuantas >= k)
            h = k;
    }
    return h;
}

// Guarda en cada investigador su indice H calculado (para consultas y reportes)
void actualizarTodosLosIndicesH() {
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        inv->indiceH = calcularIndiceH(inv);
        inv = inv->sig;
    }
}

// Muestra las 5 metricas de un investigador
void mostrarMetricas(investigador* inv) {
    if (inv == NULL) {
        cout << "Investigador no encontrado." << endl;
        return;
    }
    int totalPub = contarPublicaciones(inv);
    int totalCitas = calcularTotalCitas(inv);
    float promedio = 0;
    if (totalPub > 0)                       // evita dividir entre cero
        promedio = (float)totalCitas / totalPub;

    cout << "Metricas de " << inv->nombreCompleto << endl;
    cout << "   Indice H: " << calcularIndiceH(inv) << endl;
    cout << "   Total de citas: " << totalCitas << endl;
    cout << "   Promedio de citas por publicacion: " << promedio << endl;
    cout << "   Coautores distintos: " << contarCoautores(inv) << endl;
    cout << "   Produccion cientifica: " << totalPub << " publicaciones" << endl;
}


//---------------------------------------------Consultas de A---------------------------------------------

// Consulta 1: investigador(es) con el mayor indice H (muestra todos los empates)
void consultaMayorIndiceH() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    actualizarTodosLosIndicesH();   // para que el indice H mostrado sea el real

    // Pasada 1: encontrar el mayor indice H
    int mayor = -1;
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        int h = calcularIndiceH(inv);
        if (h > mayor)
            mayor = h;
        inv = inv->sig;
    }

    // Pasada 2: mostrar a todos los que lo tienen
    cout << "Mayor indice H: " << mayor << endl;
    inv = primerInvestigador;
    while (inv != NULL) {
        if (calcularIndiceH(inv) == mayor)
            mostrarInvestigador(inv);
        inv = inv->sig;
    }
}



// Consulta 2: investigador(es) con mas citas acumuladas (muestra todos los empates)
void consultaMasCitas() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    actualizarTodosLosIndicesH();   // para que el indice H mostrado sea el real

    // Pasada 1: encontrar el mayor total de citas
    int mayor = -1;
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        int total = calcularTotalCitas(inv);
        if (total > mayor)
            mayor = total;
        inv = inv->sig;
    }

    // Pasada 2: mostrar a todos los que lo tienen
    cout << "Mayor total de citas acumuladas: " << mayor << endl;
    inv = primerInvestigador;
    while (inv != NULL) {
        if (calcularTotalCitas(inv) == mayor)
            mostrarInvestigador(inv);
        inv = inv->sig;
    }
}


// Consulta 5: investigador(es) con mas coautores (muestra todos los empates)
void consultaMasCoautores() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    actualizarTodosLosIndicesH();   // para que el indice H mostrado sea el real

    // Pasada 1: encontrar la mayor cantidad de coautores
    int mayor = -1;
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        int cantidad = contarCoautores(inv);
        if (cantidad > mayor)
            mayor = cantidad;
        inv = inv->sig;
    }

    // Pasada 2: mostrar a todos los que la tienen
    cout << "Mayor cantidad de coautores: " << mayor << endl;
    inv = primerInvestigador;
    while (inv != NULL) {
        if (contarCoautores(inv) == mayor)
            mostrarInvestigador(inv);
        inv = inv->sig;
    }
}

// Cuenta cuantos investigadores pertenecen a una universidad
int contarInvestigadoresDeUniversidad(universidad* uni) {
    int total = 0;
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        if (inv->suUniversidad == uni)
            total++;
        inv = inv->sig;
    }
    return total;
}

// Consulta 6: universidad(es) con mas investigadores registrados (muestra todos los empates)
void consultaUniversidadMasInvestigadores() {
    if (primeraUniversidad == NULL) {
        cout << "No hay universidades registradas." << endl;
        return;
    }

    // Pasada 1: encontrar la mayor cantidad de investigadores
    int mayor = -1;
    universidad* uni = primeraUniversidad;
    while (uni != NULL) {
        int cantidad = contarInvestigadoresDeUniversidad(uni);
        if (cantidad > mayor)
            mayor = cantidad;
        uni = uni->sig;
    }

    // Pasada 2: mostrar todas las universidades que la tienen
    cout << "Mayor cantidad de investigadores registrados: " << mayor << endl;
    uni = primeraUniversidad;
    while (uni != NULL) {
        if (contarInvestigadoresDeUniversidad(uni) == mayor)
            mostrarUniversidad(uni);
        uni = uni->sig;
    }
}

//---------------------------------------------Reportes de A---------------------------------------------

// Reporte 1: todos los investigadores, cada uno con sus publicaciones
void reporteInvestigadoresConPublicaciones() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    actualizarTodosLosIndicesH();   // para que el indice H mostrado sea el real
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        mostrarInvestigador(inv);
        mostrarPublicacionesInvestigador(inv);
        cout << endl;
        inv = inv->sig;
    }
}

// Reporte 5: red de coautoria de un investigador especifico
void reporteRedCoautoria(int idInvestigador) {
    investigador* inv = buscarInvestigador(idInvestigador);
    if (inv == NULL) {
        cout << "Investigador no encontrado." << endl;
        return;
    }
    cout << "Red de coautoria de " << inv->nombreCompleto
         << " (" << contarCoautores(inv) << " coautores)" << endl;
    if (inv->coautores == NULL) {
        cout << "   (sin coautores registrados)" << endl;
        return;
    }
    coautor* c = inv->coautores;
    while (c != NULL) {
        cout << "   " << inv->nombreCompleto << " <-> " << c->nombre
             << " | " << c->universidad
             << " | Publicaciones conjuntas: " << c->cantidadPublicacionesConjuntas << endl;
        c = c->sig;
    }
}

// Reporte 8: investigadores agrupados por universidad
void reporteInvestigadoresPorUniversidad() {
    if (primeraUniversidad == NULL) {
        cout << "No hay universidades registradas." << endl;
        return;
    }
    actualizarTodosLosIndicesH();
    universidad* uni = primeraUniversidad;
    while (uni != NULL) {
        cout << uni->nombre << " (" << uni->pais << ") - "
             << contarInvestigadoresDeUniversidad(uni) << " investigadores" << endl;
        if (contarInvestigadoresDeUniversidad(uni) == 0) {
            cout << "   (sin investigadores)" << endl;
        } else {
            investigador* inv = primerInvestigador;
            while (inv != NULL) {
                if (inv->suUniversidad == uni)
                    cout << "   - " << inv->nombreCompleto << " | Area: " << inv->suArea->nombreArea
                         << " | Indice H: " << inv->indiceH << endl;
                inv = inv->sig;
            }
        }
        uni = uni->sig;
    }
}

// Reporte 9: investigadores ordenados por indice H, de mayor a menor
void reporteInvestigadoresPorIndiceH() {
    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }
    actualizarTodosLosIndicesH();

    // Copiar los punteros a un arreglo para ordenar sin alterar la lista original
    int n = 0;
    investigador* inv = primerInvestigador;
    while (inv != NULL) {
        n++;
        inv = inv->sig;
    }
    investigador** orden = new investigador*[n];
    inv = primerInvestigador;
    for (int i = 0; i < n; i++) {
        orden[i] = inv;
        inv = inv->sig;
    }

    // Ordenar por insercion (estable: en empates respeta el orden original)
    for (int i = 1; i < n; i++) {
        investigador* clave = orden[i];
        int j = i - 1;
        while (j >= 0 && orden[j]->indiceH < clave->indiceH) {
            orden[j + 1] = orden[j];
            j--;
        }
        orden[j + 1] = clave;
    }

    for (int i = 0; i < n; i++)
        cout << (i + 1) << ". " << orden[i]->nombreCompleto
             << " | Indice H: " << orden[i]->indiceH << endl;

    delete[] orden;
}
//---------------------------------------------Precarga de datos de A---------------------------------------------

void precargarUniversidades() {
    insertarUniversidad(1, "ITCR", "Costa Rica", 15);
    insertarUniversidad(2, "UCR", "Costa Rica", 10);
    insertarUniversidad(3, "UNAM", "Mexico", 5);
    insertarUniversidad(4, "Harvard", "Estados Unidos", 1);
    insertarUniversidad(5, "MIT", "Estados Unidos", 2);
}

void precargarAreas() {
    insertarArea(1, "Ciberseguridad", "Proteccion de sistemas, redes y datos");
    insertarArea(2, "Inteligencia Artificial", "Aprendizaje automatico y agentes");
    insertarArea(3, "Redes de Computadoras", "Diseno y analisis de redes");
    insertarArea(4, "Bioinformatica", "Analisis computacional de datos biologicos");
    insertarArea(5, "Ingenieria de Software", "Procesos y calidad del software");
}

void precargarInvestigadores() {
    insertarInvestigador(1, "Ana Mora", buscarUniversidad(1), "Costa Rica", buscarArea(1), "ana.mora@tec.ac.cr", 0);
    insertarInvestigador(2, "Luis Rojas", buscarUniversidad(3), "Mexico", buscarArea(2), "luis.rojas@unam.mx", 0);
    insertarInvestigador(3, "Maria Vargas", buscarUniversidad(2), "Costa Rica", buscarArea(3), "maria.vargas@ucr.ac.cr", 0);
    insertarInvestigador(4, "Carlos Nunez", buscarUniversidad(4), "Estados Unidos", buscarArea(1), "carlos.nunez@harvard.edu", 0);
    insertarInvestigador(5, "Sofia Leiva", buscarUniversidad(5), "Estados Unidos", buscarArea(4), "sofia.leiva@mit.edu", 0);
}

void precargarCoautores() {
    insertarCoautor(buscarInvestigador(1), 1, "Pedro Soto", "UCR", 3);
    insertarCoautor(buscarInvestigador(1), 2, "Marta Diaz", "UNAM", 5);
    insertarCoautor(buscarInvestigador(2), 3, "Raul Vega", "MIT", 1);
    insertarCoautor(buscarInvestigador(2), 4, "Elena Cruz", "USP", 2);
    insertarCoautor(buscarInvestigador(3), 5, "Jorge Salas", "ITCR", 4);
    insertarCoautor(buscarInvestigador(3), 6, "Laura Pineda", "UCR", 2);
    insertarCoautor(buscarInvestigador(4), 7, "Diego Herrera", "Harvard", 3);
    insertarCoautor(buscarInvestigador(4), 8, "Camila Ortiz", "UNAM", 1);
    insertarCoautor(buscarInvestigador(5), 9, "Andres Brenes", "ITCR", 6);
    insertarCoautor(buscarInvestigador(5), 10, "Valeria Chaves", "MIT", 2);
}

// Carga todos los datos de A, en este orden porque cada lista depende de la anterior
void precargarDatosA() {
    precargarUniversidades();
    precargarAreas();
    precargarInvestigadores();
    precargarCoautores();
}
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


//---------------------------------------------Modificar Revista---------------------------------------------

bool modificarRevista(int id, string nombre, string editorial,
                      string pais, float factorImpacto, string cuartil) {

    // Buscar la revista
    revista* rev = buscarRevista(id);

    if (rev == NULL) {
        cout << "Error: no existe una revista con ese ID." << endl;
        return false;
    }

    // Validar textos
    if (nombre == "" || editorial == "" || pais == "") {
        cout << "Error: nombre, editorial y pais no pueden estar vacios." << endl;
        return false;
    }

    // Validar factor de impacto
    if (factorImpacto < 0) {
        cout << "Error: el factor de impacto no puede ser negativo." << endl;
        return false;
    }

    // Validar cuartil
    if (cuartil != "Q1" &&
        cuartil != "Q2" &&
        cuartil != "Q3" &&
        cuartil != "Q4") {

        cout << "Error: el cuartil debe ser Q1, Q2, Q3 o Q4." << endl;
        return false;
    }


    // Sacar temporalmente la revista de la lista
    // porque el nombre puede cambiar y la lista debe seguir ordenada.

    if (primeraRevista == rev) {

        primeraRevista = rev->sig;

    } else {

        revista* anterior = primeraRevista;

        while (anterior != NULL && anterior->sig != rev) {
            anterior = anterior->sig;
        }

        if (anterior != NULL) {
            anterior->sig = rev->sig;
        }
    }


    // Modificar datos
    rev->nombre = nombre;
    rev->editorial = editorial;
    rev->pais = pais;
    rev->factorImpacto = factorImpacto;
    rev->cuartil = cuartil;

    rev->sig = NULL;


    // Volver a insertar la revista ordenada por nombre

    // Lista vacia o debe ir de primera
    if (primeraRevista == NULL ||
        rev->nombre < primeraRevista->nombre) {

        rev->sig = primeraRevista;
        primeraRevista = rev;

        return true;
    }


    // Buscar posicion
    revista* aux = primeraRevista;

    while (aux->sig != NULL &&
           aux->sig->nombre < rev->nombre) {

        aux = aux->sig;
    }

    rev->sig = aux->sig;
    aux->sig = rev;

    return true;
}

    //---------------------------------------------Parte de Gerald - Proyectos---------------------------------------------

// Busca un proyecto por ID. Devuelve NULL si no existe.
proyecto* buscarProyecto(int id) {
    proyecto* aux = primerProyecto;

    while (aux != NULL) {
        if (aux->idProyecto == id) {
            return aux;
        }

        aux = aux->sig;
    }

    return NULL;
}


// Inserta un proyecto ordenado por anio de inicio.
bool insertarProyecto(int id, string nombre, float financiamiento,
                      int anioInicio, int anioFinalizacion,
                      investigador* responsable) {

    // Validar ID
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }

    // Evitar IDs repetidos
    if (buscarProyecto(id) != NULL) {
        cout << "Error: ya existe un proyecto con ese ID." << endl;
        return false;
    }

    // Validar nombre
    if (nombre == "") {
        cout << "Error: el nombre del proyecto no puede estar vacio." << endl;
        return false;
    }

    // Validar financiamiento
    if (financiamiento < 0) {
        cout << "Error: el financiamiento no puede ser negativo." << endl;
        return false;
    }

    // Validar anios
    if (anioInicio <= 0 || anioFinalizacion < anioInicio) {
        cout << "Error: las fechas del proyecto no son validas." << endl;
        return false;
    }

    // Validar investigador responsable
    if (responsable == NULL) {
        cout << "Error: el investigador responsable no existe." << endl;
        return false;
    }

    // Crear nodo
    proyecto* nuevo = new proyecto;

    nuevo->idProyecto = id;
    nuevo->nombre = nombre;
    nuevo->financiamiento = financiamiento;
    nuevo->anioInicio = anioInicio;
    nuevo->anioFinalizacion = anioFinalizacion;
    nuevo->investigadorResponsable = responsable;

    nuevo->sig = NULL;
    nuevo->ant = NULL;

    // Caso 1: lista vacia
    if (primerProyecto == NULL) {
        primerProyecto = nuevo;
        return true;
    }

    // Caso 2: insertar al inicio
    if (anioInicio < primerProyecto->anioInicio) {
        nuevo->sig = primerProyecto;
        primerProyecto->ant = nuevo;
        primerProyecto = nuevo;

        return true;
    }

    // Caso 3: buscar la posicion correspondiente
    proyecto* aux = primerProyecto;

    while (aux->sig != NULL &&
           aux->sig->anioInicio <= anioInicio) {

        aux = aux->sig;
    }

    nuevo->sig = aux->sig;
    nuevo->ant = aux;

    if (aux->sig != NULL) {
        aux->sig->ant = nuevo;
    }

    aux->sig = nuevo;

    return true;
}


// Muestra un solo proyecto.
void mostrarProyecto(proyecto* proy) {
    if (proy == NULL) {
        cout << "Proyecto no encontrado." << endl;
        return;
    }

    cout << "ID: " << proy->idProyecto
         << " | Nombre: " << proy->nombre
         << " | Financiamiento: " << proy->financiamiento
         << " | Inicio: " << proy->anioInicio
         << " | Finalizacion: " << proy->anioFinalizacion
         << " | Responsable: ";

    if (proy->investigadorResponsable != NULL) {
        cout << proy->investigadorResponsable->nombreCompleto;
    } else {
        cout << "(sin responsable)";
    }

    cout << endl;
}


// Muestra todos los proyectos.
void mostrarProyectos() {
    if (primerProyecto == NULL) {
        cout << "No hay proyectos registrados." << endl;
        return;
    }

    proyecto* aux = primerProyecto;

    while (aux != NULL) {
        mostrarProyecto(aux);
        aux = aux->sig;
    }


    


}


//---------------------------------------------Parte de Gerald - Publicaciones---------------------------------------------

// Busca una publicacion por ID en la lista circular.
// Devuelve NULL si no existe.
publicacion* buscarPublicacion(int id) {

    if (primeraPublicacion == NULL) {
        return NULL;
    }

    publicacion* aux = primeraPublicacion;

    do {
        if (aux->idPublicacion == id) {
            return aux;
        }

        aux = aux->sig;

    } while (aux != primeraPublicacion);

    return NULL;
}


// Busca una publicacion dentro de la sublista de una revista.
publicacion* buscarPubRevista(revista* rev, int idPublicacion) {

    if (rev == NULL) {
        return NULL;
    }

    pubRevista* aux = rev->publicaciones;

    while (aux != NULL) {

        if (aux->laPublicacion->idPublicacion == idPublicacion) {
            return aux->laPublicacion;
        }

        aux = aux->sig;
    }

    return NULL;
}


// Agrega una publicacion a la sublista doble de una revista.
bool agregarPublicacionARevista(revista* rev, publicacion* pub) {

    if (rev == NULL) {
        cout << "Error: la revista no existe." << endl;
        return false;
    }

    if (pub == NULL) {
        cout << "Error: la publicacion no existe." << endl;
        return false;
    }

    if (buscarPubRevista(rev, pub->idPublicacion) != NULL) {
        cout << "Error: la revista ya tiene esa publicacion." << endl;
        return false;
    }

    pubRevista* nuevo = new pubRevista;

    nuevo->laPublicacion = pub;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if (rev->publicaciones == NULL) {

        rev->publicaciones = nuevo;

    } else {

        pubRevista* aux = rev->publicaciones;

        while (aux->sig != NULL) {
            aux = aux->sig;
        }

        aux->sig = nuevo;
        nuevo->ant = aux;
    }

    return true;
}


// Inserta una publicacion ordenada por anio en la lista circular.
bool insertarPublicacion(int id, string titulo, int anio,
                         string tipo, int cantidadCitas, string doi,
                         investigador* investigadorPrincipal,
                         revista* rev,
                         proyecto* proy) {

    // Validar ID
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }

    // Evitar ID repetido en la lista principal
    if (buscarPublicacion(id) != NULL) {
        cout << "Error: ya existe una publicacion con ese ID." << endl;
        return false;
    }

    // Validar titulo y DOI
    if (titulo == "" || doi == "") {
        cout << "Error: titulo y DOI no pueden estar vacios." << endl;
        return false;
    }

    // Validar anio
    if (anio <= 0) {
        cout << "Error: el anio debe ser mayor que 0." << endl;
        return false;
    }

    // Validar tipo
    if (tipo != "Articulo" &&
        tipo != "Libro" &&
        tipo != "Conferencia") {

        cout << "Error: el tipo debe ser Articulo, Libro o Conferencia." << endl;
        return false;
    }

    // Validar citas
    if (cantidadCitas < 0) {
        cout << "Error: la cantidad de citas no puede ser negativa." << endl;
        return false;
    }

    // Validar investigador
    if (investigadorPrincipal == NULL) {
        cout << "Error: el investigador principal no existe." << endl;
        return false;
    }

    // Validar revista
    if (rev == NULL) {
        cout << "Error: la revista no existe." << endl;
        return false;
    }

    // Validar proyecto
    if (proy == NULL) {
        cout << "Error: el proyecto no existe." << endl;
        return false;
    }

    // Revisar que ese ID no este ya usado en las sublistas
    if (buscarPubInvestigador(investigadorPrincipal, id) != NULL) {
        cout << "Error: el investigador ya tiene una publicacion con ese ID." << endl;
        return false;
    }

    if (buscarPubRevista(rev, id) != NULL) {
        cout << "Error: la revista ya tiene una publicacion con ese ID." << endl;
        return false;
    }


    // Crear nodo
    publicacion* nueva = new publicacion;

    nueva->idPublicacion = id;
    nueva->titulo = titulo;
    nueva->anio = anio;
    nueva->tipo = tipo;
    nueva->cantidadCitas = cantidadCitas;
    nueva->doi = doi;

    nueva->investigadorPrincipal = investigadorPrincipal;
    nueva->suRevista = rev;
    nueva->suProyecto = proy;

    nueva->citas = NULL;
    nueva->coautores = NULL;

    // Por defecto se apunta a si misma.
    // Esto permite crear correctamente la primera publicacion circular.
    nueva->sig = nueva;


    // CASO 1: lista vacia
    if (primeraPublicacion == NULL) {

        primeraPublicacion = nueva;
    }

    // CASO 2: debe convertirse en la primera
    else if (anio < primeraPublicacion->anio) {

        publicacion* ultimo = primeraPublicacion;

        while (ultimo->sig != primeraPublicacion) {
            ultimo = ultimo->sig;
        }

        nueva->sig = primeraPublicacion;
        ultimo->sig = nueva;
        primeraPublicacion = nueva;
    }

    // CASO 3: insertar en medio o al final
    else {

        publicacion* aux = primeraPublicacion;

        while (aux->sig != primeraPublicacion &&
               aux->sig->anio <= anio) {

            aux = aux->sig;
        }

        nueva->sig = aux->sig;
        aux->sig = nueva;
    }


    // Relacionar la publicacion con el investigador
    agregarPublicacionAInvestigador(investigadorPrincipal, nueva);

    // Relacionar la publicacion con la revista
    agregarPublicacionARevista(rev, nueva);

    return true;
}


// Muestra una sola publicacion.
void mostrarPublicacion(publicacion* pub) {

    if (pub == NULL) {
        cout << "Publicacion no encontrada." << endl;
        return;
    }

    cout << "ID: " << pub->idPublicacion
         << " | Titulo: " << pub->titulo
         << " | Anio: " << pub->anio
         << " | Tipo: " << pub->tipo
         << " | Citas: " << pub->cantidadCitas
         << " | DOI: " << pub->doi
         << endl;

    cout << "   Investigador: ";

    if (pub->investigadorPrincipal != NULL)
        cout << pub->investigadorPrincipal->nombreCompleto;
    else
        cout << "(sin investigador)";

    cout << " | Revista: ";

    if (pub->suRevista != NULL)
        cout << pub->suRevista->nombre;
    else
        cout << "(sin revista)";

    cout << " | Proyecto: ";

    if (pub->suProyecto != NULL)
        cout << pub->suProyecto->nombre;
    else
        cout << "(sin proyecto)";

    cout << endl;
}


// Muestra todas las publicaciones de la lista circular.
void mostrarPublicaciones() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    publicacion* aux = primeraPublicacion;

    do {
        mostrarPublicacion(aux);
        aux = aux->sig;

    } while (aux != primeraPublicacion);
}


//---------------------------------------------Parte de Gerald - Citaciones---------------------------------------------

// Busca una citacion por ID dentro de una publicacion.
// Devuelve NULL si no existe.
citacion* buscarCitacion(publicacion* pub, int id) {

    if (pub == NULL) {
        return NULL;
    }

    citacion* aux = pub->citas;

    while (aux != NULL) {

        if (aux->idCita == id) {
            return aux;
        }

        aux = aux->sig;
    }

    return NULL;
}


// Comprueba si un ID de citacion ya existe en cualquier publicacion.
bool existeIdCitacion(int id) {

    if (primeraPublicacion == NULL) {
        return false;
    }

    publicacion* pub = primeraPublicacion;

    do {

        citacion* cita = pub->citas;

        while (cita != NULL) {

            if (cita->idCita == id) {
                return true;
            }

            cita = cita->sig;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);

    return false;
}


// Inserta una citacion al final de la sublista doble de una publicacion.
bool insertarCitacion(publicacion* publicacionCitada,
                      int id,
                      int anio,
                      publicacion* publicacionCitante,
                      investigador* autorCitante) {

    // Validar que la publicacion citada exista
    if (publicacionCitada == NULL) {
        cout << "Error: la publicacion citada no existe." << endl;
        return false;
    }

    // Validar ID
    if (id <= 0) {
        cout << "Error: el ID debe ser mayor que 0." << endl;
        return false;
    }

    // Evitar IDs repetidos
    if (existeIdCitacion(id)) {
        cout << "Error: ya existe una citacion con ese ID." << endl;
        return false;
    }

    // Validar anio
    if (anio <= 0) {
        cout << "Error: el anio debe ser mayor que 0." << endl;
        return false;
    }

    // Validar publicacion citante
    if (publicacionCitante == NULL) {
        cout << "Error: la publicacion citante no existe." << endl;
        return false;
    }

    // Validar autor citante
    if (autorCitante == NULL) {
        cout << "Error: el autor citante no existe." << endl;
        return false;
    }

    // Crear nueva citacion
    citacion* nueva = new citacion;

    nueva->idCita = id;
    nueva->anio = anio;
    nueva->publicacionCitante = publicacionCitante;
    nueva->autorCitante = autorCitante;

    nueva->sig = NULL;
    nueva->ant = NULL;


    // Caso 1: la publicacion aun no tiene citaciones
    if (publicacionCitada->citas == NULL) {

        publicacionCitada->citas = nueva;

    } else {

        // Caso 2: insertar al final
        citacion* aux = publicacionCitada->citas;

        while (aux->sig != NULL) {
            aux = aux->sig;
        }

        aux->sig = nueva;
        nueva->ant = aux;
    }

    return true;
}


// Muestra una sola citacion.
void mostrarCitacion(citacion* cita) {

    if (cita == NULL) {
        cout << "Citacion no encontrada." << endl;
        return;
    }

    cout << "ID cita: " << cita->idCita
         << " | Anio: " << cita->anio
         << " | Publicacion citante: ";

    if (cita->publicacionCitante != NULL) {
        cout << cita->publicacionCitante->titulo;
    } else {
        cout << "(sin publicacion)";
    }

    cout << " | Autor citante: ";

    if (cita->autorCitante != NULL) {
        cout << cita->autorCitante->nombreCompleto;
    } else {
        cout << "(sin autor)";
    }

    cout << endl;
}


// Muestra todas las citaciones recibidas por una publicacion.
void mostrarCitaciones(publicacion* pub) {

    if (pub == NULL) {
        cout << "Publicacion no encontrada." << endl;
        return;
    }

    if (pub->citas == NULL) {
        cout << "La publicacion no tiene citaciones registradas." << endl;
        return;
    }

    cout << "Citaciones de " << pub->titulo << ":" << endl;

    citacion* aux = pub->citas;

    while (aux != NULL) {

        mostrarCitacion(aux);

        aux = aux->sig;
    }
}


//---------------------------------------------Parte de Gerald - Coautores de Publicacion---------------------------------------------

// Busca un coautor por ID dentro de una publicacion.
// Devuelve NULL si no esta asociado a esa publicacion.
coautor* buscarCoautorPublicacion(publicacion* pub, int idCoautor) {

    if (pub == NULL) {
        return NULL;
    }

    coautorPublicacion* aux = pub->coautores;

    while (aux != NULL) {

        if (aux->elCoautor != NULL &&
            aux->elCoautor->idCoautor == idCoautor) {

            return aux->elCoautor;
        }

        aux = aux->sig;
    }

    return NULL;
}


// Agrega un coautor a la sublista doble de una publicacion.
bool agregarCoautorAPublicacion(publicacion* pub, coautor* autor) {

    // Validar publicacion
    if (pub == NULL) {
        cout << "Error: la publicacion no existe." << endl;
        return false;
    }

    // Validar coautor
    if (autor == NULL) {
        cout << "Error: el coautor no existe." << endl;
        return false;
    }

    // Evitar repetir el mismo coautor en una publicacion
    if (buscarCoautorPublicacion(pub, autor->idCoautor) != NULL) {
        cout << "Error: el coautor ya esta asociado a esta publicacion." << endl;
        return false;
    }

    // Crear nodo de enlace
    coautorPublicacion* nuevo = new coautorPublicacion;

    nuevo->elCoautor = autor;
    nuevo->sig = NULL;
    nuevo->ant = NULL;


    // Caso 1: la publicacion no tiene coautores
    if (pub->coautores == NULL) {

        pub->coautores = nuevo;

    } else {

        // Caso 2: insertar al final de la sublista doble
        coautorPublicacion* aux = pub->coautores;

        while (aux->sig != NULL) {
            aux = aux->sig;
        }

        aux->sig = nuevo;
        nuevo->ant = aux;
    }

    return true;
}


// Muestra todos los coautores de una publicacion.
void mostrarCoautoresPublicacion(publicacion* pub) {

    if (pub == NULL) {
        cout << "Publicacion no encontrada." << endl;
        return;
    }

    if (pub->coautores == NULL) {
        cout << "La publicacion no tiene coautores registrados." << endl;
        return;
    }

    cout << "Coautores de " << pub->titulo << ":" << endl;

    coautorPublicacion* aux = pub->coautores;

    while (aux != NULL) {

        if (aux->elCoautor != NULL) {
            mostrarCoautor(aux->elCoautor);
        }

        aux = aux->sig;
    }
}


// Cuenta cuantos coautores tiene una publicacion.
// Nos servira despues para la consulta de la publicacion con mas autores.
int contarCoautoresPublicacion(publicacion* pub) {

    if (pub == NULL) {
        return 0;
    }

    int total = 0;

    coautorPublicacion* aux = pub->coautores;

    while (aux != NULL) {
        total++;
        aux = aux->sig;
    }

    return total;
}

//---------------------------------------------Eliminar Publicacion---------------------------------------------

// Quita el enlace de una publicacion de la sublista de su investigador.
// No elimina la publicacion, solamente el nodo pubInvestigador.
void quitarPublicacionDeInvestigador(investigador* inv, publicacion* pub) {

    if (inv == NULL || pub == NULL) {
        return;
    }

    pubInvestigador* aux = inv->publicaciones;

    while (aux != NULL && aux->laPublicacion != pub) {
        aux = aux->sig;
    }

    if (aux == NULL) {
        return;
    }

    // Si tiene nodo anterior
    if (aux->ant != NULL) {
        aux->ant->sig = aux->sig;
    } else {
        // Era el primero de la sublista
        inv->publicaciones = aux->sig;
    }

    // Si tiene nodo siguiente
    if (aux->sig != NULL) {
        aux->sig->ant = aux->ant;
    }

    delete aux;
}


// Quita el enlace de una publicacion de la sublista de su revista.
void quitarPublicacionDeRevista(revista* rev, publicacion* pub) {

    if (rev == NULL || pub == NULL) {
        return;
    }

    pubRevista* aux = rev->publicaciones;

    while (aux != NULL && aux->laPublicacion != pub) {
        aux = aux->sig;
    }

    if (aux == NULL) {
        return;
    }

    if (aux->ant != NULL) {
        aux->ant->sig = aux->sig;
    } else {
        // Era el primero de la sublista
        rev->publicaciones = aux->sig;
    }

    if (aux->sig != NULL) {
        aux->sig->ant = aux->ant;
    }

    delete aux;
}


// Elimina citaciones de otras publicaciones que tengan
// como publicacion citante a la publicacion que vamos a borrar.
void quitarReferenciasDeCitaciones(publicacion* pubEliminar) {

    if (primeraPublicacion == NULL || pubEliminar == NULL) {
        return;
    }

    publicacion* pub = primeraPublicacion;

    do {

        // Las citas propias de pubEliminar se eliminan despues.
        if (pub != pubEliminar) {

            citacion* cita = pub->citas;

            while (cita != NULL) {

                citacion* siguiente = cita->sig;

                if (cita->publicacionCitante == pubEliminar) {

                    if (cita->ant != NULL) {
                        cita->ant->sig = cita->sig;
                    } else {
                        pub->citas = cita->sig;
                    }

                    if (cita->sig != NULL) {
                        cita->sig->ant = cita->ant;
                    }

                    delete cita;
                }

                cita = siguiente;
            }
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);
}


// Libera todas las citaciones que pertenecen a la publicacion.
void liberarCitacionesPublicacion(publicacion* pub) {

    citacion* aux = pub->citas;

    while (aux != NULL) {

        citacion* siguiente = aux->sig;

        delete aux;

        aux = siguiente;
    }

    pub->citas = NULL;
}


// Libera los nodos de enlace con coautores.
// NO elimina los coautores reales.
void liberarCoautoresPublicacion(publicacion* pub) {

    coautorPublicacion* aux = pub->coautores;

    while (aux != NULL) {

        coautorPublicacion* siguiente = aux->sig;

        delete aux;

        aux = siguiente;
    }

    pub->coautores = NULL;
}


// Elimina una publicacion de la lista circular.
bool eliminarPublicacion(int id) {

    publicacion* eliminar = buscarPublicacion(id);

    if (eliminar == NULL) {
        cout << "Error: no existe una publicacion con ese ID." << endl;
        return false;
    }


    // 1. Quitar referencias desde otras citaciones
    quitarReferenciasDeCitaciones(eliminar);


    // 2. Quitar la publicacion de la sublista del investigador
    quitarPublicacionDeInvestigador(
        eliminar->investigadorPrincipal,
        eliminar
    );


    // 3. Quitar la publicacion de la sublista de la revista
    quitarPublicacionDeRevista(
        eliminar->suRevista,
        eliminar
    );


    // 4. Liberar sus propias sublistas
    liberarCitacionesPublicacion(eliminar);
    liberarCoautoresPublicacion(eliminar);


    // 5. Quitar de la lista circular

    // Caso: solamente existe una publicacion
    if (primeraPublicacion == eliminar &&
        primeraPublicacion->sig == primeraPublicacion) {

        primeraPublicacion = NULL;
    }

    // Caso: eliminar la primera y hay mas publicaciones
    else if (primeraPublicacion == eliminar) {

        publicacion* ultimo = primeraPublicacion;

        while (ultimo->sig != primeraPublicacion) {
            ultimo = ultimo->sig;
        }

        primeraPublicacion = primeraPublicacion->sig;

        ultimo->sig = primeraPublicacion;
    }

    // Caso: eliminar una publicacion que no es la primera
    else {

        publicacion* anterior = primeraPublicacion;

        while (anterior->sig != eliminar) {
            anterior = anterior->sig;
        }

        anterior->sig = eliminar->sig;
    }


    delete eliminar;

    return true;
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

    // Datos precargados
    precargarDatosA();
    // precargarDatosB();   // Gerald agrega aqui sus precargas

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

    titulo("PRUEBA DE SUBLISTA pubInvestigador");

// TEMPORAL: publicaciones creadas a mano
    publicacion* p1 = new publicacion;
    p1->idPublicacion = 1; p1->titulo = "Redes seguras"; p1->anio = 2021;
    p1->tipo = "Articulo"; p1->cantidadCitas = 25;

    publicacion* p2 = new publicacion;
    p2->idPublicacion = 2; p2->titulo = "Criptografia moderna"; p2->anio = 2022;
    p2->tipo = "Libro"; p2->cantidadCitas = 18;

    prueba("Agregar 2 publicaciones a Ana");
    agregarPublicacionAInvestigador(buscarInvestigador(1), p1);
    agregarPublicacionAInvestigador(buscarInvestigador(1), p2);
    prueba("Publicacion repetida (debe dar error)");
    agregarPublicacionAInvestigador(buscarInvestigador(1), p1);
    prueba("Investigador que no existe (debe dar error)");
    agregarPublicacionAInvestigador(buscarInvestigador(99), p1);
    prueba("Publicaciones de Ana y de Luis (Luis no tiene)");
    mostrarPublicacionesInvestigador(buscarInvestigador(1));
    mostrarPublicacionesInvestigador(buscarInvestigador(2));
    prueba("Buscar la publicacion 2 en Ana, y la 5 (no esta)");
    cout << (buscarPubInvestigador(buscarInvestigador(1), 2) != NULL ? "Encontrada" : "No esta") << endl;
    cout << (buscarPubInvestigador(buscarInvestigador(1), 5) != NULL ? "Encontrada" : "No esta") << endl;


    titulo("PRUEBA DE MODIFICAR INVESTIGADOR");
    prueba("Modificar el investigador 1 (valido)");
    modificarInvestigador(1, "Ana Mora Salas", buscarUniversidad(2), "Costa Rica", buscarArea(2), "ana.salas@ucr.ac.cr");
    mostrarInvestigador(buscarInvestigador(1));
    prueba("ID que no existe (debe dar error)");
    modificarInvestigador(99, "X", buscarUniversidad(1), "CR", buscarArea(1), "x@x.com");
    prueba("Correo invalido (debe dar error)");
    modificarInvestigador(1, "Ana", buscarUniversidad(1), "CR", buscarArea(1), "correo.com");
    prueba("Universidad que no existe (debe dar error)");
    modificarInvestigador(1, "Ana", buscarUniversidad(99), "CR", buscarArea(1), "a@x.com");
    prueba("El investigador 1 debe seguir con los datos de la primera modificacion");
    mostrarInvestigador(buscarInvestigador(1));

    titulo("PRUEBA DE METRICAS");

    // TEMPORAL: las 8 publicaciones del ejemplo del documento para Ana
    int citas[8] = {25, 18, 12, 8, 5, 4, 2, 1};
    for (int i = 0; i < 8; i++) {
        publicacion* p = new publicacion;
        p->idPublicacion = i + 1;
        p->titulo = "Pub " + to_string(i + 1);
        p->anio = 2020;
        p->tipo = "Articulo";
        p->cantidadCitas = citas[i];
        agregarPublicacionAInvestigador(buscarInvestigador(1), p);
    }

    prueba("Metricas de Ana (con publicaciones)");
    mostrarMetricas(buscarInvestigador(1));
    prueba("Metricas de Maria (sin publicaciones, no debe fallar)");
    mostrarMetricas(buscarInvestigador(3));
    prueba("Actualizar el indice H guardado y mostrar");
    actualizarTodosLosIndicesH();
    mostrarInvestigador(buscarInvestigador(1));

    
titulo("PRUEBA DE PUBLICACIONES");

prueba("Insertar 3 publicaciones validas");

insertarPublicacion(
    101,
    "Inteligencia Artificial y Educacion",
    2024,
    "Articulo",
    15,
    "10.1000/ia101",
    buscarInvestigador(1),
    buscarRevista(1),
    buscarProyecto(1)
);

insertarPublicacion(
    102,
    "Seguridad en Redes Modernas",
    2021,
    "Conferencia",
    8,
    "10.1000/red102",
    buscarInvestigador(2),
    buscarRevista(2),
    buscarProyecto(2)
);

insertarPublicacion(
    103,
    "Sistemas Distribuidos",
    2023,
    "Libro",
    12,
    "10.1000/sis103",
    buscarInvestigador(1),
    buscarRevista(3),
    buscarProyecto(3)
);


prueba("Lista de publicaciones ordenada por anio");
mostrarPublicaciones();


prueba("ID repetido (debe dar error)");

insertarPublicacion(
    101,
    "Publicacion repetida",
    2025,
    "Articulo",
    5,
    "10.1000/repetida",
    buscarInvestigador(1),
    buscarRevista(1),
    buscarProyecto(1)
);


prueba("Investigador inexistente (debe dar error)");

insertarPublicacion(
    104,
    "Publicacion sin investigador",
    2025,
    "Articulo",
    5,
    "10.1000/error104",
    buscarInvestigador(99),
    buscarRevista(1),
    buscarProyecto(1)
);


prueba("Buscar publicacion 102 y publicacion 999");

mostrarPublicacion(buscarPublicacion(102));

mostrarPublicacion(buscarPublicacion(999));


titulo("PRUEBA DE CITACIONES");

prueba("Insertar 3 citaciones validas");

insertarCitacion(
    buscarPublicacion(101),
    1001,
    2025,
    buscarPublicacion(102),
    buscarInvestigador(2)
);

insertarCitacion(
    buscarPublicacion(101),
    1002,
    2026,
    buscarPublicacion(103),
    buscarInvestigador(1)
);

insertarCitacion(
    buscarPublicacion(102),
    1003,
    2024,
    buscarPublicacion(101),
    buscarInvestigador(1)
);


prueba("Mostrar citaciones de la publicacion 101");
mostrarCitaciones(buscarPublicacion(101));


prueba("Mostrar citaciones de la publicacion 102");
mostrarCitaciones(buscarPublicacion(102));


prueba("ID repetido (debe dar error)");

insertarCitacion(
    buscarPublicacion(102),
    1001,
    2025,
    buscarPublicacion(103),
    buscarInvestigador(1)
);


prueba("Publicacion citada inexistente (debe dar error)");

insertarCitacion(
    buscarPublicacion(999),
    1004,
    2025,
    buscarPublicacion(101),
    buscarInvestigador(1)
);


prueba("Publicacion citante inexistente (debe dar error)");

insertarCitacion(
    buscarPublicacion(101),
    1005,
    2025,
    buscarPublicacion(999),
    buscarInvestigador(1)
);


prueba("Autor citante inexistente (debe dar error)");

insertarCitacion(
    buscarPublicacion(101),
    1006,
    2025,
    buscarPublicacion(102),
    buscarInvestigador(99)
);


prueba("Buscar cita 1002 y cita 9999");

mostrarCitacion(
    buscarCitacion(buscarPublicacion(101), 1002)
);

mostrarCitacion(
    buscarCitacion(buscarPublicacion(101), 9999)
);



titulo("PRUEBA DE COAUTORES DE PUBLICACION");

prueba("Agregar coautores a las publicaciones");

// Pedro Soto y Marta Diaz son coautores del investigador 1
agregarCoautorAPublicacion(
    buscarPublicacion(101),
    buscarCoautor(buscarInvestigador(1), 1)
);

agregarCoautorAPublicacion(
    buscarPublicacion(101),
    buscarCoautor(buscarInvestigador(1), 2)
);

// Raul Vega pertenece a la sublista del investigador 2
agregarCoautorAPublicacion(
    buscarPublicacion(102),
    buscarCoautor(buscarInvestigador(2), 3)
);


prueba("Mostrar coautores de la publicacion 101");
mostrarCoautoresPublicacion(buscarPublicacion(101));


prueba("Mostrar coautores de la publicacion 102");
mostrarCoautoresPublicacion(buscarPublicacion(102));


prueba("Coautor repetido en la publicacion 101 (debe dar error)");

agregarCoautorAPublicacion(
    buscarPublicacion(101),
    buscarCoautor(buscarInvestigador(1), 1)
);


prueba("Publicacion inexistente (debe dar error)");

agregarCoautorAPublicacion(
    buscarPublicacion(999),
    buscarCoautor(buscarInvestigador(1), 1)
);


prueba("Coautor inexistente (debe dar error)");

agregarCoautorAPublicacion(
    buscarPublicacion(101),
    buscarCoautor(buscarInvestigador(1), 999)
);


prueba("Buscar coautor 2 en publicacion 101 y coautor 3");

mostrarCoautor(
    buscarCoautorPublicacion(buscarPublicacion(101), 2)
);

mostrarCoautor(
    buscarCoautorPublicacion(buscarPublicacion(101), 3)
);


prueba("Cantidad de coautores de la publicacion 101");

cout << "Cantidad: "
     << contarCoautoresPublicacion(buscarPublicacion(101))
     << endl;


titulo("PRUEBA DE MODIFICAR REVISTA");

prueba("Modificar revista ID 3");

modificarRevista(
    3,
    "AA Science",
    "Editorial Cientifica",
    "Costa Rica",
    7.5,
    "Q2"
);

prueba("Lista despues de modificar");
mostrarRevistas();


prueba("Modificar revista inexistente (debe dar error)");

modificarRevista(
    99,
    "Revista inexistente",
    "Editorial",
    "Costa Rica",
    5.0,
    "Q1"
);


prueba("Cuartil invalido (debe dar error)");

modificarRevista(
    3,
    "AA Science",
    "Editorial Cientifica",
    "Costa Rica",
    7.5,
    "Q9"
);

    return 0;
}