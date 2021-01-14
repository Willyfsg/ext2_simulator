#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
// void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
// void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
// void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
// void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main(){
	char *comando[LONGITUD_COMANDO];
	char *orden[LONGITUD_COMANDO];
	char *argumento1[LONGITUD_COMANDO];
	char *argumento2[LONGITUD_COMANDO];
	
	int i,j;
	unsigned long int m;
   EXT_SIMPLE_SUPERBLOCK ext_superblock;
   EXT_BYTE_MAPS ext_bytemaps;
   EXT_BLQ_INODOS ext_blq_inodos;
   EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
   EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
   EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
   int entradadir;
   int grabardatos;
   FILE *fent;
     
   // Lectura del fichero completo de una sola vez
       
   fent = fopen("particion.bin","r+b");
   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     
   memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
   memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
   memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
   memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
   memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
    
   // Buce de tratamiento de comandos
   for (;;){
	   do {
		   printf (">> ");
		   fflush(stdin);
		   fgets(comando, LONGITUD_COMANDO, stdin);
		}while (ComprobarComando(comando,orden,argumento1,argumento2) == 0);
	     
      if(strcmp(orden, "dir") == 0){
         Directorio(&directorio, &ext_blq_inodos);
         continue;
      }
      
      if(strcmp(orden, "bytemaps") == 0){
         Printbytemaps(&ext_bytemaps);
         continue;
      }

      if(strcmp(orden, "info") == 0){
         LeeSuperBloque(&ext_superblock);
         continue;
      }

      if(strcmp(orden, "rename") == 0){
         Renombrar(&directorio, &ext_blq_inodos, &argumento1, &argumento2);
         continue;
      }

      if(strcmp(orden, "imprimir") == 0){
         Imprimir(&directorio, &ext_blq_inodos, &memdatos, &argumento1);
         continue;
      }

      if(strcmp(orden, "copy") == 0){
         Copiar(&directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, &memdatos, &argumento1, &argumento2, &fent);
      }
      
      if(strcmp(orden, "remove") == 0){
         Borrar(&directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, &fent);
      }
      
      // Escritura de metadatos en comandos rename, remove, copy     
      // Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
      // GrabarByteMaps(&ext_bytemaps,fent);
      // GrabarSuperBloque(&ext_superblock,fent);
      
      if (grabardatos){
         // GrabarDatos(&memdatos,fent);
         grabardatos = 0;
      }
      //Si el comando es salir se habrán escrito todos los metadatos
      //faltan los datos y cerrar
      
      if (strcmp(orden,"salir")==0){
         // GrabarDatos(&memdatos,fent);
         fclose(fent);
         return 0;
      }
      
   }
}

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){

   int * aux1, * aux2;

   strcomando = strtok(strcomando, " ");
   if(strcomando != NULL){
      // printf("valor string de orden: %s\n", strcomando);
      aux1 = strtok(NULL, " ");
      if(aux1 != NULL){
         // printf("el valor del arg1: %s\n", aux1);
         aux2 = strtok(NULL, " ");
         if(aux2 != NULL){
            // printf("el valor del arg2: %s\n", aux2);
         }
      }
   }

   if(strcmp(strcomando, "info") == 0 || strcmp(strcomando, "info\n") == 0){
      strcpy(orden, "info");
   }else if(strcmp(strcomando, "dir") == 0 || strcmp(strcomando, "dir\n") == 0){
      strcpy(orden, "dir");
   }else if(strcmp(strcomando, "bytemaps") == 0 ||  strcmp(strcomando, "bytemaps\n") == 0){
      strcpy(orden, "bytemaps");
   }else if(strcmp(strcomando, "imprimir") == 0 || strcmp(strcomando, "imprimir\n") == 0){
      if(argumento1 != NULL){
         strcpy(orden, "imprimir");
         strcpy(argumento1, aux1);
         if(argumento1[strlen(argumento1)-1] == '\n'){
            argumento1[strlen(argumento1)-1] = '\0';
         }
      }
   }else if(strcmp(strcomando, "rename") == 0 || strcmp(strcomando, "rename\n") == 0){
      if( aux1 != NULL && aux2 != NULL){
         strcpy(orden, "rename");
         strcpy(argumento1, aux1);
         if(argumento1[strlen(argumento1)-1] == '\n'){
            argumento1[strlen(argumento1)-1] = '\0';
         }
         strcpy(argumento2, aux2);
         if(argumento2[strlen(argumento1)-1] == '\n'){
            argumento2[strlen(argumento1)-1] = '\0';
         }
         
      }
   }else if(strcmp(strcomando, "salir") == 0 || strcmp(strcomando, "salir\n") == 0){
      strcpy(orden, "salir");
   }else if(strcmp(strcomando, "copy") == 0 || strcmp(strcomando, "copy\n") == 0){
      
      strcpy(orden, "copy");
      strcpy(argumento1, aux1);
      if(argumento1[strlen(argumento1)-1] == '\n'){
         argumento1[strlen(argumento1)-1] = '\0';
      }
      strcpy(argumento2, aux2);
      if(argumento2[strlen(argumento1)-1] == '\n'){
         argumento2[strlen(argumento1)-1] = '\0';
      }
   }else if(strcmp(strcomando, "remove") == 0 || strcmp(strcomando, "remove\n") == 0){
      strcpy(orden, "remove");
      strcpy(argumento1, aux1);
      if(argumento1[strlen(argumento1)-1] == '\n'){
         argumento1[strlen(argumento1)-1] = '\0';
      }
      
   }else{
      printf("ERROR: Comando ilegal [bytemaps,copy,dir,info,imprimir,rename,remove,salir]\n");
   }
   

   // printf("\nsaliendo de las comprobaciones\n");

   return;

}




