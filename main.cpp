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


//---------------------------------------------Mostrar publicaciones de una revista---------------------------------------------

void mostrarPublicacionesRevista(revista* rev) {

    if (rev == NULL) {
        cout << "Revista no encontrada." << endl;
        return;
    }

    if (rev->publicaciones == NULL) {
        cout << "La revista " << rev->nombre
             << " no tiene publicaciones registradas." << endl;
        return;
    }

    cout << "Publicaciones de la revista " << rev->nombre << ":" << endl;

    pubRevista* aux = rev->publicaciones;

    while (aux != NULL) {

        mostrarPublicacion(aux->laPublicacion);

        aux = aux->sig;
    }
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


//---------------------------------------------Precarga de datos de B---------------------------------------------

void precargarRevistasB() {

    insertarRevista(
        11,
        "Artificial Intelligence Research",
        "Tech Press",
        "Estados Unidos",
        14.2,
        "Q1"
    );

    insertarRevista(
        12,
        "Cybersecurity Today",
        "Secure Press",
        "Reino Unido",
        11.5,
        "Q1"
    );

    insertarRevista(
        13,
        "Data Science Review",
        "Data Editorial",
        "Costa Rica",
        8.4,
        "Q2"
    );

    insertarRevista(
        14,
        "Networks and Systems",
        "Global Networks",
        "Alemania",
        5.1,
        "Q3"
    );

    insertarRevista(
        15,
        "Software Engineering Journal",
        "Computing House",
        "Canada",
        3.6,
        "Q4"
    );
}


void precargarProyectosB() {

    insertarProyecto(
        11,
        "Educacion con Inteligencia Artificial",
        1200000,
        2019,
        2023,
        buscarInvestigador(1)
    );

    insertarProyecto(
        12,
        "Seguridad de Redes",
        950000,
        2020,
        2024,
        buscarInvestigador(2)
    );

    insertarProyecto(
        13,
        "Analisis de Datos Cientificos",
        800000,
        2021,
        2025,
        buscarInvestigador(1)
    );

    insertarProyecto(
        14,
        "Sistemas Distribuidos Inteligentes",
        1100000,
        2022,
        2026,
        buscarInvestigador(1)
    );

    insertarProyecto(
        15,
        "Infraestructura de Redes",
        700000,
        2023,
        2027,
        buscarInvestigador(2)
    );
}


void precargarPublicacionesB() {

    insertarPublicacion(
        201,
        "Aprendizaje Automatico en Educacion",
        2020,
        "Articulo",
        3,
        "10.2000/pub201",
        buscarInvestigador(1),
        buscarRevista(11),
        buscarProyecto(11)
    );

    insertarPublicacion(
        202,
        "Proteccion de Redes Universitarias",
        2021,
        "Articulo",
        1,
        "10.2000/pub202",
        buscarInvestigador(2),
        buscarRevista(12),
        buscarProyecto(12)
    );

    insertarPublicacion(
        203,
        "Analisis Computacional de Datos",
        2021,
        "Libro",
        0,
        "10.2000/pub203",
        buscarInvestigador(1),
        buscarRevista(13),
        buscarProyecto(13)
    );

    insertarPublicacion(
        204,
        "Modelos Inteligentes Distribuidos",
        2023,
        "Articulo",
        1,
        "10.2000/pub204",
        buscarInvestigador(1),
        buscarRevista(11),
        buscarProyecto(14)
    );

    insertarPublicacion(
        205,
        "Arquitecturas de Redes Modernas",
        2024,
        "Conferencia",
        0,
        "10.2000/pub205",
        buscarInvestigador(2),
        buscarRevista(14),
        buscarProyecto(15)
    );
}


void precargarCitacionesB() {

    // La publicacion 201 recibe tres citas
    insertarCitacion(
        buscarPublicacion(201),
        3001,
        2021,
        buscarPublicacion(202),
        buscarInvestigador(2)
    );

    insertarCitacion(
        buscarPublicacion(201),
        3002,
        2022,
        buscarPublicacion(203),
        buscarInvestigador(1)
    );

    insertarCitacion(
        buscarPublicacion(201),
        3003,
        2023,
        buscarPublicacion(204),
        buscarInvestigador(1)
    );

    // La publicacion 202 recibe una cita
    insertarCitacion(
        buscarPublicacion(202),
        3004,
        2024,
        buscarPublicacion(205),
        buscarInvestigador(2)
    );

    // La publicacion 204 recibe una cita
    insertarCitacion(
        buscarPublicacion(204),
        3005,
        2024,
        buscarPublicacion(201),
        buscarInvestigador(1)
    );
}


void precargarCoautoresPublicacionB() {

    // Publicacion 201 tiene dos coautores
    agregarCoautorAPublicacion(
        buscarPublicacion(201),
        buscarCoautor(buscarInvestigador(1), 1)
    );

    agregarCoautorAPublicacion(
        buscarPublicacion(201),
        buscarCoautor(buscarInvestigador(1), 2)
    );

    // Publicacion 202 tiene un coautor
    agregarCoautorAPublicacion(
        buscarPublicacion(202),
        buscarCoautor(buscarInvestigador(2), 3)
    );

    // Publicacion 203 tiene un coautor
    agregarCoautorAPublicacion(
        buscarPublicacion(203),
        buscarCoautor(buscarInvestigador(1), 1)
    );

    // Publicacion 204 tiene un coautor
    agregarCoautorAPublicacion(
        buscarPublicacion(204),
        buscarCoautor(buscarInvestigador(1), 2)
    );
}


// Carga todos los datos correspondientes a Persona B.
// Los investigadores y coautores deben existir antes de llamar esta funcion.
void precargarDatosB() {

    precargarRevistasB();
    precargarProyectosB();
    precargarPublicacionesB();
    precargarCitacionesB();
    precargarCoautoresPublicacionB();
}

//---------------------------------------------Consulta 3---------------------------------------------

void consultaPublicacionMasCitada() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    // Primero buscamos la mayor cantidad de citas
    int mayorCitas = primeraPublicacion->cantidadCitas;

    publicacion* aux = primeraPublicacion->sig;

    while (aux != primeraPublicacion) {

        if (aux->cantidadCitas > mayorCitas) {
            mayorCitas = aux->cantidadCitas;
        }

        aux = aux->sig;
    }


    // Luego mostramos todas las publicaciones que tengan esa cantidad
    cout << "Publicacion(es) con mayor cantidad de citas:" << endl;

    aux = primeraPublicacion;

    do {

        if (aux->cantidadCitas == mayorCitas) {
            mostrarPublicacion(aux);
        }

        aux = aux->sig;

    } while (aux != primeraPublicacion);

    cout << "Mayor cantidad de citas: " << mayorCitas << endl;
}

