#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	bool **plano;
}dosDPlano;

typedef struct{
	dosDPlano *plano;
}tresDPlano;

typedef struct nodo{
	tresDPlano pieza;
	struct nodo *siguiente;
}Lista;

bool sx = 0;
bool sy = 0;
bool sz = 0;
tresDPlano check;
int count = 0;
int stats[13];

void init(Lista*,tresDPlano*);
void liberar(Lista*,tresDPlano*);
void vaciar(tresDPlano*);
void copiar(tresDPlano*,tresDPlano*);
void mostrar(tresDPlano*,int);
void moverI(tresDPlano*);
void moverA(tresDPlano*);
void moverB(tresDPlano*);
void rotacionx(tresDPlano*,tresDPlano*,int);
void rotaciony(tresDPlano*,tresDPlano*,int);
void rotacionz(tresDPlano*,tresDPlano*,int);
void mostrarStats();
bool agregar(tresDPlano*,tresDPlano*,int,int,int);
void quitar(tresDPlano*,tresDPlano*,int,int,int);
bool resolver(tresDPlano*,Lista*,Lista*,int);
//Para optimizar codigo
bool imposible(tresDPlano*);
bool checar(tresDPlano*,int,int,int);
//Para checar que no se repitan en la lista
bool existe(tresDPlano*,Lista*);
bool iguales(tresDPlano*,tresDPlano*);
//Para copiar entre Listas
bool pasar(Lista*,Lista*);

int main(){
	//variables que se van a usar
	Lista piezas[13];
	Lista lista[13];
	tresDPlano plano;
	//se inicializan las variables
	init(piezas,&plano);
	resolver(&plano,piezas,lista,0);
	printf("\nPlano:\n");
	mostrar(&plano,4);
	//se destruyen las variables inicializadas
	liberar(piezas,&plano);
	getchar();
}


//resuelve el problema
bool resolver(tresDPlano *plano,Lista *piezas,Lista *lista,int n){
	if(n==13)return 1;
	if(n>1&&imposible(plano))return 0;
	pasar(&lista[n],&piezas[n]);
	do{
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				for(int z=0;z<4;z++){
					if(n==0){
						x=1;
						y=1;
						z=1;
					}
					if(agregar(plano,&lista[n].pieza,x,y,z)){
						stats[n]++;
						if(stats[n]>20000)stats[n]=0;
						count++;
						if(count==20000){
							count = 0;
							mostrarStats();
						}
						if(resolver(plano,piezas,lista,n+1)){
							printf("\nPieza %d, x %d, y %d, z %d\n",n+1,x,y,z);
							mostrar(&lista[n].pieza,3);
							return 1;
						}else{
							if(n==0)return 1;
							quitar(plano,&lista[n].pieza,x,y,z);
						}
					}
				}
			}
		}
		if(!pasar(&lista[n],lista[n].siguiente) || n==0){
			return 0;
		}
	}while(n!=0);
	
	return 0;
}

bool pasar(Lista *x,Lista *y){
	if(y==NULL){
		return 0;
	}
	x->pieza=y->pieza;
	x->siguiente=y->siguiente;
	return 1;
}

//checa algunas opciones del plano y dice si de puede resolver o no
bool imposible(tresDPlano *plano){
	sx=0;
	sy=0;
	sz=0;
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			for(int z=0;z<4;z++){
				if(plano->plano[x].plano[y][z])continue;
				vaciar(&check);
				if(!checar(plano,x,y,z)){
					return 1;
				}
			}
		}
	}
	return 0;
}

