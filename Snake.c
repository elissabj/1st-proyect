/* INSTITUTO POLITECNICO NACIONAL
ESCUELA SUPERIOR DE COMPUTO
ALGORITMIA Y PROGRAMACION
PROYECTO FINAL
JUEGO DE SNAKE
INTEGRANTES: DELGADILLO ARRENDODO MELVIN IVAN 
             RAMOS GOMEZ ELISA 
GRUPO: 1CV2
*/

#include <stdio.h>
#include <time.h>
#include <allegro.h>
#define TAM 100
#define tam_ini 3
#define fil 20
#define col 30

BITMAP *buffer; //Bitmap son áreas de memoria que contienen imágenes rectangulares, almacenadas en arrays de packs de bytes
BITMAP *limite;
BITMAP *snake;
BITMAP *snake_cabeza;
BITMAP *snake_cola;
BITMAP *fondo;
BITMAP *comida;
BITMAP *menu_ini;
BITMAP *menu_op;

FILE   *archivo; //Guardará los datos de partida

int direccion, px[TAM], py[TAM],puntuacion,tamano,com,tiempo,puntuaciones[5]={0,0,0,0,0};
int	tiempo_def=150,dificultad_def=3,puntuacion_def=100;
char escenario[fil][col]=
{
	"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LFFFFFFFFFFFFFFFFFFFFFFFFFFFFL",
	"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
}; //la matriz del juego siendo l el limite

void organiza_puntuaciones();
void menu_snake();
void coloca_fondo();
void mueve_snake();
void inicializa_snake();
void aparece_comida();
void condiciones_snake();
void dibuja_snake();
void guarda_partida();
void continua_partida();
	
int main()
{	
	srand(time(NULL)); //para la comida aleatoria
	allegro_init(); //inicializacion de allegro
	install_keyboard(); 
	set_color_depth(32); //paleta de colores
	
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,1100,600,0,0); //tamaño de impresion
	
	buffer=create_bitmap(1100,600); //impresion del campo
	limite=load_bitmap("limite.bmp",NULL); //subida de imagenes
	fondo=load_bitmap("fondo.bmp",NULL);
	snake=load_bitmap("snake.bmp",NULL);
	snake_cabeza=load_bitmap("snake_cabeza.bmp",NULL);
	snake_cola=load_bitmap("snake_cola.bmp",NULL);
	comida=load_bitmap("comida.bmp",NULL);
	menu_ini=load_bitmap("menu_ini.bmp",NULL);
	menu_op=load_bitmap("menu_op.bmp",NULL);
	
	continua_partida();
	organiza_puntuaciones();
	menu_snake();
	
	while(!key[KEY_ESC])
	{
		textprintf(buffer,font,930,30,palette_color[15],"Puntuacion: %d",puntuacion);
		textprintf(buffer,font,930,90,palette_color[15],"Dificultad: %d",(15-(tiempo/10)));
		
		coloca_fondo();
		mueve_snake();
		aparece_comida();
		condiciones_snake();
		dibuja_snake();
		blit(buffer,screen,0,0,0,0,1100,600);
		clear(buffer);
		rest(tiempo);
		
		if(key[KEY_ESC])
		{
			guarda_partida();
			rest(300); //rest funcion de allegro de temporizador está esperando que pase el tiempo requerido.
			menu_snake();
		}
	}
	
}
END_OF_MAIN();

void menu_snake()
{
	int nivel=0;
	
	while(!key[KEY_ENTER] && !key[KEY_ESC])
	{	
		blit(menu_ini,screen,0,0,0,0,1100,600);
		
		if(key[KEY_C])
		{
			return;
		}
		
		if(key[KEY_O])
		{
			clear(screen);
			while(!key[KEY_ENTER] && !key[KEY_ESC])
			{
				blit(menu_op,screen,0,0,0,0,1100,600);
				textprintf(screen,font,100,100,palette_color[15],"SELECCIONE LA DIFICULTAD");
				textprintf(screen,font,150,130,palette_color[15],"Presiona 1.- Facil");
				textprintf(screen,font,150,160,palette_color[15],"Presiona 2.- Medio");
				textprintf(screen,font,150,190,palette_color[15],"Presiona 3.- Dificil");
				textprintf(screen,font,100,250,palette_color[15],"PUNTUACIONES MAS ALTAS");
				textprintf(screen,font,150,280,palette_color[15],"1.- %d",puntuaciones[0]);
				textprintf(screen,font,150,310,palette_color[15],"2.- %d",puntuaciones[1]);
				textprintf(screen,font,150,340,palette_color[15],"3.- %d",puntuaciones[2]);
				textprintf(screen,font,150,370,palette_color[15],"4.- %d",puntuaciones[3]);
				textprintf(screen,font,150,400,palette_color[15],"5.- %d",puntuaciones[4]);
				textprintf(screen,font,450,500,palette_color[15],"PRESIONA ENTER PARA COMENZAR");
				
				
				if(key[KEY_1]) nivel=1;
				else if(key[KEY_2]) nivel=2;
				else if(key[KEY_3]) nivel=3;
				
				if(nivel==1)
				{
					textprintf(screen,font,150,130,palette_color[12],"Presiona 1.- Facil");
					tiempo_def=150;
					dificultad_def=3;
					puntuacion_def=100;
				}
				else if(nivel==2)
				{
					textprintf(screen,font,150,160,palette_color[12],"Presiona 2.- Medio");
					tiempo_def=100;
					dificultad_def=2;
					puntuacion_def=200;
				}
				else if(nivel==3)
				{
					textprintf(screen,font,150,190,palette_color[12],"Presiona 3.- Dificil");
					tiempo_def=50;
					dificultad_def=1;
					puntuacion_def=400;
				}
				rest(20);
			}
		}
		
	}
	inicializa_snake();
}

