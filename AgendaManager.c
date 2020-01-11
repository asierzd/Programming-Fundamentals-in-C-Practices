#include <stdio.h>
#include <stdlib.h>
#define MAXNOM 21
struct Tpersona
{
    char nombre[MAXNOM];
    int telefono;
    struct Tpersona *siguiente;
};
struct Tagenda
{
    int numContactos;
    struct Tpersona *cabecera;
};
void borrarLista(struct Tagenda *l);
void guardarAgenda(struct Tagenda l);
void listarContactos(struct Tagenda l);
int modificarTelefono(struct Tagenda *l, char nombre[MAXNOM],int nuevotlf);
int esMenor(char c1[MAXNOM], char c2[MAXNOM]);
int aniadirContacto(struct Tagenda *l);
int eliminarContacto(char nombre[MAXNOM], struct Tagenda *l);
int nomIguales(char c1[MAXNOM], char c2[MAXNOM]);
struct Tpersona* buscarContacto(char nombre[MAXNOM], struct Tagenda *l);
void menu();
void leerFichero(struct Tagenda *l);
int insertar(struct Tagenda *l, struct Tpersona *p);
void inicializarLista(struct Tagenda *l);
int main()
{
    struct Tagenda agenda;
    struct Tpersona *aux;
    char nomaux[MAXNOM];
    int opc,nuevotlf;
    printf("GESTOR DE AGENDA DE CONTACTOS:\n\n");
    inicializarLista(&agenda);
    leerFichero(&agenda);
    do
    {
        menu();
        scanf("%d",&opc);
        switch(opc)
        {
        case 0:
            printf("Fin del programa\n");
            break;
        case 1://Buscar Contacto
            if(agenda.numContactos>0)
            {
                printf("Escriba el nombre que quiere buscar: ");
                fflush(stdin);
                scanf("%s",nomaux);
                aux=buscarContacto(nomaux,&agenda);
                if(aux!=NULL)
                {
                    printf("Encontrado, datos:\n");
                    printf("Nombre: %s, Telefono: %d\n",aux->nombre,aux->telefono);
                }
                else printf("No encontrado\n");
            }
            else printf("No hay contactos\n");
            break;
        case 2:// Eliminar Contacto
            if(agenda.numContactos>0)
            {
                printf("Escriba el nombre de la persona que quiere eliminar: ");
                fflush(stdin);
                scanf("%s",nomaux);
                if(eliminarContacto(nomaux,&agenda))
                {
                    printf("Eliminado correctamente\n");
                }
                else printf("Error al eliminar contacto\n");
            }
            else printf("No hay contactos\n");
            break;
        case 3://Aniadir contacto
            if(agenda.numContactos<100)  // Limita a 100 contactos
            {
                if(aniadirContacto(&agenda))
                {
                    printf("Aniadido correctamente\n");
                }
                else printf("Error al aniadir\n");
            }
            else printf("No se puede aniadir mas de 100 contactos\n");
            break;
        case 4://Modificar telefono
            if(agenda.numContactos>0)
            {
                printf("Introduzca el nombre: ");
                fflush(stdin);
                scanf("%s",nomaux);
                printf("Introduzca el nuevo telefono: ");
                scanf("%d",&nuevotlf);
                modificarTelefono(&agenda,nomaux,nuevotlf);
            }
            else printf("No hay contactos\n");
            break;
        case 5://Listar
            if(agenda.numContactos>0)
            {
                listarContactos(agenda);
            }
            else printf("No hay contactos\n");
            break;
        case 6://Guardar
            guardarAgenda(agenda);
            break;
        case 7://Num Elementos
            printf("%d\n",agenda.numContactos);
            break;
        case 8://Borrar todo
            borrarLista(&agenda);
            break;
        default:
            printf("Has pulsado una opcion incorrecta\n");
            break;
        }
    }
    while(opc!=0);
    borrarLista(&agenda);

    return 0;
}

void borrarLista(struct Tagenda *l)
{
    struct Tpersona *aux;
    aux=l->cabecera;
    while(aux!=NULL)
    {
        l->cabecera=aux->siguiente;
        free(aux);
        aux=l->cabecera;
    }
    l->numContactos=0;
}

void guardarAgenda(struct Tagenda l)
{
    FILE *F;
    struct Tpersona *aux;

    if((F=fopen("contactos.txt","w"))==NULL) printf("Error al abrir fichero");
    else
    {
        aux=l.cabecera;
        while(aux!=NULL)
        {
            fprintf(F,"%s\n",aux->nombre);
            fprintf(F,"%d\n",aux->telefono);
            aux=aux->siguiente;
        }
        fclose(F);
    }

}

void listarContactos(struct Tagenda l)
{
    struct Tpersona *aux;
    aux=l.cabecera;
    while(aux!=NULL)
    {
        printf("%s : %d\n",aux->nombre,aux->telefono);
        aux=aux->siguiente;
    }
}

int modificarTelefono(struct Tagenda *l, char nombre[MAXNOM],int nuevotlf)
{
    struct Tpersona *aux;
    aux=l->cabecera;
    int modif=0;
    while(aux!=NULL && !modif)
    {
        if(nomIguales(nombre,aux->nombre))
        {
            aux->telefono=nuevotlf;
            modif=1;
        }
        else aux=aux->siguiente;
    }
    return modif;
}