void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){

   for(int i = 1; i < sizeof(directorio); i++){

      if((directorio+i)->dir_inodo != NULL_INODO && (directorio+i)->dir_nfich != NULL){
         printf("%s", directorio[i].dir_nfich);
         //Imprimimos tamaño del fichero, inodo del fichero, y bloques que ocupa.
         printf("   tamaño: %d   ", inodos->blq_inodos[(directorio+i)->dir_inodo].size_fichero);
         printf(" inodo: %d   ", (directorio+i)->dir_inodo);
         printf(" bloques: ");
         for (int k = 0; k < sizeof(inodos->blq_inodos[(directorio+i)->dir_inodo].i_nbloque); k++){
            if(inodos->blq_inodos[(directorio+i)->dir_inodo].i_nbloque[k] != NULL_BLOQUE && inodos->blq_inodos[(directorio+i)->dir_inodo].i_nbloque[k] != 0){  
               // printf("valor de la K: %d\n", k);
               printf(" %d", inodos->blq_inodos[(directorio+i)->dir_inodo].i_nbloque[k]);            

            }
         }
         printf("\n");
      }
   }
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   
   printf("Inodos: ");
   for(int i = 0; i < 24; i++){
      printf("%d ", ext_bytemaps->bmap_bloques[i]);
   }

   printf("\nBloques [0-25]: ");
   for(int i = 1; i < 25; i++){
      printf("%d ", ext_bytemaps->bmap_inodos[i]);
   }

   printf("\n");
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){

   printf("Bloque %d Bytes\n", psup->s_block_size);
   printf("i-nodos de particion: %d\n", psup->s_inodes_count);
   printf("i-nodos libres: %d\n", psup->s_free_inodes_count);
   printf("Bloques de particion: %i\n", psup->s_blocks_count);
   printf("Bloques libres: %d\n", psup->s_free_blocks_count);
   printf("Primer Bloque de Datos: %d\n", psup->s_first_data_block);

}

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){
   int i,tam;

   for(i = 0 ; i < MAX_FICHEROS; i++){

      if (directorio[i].dir_inodo != NULL_INODO){

         tam = inodos->blq_inodos[directorio[i].dir_inodo].size_fichero;
         if (tam > 0){
            if (strcmp(nombre, directorio[i].dir_nfich) == 0){
               
               return(i); //Entrada del directorio
            }
         }
      }
   }

   return -1; //No encontrado
}


int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
   int dirFich = BuscaFich(directorio, inodos, nombre);
   int bloque = 0;
   
   if(dirFich != -1){
      for (int i = 0; i < sizeof(inodos->blq_inodos->i_nbloque); i++){
         
         bloque = inodos->blq_inodos[directorio[dirFich].dir_inodo].i_nbloque[i];
         
         if(bloque != NULL_BLOQUE && bloque != 0){
            
            // printf("valor del bloque %d\n", bloque);
            //bloque-4 IMPORTANTE
            printf("%s", memdatos[bloque-4].dato);
         }
      }
      
      printf("\n");

   }else{
      printf("ERROR: el Fichero %s no existe.\n", nombre);
      return -1;
   }
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich){

   int dirFich = BuscaFich(directorio, inodos, nombre);
   if(dirFich != -1){
      printf("Encontramos el archivo.\n");
      for(int i = 0; i < sizeof(ext_bytemaps->bmap_bloques); i++){
         
         if(ext_bytemaps->bmap_bloques[i] == 1){
            if((directorio+dirFich)->dir_inodo != NULL_INODO && (directorio+dirFich)->dir_nfich != NULL){
               printf("se borra el bloque: %d\n", i);
               inodos->blq_inodos[(directorio+dirFich)->dir_inodo].i_nbloque[i] = NULL_BLOQUE;
               ext_bytemaps->bmap_bloques[i] = 0;
            }
         }
      }
      
      for(int i = 0; i < sizeof(ext_bytemaps->bmap_inodos); i++){
         if(ext_bytemaps->bmap_inodos[i] == 1){
            if((directorio+dirFich)->dir_inodo == i){
               printf("se borra el inodo %d\n", i);
               directorio[dirFich].dir_inodo = NULL_INODO;
               ext_bytemaps->bmap_inodos[i] = 0;
               break;
            }
         }
      }

      printf("se borra el nombre \n");
      *directorio[dirFich].dir_nfich = NULL;
   }else{
      printf("El fichero %s no existe.\n", nombre);
      return -1;
   }

}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo){
   int dirFich = BuscaFich(directorio, inodos, nombreantiguo);
   int nFich = 0;

   if(dirFich != -1){

      for (int i = 0; i < sizeof(directorio); i++){

         if(strcmp(directorio[i].dir_nfich, nombrenuevo) == 0){
            printf("Ese nombre ya existe en el directorio.\n");
            return -1;
         }
      }

      if(strcmp(directorio[dirFich].dir_nfich, nombreantiguo) == 0){
         strcpy(directorio[dirFich].dir_nfich, nombrenuevo);
         if(directorio[dirFich].dir_nfich[strlen(directorio[dirFich].dir_nfich)-1] == 10){
            directorio[dirFich].dir_nfich[strlen(directorio[dirFich].dir_nfich)-1] = NULL;
         }
         
      }
     
   }else{

      return -1;
   }
}

