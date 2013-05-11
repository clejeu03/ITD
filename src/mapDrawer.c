#include "mapDrawer.h"
#include "graphics.h"

void drawWorld(const World* world){
	if(world == NULL) return;
	int i = 0;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Important car les textures sont multipliées par la couleur courante
	glColor3ub(255,255,255);
	glPushMatrix();
	glTranslatef(world->cameraPosition.x, world->cameraPosition.y, world->cameraPosition.z);
	drawMap(&(world->map));
	
	drawPath(&(world->map));
	
	if( ! world->isBetweenWaves){
		for(i = 0; i < MONSTERS_PER_WAVE; ++i){
			if(world->monsters[i].life > 0)
				drawMonster(&(world->monsters[i]));
		}
	}
	
	drawTowers(world->towersList);
	glPopMatrix();
}

//TODO (rajouter le chemin etc)
void drawMap(const Map* map){
	glPushMatrix();
	glScalef(map->width,map->height,1.);
	drawTexturedQuad(GAME_TEXTURES_ID.MAP_ID);
	glPopMatrix();
}

void drawMonster(const Monster* monster){
	if(monster == NULL) return;
	GLuint monsterTexture = 0;
	switch(monster->type){
		case BLUE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;
		case ORANGE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID;
		break;	
		case GREEN_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID;
		break;	
		default : return;
		break;
	}
	Point3D oglPosition = sdlToOpenGL(monster->position);	
	
	glPushMatrix();
	glColor3ub(255,255,255);
	glTranslatef(oglPosition.x, oglPosition.y, 0.0);
	glScalef(MONSTER_WIDTH_PX,MONSTER_WIDTH_PX,1.0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);
	drawTexturedQuad(monsterTexture);
	glDisable(GL_ALPHA_TEST);
	
	glPopMatrix();
}

void drawTowers(List* towersList){
	if(towersList == NULL) return;
	
	Tower* cur = NULL;
	GLuint textureId = -1;
	goToHeadList(towersList);
	while ( (cur = (Tower*) nextData(towersList)) != NULL){
		switch(cur->type){
		case GUN : textureId = GAME_TEXTURES_ID.GUN_TOWER_ID;
		break;
		case LASER : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
		break;
		case ROCKET : textureId = GAME_TEXTURES_ID.ROCKET_TOWER_ID;
		break;
		case HYBRID : textureId = GAME_TEXTURES_ID.HYBRID_TOWER_ID;
		break;
		default : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
		break;
		}
		glPushMatrix();
		glColor3ub(255,255,255);
		Point3D oglPosition = sdlToOpenGL(cur->position);
		glTranslatef(oglPosition.x, oglPosition.y, oglPosition.z);
		glScalef(TOWER_WIDTH_PX, TOWER_HEIGHT_PX, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.0f);
		drawTexturedQuad(textureId);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}	
}

//Pour l'instant taille des cercles etc fixée en dur
void drawPath(const Map* map){
	if(map == NULL) return;
	int radius = 10;
	
	//Dessin des cercles, en prenant en compte les bonnes couleurs
	Point3D startNode = getStartPoint(map);
	Point3D endNode = getEndPoint(map);
	Point3D oglStartNode = sdlToOpenGL(startNode);
	Point3D oglEndNode = sdlToOpenGL(endNode);
	
	Point3D curNode = nextNode(map->pathNodeList, startNode);
	Point3D oglNode;
	
	//Dessin du chemin
	glPushMatrix();
	glColor3ub(map->pathColor.red,map->pathColor.green,map->pathColor.blue);
	glBegin(GL_LINE_STRIP);
	glVertex2f(oglStartNode.x, oglStartNode.y);
	do{
		oglNode = sdlToOpenGL(curNode);
		glVertex2f(oglNode.x,oglNode.y);
		curNode = nextNode(map->pathNodeList, curNode);
	}while(! arePointsEquals(curNode, endNode));	
	glVertex2f(oglEndNode.x, oglEndNode.y);
	glEnd();
	glPopMatrix();

	//Dessin du point de départ 
	glPushMatrix();
	glColor3ub(map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue);
	glTranslatef(oglStartNode.x,oglStartNode.y,oglStartNode.z);
	glScalef(radius, radius, 1.);
	drawCircle(1);
	glPopMatrix();
	
	//Dessin du point d'arrivée
	glPushMatrix();
	glColor3ub(map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue);
	glTranslatef(oglEndNode.x,oglEndNode.y,oglEndNode.z);
	glScalef(radius, radius, 1.);
	drawCircle(1);
	glPopMatrix();
	
	//Dessin des points intermédiaires
	curNode = nextNode(map->pathNodeList, startNode);
	glPushMatrix();	
	glColor3ub(map->pathColor.red,map->pathColor.green,map->pathColor.blue);
	do{
		oglNode = sdlToOpenGL(curNode);
		
		glPushMatrix();
		glTranslatef(oglNode.x, oglNode.y, oglNode.z);
		glScalef(radius, radius, 1.);
		drawCircle(1);
		glPopMatrix();
		curNode = nextNode(map->pathNodeList, curNode);
	}while(! arePointsEquals(curNode, endNode));
	

	glPopMatrix();
}