//checa espacios para ver si hay acceso en las tres dimensiones
bool checar(tresDPlano *plano,int x,int y,int z){
	if(check.plano[x].plano[y][z])return 0;
	check.plano[x].plano[y][z] = 1;
	//checar si hay espacio vacio en x
	if(!sx){
		switch(x){
			case 0:
				if(!plano->plano[x+1].plano[y][z])sx=1;
				break;
			case 3:
				if(!plano->plano[x-1].plano[y][z])sx=1;
				break;
			default:
				if(!plano->plano[x+1].plano[y][z])sx=1;
				if(!plano->plano[x-1].plano[y][z])sx=1;
				break;
		}
	}
	//checar si hay espacio vacio en y
	if(!sy){
		switch(y){
			case 0:
				if(!plano->plano[x].plano[y+1][z])sy=1;
				break;
			case 3:
				if(!plano->plano[x].plano[y-1][z])sy=1;
				break;
			default:
				if(!plano->plano[x].plano[y+1][z])sy=1;
				if(!plano->plano[x].plano[y-1][z])sy=1;
				break;
		}
	}
	//checar si hay espacio vacio en z
	if(!sz){
		switch(z){
			case 0:
				if(!plano->plano[x].plano[y][z+1])sz=1;
				break;
			case 3:
				if(!plano->plano[x].plano[y][z-1])sz=1;
				break;
			default:
				if(!plano->plano[x].plano[y][z+1])sz=1;
				if(!plano->plano[x].plano[y][z-1])sz=1;
				break;
		}
	}
	//verdadero ya que hay acceso en las tres dimensiones
	if(sx&&sy&&sz)return 1;
	//checamos si hay acceso a las dimensiones que faltan
	//para x
	if(sx){
		switch(x){
			case 0:
				if(!plano->plano[x+1].plano[y][z]&&checar(plano,x+1,y,z))return 1;
				break;
			case 3:
				if(!plano->plano[x-1].plano[y][z]&&checar(plano,x-1,y,z))return 1;
				break;
			default:
				if(!plano->plano[x+1].plano[y][z]&&checar(plano,x+1,y,z))return 1;
				if(!plano->plano[x-1].plano[y][z]&&checar(plano,x-1,y,z))return 1;
				break;
		}
	}
	//para y
	if(sy){
		switch(y){
			case 0:
				if(!plano->plano[x].plano[y+1][z]&&checar(plano,x,y+1,z))return 1;
				break;
			case 3:
				if(!plano->plano[x].plano[y-1][z]&&checar(plano,x,y-1,z))return 1;
				break;
			default:
				if(!plano->plano[x].plano[y+1][z]&&checar(plano,x,y+1,z))return 1;
				if(!plano->plano[x].plano[y-1][z]&&checar(plano,x,y-1,z))return 1;
				break;
		}
	}
	//para z
	if(sz){
		switch(z){
			case 0:
				if(!plano->plano[x].plano[y][z+1]&&checar(plano,x,y,z+1))return 1;
				break;
			case 3:
				if(!plano->plano[x].plano[y][z-1]&&checar(plano,x,y,z-1))return 1;
				break;
			default:
				if(!plano->plano[x].plano[y][z+1]&&checar(plano,x,y,z+1))return 1;
				if(!plano->plano[x].plano[y][z-1]&&checar(plano,x,y,z-1))return 1;
				break;
		}
	}
	return 0;
}

//para agregar en el plano
bool agregar(tresDPlano *plano,tresDPlano *aux,int px,int py,int pz){
	//checamos si se puede agregar
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			for(int k=0;k<3;k++){
				//checamos que este dentro del plano
				if(aux->plano[i].plano[j][k]&&(px+i>3||py+j>3||pz+k>3)){
					return 0;
				}
				//checamos que no colisione con otra pieza
				if(aux->plano[i].plano[j][k]&&plano->plano[i+px].plano[j+py][k+pz]){
					return 0;
				}
			}
		}
	}
	//agregamos al plano
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			for(int k=0;k<3;k++){
				if(aux->plano[i].plano[j][k]){
					plano->plano[i+px].plano[j+py][k+pz]=1;
				}
			}
		}
	}
	return 1;
}

//para quitar del plano
void quitar(tresDPlano *plano,tresDPlano *aux,int px,int py,int pz){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			for(int k=0;k<3;k++){
				if(aux->plano[i].plano[j][k]){
					plano->plano[i+px].plano[j+py][k+pz]=0;
				}
			}
		}
	}
}

//hace rotacion en la pieza
void rotacionx(tresDPlano *x,tresDPlano *tmp,int n){
	//se escoje cuantas rotaciones se quiere
	switch(n){
		case 1:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[i].plano[j][k]=x->plano[i].plano[2-k][j];
					}
				}
			}
			break;
		case 2:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[i].plano[j][k]=x->plano[i].plano[2-j][2-k];
					}
				}
			}
			break;
		case 3:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[i].plano[j][k]=x->plano[i].plano[k][2-j];
					}
				}
			}
			break;
		default:
			return;
	}
	//movemos la pieza temporal para que quede en un lugar deseado
	//movemos a la izquierda
	moverI(tmp);
	//movemos hacia arriba
	moverA(tmp);
	//copiamos la pieza temporal a la pieza
	copiar(x,tmp);
}

