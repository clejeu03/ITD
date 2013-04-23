#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Map.h"
#include "utils.h"
#define MAX_LENGHT 30

Color3u initColor(void){
	Color3u newColor;
	newColor.red = newColor.green = newColor.blue = 0;
	return newColor;
}

Map initMap(void){
	Map newMap;
	newMap.name = NULL;
	newMap.width = 0;
	newMap.height = 0;

	newMap.pathColor = initColor();
	newMap.nodeColor = initColor();
	newMap.constructAreaColor = initColor();
	newMap.inAreaColor = initColor();
	newMap.outAreaColor = initColor();

newMap.nodeList = createEmptyList();
	newMap.nodeList->head = NULL;
	newMap.nodeList->bottom = NULL;
	newMap.nodeList->cursor = NULL;
	newMap.nodeList->size = 0;
	newMap.nodeList->position = 0;


	return newMap;
}

int loadMap(Map* map){	

	/* On ouvre le fichier */
	FILE* file;

	file = fopen("map/map1.itd","r");
	if(file == NULL){
		return 0;
	}
	else{
		char versionMap [MAX_LENGHT];
		int k;
		for (k=0; k<MAX_LENGHT;k++){
			versionMap[k]= 0;
		}
		int i;
		for (i=0; i< 6; i++){
			fscanf(file,"%c",versionMap+i);
			printf("version : %s\n",versionMap+i);
		}
		/*la je sais pas du tout si c'est comme ça qu'il faut faire*/
		if (strcmp(versionMap,"@ITD 1")!= 0 && strcmp(versionMap,"@ITD 2")!=0 ){
			printf("Fichier incompatible");
			return 0;
		} 

		if (strcmp(versionMap,"@ITD 1")==0){

			/* nom de l'image*/
			(map->name) = (char*)malloc(sizeof(char)*MAX_LENGHT);
 /*  on a dit que le nom de la carte ferait 30 caractères maxi*/
			fscanf(file,"%s \n",map->name);
			printf("nom image : %s\n",map->name); /* par contre je sais pas comment faire vu qu'on connait pas la taille du nom. %s va surement pas marcher*/

			/*rajouter width et height quand on aura le fichier .ppm*/
			int R,V,B;
			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->pathColor.red =(unsigned char)R;
			map->pathColor.green = (unsigned char)V;
			map->pathColor.blue =(unsigned char)B;
			printf("couleur des chemins : %d %d %d\n",map->pathColor.red,map->pathColor.green,map->pathColor.blue);


			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->nodeColor.red = (unsigned char)R;
			map->nodeColor.green = (unsigned char)V;
			map->nodeColor.blue = (unsigned char)B;
			printf("couleur des noeuds : %d %d %d\n",map->nodeColor.red,map->nodeColor.green,map->nodeColor.blue);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->constructAreaColor.red = (unsigned char)R;
			map->constructAreaColor.green = (unsigned char)V;
			map->constructAreaColor.blue = (unsigned char)B;
			printf("couleur des zones constructibles : %d %d %d\n",map->constructAreaColor.red,map->constructAreaColor.green,map->constructAreaColor.blue);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->inAreaColor.red = (unsigned char)R;
			map->inAreaColor.green = (unsigned char)V;
			map->inAreaColor.blue = (unsigned char)B;
			printf("couleur des zones d'entrée : %d %d %d\n",map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue);


			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->outAreaColor.red = (unsigned char)R;
			map->outAreaColor.green = (unsigned char)V;
			map->outAreaColor.blue = (unsigned char)B;
			printf("couleur des zones de sortie : %d %d %d\n",map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue);

			fscanf(file,"%d\n",&(map->nodeList->size));
			printf("nombre de noeuds : %d\n",map->nodeList->size);


			PathNode* node1 = (PathNode*)malloc (sizeof(PathNode)); 
			fscanf(file,"%d %d\n",&(node1->x),&(node1->y));
			/*printf(" noeud : %d %d\n",node1->x,node1->y);*/
			
			int size = map->nodeList->size;
			/*printf("taille = %d",size);*/
			map->nodeList = createList((void*)node1); 

			int j=0;


			while (j<size-1){

				PathNode* node = (PathNode*)malloc (sizeof(PathNode));		
				fscanf(file,"%d %d\n",&(node->x),&(node->y));
			/*printf(" noeud : %d %d\n",node->x,node->y);*/
				insertBottomCell(map->nodeList,(void*)node);
				j++;
			}



			/* On vide  le buffer et on ferme le fichier*/
			fflush(file);
			fclose(file);
			return 1;
		}
	}
}

void dumpColor3u(Color3u color){
	printf("(r:%d, g:%d, b:%d)", color.red, color.green, color.blue);
}

void dumpMap(Map map){
	printf ("coucou\n");
	printf("-------- DUMP MAP %s --------\n", map.name == NULL ? "" : map.name);
	printf("Width : %d\n", map.width);
	printf("Height : %d\n", map.height);
	printf("Path color : ");
	dumpColor3u(map.pathColor);
	printf("\nNode color : ");
	dumpColor3u(map.nodeColor);
	printf("\nConstructable area color : ");
	dumpColor3u(map.constructAreaColor);
	printf("\nIn Area color : ");
	dumpColor3u(map.inAreaColor);	
	printf("\nOut Area color : ");
	dumpColor3u(map.outAreaColor);
	
	printf("\nNode list (%d nodes)\n", map.nodeList->size);
	goToHeadList(map.nodeList);
	
	PathNode* cur = NULL;
	while( (cur = nextData(map.nodeList)) != NULL){
		printf("Node %d : x=%d y=%d\n", map.nodeList->position, cur->x, cur->y);
	}
}
