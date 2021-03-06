#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
using namespace std;

//definimos los controles los cuales ser?n las flechas del teclado
//con ayuda del c?digo ascii
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77


/*Funci?n que nos ayuda a posicionar el cursor
en cualquier parte de la ventana.*/
void gotoxy(int x, int y){

    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);

}

void reproducir_sonido(int nEfecto){

    if(nEfecto==1)PlaySound(TEXT("disparo.wav"),NULL,SND_ASYNC);
    if(nEfecto==2)PlaySound(TEXT("Cubik2.wav"),NULL,SND_ASYNC);


}

void ocultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;
    cci.bVisible = FALSE;

    //Funcion que nos ayuda a ocultar el cursor
    // recibe dos parametros el primero el HANDLE
    //que identifica a la consola y el segundo
    //la referencia del console cursor info para poder modificarlo
    SetConsoleCursorInfo(hCon, &cci);

}

void pintar_limites(){
    for(int i=2; i<78;i++){
        gotoxy(i,3); printf("%c",205);
        gotoxy(i,33); printf("%c",205);
    }

    for(int i=4;i<33;i++){
        gotoxy(2,i); printf("%c",186);
        gotoxy(77,i); printf("%c",186);
    }

    gotoxy(2,3); printf("%c",201);
    gotoxy(2,33); printf("%c",200);
    gotoxy(77,3); printf("%c",187);
    gotoxy(77,33); printf("%c",188);

}

//Definimos la clase nave con los atributos y m?todos necesarios
class NAVE{
    int x,y;
    int corazones;
    int vidas;

public:
    //defininimos el constructor con una lista de inicializaci?n por defecto
    //con el operador :, de esta manera evitamos tener que hacerlo por asignaci?n
    //y reducimos lineas de c?digo
    NAVE(int _x, int _y, int _corazones, int _vidas):x(_x),y(_y),corazones(_corazones),vidas(_vidas){}
    void pintar();
    void mover();
    void borrar();
    void pintar_corazones();
    void morir();
    int getX(){return x;}
    int getY(){return y;}
    int getVidas(){return vidas;}
    void salud(){corazones--;}
};

//M?todo que nos ayuda a dibujar la nave en consola
void NAVE::pintar(){

    gotoxy(x,y); printf("  %c",30);
    gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);
}
//M?todo que nos ayuda a borrar la nave de la posicion anterior
void NAVE::borrar(){

    gotoxy(x,y);   printf("       ");
    gotoxy(x,y+1); printf("       ");
    gotoxy(x,y+2); printf("       ");
}

void NAVE::mover(){
    if(kbhit()){//detectar cuando se presiona una tecla
            char tecla = getch(); //obtener que tecla fue presionada
            borrar(); //borrar el caracter de la posicion inicial
            if(tecla==IZQUIERDA && x>3)x--;//disminuir x para mover caracter a la izquierda
            if(tecla==DERECHA && x+5 < 77)x++;// aumentar x para mover el caracter a la derecha
            if(tecla==ARRIBA && y>4)y--;// disminuir y para subir el caracter
            if(tecla==ABAJO && y+3<33)y++;// aumentar y para bajar el caracter
            if(tecla=='e')corazones--;
            pintar();// mover el cursor a la nueva posicion y pintar el caracter *
            pintar_corazones();
        }

}

void NAVE::pintar_corazones(){
    gotoxy(50,2); printf("Vidas %d",vidas);
    gotoxy(64,2); printf("Salud");
    gotoxy(70,2); printf("     ");

    for(int i=0;i<corazones;i++){
        gotoxy(70+i,2); printf("%c",3);
    }
}

void NAVE::morir(){
    if(corazones==0){
        borrar();
        gotoxy(x,y);  printf("   **   ");
        gotoxy(x,y+1);printf("  ****  ");
        gotoxy(x,y+2);printf("   **   ");
        Sleep(200);
        borrar();
        gotoxy(x,y);  printf(" * ** * ");
        gotoxy(x,y+1);printf("  ****  ");
        gotoxy(x,y+2);printf(" * ** * ");
        Sleep(200);
        vidas--;
        corazones = 3;
        pintar_corazones();
        borrar();
        pintar();

    }
}