//hace rotacion en la pieza
void rotaciony(tresDPlano *x,tresDPlano *tmp,int n){
	//se escoje cuantas rotaciones se quiere
	switch(n){
		case 1:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[k][i]=x->plano[2-k].plano[j][i];
					}
				}
			}
			break;
		case 2:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[k][i]=x->plano[2-j].plano[2-k][i];
					}
				}
			}
			break;
		case 3:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[k][i]=x->plano[k].plano[2-j][i];
					}
				}
			}
			break;
		default:
			return;
	}
	//movemos la pieza temporal para que quede en un lugar deseado
	//movemos hacia arriba
	moverA(tmp);
	//movemos plano hacia abajo
	moverB(tmp);
	//copiamos la pieza temporal a la pieza
	copiar(x,tmp);
}

//hace rotacion en la pieza
void rotacionz(tresDPlano *x,tresDPlano *tmp,int n){
	//se escoje cuantas rotaciones se quiere
	switch(n){
		case 1:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[i][k]=x->plano[2-k].plano[i][j];
					}
				}
			}
			break;
		case 2:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[i][k]=x->plano[2-j].plano[i][2-k];
					}
				}
			}
			break;
		case 3:
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					for(int k=0;k<3;k++){
						tmp->plano[j].plano[i][k]=x->plano[k].plano[i][2-j];
					}
				}
			}
			break;
		default:
				return;
	}
	//movemos la pieza temporal para que quede en un lugar deseado
	//movemos a la izquierda
	moverI(tmp);
	//movemos plano hacia abajo
	moverB(tmp);
	//copiamos la pieza temporal a la pieza
	copiar(x,tmp);
}