//---------------------------------------------Consulta 4---------------------------------------------

void consultaRevistaMayorImpacto() {

    if (primeraRevista == NULL) {
        cout << "No hay revistas registradas." << endl;
        return;
    }

    // Primero buscamos el mayor factor de impacto
    float mayorImpacto = primeraRevista->factorImpacto;

    revista* aux = primeraRevista->sig;

    while (aux != NULL) {

        if (aux->factorImpacto > mayorImpacto) {
            mayorImpacto = aux->factorImpacto;
        }

        aux = aux->sig;
    }


    // Luego mostramos todas las revistas que tengan ese valor
    cout << "Revista(s) con mayor factor de impacto:" << endl;

    aux = primeraRevista;

    while (aux != NULL) {

        if (aux->factorImpacto == mayorImpacto) {
            mostrarRevista(aux);
        }

        aux = aux->sig;
    }

    cout << "Mayor factor de impacto: "
         << mayorImpacto << endl;
}

//---------------------------------------------Consulta 7---------------------------------------------

// Cuenta cuantas publicaciones pertenecen a investigadores de un area.
int contarPublicacionesArea(areaInvestigacion* area) {

    if (area == NULL || primeraPublicacion == NULL) {
        return 0;
    }

    int total = 0;

    publicacion* pub = primeraPublicacion;

    do {

        if (pub->investigadorPrincipal != NULL &&
            pub->investigadorPrincipal->suArea == area) {

            total++;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);

    return total;
}


// Muestra el area o las areas con mayor cantidad de publicaciones.
void consultaAreaMasPublicaciones() {

    if (primeraArea == NULL) {
        cout << "No hay areas de investigacion registradas." << endl;
        return;
    }

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }


    // Primero buscamos la mayor cantidad de publicaciones.
    int mayorCantidad = 0;

    areaInvestigacion* area = primeraArea;

    while (area != NULL) {

        int cantidad = contarPublicacionesArea(area);

        if (cantidad > mayorCantidad) {
            mayorCantidad = cantidad;
        }

        area = area->sig;
    }


    cout << "Area(s) con mayor cantidad de publicaciones:" << endl;

    // Segundo recorrido para mostrar posibles empates.
    area = primeraArea;

    while (area != NULL) {

        int cantidad = contarPublicacionesArea(area);

        if (cantidad == mayorCantidad) {

            mostrarArea(area);

            cout << "Cantidad de publicaciones: "
                 << cantidad << endl;
        }

        area = area->sig;
    }
}

//---------------------------------------------Consulta 8---------------------------------------------

