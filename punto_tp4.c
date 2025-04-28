#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int idTareas = 999;

typedef struct {
    int TareaID;        //Numérico autoincremental comenzando en 1000
    char *Descripcion;  //
    int Duracion;       // entre 10 – 100
}Tarea;

typedef struct Nodo{
    Tarea tarea;
    struct Nodo *siguiente;
}Nodo;

void limpiarbuffer();
int validarEntero();

Nodo * crearListaVacia();
Nodo * crearNodo(Tarea unaTarea);
Tarea *crearTarea();

void insertarNodo(Nodo ** start, Nodo * nodoNuevo);
void mostrarLista(Nodo *start);
void mostrarNodo(Nodo * nodo);

Nodo * quitarNodo(Nodo ** start, int ID);
void eliminarNodo(Nodo * nodoEliminado);

Nodo * buscarNodoPorID(Nodo *start, int ID);
Nodo * buscarNodoPorPalabra(Nodo *start, char *palabraClave);

void menu(Nodo * pendientes, Nodo * realizadas);
void menuCrearTareas(Nodo ** pendientes);
void menuFinalizarTarea(Nodo ** pendientes, Nodo ** realizadas);
void menuListarTareas(Nodo *pendientes,Nodo *realizadas);
void menuBuscarTarea(Nodo * pendientes, Nodo * realizadas);

int main(){

    Nodo *startPendientes, *startRealizadas;
    startPendientes = crearListaVacia();
    startRealizadas = crearListaVacia();
    menu(startPendientes,startRealizadas);
    return 0;
}


void limpiarbuffer(){
    char c;
    while((c=getchar()) != '\n' && c != EOF );
}
Nodo * crearListaVacia(){
    return NULL;
}
Nodo * crearNodo(Tarea unaTarea){
    Nodo *nuevoNodo = (Nodo *) malloc(sizeof(Nodo));
    nuevoNodo->tarea = unaTarea;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}
void insertarNodo(Nodo ** start, Nodo * nodoNuevo){
    nodoNuevo->siguiente = *start;
    *start = nodoNuevo;
}

Tarea * crearTarea(){
    Tarea * nueva = (Tarea *) malloc (sizeof(Tarea));
    char buff[100];
    int duracion;
    printf("\nDescriba la tarea a realizar:");
    scanf("%s",&buff);
    limpiarbuffer();
    printf("\nIngrese la duracion de la tarea:");
    scanf("%d",&duracion);
    getchar();
    nueva->Descripcion = (char *) malloc(strlen(buff) + 1);
    nueva->TareaID = ++idTareas;
    strcpy(nueva->Descripcion,buff);
    nueva->Duracion = duracion;
    return nueva;
}

void mostrarLista(Nodo *start){
    Nodo * aux = start;
    while(aux != NULL){
        mostrarNodo(aux);
        aux = aux->siguiente;
    }
}
void mostrarNodo(Nodo * nodo){
    printf("\n-----------------------\n");
    printf("Identifidacor de tarea :%d -- Duracion de tarea: %d ", nodo->tarea.TareaID, nodo->tarea.Duracion);
    printf("\nDescripcion: %s", nodo->tarea.Descripcion);
    printf("\n-----------------------\n");
}

Nodo * quitarNodo(Nodo ** start, int ID){
    Nodo ** aux = start;
    while(aux != NULL && (*aux)->tarea.TareaID != ID){
        aux = &(*aux)->siguiente;
    }
    if(*aux){
        Nodo * temp = *aux;
        *aux = (*aux)->siguiente;
        temp->siguiente = NULL;
        return temp;
    }
    return NULL;
}
void liberarLista(Nodo *start){
    Nodo *temp;
    while (start != NULL) {
        temp = start;
        start = start->siguiente;

        free(temp->tarea.Descripcion);
        eliminarNodo(temp);
    }
}
void eliminarNodo(Nodo * nodoEliminado){
    free(nodoEliminado);
}

