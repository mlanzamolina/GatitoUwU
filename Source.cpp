//#include "stdafx.h"

#include <allegro5\allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#pragma warning(disable:4996)
typedef struct ALLEGRO_DISPLAY ALLEGRO_DISPLAY;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
using namespace std;
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string.h>
#include <fstream>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
std::chrono::time_point<chrono::system_clock> m_StartTime;
std::chrono::time_point<chrono::system_clock> m_EndTime;
static ALLEGRO_COLOR red;

bool  m_bRunning = false;
int score;
int scores[10];
int contScores = 0;
int contScoresF = 0;
void leerFile() {
	ifstream scoresIn("score.dat", ios::in);
	if (!scoresIn) {
		cout << "Error al intentar abrir el archivo scores.dat" << endl;
		return;
	}
	int score;

	while (scoresIn >> score) {
		scores[contScoresF] = score;
		contScoresF++;
	}
	scoresIn.close();
}
void ingresarFile(int score) {
	ofstream scoresOut("score.dat", ios::app);

	if (!scoresOut) {
		cout << "Error al intentar abrir archivo alumnos.dat" << endl;
		return;
	}

	scoresOut << score << "\n";

	scoresOut.close();

}

double elapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::system_clock> endTime;

	if (m_bRunning)
	{
		endTime = std::chrono::system_clock::now();
	}

	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}
double elapsedSeconds()
{
	return elapsedMilliseconds() / 1000.0;
}
void Start()
{
	m_StartTime = chrono::system_clock::now();
	m_bRunning = true;
}
void UltimasPartidas() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219) << endl;
	cout << "PLAYER" << endl;
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	for (int i = 0; i < contScores; i++) {
		ingresarFile(scores[i]);

	}
	leerFile();
	for (int i = 0; i < contScoresF; i++) {

		cout << "Partida " << i + 1 << " el puntaje del jugador fue de: " << scores[i] << endl;

	}



}