// Cuenta cuantas publicaciones existen en un anio especifico.
int contarPublicacionesAnio(int anio) {

    if (primeraPublicacion == NULL) {
        return 0;
    }

    int total = 0;

    publicacion* pub = primeraPublicacion;

    do {

        if (pub->anio == anio) {
            total++;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);

    return total;
}


// Muestra el anio o los anios con mayor produccion cientifica.
void consultaAnioMayorProduccion() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    int mayorCantidad = 0;

    publicacion* pub = primeraPublicacion;

    // Primera pasada: encontrar la mayor cantidad de publicaciones en un anio
    do {

        int cantidad = contarPublicacionesAnio(pub->anio);

        if (cantidad > mayorCantidad) {
            mayorCantidad = cantidad;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);


    cout << "Anio(s) con mayor produccion cientifica:" << endl;

    // Como la lista esta ordenada por anio, evitamos mostrar
    // el mismo anio mas de una vez.
    bool primerAnioMostrado = true;
    int ultimoAnioMostrado = 0;

    pub = primeraPublicacion;

    do {

        int cantidad = contarPublicacionesAnio(pub->anio);

        if (cantidad == mayorCantidad &&
            (primerAnioMostrado || pub->anio != ultimoAnioMostrado)) {

            cout << "Anio: " << pub->anio
                 << " | Publicaciones: " << cantidad
                 << endl;

            ultimoAnioMostrado = pub->anio;
            primerAnioMostrado = false;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);
}

//---------------------------------------------Consulta 9---------------------------------------------

// Cuenta al investigador principal y a todos los coautores.
int contarAutoresPublicacion(publicacion* pub) {

    if (pub == NULL) {
        return 0;
    }

    int total = contarCoautoresPublicacion(pub);

    // El investigador principal tambien cuenta como autor.
    if (pub->investigadorPrincipal != NULL) {
        total++;
    }

    return total;
}


// Muestra la publicacion o publicaciones con mayor cantidad de autores.
void consultaPublicacionMasAutores() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    int mayorCantidad = 0;

    publicacion* pub = primeraPublicacion;

    // Primera pasada: buscar la mayor cantidad de autores.
    do {

        int cantidad = contarAutoresPublicacion(pub);

        if (cantidad > mayorCantidad) {
            mayorCantidad = cantidad;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);


    cout << "Publicacion(es) con mayor cantidad de autores:" << endl;

    // Segunda pasada: mostrar todas las que empaten.
    pub = primeraPublicacion;

    do {

        int cantidad = contarAutoresPublicacion(pub);

        if (cantidad == mayorCantidad) {

            mostrarPublicacion(pub);

            cout << "Cantidad total de autores: "
                 << cantidad << endl;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);
}

//---------------------------------------------Consulta 10---------------------------------------------

// Cuenta cuantos articulos de un investigador fueron publicados en revistas Q1.
int contarArticulosQ1(investigador* inv) {

    if (inv == NULL) {
        return 0;
    }

    int total = 0;

    pubInvestigador* aux = inv->publicaciones;

    while (aux != NULL) {

        publicacion* pub = aux->laPublicacion;

        if (pub != NULL &&
            pub->tipo == "Articulo" &&
            pub->suRevista != NULL &&
            pub->suRevista->cuartil == "Q1") {

            total++;
        }

        aux = aux->sig;
    }

    return total;
}


// Muestra el investigador o investigadores con mas articulos en revistas Q1.
void consultaInvestigadorMasArticulosQ1() {

    if (primerInvestigador == NULL) {
        cout << "No hay investigadores registrados." << endl;
        return;
    }

    int mayorCantidad = 0;

    investigador* inv = primerInvestigador;

    // Primera pasada: encontrar la mayor cantidad.
    while (inv != NULL) {

        int cantidad = contarArticulosQ1(inv);

        if (cantidad > mayorCantidad) {
            mayorCantidad = cantidad;
        }

        inv = inv->sig;
    }


    if (mayorCantidad == 0) {
        cout << "No hay articulos publicados en revistas Q1." << endl;
        return;
    }


    cout << "Investigador(es) con mas articulos en revistas Q1:" << endl;

    // Segunda pasada: mostrar posibles empates.
    inv = primerInvestigador;

    while (inv != NULL) {

        int cantidad = contarArticulosQ1(inv);

        if (cantidad == mayorCantidad) {

            mostrarInvestigador(inv);

            cout << "Articulos publicados en revistas Q1: "
                 << cantidad << endl;
        }

        inv = inv->sig;
    }
}

//---------------------------------------------Reporte 2---------------------------------------------

void reportePublicacionesPorAnio() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    cout << "PUBLICACIONES ORGANIZADAS POR ANIO" << endl;

    publicacion* pub = primeraPublicacion;
    int anioActual = -1;

    do {

        // Como la lista circular ya esta ordenada por anio,
        // cuando cambia el anio mostramos un nuevo encabezado.
        if (pub->anio != anioActual) {

            anioActual = pub->anio;

            cout << endl;
            cout << "Anio " << anioActual << ":" << endl;
        }

        mostrarPublicacion(pub);

        pub = pub->sig;

    } while (pub != primeraPublicacion);
}

//---------------------------------------------Reporte 3---------------------------------------------

void reportePublicacionesPorCitas() {

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    // Primero contamos cuantas publicaciones existen.
    int cantidad = 0;

    publicacion* pub = primeraPublicacion;

    do {
        cantidad++;
        pub = pub->sig;
    } while (pub != primeraPublicacion);


    // Creamos un arreglo de punteros.
    // No copiamos las publicaciones, solamente guardamos sus direcciones.
    publicacion** orden = new publicacion*[cantidad];

    pub = primeraPublicacion;

    for (int i = 0; i < cantidad; i++) {
        orden[i] = pub;
        pub = pub->sig;
    }


    // Ordenamiento por insercion:
    // mayor cantidad de citas primero.
    for (int i = 1; i < cantidad; i++) {

        publicacion* actual = orden[i];
        int j = i - 1;

        while (j >= 0 &&
               orden[j]->cantidadCitas < actual->cantidadCitas) {

            orden[j + 1] = orden[j];
            j--;
        }

        orden[j + 1] = actual;
    }


    cout << "PUBLICACIONES ORDENADAS POR CANTIDAD DE CITAS" << endl;

    for (int i = 0; i < cantidad; i++) {

        cout << endl;
        cout << "Posicion " << (i + 1) << ":" << endl;

        mostrarPublicacion(orden[i]);
    }


    delete[] orden;
}

//---------------------------------------------Reporte 4---------------------------------------------

void reporteRevistasFactorImpacto() {

    if (primeraRevista == NULL) {
        cout << "No hay revistas registradas." << endl;
        return;
    }

    cout << "REVISTAS Y SU FACTOR DE IMPACTO" << endl;
    cout << endl;

    revista* rev = primeraRevista;

    while (rev != NULL) {

        cout << "ID: " << rev->idRevista
             << " | Revista: " << rev->nombre
             << " | Factor de impacto: " << rev->factorImpacto
             << " | Cuartil: " << rev->cuartil
             << endl;

        rev = rev->sig;
    }
}

//---------------------------------------------Reporte 6---------------------------------------------

void reportePublicacionesDeRevista(int idRevista) {

    revista* rev = buscarRevista(idRevista);

    if (rev == NULL) {
        cout << "No existe una revista con ese ID." << endl;
        return;
    }

    cout << "PUBLICACIONES DE LA REVISTA: "
         << rev->nombre << endl;
    cout << endl;

    mostrarPublicacionesRevista(rev);
}

//---------------------------------------------Reporte 7---------------------------------------------

void reportePublicacionesDeArea(int idArea) {

    areaInvestigacion* area = buscarArea(idArea);

    if (area == NULL) {
        cout << "No existe un area de investigacion con ese ID." << endl;
        return;
    }

    if (primeraPublicacion == NULL) {
        cout << "No hay publicaciones registradas." << endl;
        return;
    }

    cout << "PUBLICACIONES DEL AREA: "
         << area->nombreArea << endl;
    cout << endl;

    bool encontrada = false;

    publicacion* pub = primeraPublicacion;

    do {

        if (pub->investigadorPrincipal != NULL &&
            pub->investigadorPrincipal->suArea == area) {

            mostrarPublicacion(pub);
            encontrada = true;
        }

        pub = pub->sig;

    } while (pub != primeraPublicacion);


    if (!encontrada) {
        cout << "No hay publicaciones asociadas a esta area." << endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------------------------------------------------



// ---------------------------------------------MENU DE CONSULTAS---------------------------------------------

void menuConsultas() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                    CONSULTAS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Investigador con mayor indice H" << endl;
        cout << "2. Investigador con mas citas" << endl;
        cout << "3. Publicacion con mas citas" << endl;
        cout << "4. Revista con mayor factor de impacto" << endl;
        cout << "5. Investigador con mas coautores" << endl;
        cout << "6. Universidad con mas investigadores" << endl;
        cout << "7. Area con mas publicaciones" << endl;
        cout << "8. Anio con mayor produccion cientifica" << endl;
        cout << "9. Publicacion con mas autores" << endl;
        cout << "10. Investigador con mas articulos Q1" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1:
            consultaMayorIndiceH();
            break;

        case 2:
            consultaMasCitas();
            break;

        case 3:
            consultaPublicacionMasCitada();
            break;

        case 4:
            consultaRevistaMayorImpacto();
            break;

        case 5:
            consultaMasCoautores();
            break;

        case 6:
            consultaUniversidadMasInvestigadores();
            break;

        case 7:
            consultaAreaMasPublicaciones();
            break;

        case 8:
            consultaAnioMayorProduccion();
            break;

        case 9:
            consultaPublicacionMasAutores();
            break;

        case 10:
            consultaInvestigadorMasArticulosQ1();
            break;

        case 0:
            cout << "Volviendo al menu principal..." << endl;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU DE REPORTES---------------------------------------------

void menuReportes() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                     REPORTES" << endl;
        cout << "====================================================" << endl;
        cout << "1. Investigadores con sus publicaciones" << endl;
        cout << "2. Publicaciones organizadas por anio" << endl;
        cout << "3. Publicaciones ordenadas por cantidad de citas" << endl;
        cout << "4. Revistas y factor de impacto" << endl;
        cout << "5. Red de coautoria de un investigador" << endl;
        cout << "6. Publicaciones de una revista" << endl;
        cout << "7. Publicaciones de un area" << endl;
        cout << "8. Investigadores agrupados por universidad" << endl;
        cout << "9. Investigadores ordenados por indice H" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1:
            reporteInvestigadoresConPublicaciones();
            break;

        case 2:
            reportePublicacionesPorAnio();
            break;

        case 3:
            reportePublicacionesPorCitas();
            break;

        case 4:
            reporteRevistasFactorImpacto();
            break;

        case 5: {
            int idInvestigador;

            cout << "Digite el ID del investigador: ";
            cin >> idInvestigador;

            reporteRedCoautoria(idInvestigador);

            break;
        }

        case 6: {
            int idRevista;

            cout << "Digite el ID de la revista: ";
            cin >> idRevista;

            reportePublicacionesDeRevista(idRevista);

            break;
        }

        case 7: {
            int idArea;

            cout << "Digite el ID del area: ";
            cin >> idArea;

            reportePublicacionesDeArea(idArea);

            break;
        }

        case 8:
            reporteInvestigadoresPorUniversidad();
            break;

        case 9:
            reporteInvestigadoresPorIndiceH();
            break;

        case 0:
            cout << "Volviendo al menu principal..." << endl;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU INSERTAR DATOS---------------------------------------------

void menuInsertarDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                   INSERTAR DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Insertar investigador" << endl;
        cout << "2. Insertar revista" << endl;
        cout << "3. Insertar area de investigacion" << endl;
        cout << "4. Insertar publicacion" << endl;
        cout << "5. Insertar proyecto" << endl;
        cout << "6. Insertar universidad" << endl;
        cout << "7. Insertar coautor a un investigador" << endl;
        cout << "8. Insertar citacion a una publicacion" << endl;
        cout << "9. Asociar coautor a una publicacion" << endl;
        cout << "0. Volver a gestion de datos" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        // -------------------------------------------------
        // INVESTIGADOR
        // -------------------------------------------------
        case 1: {

            int id;
            int idUniversidad;
            int idArea;

            string nombre;
            string pais;
            string correo;

            cout << "Digite el ID del investigador: ";
            cin >> id;

            cout << "Digite el nombre completo: ";
            getline(cin >> ws, nombre);

            cout << "Digite el ID de la universidad: ";
            cin >> idUniversidad;

            cout << "Digite el pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el ID del area de investigacion: ";
            cin >> idArea;

            cout << "Digite el correo: ";
            getline(cin >> ws, correo);

            insertarInvestigador(
                id,
                nombre,
                buscarUniversidad(idUniversidad),
                pais,
                buscarArea(idArea),
                correo,
                0
            );

            break;
        }


        // -------------------------------------------------
        // REVISTA
        // -------------------------------------------------
        case 2: {

            int id;
            float factorImpacto;

            string nombre;
            string editorial;
            string pais;
            string cuartil;

            cout << "Digite el ID de la revista: ";
            cin >> id;

            cout << "Digite el nombre de la revista: ";
            getline(cin >> ws, nombre);

            cout << "Digite la editorial: ";
            getline(cin >> ws, editorial);

            cout << "Digite el pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el factor de impacto: ";
            cin >> factorImpacto;

            cout << "Digite el cuartil (Q1, Q2, Q3 o Q4): ";
            cin >> cuartil;

            insertarRevista(
                id,
                nombre,
                editorial,
                pais,
                factorImpacto,
                cuartil
            );

            break;
        }


        // -------------------------------------------------
        // AREA
        // -------------------------------------------------
        case 3: {

            int id;

            string nombre;
            string descripcion;

            cout << "Digite el ID del area: ";
            cin >> id;

            cout << "Digite el nombre del area: ";
            getline(cin >> ws, nombre);

            cout << "Digite la descripcion: ";
            getline(cin >> ws, descripcion);

            insertarArea(
                id,
                nombre,
                descripcion
            );

            break;
        }


        // -------------------------------------------------
        // PUBLICACION
        // -------------------------------------------------
        case 4: {

            int id;
            int anio;
            int cantidadCitas;

            int idInvestigador;
            int idRevista;
            int idProyecto;

            string titulo;
            string tipo;
            string doi;

            cout << "Digite el ID de la publicacion: ";
            cin >> id;

            cout << "Digite el titulo: ";
            getline(cin >> ws, titulo);

            cout << "Digite el anio: ";
            cin >> anio;

            cout << "Digite el tipo (Articulo, Libro o Conferencia): ";
            getline(cin >> ws, tipo);

            cout << "Digite la cantidad de citas: ";
            cin >> cantidadCitas;

            cout << "Digite el DOI: ";
            getline(cin >> ws, doi);

            cout << "Digite el ID del investigador principal: ";
            cin >> idInvestigador;

            cout << "Digite el ID de la revista: ";
            cin >> idRevista;

            cout << "Digite el ID del proyecto: ";
            cin >> idProyecto;

            bool insertada = insertarPublicacion(
                id,
                titulo,
                anio,
                tipo,
                cantidadCitas,
                doi,
                buscarInvestigador(idInvestigador),
                buscarRevista(idRevista),
                buscarProyecto(idProyecto)
            );

            if (insertada) {
                actualizarTodosLosIndicesH();
            }

            break;
        }


        // -------------------------------------------------
        // PROYECTO
        // -------------------------------------------------
        case 5: {

            int id;
            float financiamiento;
            int anioInicio;
            int anioFinalizacion;
            int idInvestigador;

            string nombre;

            cout << "Digite el ID del proyecto: ";
            cin >> id;

            cout << "Digite el nombre del proyecto: ";
            getline(cin >> ws, nombre);

            cout << "Digite el financiamiento: ";
            cin >> financiamiento;

            cout << "Digite el anio de inicio: ";
            cin >> anioInicio;

            cout << "Digite el anio de finalizacion: ";
            cin >> anioFinalizacion;

            cout << "Digite el ID del investigador responsable: ";
            cin >> idInvestigador;

            insertarProyecto(
                id,
                nombre,
                financiamiento,
                anioInicio,
                anioFinalizacion,
                buscarInvestigador(idInvestigador)
            );

            break;
        }


        // -------------------------------------------------
        // UNIVERSIDAD
        // -------------------------------------------------
        case 6: {

            int id;
            int ranking;

            string nombre;
            string pais;

            cout << "Digite el ID de la universidad: ";
            cin >> id;

            cout << "Digite el nombre de la universidad: ";
            getline(cin >> ws, nombre);

            cout << "Digite el pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el ranking: ";
            cin >> ranking;

            insertarUniversidad(
                id,
                nombre,
                pais,
                ranking
            );

            break;
        }


        // -------------------------------------------------
        // COAUTOR
        // -------------------------------------------------
        case 7: {

            int idInvestigador;
            int idCoautor;
            int publicacionesConjuntas;

            string nombre;
            string universidad;

            cout << "Digite el ID del investigador: ";
            cin >> idInvestigador;

            cout << "Digite el ID del coautor: ";
            cin >> idCoautor;

            cout << "Digite el nombre del coautor: ";
            getline(cin >> ws, nombre);

            cout << "Digite la universidad del coautor: ";
            getline(cin >> ws, universidad);

            cout << "Digite la cantidad de publicaciones conjuntas: ";
            cin >> publicacionesConjuntas;

            insertarCoautor(
                buscarInvestigador(idInvestigador),
                idCoautor,
                nombre,
                universidad,
                publicacionesConjuntas
            );

            break;
        }


        // -------------------------------------------------
        // CITACION
        // -------------------------------------------------
        case 8: {

            int idPublicacionCitada;
            int idCitacion;
            int anio;
            int idPublicacionCitante;
            int idAutorCitante;

            cout << "Digite el ID de la publicacion citada: ";
            cin >> idPublicacionCitada;

            cout << "Digite el ID de la citacion: ";
            cin >> idCitacion;

            cout << "Digite el anio de la citacion: ";
            cin >> anio;

            cout << "Digite el ID de la publicacion citante: ";
            cin >> idPublicacionCitante;

            cout << "Digite el ID del autor citante: ";
            cin >> idAutorCitante;

            insertarCitacion(
                buscarPublicacion(idPublicacionCitada),
                idCitacion,
                anio,
                buscarPublicacion(idPublicacionCitante),
                buscarInvestigador(idAutorCitante)
            );

            break;
        }


        // -------------------------------------------------
        // COAUTOR DE PUBLICACION
        // -------------------------------------------------
        case 9: {

            int idPublicacion;
            int idInvestigador;
            int idCoautor;

            cout << "Digite el ID de la publicacion: ";
            cin >> idPublicacion;

            cout << "Digite el ID del investigador al que pertenece el coautor: ";
            cin >> idInvestigador;

            investigador* inv =
                buscarInvestigador(idInvestigador);

            if (inv == NULL) {
                cout << "Investigador no encontrado." << endl;
                break;
            }

            cout << "Digite el ID del coautor: ";
            cin >> idCoautor;

            agregarCoautorAPublicacion(
                buscarPublicacion(idPublicacion),
                buscarCoautor(inv, idCoautor)
            );

            break;
        }


        case 0:

            cout << "Volviendo a gestion de datos..." << endl;

            break;


        default:

            cout << "Opcion invalida." << endl;

            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU BUSCAR DATOS---------------------------------------------

void menuBuscarDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                    BUSCAR DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Buscar investigador" << endl;
        cout << "2. Buscar revista" << endl;
        cout << "3. Buscar area de investigacion" << endl;
        cout << "4. Buscar publicacion" << endl;
        cout << "5. Buscar proyecto" << endl;
        cout << "6. Buscar universidad" << endl;
        cout << "7. Buscar coautor de un investigador" << endl;
        cout << "8. Buscar citacion de una publicacion" << endl;
        cout << "9. Buscar coautor de una publicacion" << endl;
        cout << "0. Volver a gestion de datos" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1: {
            int id;

            cout << "Digite el ID del investigador: ";
            cin >> id;

            mostrarInvestigador(buscarInvestigador(id));

            break;
        }

        case 2: {
            int id;

            cout << "Digite el ID de la revista: ";
            cin >> id;

            mostrarRevista(buscarRevista(id));

            break;
        }

        case 3: {
            int id;

            cout << "Digite el ID del area: ";
            cin >> id;

            mostrarArea(buscarArea(id));

            break;
        }

        case 4: {
            int id;

            cout << "Digite el ID de la publicacion: ";
            cin >> id;

            mostrarPublicacion(buscarPublicacion(id));

            break;
        }

        case 5: {
            int id;

            cout << "Digite el ID del proyecto: ";
            cin >> id;

            mostrarProyecto(buscarProyecto(id));

            break;
        }

        case 6: {
            int id;

            cout << "Digite el ID de la universidad: ";
            cin >> id;

            mostrarUniversidad(buscarUniversidad(id));

            break;
        }

        case 7: {
            int idInvestigador;
            int idCoautor;

            cout << "Digite el ID del investigador: ";
            cin >> idInvestigador;

            investigador* inv = buscarInvestigador(idInvestigador);

            if (inv == NULL) {
                cout << "Investigador no encontrado." << endl;
                break;
            }

            cout << "Digite el ID del coautor: ";
            cin >> idCoautor;

            mostrarCoautor(
                buscarCoautor(inv, idCoautor)
            );

            break;
        }

        case 8: {
            int idPublicacion;
            int idCitacion;

            cout << "Digite el ID de la publicacion: ";
            cin >> idPublicacion;

            publicacion* pub = buscarPublicacion(idPublicacion);

            if (pub == NULL) {
                cout << "Publicacion no encontrada." << endl;
                break;
            }

            cout << "Digite el ID de la citacion: ";
            cin >> idCitacion;

            mostrarCitacion(
                buscarCitacion(pub, idCitacion)
            );

            break;
        }

        case 9: {
            int idPublicacion;
            int idCoautor;

            cout << "Digite el ID de la publicacion: ";
            cin >> idPublicacion;

            publicacion* pub = buscarPublicacion(idPublicacion);

            if (pub == NULL) {
                cout << "Publicacion no encontrada." << endl;
                break;
            }

            cout << "Digite el ID del coautor: ";
            cin >> idCoautor;

            mostrarCoautor(
                buscarCoautorPublicacion(pub, idCoautor)
            );

            break;
        }

        case 0:
            cout << "Volviendo a gestion de datos..." << endl;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU MOSTRAR DATOS---------------------------------------------

void menuMostrarDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                   MOSTRAR DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Mostrar investigadores" << endl;
        cout << "2. Mostrar revistas" << endl;
        cout << "3. Mostrar areas de investigacion" << endl;
        cout << "4. Mostrar publicaciones" << endl;
        cout << "5. Mostrar proyectos" << endl;
        cout << "6. Mostrar universidades" << endl;
        cout << "7. Mostrar coautores de un investigador" << endl;
        cout << "8. Mostrar citaciones de una publicacion" << endl;
        cout << "9. Mostrar coautores de una publicacion" << endl;
        cout << "0. Volver a gestion de datos" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1:
            mostrarInvestigadores();
            break;

        case 2:
            mostrarRevistas();
            break;

        case 3:
            mostrarAreas();
            break;

        case 4:
            mostrarPublicaciones();
            break;

        case 5:
            mostrarProyectos();
            break;

        case 6:
            mostrarUniversidades();
            break;

        case 7: {
            int idInvestigador;

            cout << "Digite el ID del investigador: ";
            cin >> idInvestigador;

            mostrarCoautores(
                buscarInvestigador(idInvestigador)
            );

            break;
        }

        case 8: {
            int idPublicacion;

            cout << "Digite el ID de la publicacion: ";
            cin >> idPublicacion;

            mostrarCitaciones(
                buscarPublicacion(idPublicacion)
            );

            break;
        }

        case 9: {
            int idPublicacion;

            cout << "Digite el ID de la publicacion: ";
            cin >> idPublicacion;

            mostrarCoautoresPublicacion(
                buscarPublicacion(idPublicacion)
            );

            break;
        }

        case 0:
            cout << "Volviendo a gestion de datos..." << endl;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU MODIFICAR DATOS---------------------------------------------

void menuModificarDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                  MODIFICAR DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Modificar universidad" << endl;
        cout << "2. Modificar investigador" << endl;
        cout << "3. Modificar revista" << endl;
        cout << "0. Volver a gestion de datos" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1: {

            int id;
            int ranking;

            string nombre;
            string pais;

            cout << "Digite el ID de la universidad: ";
            cin >> id;

            cout << "Digite el nuevo nombre: ";
            getline(cin >> ws, nombre);

            cout << "Digite el nuevo pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el nuevo ranking: ";
            cin >> ranking;

            modificarUniversidad(
                id,
                nombre,
                pais,
                ranking
            );

            break;
        }


        case 2: {

            int id;
            int idUniversidad;
            int idArea;

            string nombre;
            string pais;
            string correo;

            cout << "Digite el ID del investigador: ";
            cin >> id;

            cout << "Digite el nuevo nombre: ";
            getline(cin >> ws, nombre);

            cout << "Digite el ID de la nueva universidad: ";
            cin >> idUniversidad;

            cout << "Digite el nuevo pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el ID de la nueva area: ";
            cin >> idArea;

            cout << "Digite el nuevo correo: ";
            getline(cin >> ws, correo);

            modificarInvestigador(
                id,
                nombre,
                buscarUniversidad(idUniversidad),
                pais,
                buscarArea(idArea),
                correo
            );

            break;
        }


        case 3: {

            int id;
            float factorImpacto;

            string nombre;
            string editorial;
            string pais;
            string cuartil;

            cout << "Digite el ID de la revista: ";
            cin >> id;

            cout << "Digite el nuevo nombre: ";
            getline(cin >> ws, nombre);

            cout << "Digite la nueva editorial: ";
            getline(cin >> ws, editorial);

            cout << "Digite el nuevo pais: ";
            getline(cin >> ws, pais);

            cout << "Digite el nuevo factor de impacto: ";
            cin >> factorImpacto;

            cout << "Digite el nuevo cuartil (Q1, Q2, Q3 o Q4): ";
            cin >> cuartil;

            modificarRevista(
                id,
                nombre,
                editorial,
                pais,
                factorImpacto,
                cuartil
            );

            break;
        }


        case 0:

            cout << "Volviendo a gestion de datos..." << endl;
            break;


        default:

            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU ELIMINAR DATOS---------------------------------------------

void menuEliminarDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                   ELIMINAR DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Eliminar universidad" << endl;
        cout << "2. Eliminar investigador" << endl;
        cout << "3. Eliminar publicacion" << endl;
        cout << "0. Volver a gestion de datos" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1: {

            int id;

            cout << "Digite el ID de la universidad: ";
            cin >> id;

            eliminarUniversidad(id);

            break;
        }

        case 2: {

            int id;

            cout << "Digite el ID del investigador: ";
            cin >> id;

            eliminarInvestigador(id);

            actualizarTodosLosIndicesH();

            break;
        }

        case 3: {

            int id;

            cout << "Digite el ID de la publicacion: ";
            cin >> id;

            eliminarPublicacion(id);

            actualizarTodosLosIndicesH();

            break;
        }

        case 0:

            cout << "Volviendo a gestion de datos..." << endl;
            break;

        default:

            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}

// ---------------------------------------------MENU DE GESTION DE DATOS---------------------------------------------

void menuGestionDatos() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << "                 GESTION DE DATOS" << endl;
        cout << "====================================================" << endl;
        cout << "1. Insertar datos" << endl;
        cout << "2. Modificar datos" << endl;
        cout << "3. Eliminar datos" << endl;
        cout << "4. Buscar datos" << endl;
        cout << "5. Mostrar datos" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1:
            menuInsertarDatos();
            break;

        case 2:
            menuModificarDatos();
            break;

        case 3:
            menuEliminarDatos();
            break;

        case 4:
            menuBuscarDatos();
            break;

        case 5:
            menuMostrarDatos();
            break;

        case 0:
            cout << "Volviendo al menu principal..." << endl;
            break;

        default:
            cout << "Opcion invalida." << endl;
            break;
        }

    } while (opcion != 0);
}