Nodo * buscarNodoPorID(Nodo *start, int ID){
    Nodo * aux = start;
    while(aux != NULL){
        if (aux->tarea.TareaID == ID)
        {
            return aux;
        }
        aux = aux->siguiente;
    }
    return NULL;
}
Nodo *buscarNodoPorPalabra(Nodo *start, char *palabraClave) {
    Nodo *resultado = NULL;  // Lista nueva para resultados
    Nodo *aux = start;

    while(aux != NULL) {
        if (strstr(aux->tarea.Descripcion, palabraClave)) {
            Nodo *nuevoNodo = crearNodo(aux->tarea);
            insertarNodo(&resultado, nuevoNodo);    
        }
        aux = aux->siguiente;
    }

    return resultado;
}
void menu(Nodo * pendientes, Nodo * realizadas){
    int eleccion = 1;
    do{
        printf("\nMenu de tareas\n");
       
        printf("Ingrese 1 para registrar una nueva tarea\n");
        printf("Ingrese 2 para marcar una tarea como finalizada\n");
        printf("Ingrese 3 para listar las tareas \n");
        printf("Ingrese 4 para buscar una tarea \n");
        printf("Ingrese 0 para salir del programa \n");
        eleccion = validarEntero();
       
        switch (eleccion)
        {
            case 1:
            menuCrearTareas(&pendientes);
            break;
            case 2:
            printf("\nlistado de tareas pendientes\n");
            mostrarLista(pendientes);
            menuFinalizarTarea(&pendientes,&realizadas);
            break;
            case 3:
            menuListarTareas(pendientes,realizadas);
            break;
            case 4:
            menuBuscarTarea(pendientes,realizadas);
            break;
            default:
            break;
        }
    }while(eleccion != 0);
}
void menuCrearTareas(Nodo ** pendientes){

    Tarea * unaTarea = (Tarea *) malloc (sizeof(Tarea));
    unaTarea = crearTarea();
    Nodo * nuevoNodo = crearNodo(*unaTarea);
    insertarNodo(pendientes,nuevoNodo);

    char opcion[4];
    do{
        printf("\nDesea agregar otra tarea?\n'si' o 'no': ");
        scanf("%3s",&opcion);
        if(strcmp(opcion, "si") == 0){
            menuCrearTareas(pendientes);
        }
    }while(strcmp(opcion, "no") != 0);

}
void menuFinalizarTarea(Nodo ** pendientes, Nodo ** realizadas){
    int id;
    printf("Escriba el ID de la tarea finalizada: ");
    scanf("%d",&id);
    if(buscarNodoPorID(*pendientes,id) != NULL){
        Nodo * nodoFinalizado = quitarNodo(pendientes,id);
        insertarNodo(realizadas,nodoFinalizado);
        printf("Se registró la finalizacion de la tarea... \n\n");
    }else{
        printf("No se encontró la tarea para el id: %d ingresado", id);
    }
}
void menuListarTareas(Nodo *pendientes,Nodo *realizadas){
    int i = 0;
    do{
        printf("\nIngrese 1 para listar tareas pendientes");
        printf("\nIngrese 2 para listar tareas realizadas");
        printf("\nIngrese 0 para volver");
        printf("\n\nSu opcion:");
        scanf("%d",&i);
        switch (i)
        {
        case 1:
        mostrarLista(pendientes);
            break;
        case 2:
        mostrarLista(realizadas);
            break;
        case 0:
        break;                        
        default:
        printf("\ningrese una opcion valida...\n");
            break;
        }
    }while(i != 0);
}
void menuBuscarTarea(Nodo * pendientes, Nodo * realizadas){
    int i = 0;
    Nodo * recuperado = NULL;
    int idNum;
    char palabraClave[20];
    do{
        printf("\nIngrese 1 para buscar tareas por ID");
        printf("\nIngrese 2 para buscar tareas por palabra clave");
        printf("\nIngrese 0 para volver");
        printf("\n\nSu opcion:");
        scanf("%d",&i);
        switch (i)
        {
        case 1:                                     //busqueda por id
        printf("\n\nIngrese el ID de la tarea buscada:");
        scanf("%d",&idNum);
        recuperado = buscarNodoPorID(pendientes,idNum);
        if (recuperado != NULL)
        {
            printf("\nTarea PENDIENTE\n");
            mostrarNodo(recuperado);
        }else{
            recuperado = buscarNodoPorID(realizadas,idNum);
            if (recuperado != NULL){
                printf("\nTarea REALIZADA\n");
                mostrarNodo(recuperado);
            }else{
                printf("\nNo se encontro una tarea para el id: %d\n",idNum);
            }      
        }            
            break;
        case 2:                                     //busqueda por palabra clave
        printf("\nIngrese una palabra clave para buscar tareas: ");
        scanf("%19s",palabraClave);
        Nodo * resultados = buscarNodoPorPalabra(pendientes,palabraClave);
        if (resultados != NULL)
        {
            printf("\nTareas PENDIENTES\n");
            mostrarLista(resultados);
            liberarLista(resultados);
        }
        resultados = buscarNodoPorPalabra(realizadas,palabraClave);
        if (resultados != NULL)
        {
            printf("\nTareas REALIZADAS\n");
            mostrarLista(resultados);
            liberarLista(resultados);
        }
        free(resultados);
        break;
        case 0:
        break;                        
        default:
        printf("\ningrese una opcion valida...\n");
            break;
        }
    }while(i != 0);
}

int validarEntero(){
    int numero;
    do{
        printf("Su opcion: ");
        scanf("%d",&numero);
        if(numero< 0 || numero >4){
            printf("\n**Ingrese una opcion valida**\n");
        }
    }while(numero< 0 || numero >4);
    return numero;
}