int aniadirContacto(struct Tagenda *l)
{
    struct Tpersona *nuevo;
    int aniadido=0;
    nuevo=(struct Tpersona*)malloc(sizeof(struct Tpersona));
    printf("\nIntroduzca el nombre: ");
    fflush(stdin);
    scanf("%s",nuevo->nombre);
    printf("Introduzca el numero de telefono: ");
    scanf("%d",&nuevo->telefono);
    aniadido=insertar(l,nuevo);
    return aniadido;
}

int eliminarContacto(char nombre[MAXNOM], struct Tagenda *l)
{
    int borrado=0;
    struct Tpersona *aux;
    struct Tpersona *ant;
    aux=l->cabecera;
    ant=l->cabecera;

    if(buscarContacto(nombre,l)!=NULL)
    {
        while(aux!=NULL && !borrado)
        {
            if(l->numContactos==1)
            {
                free(aux);
                l->cabecera=NULL;
                borrado=1;
            }
            else if(nomIguales(nombre,aux->nombre))
            {
                if(aux==l->cabecera)
                {
                    l->cabecera=aux->siguiente;
                    free(aux);
                    borrado=1;
                }
                else if(aux->siguiente==NULL)
                {
                    ant->siguiente=NULL;
                    free(aux);
                    borrado=1;
                }
                else
                {
                    ant->siguiente=aux->siguiente;
                    free(aux);
                    borrado=1;
                }
            }
            else
            {
                ant=aux;
                aux=aux->siguiente;
            }
        }
    }

    if(borrado) l->numContactos--;

    return borrado;
}

int nomIguales(char c1[MAXNOM], char c2[MAXNOM])
{
    int iguales=1,i=0;
    while(iguales && c1[i]!='\0' && c2[i]!='\0')
    {
        if(c1[i]!=c2[i]) iguales=0;
        else i++;
    }
    return iguales && c1[i]==c2[i];
}

struct Tpersona* buscarContacto(char nombre[MAXNOM], struct Tagenda *l)
{
    struct Tpersona *aux;
    int enc=0,fin=0,i=0;
    aux=l->cabecera;
    while(aux!=NULL && !enc && !fin)
    {
        if(nombre[0]<aux->nombre[0]) fin=1;
        else
        {
            if(nomIguales(aux->nombre,nombre))
            {
                enc=1;
            }
            else
            {
                i++;
                aux=aux->siguiente;
            }
        }
    }
    if(enc) return aux;
    else return NULL;
}

void menu()
{
    printf("\n1.- Buscar Contacto");
    printf("\n2.- Eliminar Contacto");
    printf("\n3.- Aniadir Contacto");
    printf("\n4.- Modificar (telefono) de un contacto");
    printf("\n5.- Listar contactos");
    printf("\n6.- Guardar agenda");
    printf("\n7.- Numero de personas en la lista");
    printf("\n8.- Borrar todas las personas de la lista");
    printf("\n0.- Acabar");
    printf("\nOPCION: ");
}

void leerFichero(struct Tagenda *l)
{
    FILE *F;
    struct Tpersona *aux;
    if((F=fopen("contactos.txt","r"))==NULL) printf("Error abriendo fichero para lectura\n");
    else
    {
        while(!feof(F))
        {
            aux=(struct Tpersona*)malloc(sizeof(struct Tpersona));
            fscanf(F,"%s",aux->nombre);
            if(aux->nombre[0]!='\0')  // Resuelve bug, aniadia persona nula
            {
                fscanf(F,"%d",&aux->telefono);
                insertar(l,aux);
            }
        }
        fclose(F);
    }
}

int insertar(struct Tagenda *l, struct Tpersona *p)
{
    int ins=0;
    struct Tpersona *aux,*ant;
    aux=l->cabecera;
    ant=l->cabecera;

    if(l->numContactos==0)
    {
        p->siguiente=l->cabecera;
        l->cabecera=p;
        ins=1;
    }
    else
    {
        while(aux!=NULL && !ins)
        {
            if(esMenor(aux->nombre,p->nombre)>0)
            {
                if(aux==l->cabecera)
                {
                    l->cabecera=p;
                    p->siguiente=aux;
                    ins=1;
                }
                else
                {
                    ant->siguiente=p;
                    p->siguiente=aux;
                    ins=1;
                }
            }
            else if(aux->siguiente==NULL)
            {
                aux->siguiente=p;
                ins=1;
            }
            else
            {
                ant=aux;
                aux=aux->siguiente;
            }
        }

    }

    if(ins) l->numContactos++;

    return ins;
}

int esMenor(char c1[MAXNOM], char c2[MAXNOM])  //strcmp
{
    int i=0,menor=0;
    while(menor == 0 && c1[i]!='\0' && c2[i]!='\0')
    {
        if (c1[i]>c2[i]) menor=1;
        else if(c1[i]<c2[i]) menor=-1;
        i++;
    }
    return menor;
}

void inicializarLista(struct Tagenda *l)
{
    l->cabecera=NULL;
    l->numContactos=0;
}