// ---------------------------------------------MENU PRINCIPAL---------------------------------------------



void menuPrincipal() {

    int opcion;

    do {

        cout << endl;
        cout << "====================================================" << endl;
        cout << " SISTEMA DE GESTION DE PRODUCCION CIENTIFICA" << endl;
        cout << "====================================================" << endl;
        cout << "1. Gestion de datos" << endl;
        cout << "2. Consultas" << endl;
        cout << "3. Reportes" << endl;
        cout << "4. Metricas academicas" << endl;
        cout << "0. Salir" << endl;
        cout << "====================================================" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        cout << endl;

        switch (opcion) {

        case 1:

            menuGestionDatos();    

            break;


        case 2:

            menuConsultas();

            break;


        case 3:

            menuReportes();

            break;


        case 4: {

            int idInvestigador;

            cout << "Digite el ID del investigador: ";
            cin >> idInvestigador;

            investigador* inv =
                buscarInvestigador(idInvestigador);

            mostrarMetricas(inv);

            break;
        }


        case 0:

            cout << "Saliendo del sistema..." << endl;

            break;


        default:

            cout << "Opcion invalida." << endl;

            break;
        }

    } while (opcion != 0);
}


// ---------------------------------------------MAIN FINAL---------------------------------------------

int main() {

    // Inicializar listas principales
    primerInvestigador = NULL;
    primeraRevista = NULL;
    primeraArea = NULL;
    primeraPublicacion = NULL;
    primerProyecto = NULL;
    primeraUniversidad = NULL;


    // Cargar datos iniciales del sistema
    precargarDatosA();
    precargarDatosB();


    // Calcular los indices H con los datos precargados
    actualizarTodosLosIndicesH();


    // Iniciar el programa
    menuPrincipal();


    return 0;
}