//inicializa las variables
void init(Lista *piezas,tresDPlano *plano){
	Lista *lista = NULL;
	Lista *listaux = NULL;
	tresDPlano tmp;
	tresDPlano aux;
	//se crea el plano donde se van a colocar las piezas
	plano->plano=(dosDPlano*)calloc(4,sizeof(dosDPlano));
	check.plano=(dosDPlano*)calloc(4,sizeof(dosDPlano));
	for(int i=0;i<4;i++){
		plano->plano[i].plano=(bool**)calloc(4,sizeof(bool*));
		check.plano[i].plano=(bool**)calloc(4,sizeof(bool*));
		for(int j=0;j<4;j++){
			plano->plano[i].plano[j]=(bool*)calloc(4,sizeof(bool));
			check.plano[i].plano[j]=(bool*)calloc(4,sizeof(bool));
		}
	}
	//se crean las piezas sin valor y rutas
	//piezas=(tresDPlano*)calloc(13,sizeof(tresDPlano));
	for(int n=0;n<13;n++){
		piezas[n].siguiente=NULL;
		piezas[n].pieza.plano=(dosDPlano*)calloc(3,sizeof(dosDPlano));
		for(int i=0;i<3;i++){
			piezas[n].pieza.plano[i].plano=(bool**)calloc(3,sizeof(bool*));
			for(int j=0;j<3;j++){
				piezas[n].pieza.plano[i].plano[j]=(bool*)calloc(3,sizeof(bool));
			}
		}
	}
	//se crean los valores para las piezas
	//primera pieza
	piezas[0].pieza.plano[0].plano[0][1]=1;
	piezas[0].pieza.plano[1].plano[0][0]=1;
	piezas[0].pieza.plano[1].plano[0][1]=1;
	piezas[0].pieza.plano[1].plano[0][2]=1;
	piezas[0].pieza.plano[2].plano[0][1]=1;
	//segunda pieza
	piezas[2].pieza.plano[0].plano[0][0]=1;
	piezas[2].pieza.plano[0].plano[0][1]=1;
	piezas[2].pieza.plano[0].plano[0][2]=1;
	piezas[2].pieza.plano[0].plano[1][1]=1;
	piezas[2].pieza.plano[1].plano[0][1]=1;
	//tercera pieza
	piezas[3].pieza.plano[0].plano[0][0]=1;
	piezas[3].pieza.plano[0].plano[1][0]=1;
	piezas[3].pieza.plano[0].plano[1][1]=1;
	piezas[3].pieza.plano[1].plano[1][1]=1;
	piezas[3].pieza.plano[1].plano[2][1]=1;
	//cuarta pieza
	piezas[4].pieza.plano[0].plano[0][0]=1;
	piezas[4].pieza.plano[0].plano[0][1]=1;
	piezas[4].pieza.plano[0].plano[0][2]=1;
	piezas[4].pieza.plano[0].plano[1][0]=1;
	piezas[4].pieza.plano[1].plano[0][2]=1;
	//quinta pieza
	piezas[1].pieza.plano[0].plano[0][1]=1;
	piezas[1].pieza.plano[0].plano[0][2]=1;
	piezas[1].pieza.plano[1].plano[0][0]=1;
	piezas[1].pieza.plano[1].plano[0][1]=1;
	piezas[1].pieza.plano[2].plano[0][0]=1;
	//sexta pieza
	piezas[12].pieza.plano[0].plano[0][0]=1;
	piezas[12].pieza.plano[0].plano[1][0]=1;
	piezas[12].pieza.plano[0].plano[0][1]=1;
	piezas[12].pieza.plano[1].plano[1][0]=1;
	//septima pieza
	piezas[5].pieza.plano[0].plano[0][0]=1;
	piezas[5].pieza.plano[0].plano[0][1]=1;
	piezas[5].pieza.plano[0].plano[0][2]=1;
	piezas[5].pieza.plano[0].plano[1][0]=1;
	piezas[5].pieza.plano[1].plano[0][0]=1;
	//octava pieza
	piezas[6].pieza.plano[0].plano[0][0]=1;
	piezas[6].pieza.plano[0].plano[1][0]=1;
	piezas[6].pieza.plano[0].plano[1][1]=1;
	piezas[6].pieza.plano[0].plano[2][1]=1;
	piezas[6].pieza.plano[1].plano[0][0]=1;
	//novena pieza
	piezas[7].pieza.plano[0].plano[0][0]=1;
	piezas[7].pieza.plano[0].plano[1][0]=1;
	piezas[7].pieza.plano[0].plano[1][2]=1;
	piezas[7].pieza.plano[0].plano[1][1]=1;
	piezas[7].pieza.plano[1].plano[0][0]=1;
	//decima pieza
	piezas[8].pieza.plano[0].plano[0][1]=1;
	piezas[8].pieza.plano[0].plano[1][0]=1;
	piezas[8].pieza.plano[0].plano[1][2]=1;
	piezas[8].pieza.plano[0].plano[1][1]=1;
	piezas[8].pieza.plano[0].plano[2][0]=1;
	//decimaprimera pieza
	piezas[9].pieza.plano[0].plano[0][0]=1;
	piezas[9].pieza.plano[0].plano[0][1]=1;
	piezas[9].pieza.plano[0].plano[0][2]=1;
	piezas[9].pieza.plano[0].plano[1][1]=1;
	piezas[9].pieza.plano[1].plano[0][2]=1;
	//decimasegunda pieza
	piezas[10].pieza.plano[0].plano[0][0]=1;
	piezas[10].pieza.plano[0].plano[0][1]=1;
	piezas[10].pieza.plano[0].plano[0][2]=1;
	piezas[10].pieza.plano[0].plano[1][1]=1;
	piezas[10].pieza.plano[1].plano[1][1]=1;
	//decimatercera pieza
	piezas[11].pieza.plano[0].plano[0][1]=1;
	piezas[11].pieza.plano[0].plano[0][2]=1;
	piezas[11].pieza.plano[0].plano[1][0]=1;
	piezas[11].pieza.plano[0].plano[1][1]=1;
	piezas[11].pieza.plano[1].plano[0][1]=1;
	
	//se le asigna espacio a esa pieza temporal y a la pieza temporal
	tmp.plano=(dosDPlano*)calloc(3,sizeof(dosDPlano));
	aux.plano=(dosDPlano*)calloc(3,sizeof(dosDPlano));
	for(int i=0;i<3;i++){
		tmp.plano[i].plano=(bool**)calloc(3,sizeof(bool*));
		aux.plano[i].plano=(bool**)calloc(3,sizeof(bool*));
		for(int j=0;j<3;j++){
			tmp.plano[i].plano[j]=(bool*)calloc(3,sizeof(bool));
			aux.plano[i].plano[j]=(bool*)calloc(3,sizeof(bool));
		}
	}
	
	//agregamos todas las posibles combinaciones de las piezas de acuerdo a sus rotaciones
	for(int n=0;n<13;n++){
		lista=&piezas[n];
		for(int rx=0;rx<4;rx++){
			for(int ry=0;ry<4;ry++){
				for(int rz=0;rz<4;rz++){
					copiar(&aux,&piezas[n].pieza);
					rotacionx(&aux,&tmp,rx);
					rotaciony(&aux,&tmp,ry);
					rotacionz(&aux,&tmp,rz);
					if(!existe(&aux,&piezas[n])){
						listaux=malloc(sizeof(Lista));
						listaux->pieza.plano=(dosDPlano*)calloc(3,sizeof(dosDPlano));
						for(int i=0;i<3;i++){
							listaux->pieza.plano[i].plano=(bool**)calloc(3,sizeof(bool*));
							for(int j=0;j<3;j++){
								listaux->pieza.plano[i].plano[j]=(bool*)calloc(3,sizeof(bool));
							}
						}
						listaux->siguiente=NULL;
						copiar(&listaux->pieza,&aux);
						lista->siguiente=listaux;
						lista=lista->siguiente;
					}
				}
			}
		}
	}

	//se destruye la pieza temporal y la pieza auxiliar
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			free(tmp.plano[i].plano[j]);
			free(aux.plano[i].plano[j]);
		}
		free(tmp.plano[i].plano);
		free(aux.plano[i].plano);
	}
	free(tmp.plano);
	free(aux.plano);
	
}