void inicializa_snake()
{
	int i;	
	tamano=tam_ini;
	puntuacion=0;
	direccion=0;
	com=0;
	tiempo=tiempo_def;
	for(i=0;i<tamano;i++)
	{
		px[i]=15*30;
		py[i]=10*30;	
	}
}
void coloca_fondo()
{
	int i,j;
	
	for(i=0;i<fil;i++)
	{
		for(j=0;j<col;j++)
		{
			if(escenario[i][j]=='L') draw_sprite(buffer,limite,j*30,i*30);
			else if(escenario[i][j]=='F') draw_sprite(buffer,fondo,j*30,i*30);
			else if(escenario[i][j]=='C') draw_sprite(buffer,comida,j*30,i*30);
		}
	}
}

void mueve_snake()
{
	int i,k;
	
	if(key[KEY_RIGHT] && direccion!=1) direccion=0;
	else if(key[KEY_LEFT] && direccion!=0) direccion=1;
	else if(key[KEY_UP] && direccion!=3) direccion=2;
	else if(key[KEY_DOWN] && direccion!=2) direccion=3;
	
	for(i=0;i<tamano;i++)
	{
		px[tamano-i]=px[tamano-i-1];
		py[tamano-i]=py[tamano-i-1];
	}
	
	if(direccion==0) px[0]+=30;
	else if(direccion==1) px[0]-=30;
	else if(direccion==2) py[0]-=30;
	else if(direccion==3) py[0]+=30;
}

void aparece_comida()
{
	int i,j;
	
	for(i=0;i<fil;i++)
	{
		for(j=0;j<col;j++)
		{
			if(escenario[i][j]=='C')
			{
				com++;
			}
		}
	}
	
	if(com==0)
	{
		i=1+rand()%18;
		j=1+rand()%28;
		escenario[i][j]='C';
	}
	
}

void condiciones_snake()
{
	int i,v=2;
	
	if(escenario[py[0]/30][px[0]/30]=='L')
	{ 
		do
		{
			v-=1;
		} while(v!=0);
			if (v==0)
			{
			organiza_puntuaciones();
			inicializa_snake();
			menu_snake();		
			}
	
	}
	
	if(escenario[py[0]/30][px[0]/30]=='C')
	{
		escenario[py[0]/30][px[0]/30]='F';
		puntuacion+=puntuacion_def;
		tamano++;
		com=0;
		tiempo-=dificultad_def;
		px[tamano-1]=px[tamano-2];
		py[tamano-1]=py[tamano-2];
	}
	
	for(i=1;i<tamano;i++)
	{
		if(px[0]==px[i] && py[0]==py[i] && puntuacion!=0 && v!=0)
		{
			do
		{
			v-=1;
		} while(v!=0);
			if (v==0)
			{
			organiza_puntuaciones();
			inicializa_snake();
			menu_snake();		
			}
		}
	}
}

void dibuja_snake()
{
	int i,j;
	
	for(i=1;i<tamano-1;i++)
	{
		blit(snake,buffer,0,0,px[i],py[i],30,30);
	}
	
		blit(snake_cabeza,buffer,30*direccion,0,px[0],py[0],30,30);
		blit(snake_cola,buffer,0,0,px[tamano-1],py[tamano-1],30,30);
}

void organiza_puntuaciones()
{
	int i,j,aux;
	
	puntuaciones[5]=puntuacion;

	for(i=5;i>0;i--)
	{
		while(puntuaciones[i]>puntuaciones[i-1])
		{
			aux=puntuaciones[i-1];
			puntuaciones[i-1]=puntuaciones[i];
			puntuaciones[i]=aux;
		}
	}
}

void guarda_partida()
{
	int i,j;
	
	archivo = fopen("archivo.txt","wt");
	
	fprintf(archivo,"%d,",direccion);
	fprintf(archivo,"%d,",puntuacion);
	fprintf(archivo,"%d,",tamano);
	fprintf(archivo,"%d,",com);
	fprintf(archivo,"%d,",tiempo);
	fprintf(archivo,"%d,",tiempo_def);
	fprintf(archivo,"%d,",dificultad_def);
	fprintf(archivo,"%d,",puntuacion_def);
	
	for(i=0;i<TAM;i++)
	{
	fprintf(archivo,"%d,",px[i]);
	fprintf(archivo,"%d,",py[i]);
	}
	
	for(i=0;i<=5;i++)
	{
	fprintf(archivo,"%d,",puntuaciones[i]);
	}
	
	for(i=0;i<fil;i++)
	{
		for(j=0;j<col;j++)
		{
			fprintf(archivo,"%c,",escenario[i][j]);
		}
	}
}

void continua_partida()
{
	int i,j;
	
	archivo = fopen("archivo.txt","r");
	
	fscanf(archivo,"%d,",&direccion);
	fscanf(archivo,"%d,",&puntuacion);
	fscanf(archivo,"%d,",&tamano);
	fscanf(archivo,"%d,",&com);
	fscanf(archivo,"%d,",&tiempo);
	fscanf(archivo,"%d,",&tiempo_def);
	fscanf(archivo,"%d,",&dificultad_def);
	fscanf(archivo,"%d,",&puntuacion_def);
	
	for(i=0;i<TAM;i++)
	{
		fscanf(archivo,"%d,",&px[i]);
		fscanf(archivo,"%d,",&py[i]);
	}
	
	for(i=0;i<=5;i++)
	{
		fscanf(archivo,"%d,",&puntuaciones[i]);
	}
	
	for(i=0;i<fil;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(archivo,"%c,",&escenario[i][j]);
		}
	}
}
