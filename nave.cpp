#include<stdio.h>
#include<windows.h>
#include<conio.h>

//definimos los controles los cuales serán las flechas del teclado
//con ayuda del código ascii
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77


/*Función que nos ayuda a posicionar el cursor
en cualquier parte de la ventana.*/
void gotoxy(int x, int y){

    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);

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

//Definimos la clase nave con los atributos y métodos necesarios
class NAVE{
    int x,y;
    int corazones;

public:
    //defininimos el constructor con una lista de inicialización por defecto
    //con el operador :, de esta manera evitamos tener que hacerlo por asignación
    //y reducimos lineas de código
    NAVE(int _x, int _y, int _corazones):x(_x),y(_y),corazones(_corazones){}
    void pintar();
    void mover();
    void borrar();
    void pintar_corazones();
};

//Método que nos ayuda a dibujar la nave en consola
void NAVE::pintar(){

    gotoxy(x,y); printf("  %c",30);
    gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);
}
//Método que nos ayuda a borrar la nave de la posicion anterior
void NAVE::borrar(){

    gotoxy(x,y);   printf("     ");
    gotoxy(x,y+1); printf("     ");
    gotoxy(x,y+2); printf("     ");
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
    gotoxy(64,2); printf("Salud");
    gotoxy(70,2); printf("     ");

    for(int i=0;i<corazones;i++){
        gotoxy(70+i,2); printf("%c",3);
    }
}

int main(){

    ocultarCursor(); // ocultamos el cursor de la consola para que no esté parpadeando
    NAVE N(7,7,3); // definimos un objeto de la clase nave con las coordenadas deseadas
    N.pintar(); // llamamos al método que nos ayuda a pintar la nave en consola
    N.pintar_corazones();
    pintar_limites();
    // defini9mos una variable booleana para usarla como condición de escape del ciclo while
    bool game_over = false;

    //ciclo que mantiene la partida mientras no ocurra un game over
    while(!game_over){

        N.mover(); // Método de la clase nave para detectar las teclas
        Sleep(30); // damos un tiempo de espera con el objetivo de no saturar la memoria
    }

    return 0;
}