class ASTEROIDE{
 int x,y;
 public:
     ASTEROIDE(int _x, int _y):x(_x),y(_y){}
     void pintar();
     void mover();
     void detectar_choque(NAVE &N);
     int getX(){return x;}
     int getY(){return y;}
};

void ASTEROIDE::pintar(){
    gotoxy(x,y); printf("%c",184);
}

void ASTEROIDE::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y>32){
        x = rand()%71 + 4;
        y = 4;
    }
    pintar();
}

void ASTEROIDE::detectar_choque(NAVE &N){
    if(x >= N.getX() && x<=N.getX()+5 && y>=N.getY() && y<=N.getY()+2 ){
        N.salud();
        N.pintar_corazones();
        N.borrar();
        N.pintar();
        x = rand()%71 + 4;
        y = 4;
    }

}

class BALA{
int x,y;
public:
    BALA(int _x, int _y):x(_x),y(_y){}
    int getX(){return x;}
    int getY(){return y;}
    void mover();
    bool fuera();
};

void BALA::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("*");
}

bool BALA::fuera(){

    if(y==4)return true;
    return false;
}

int main(){

    ocultarCursor(); // ocultamos el cursor de la consola para que no est? parpadeando
    NAVE N(37,30,3,3); // definimos un objeto de la clase nave con las coordenadas deseadas
    N.pintar(); // llamamos al m?todo que nos ayuda a pintar la nave en consola
    N.pintar_corazones();
    pintar_limites();
    list<ASTEROIDE*> A;
    list<ASTEROIDE*>::iterator itA;

    for(int i=0;i<5;i++){
        A.push_back(new ASTEROIDE(rand()%75+3,rand()%5+4));
    }

    list<BALA*> B;
    list<BALA*>::iterator it;



    // defini9mos una variable booleana para usarla como condici?n de escape del ciclo while
    bool game_over = false;
    int puntos = 0;

    //ciclo que mantiene la partida mientras no ocurra un game over
    while(!game_over){

    gotoxy(4,2); printf("Puntos   %d",puntos);
        if(kbhit()){
            char tecla = getch();
            if(tecla=='a'){
                B.push_back(new BALA(N.getX()+2,N.getY()-1));
                reproducir_sonido(1);

            }
        }

        for(it=B.begin(); it!= B.end();it++){
            (*it)->mover();
            if((*it)->fuera()){
                gotoxy((*it)->getX(),(*it)->getY());printf(" ");
                delete(*it);
                it = B.erase(it);
            }
        }

        for(itA = A.begin();itA!=A.end();itA++){
            (*itA)->mover();
            (*itA)->detectar_choque(N);
        }

         for(itA = A.begin();itA!=A.end();itA++){

            for(it = B.begin();it!=B.end();it++){
                if((*itA)->getX()==(*it)->getX() && (((*itA)->getY() +1 == (*it)->getY()) || ((*itA)->getY() == (*it)->getY())) ){

                gotoxy((*it)->getX(),(*it)->getY()); printf(" ");
                delete(*it);
                it = B.erase(it);

                A.push_back(new ASTEROIDE(rand()%74+3, 4));
                gotoxy((*itA)->getX(),(*itA)->getY()); printf(" ");
                delete(*itA);
                itA = A.erase(itA);

                puntos+=5;

                }

            }
        }

        if(N.getVidas()==0)game_over=true;
        N.morir();
        N.mover(); // M?todo de la clase nave para detectar las teclas
        Sleep(30); // damos un tiempo de espera con el objetivo de no saturar la memoria
    }
    gotoxy(35,18); printf("Game Over");
    Sleep(3000);
    return 0;
}