//revisa que no se repitan en la lista
bool existe(tresDPlano *x,Lista *lista){
	Lista *aux;
	aux=lista;
	while(aux!=NULL){
		if(iguales(x,&aux->pieza)){
			return 1;
		}
		aux=aux->siguiente;
	}
	return 0;
}

//revisa si son iguales
bool iguales(tresDPlano *x,tresDPlano *y){
	for(int i=0;i< 3;i++){
		for(int j=0;j< 3;j++){
			for(int k=0;k< 3;k++){
				if(x->plano[i].plano[j][k]!=y->plano[i].plano[j][k]){
					return 0;
				}
			}
		}
	}
	return 1;
}

//destruye las variables inizializadas
void liberar(Lista *piezas,tresDPlano *plano){
	Lista *lista;	
	//se destruye el plano
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			free(plano->plano[i].plano[j]);
			free(check.plano[i].plano[j]);
		}
		free(plano->plano[i].plano);
		free(check.plano[i].plano);
	}
	free(plano->plano);
	free(check.plano);
	//se destruyen las piezas y rutas
	for(int n=1;n<13;n++){
		lista=&piezas[n];
		while(lista!=NULL){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					free(lista->pieza.plano[i].plano[j]);
				}
				free(lista->pieza.plano[i].plano);
			}
			free(lista->pieza.plano);
			lista=lista->siguiente;
		}
	}
}

//copia una pieza a otra
void copiar(tresDPlano *x,tresDPlano *y){
	for(int i=0;i< 3;i++){
		for(int j=0;j< 3;j++){
			for(int k=0;k< 3;k++){
				x->plano[i].plano[j][k]=y->plano[i].plano[j][k];
			}
		}
	}
}

//muestra una pieza
void mostrar(tresDPlano *x,int size){
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			for(int k=0;k<size;k++){
				printf("%d ",x->plano[i].plano[j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

//mueve a la izquierda una pieza
void moverI(tresDPlano *x){
	while(1){
		//checa si se puede mover
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(x->plano[i].plano[j][0]){
					return;
				}
			}
		}
		//movemos
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				for(int k=0;k<2;k++){
					x->plano[i].plano[j][k]=x->plano[i].plano[j][k+1];
					x->plano[i].plano[j][k+1]=0;
				}
				
			}
		}
	}
}

//mueve hacia arriba una pieza
void moverA(tresDPlano *x){
	while(1){
		//checa si se puede mover
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(x->plano[i].plano[0][j]){
					return;
				}
			}
		}
		//movemos
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				for(int k=0;k<2;k++){
					x->plano[i].plano[k][j]=x->plano[i].plano[k+1][j];
					x->plano[i].plano[k+1][j]=0;
				}
			}
		}
	}
}

//mueve plano hacia abajo una pieza
void moverB(tresDPlano *x){
	while(1){
		//checa si se puede mover
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(x->plano[0].plano[i][j]){
					return;
				}
			}
		}
		//movemos
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				for(int k=0;k<2;k++){
					x->plano[k].plano[i][j]=x->plano[k+1].plano[i][j];
					x->plano[k+1].plano[i][j]=0;
				}
			}
		}
	}
}

//vaciar plano
void vaciar(tresDPlano* plano){
	for(int i=0;i< 4;i++){
		for(int j=0;j< 4;j++){
			for(int k=0;k< 4;k++){
				plano->plano[i].plano[j][k]=0;
			}
		}
	}
}

void mostrarStats(){
	printf("\n");
	for(int i=0;i<13;i++)printf("%d. %d\n",i+1,stats[i]);
	printf("\n");
}