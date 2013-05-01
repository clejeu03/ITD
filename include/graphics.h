#ifndef ITD_GRAPHICS_H
#define ITD_GRAPHICS_H
#ifdef MAC
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else 
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "Point3D.h"

typedef struct{
	GLuint BLUE_OCTOPUS_ID;
	GLuint ORANGE_OCTOPUS_ID;
	GLuint GREEN_OCTOPUS_ID;
	GLuint GUN_TOWER_ID;
	GLuint LASER_TOWER_ID;
	GLuint ROCKET_TOWER_ID;
	GLuint HYBRID_TOWER_ID;
	GLuint MAP_ID;
}ITD_Game_Textures;

ITD_Game_Textures GAME_TEXTURES_ID;

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH;
unsigned int WINDOW_HEIGHT;

/* Valeurs max et min des axes OpenGL (donnés à glOrtho2d) 
(valeurs recalculées par la suite). */
float GL_X_AXIS_MIN;
float GL_X_AXIS_MAX;
float GL_Y_AXIS_MIN;
float GL_Y_AXIS_MAX; 

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;


/**
 * Charge les textures du jeu dans la carte
 * graphique et récupère les textures id qui leurs
 * sont liés. Les stocke dans la variable globale 
 * GAME_TEXTURES_ID.
 */
void initGameGraphics(const SDL_Surface* map);

/**
 * Initialise la fenêtre SDL du
 * programme ITD. Créé le contexte 
 * OpenGL.
 */
int initWindow();

/**
 * Renvoie le Point3D fourni en paramètre en coordonnées
 * OpenGL.
 * Note : le point fourni en paramètre doit être exprimé en
 * coordonnées SDL.
 */
Point3D sdlToOpenGL(Point3D sdlPoint);

/**
 * Charge une texture dans la carte graphique à partir de
 * l'image dont le chemin d'accès est passé en paramètre.
 * Retourne l'id de la texture générée.
 */
GLuint makeTextureFromFile(const char* imagePath);

/**
 * Charge une texture à partir d'une SDL_Surface.
 * Retourne l'id de la texture générée.
 */
GLuint makeTextureFromSurface(const SDL_Surface* surface);

/**
 * Appelée quand la fenêtre est redimensionnée.
 */
void reshape();

int setVideoMode();

#endif