//NO SE PORQUE FALLA LA COPIA DEL BLOQUE!!!!!
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich){
   int dirFich = BuscaFich(directorio, inodos, nombreorigen);
   int dirNuev = 0;
   int bloqueantiguo = 0;
   unsigned int size;


   if(dirFich != -1){
      //buscar un directorio vacio;
      for(int i = 0; i < sizeof(directorio); i++){
         if((directorio+i)->dir_inodo != NULL_INODO && (directorio+i)->dir_nfich != NULL){
         }else{

            dirNuev = i;
            for(int j = 0; j < MAX_INODOS; j++){

               if(ext_bytemaps->bmap_inodos[j] == 0){
                  //Asignamos el inodo al primer directorio vacio, por tanto se pone en ocupado.
                  (directorio+dirNuev)->dir_inodo = j;
                  ext_bytemaps->bmap_inodos[j] = 1;
                  break;
               }

            }

            for(int k = 0; k < MAX_NUMS_BLOQUE_INODO ; k++){
               //buscamos la cantidad de bloques necesarios y los reservamos para le nuevo fichero.
               if(inodos->blq_inodos[(directorio+dirFich)->dir_inodo].i_nbloque[k] != NULL_BLOQUE && inodos->blq_inodos[(directorio+dirFich)->dir_inodo].i_nbloque[k] != 0){
                  
                  for(int i = 0; i < MAX_BLOQUES_PARTICION; i++){
                     //buscamos un bloque libre.
                     if(ext_bytemaps->bmap_bloques[i] == 0){

                        bloqueantiguo = inodos->blq_inodos[(directorio+dirFich)->dir_inodo].i_nbloque[k];
                        
                        for (int x = 0; x < MAX_NUMS_BLOQUE_INODO; x++){
                           //buscamos bloques libre dentro del nuevo directorio.                           
                           if(inodos->blq_inodos[(directorio+dirNuev)->dir_inodo].i_nbloque[x] != NULL_BLOQUE && inodos->blq_inodos[(directorio+dirNuev)->dir_inodo].i_nbloque[x] != 0){
                           }else{

                              inodos->blq_inodos[(directorio+dirNuev)->dir_inodo].i_nbloque[x] = i; // bloque+2 muy cerca parece. claramente porque es el bloqueantiguo
                              
                              memcpy(memdatos[i].dato, memdatos[bloqueantiguo+2].dato, SIZE_BLOQUE);
                              ext_bytemaps->bmap_bloques[i] = 1;

                              break;
                           }    
                        }
                        break;
                     }
                  }      
               }
            }

            //asignamos el nombre del fichero 
            strcpy(directorio[dirNuev].dir_nfich, nombredestino);
            if(directorio[dirNuev].dir_nfich[strlen(directorio[dirNuev].dir_nfich)-1] == 10){
               directorio[dirNuev].dir_nfich[strlen(directorio[dirNuev].dir_nfich)-1] = NULL;
            }

            // asignamos el size
            size = inodos->blq_inodos[directorio[dirFich].dir_inodo].size_fichero;
            if(inodos->blq_inodos[directorio[dirNuev].dir_inodo].size_fichero == 0){
               inodos->blq_inodos[directorio[dirNuev].dir_inodo].size_fichero = size;
            }

            break;
         }
      }


   }else{
      printf("ERROR: el fichero %s no existe.\n", nombreorigen);
      return -1;
   }

}