void juego()
{

	system("color E");
	red = al_map_rgb(255, 0, 0);
	const float FPS = 60;

	bool draw = true;
	bool done = false;
	//creando variables para funciones arreglo
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_BITMAP* bitmap = NULL;
	ALLEGRO_BITMAP* bitmapfueguito = NULL;
	ALLEGRO_BITMAP* bitmapFondoJetPack = NULL;
	//ALLEGRO_BITMAP* bitmapMoneda = NULL;
	ALLEGRO_BITMAP* bitmapGatitoPiso = NULL;
	ALLEGRO_BITMAP* bitmapGatitoMedioSalto = NULL;
	ALLEGRO_BITMAP* bitmapGatitoFullSalto = NULL;
	ALLEGRO_BITMAP* GO = NULL;
	al_init_font_addon();
	al_init_ttf_addon();

	al_install_audio();
	al_init_acodec_addon();
	//inicia el display
	al_init();
	al_reserve_samples(2);

	ALLEGRO_SAMPLE* soundEffect = al_load_sample("SoundEffect.wav");
	srand(time(NULL));
	int cancion = rand() % 3 + 1;

	ALLEGRO_SAMPLE* song = al_load_sample(("Song" + std::to_string(cancion) + ".ogg").c_str());
	ALLEGRO_FONT* font24 = al_load_font("ComicSans.ttf", 28, NULL);
	//(("file" + std::to_string(i)).c_str())
	ALLEGRO_SAMPLE_INSTANCE* songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	display = al_create_display(640, 480);//resolucion de la pantalla creada 640x480
	queue = al_create_event_queue();
	al_init_primitives_addon();
	al_install_keyboard();//instala funciones para el teclado
	al_register_event_source(queue, al_get_keyboard_event_source()); 
	al_register_event_source(queue, al_get_display_event_source(display));
	ALLEGRO_TIMER* timer = al_create_timer(1 / FPS);
	ALLEGRO_TIMER* timerstop = al_create_timer(1);
	al_init_image_addon();

	bitmap = al_load_bitmap("CanvasJuegoPantalla.png");//trae una variable con las imagenes

	bitmapfueguito = al_load_bitmap("fueguito.png");
	bitmapFondoJetPack = al_load_bitmap("CanvasJuegoPantallaDos.png");
	//bitmapMoneda = al_load_bitmap("Moneda.png");
	bitmapGatitoPiso = al_load_bitmap("ScratchCatFinal.png");
	GO = al_load_bitmap("GameOver.png");

	assert(bitmap != NULL);

	bool running = true;
	float x = 0;
	bool start = false;
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	while (!start)
	{
		al_draw_text(font24, al_map_rgb(255, 255, 255), 20, 100, 0, "tutorial presione !SPACEBAR! para empezar");
		al_draw_text(font24, al_map_rgb(255, 255, 255), 20, 200, 0, "Q-W-E para moverse");
		al_flip_display();
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		switch (events.keyboard.keycode)
		{
		case ALLEGRO_KEY_SPACE:
			start = true;
			break;
		}
	}


	int width = al_get_display_width(display);

	float positionX1 = 0;
	float positionX2 = width;
	float positionX3 = width * 2;
	float positionX4 = width * 3;
	int posicion = 300;
	float posicionX = width;
	int posicionY;
	Start();
	int tiempo = 0;
	float tmp = 3;
	float tmpDos = 4;
	int posicionRandom = 1;
	float cinco = 5;
	float tmpNum = 0;
	int tempTiempo = 0;
	int temp = 0;
	int contlvl = 0, contveces = 0;
	
	al_play_sample_instance(songInstance);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	//cout << "tutorial presione Q-W-E para empezar" << endl;
	
	
	//cout << "SCORE: ";
	int colX = 0;
	bool stats = false;

	while (running) {

		//pantallas++;// posicion verdadera
		srand(time(NULL));
		posicionRandom = rand() % 3 + 1;
		posicionRandom *= 100;
		
		score = elapsedSeconds();
		tiempo = elapsedSeconds();
        posicionY = posicionRandom;

		
		if (tempTiempo != score)
		{
			//cout << score << "-";
		}
		tempTiempo = score;
		// textprintf(screen, font, 100, 40, palette_color[12], "Distancia: %d", elapsedSeconds()); 
			/*ALLEGRO_EVENT events;
			al_wait_for_event(event_queue,&events);*///No funcionaa 
		al_draw_bitmap(bitmap, positionX1, 0, 0);//posicion de las imagenes 
		// porque esta doble?  al_draw_bitmap(bitmapGatitoPiso, 100, posicion, 0);
		al_draw_bitmap(bitmapFondoJetPack, positionX2, 0, 0);
		al_draw_bitmap(bitmapfueguito, posicionX, posicionY, 0);
		al_draw_bitmap(bitmapGatitoPiso, 100, posicion, 0); 
		al_draw_text(font24, al_map_rgb(0, 0, 0), 500, 25, 0, (("Score: " + std::to_string(score)).c_str()));
		
		
		colX = posicionX;
		if (posicionX<0)
		{
			colX = posicionX * (-1);
		}
		if (stats)
		{
			al_draw_text(font24, al_map_rgb(0, 0, 0), 250, 60, 0, (("Posicionfuego X: " + std::to_string(colX)).c_str()));
			al_draw_text(font24, al_map_rgb(0, 0, 0), 250, 95, 0, (("Posicionfuego Y: " + std::to_string(posicionY)).c_str()));
			al_draw_text(font24, al_map_rgb(0, 0, 0), 250, 130, 0, (("PosicionCat   X: " + std::to_string(posicion)).c_str()));
			al_draw_text(font24, al_map_rgb(0, 0, 0), 250, 165, 0, (("PosicionCat   Y: " + std::to_string(100)).c_str()));
		}
		
		
		//system("cls");
		if (colX == 100 && posicionY  == posicion)
		{
			al_draw_bitmap(GO, 0, 0, 0);
			al_flip_display();
			al_stop_timer(timerstop);
			running = false;
			//Sleep(3000);
			srand(time(NULL));
			cancion = rand() % 3 + 1;
			ALLEGRO_SAMPLE* song = al_load_sample(("Song" + std::to_string(cancion) + ".ogg").c_str());
			break;
		}


		positionX1 -= tmp;//moviendo imagenes en el x axis
		positionX2 -= tmp;
		posicionX -= tmp;




		al_flip_display();//literal flips display

		if ((positionX1 + width) < 0)positionX1 = width;//condiciones para mandar las pantallas a width
		if ((positionX2 + width) < 0)positionX2 = width;
		if ((posicionX + width) < 0)posicionX = width;

		int tempDos = 0;



		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		switch (events.keyboard.keycode)
		{
		case ALLEGRO_KEY_Q:
			temp = 100;
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

			break;
		case ALLEGRO_KEY_W:
			temp = 200;
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case ALLEGRO_KEY_E:
			temp = 300;
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case ALLEGRO_KEY_S:
			stats = true;
			break;
		case ALLEGRO_KEY_D:
			stats = false;
			break;


		case ALLEGRO_KEY_ESCAPE:
			al_draw_bitmap(GO, 0, 0, 0);
			al_flip_display();
			al_stop_timer(timerstop);
			running = false;
			//Sleep(3000);
			srand(time(NULL));
			cancion = rand() % 3 + 1;
			ALLEGRO_SAMPLE* song = al_load_sample(("Song" + std::to_string(cancion) + ".ogg").c_str());
			break;



		}



		posicion = temp;
		/* Colision vieja
		if (posicion == posicionRandom && posicionX == 100+49)
		{
			al_draw_bitmap(GO, 0, 0, 0);
			al_flip_display();
			al_stop_timer(timerstop);
			running = false;
			Sleep(3000);
			break;

		}*/
		
		

	}

	//destroys all functions
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_bitmap(bitmap);
	al_destroy_sample(soundEffect);
	al_destroy_sample(song);
	al_destroy_sample_instance(songInstance);

	system("pause");


}
int main()
{
	int opcion;
	bool regresar = false;
	//leerFile();
	do {
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219) << endl;
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "JETPACK \n 1.JUGAR \n 2.ULTIMAS PARTIDAS \n 3. EXIT \n Ingrese la opcion deseada" << endl;
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219); cout << char(219) << endl;
		cin >> opcion;
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		regresar = false;
		switch (opcion) {
		case 1:
			/*HWND hidden_window;
			AllocConsole();
			hidden_window = FindWindowA("ConsoleWindowClass", NULL);
			ShowWindow(hidden_window, NULL);*/
			juego();
			scores[contScores] = score;
			contScores++;
			regresar = true;
			break;
		case 2:
			UltimasPartidas();
			regresar = true;
			break;
		case 3:
			exit(0);

		default:
			cout << "Ingrese solamente una de las opciones" << endl;
			regresar = true;
			break;
		}

	} while (regresar);

	system("pause");

	return 0;
}