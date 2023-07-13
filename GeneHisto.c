#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "GeneHisto.h"
#define TAILLE_MAX 500

// Vérifie si un fichier existe
int fichierExiste(const char* nomFichier) {

    return access(nomFichier, F_OK) != -1;
}


int wholeDiv( int a, int b ) {
    if (b == 0) {
        printf("Error: division by zero\n");
        return 0;
    }
    int q = 0;
    if ((a >= 0 && b > 0) || (a <= 0 && b < 0)) {
        while (a >= b)
        {
           a -= b;
           q++;
        }
    } else {
        while (a <= -b)
        {
           a += b;
           q++;
        }
        q = -q;
    }
    return q;
}


/* ALLOUER DYNAMIQUEMENT UNE MATRICE 2D*/
int **allocation_dynamique(int row, int col)
{
    int **ret_val;
    int i;

    ret_val = (int **)calloc(row, sizeof(int *));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i) {
        ret_val[i] = (int *)calloc(col, sizeof(int));
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
    return ret_val;
}


float **allocation_dynamiqueFloat(int row, int col)
{
    float **ret_val;
    int i;

    ret_val = (float **)calloc(row, sizeof(float *));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i) {
        ret_val[i] = (float *)calloc(col, sizeof(float));
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
    return ret_val;
}


centroid **allocationListCentroid( int row, int col ) {
    
    centroid **ret_val;
    int i;

    ret_val = (centroid **)malloc(sizeof(centroid *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i) {
        ret_val[i] = (centroid *)malloc(sizeof(centroid) * col);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    /* Initialisation */
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
           ret_val[i][j].indice = (int *)malloc(sizeof(int) * 2);
           ret_val[i][j].indice[0] = 0;
           ret_val[i][j].indice[1] = 0;

           ret_val[i][j].valChannel = (int *)malloc(sizeof(int) * 3);
           ret_val[i][j].valChannel[0] = 0;
           ret_val[i][j].valChannel[1] = 0;
           ret_val[i][j].valChannel[2] = 0;
        }
    }

    return ret_val;
}


/*  Marice 3D*/
int ***allocateDynamiq( int row, int col, int deep  )
{
    int ***ret_val;
    int i;
    ret_val = (int ***)calloc(row, sizeof(int **));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
    for( i= 0; i < row; ++i){
        ret_val[i] = allocation_dynamique(col, deep);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
   
    return ret_val;
}

/*  Marice 3D*/
float ***allocateDynamiqFloat( int row, int col, int deep )
{
    float ***ret_val;
    int i;
    ret_val = (float ***)calloc(row, sizeof(float **));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
    for( i= 0; i < row; ++i){
        ret_val[i] = allocation_dynamiqueFloat(col, deep);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
    return ret_val;
}


/* DESALLOUER DYNAMIQUEMENT UNE MATRICE 2D*/

void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;

    for (i = 0; i < row; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);
}


Image split( char *file, int row, int col ) {

    Image img;
    img.height = row;
    img.width = col;
    img.deep = 3;

    FILE* fichier = NULL;

    int** redChannelValues = NULL;
    int** greenChannelValues = NULL;
    int** blueChannelValues = NULL; 

    redChannelValues = allocation_dynamique(row, col);
    greenChannelValues = allocation_dynamique(row, col);
    blueChannelValues = allocation_dynamique(row, col);

	char chaine[TAILLE_MAX] = "";

	fichier = fopen(file, "r+");

	if(fichier != NULL)
	{
        //Vérifier si le fichier est au format PPM valide
        fgets(chaine, TAILLE_MAX, fichier);
        if( strncmp(chaine, "P3", 2) != 0 ){
            fprintf( stderr, "Invalid file format: not a correct PPM file\n");
            exit(EXIT_FAILURE);
        }

		int k = 0;
		while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
		{
			k++;
			if(k == 3)
			{
				for(int i = 0; i < row; i++)
				{
                    int r = 0;
                    int g = 0;
                    int b = 0;

					for(int j = 0; j < col*3; j++)
					{
                       if(fgets(chaine, TAILLE_MAX, fichier) != NULL)
						{
                            if( j % 3 == 0 ) {
                                redChannelValues[i][r] = atoi(chaine);
                                r++;
                            }
                            else if( j % 3 == 1 ) {
                                greenChannelValues[i][g] = atoi(chaine);
                                g++;
                            }
                            else {
                                blueChannelValues[i][b] = atoi(chaine);
                                b++;
                            }
						} 	
					}
				}
			}
		}

		fclose(fichier);
	}
    else {
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }

    img.imgRed = redChannelValues;
    img.imgGreen = greenChannelValues;
    img.imgBlue = blueChannelValues;


    // desallouer les matrices
    // deallocate_dynamic_matrix(redChannelValues, row);
    // deallocate_dynamic_matrix(greenChannelValues, row);
    // deallocate_dynamic_matrix(blueChannelValues, row);

    return img;
}


ImagePix imread( char *file, int row, int col ) {

    ImagePix image;
    pixel* pixelValues = (pixel *)calloc((row*col), sizeof(pixel));  //l'ensemble de tous les pixels d'une image

    //On initialise tous les pixels à 0
    for(int i = 0; i < row*col; i++) { 
        pixel p;
        p.rgb =  (int *)calloc(3, sizeof(int));
        if( p.rgb == NULL) {
            fprintf(stderr, "Memory allocation failled\n");
            exit(EXIT_FAILURE);
        }

        p.rgb[0] = 0;
        p.rgb[1] = 0;
        p.rgb[2] = 0;
        
        pixelValues[i] = p;
    }

    image.img = pixelValues;

    FILE* fichier = NULL;

    char chaine[TAILLE_MAX] = "";

	fichier = fopen(file, "r+");

    if(fichier != NULL)
	{
        // Vérifier que le fichier est au frormat PPM valide
        fgets(chaine, TAILLE_MAX, fichier);

        if (strncmp(chaine, "P3", 2) != 0) {

            fprintf(stderr, "Invalid file format: not a correct PPM file\n");
            exit(EXIT_FAILURE);
        }

		int k = 0;
		while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
		{
			k++;
			if(k == 3)
			{
                int index = 0;
                int count = 0;
				for(int i = 0; i < row*col; i++)
				{
					for(int j = 0; j < col*3; j++)
					{
                       if( count == 3 ) {
                        index ++;
                        count = 0;
                       }

                       if(fgets(chaine, TAILLE_MAX, fichier) != NULL)
						{
                            if( j % 3 == 0 ) {
                                image.img[index].rgb[0] = atoi(chaine);
                            }
                            else if( j % 3 == 1 ) {
                                image.img[index].rgb[1] = atoi(chaine);
                            }
                            else {
                                image.img[index].rgb[2] = atoi(chaine);
                            }
						}
                        count++;

					}
				}
			}
		}

		fclose(fichier);
	}
    else {
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }

    return image;
}


int **slicing( int **matrix, int startRow, int endRow, int startCol, int endCol ) {

    //printf("startRow = %d - endRow = %d \t startCol = %d - endCol = %d\n", startRow, endRow, startCol, endCol);

    if (startRow < 0 || endRow < 0 || startCol < 0 || endCol < 0) {
        fprintf(stderr, "Invalid indices\n");
        exit(EXIT_FAILURE);
    }
    if (endRow - startRow <= 0 || endCol - startCol <= 0) {
        fprintf(stderr, "Invalid submatrix dimensions\n");
        exit(EXIT_FAILURE);
    }

    int row  = endRow - startRow;
    int col = endCol - startCol;

    int **nestedTab = allocation_dynamique(row, col);

    int k = 0;

    for(int i = startRow; i < endRow; i++) {
        int l = 0;
        for (int j = startCol; j < endCol; j++)
        {
            nestedTab[k][l] = matrix[i][j];
            l++;
        }
        k ++;
    }
    return nestedTab;
}


int contains( point *tab, point p, int taille) {

    if( tab == NULL || taille < 0  ) {
        fprintf(stderr, "Error: paramètre non valide");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < taille; i++)
    {
        if( tab[i].x == p.x && tab[i].y == p.y ) {
            return 1;
        }
    }
    return 0;
}


// centroid* centroides( char *imageFile, int row, int col ) {
    
// }
centroid* centroide( char *imageFile, int row, int col ) {

    int p, q, rp, rq, p1, q1, i, j, k, l;
    time_t t;
    point pt;

    Image img = split( imageFile, row, col );
    p1 = 17;
    q1 = 3;
    p = wholeDiv(img.height, p1);
    q = wholeDiv(img.width, q1);
    rp = img.height % p1;
    rq = img.width % q1;
    int nbreClusters = p1*q1;
    point *tab = (point *)calloc(nbreClusters, sizeof(point));
    centroid** C = allocationListCentroid( p1, q1 ); //#Ici nous allons mettre l'ensemble des centroïdes dans ce vecteur
    int s, b, k1, k2, k3;

    srand((unsigned) time(&t));

    for(i = 0; i < p1-1; i++) { //#On affiche les p1-1 fragments de la dernière colonne de la partition

        s = rand() % p;  //#On choisit aléatoirement un indice dans la partition considérée
        b = rand() % (rq+q-1)+1; //#On choisit aléatoirement un indice dans la partition considérée
        pt.x = s;
        pt.y = b;
        while(contains( tab, pt, nbreClusters) == 1) {
            s = rand() % p;
            b = rand() % (rq+q-1)+1;
            pt.x = s;
            pt.y = b;
        }
        //printf("i = %d\n",i);
        tab[i] = pt;
        k1 = slicing( img.imgBlue, i*p, (i+1)*p, (q1-1)*q, img.width )[s][b]; //#On récupère la composante bleue de ce pixel en question
        k2 = slicing( img.imgGreen, i*p, (i+1)*p, (q1-1)*q, img.width )[s][b]; //#On récupère la composante verte de ce pixel en question
        k3 = slicing( img.imgRed, i*p, (i+1)*p, (q1-1)*q, img.width )[s][b]; //#On récupère la composante rouge de ce pixel en question

        //#Puis on range le résultat des composantes à l'endroit indiquée du centroïde      
        C[i][q1-1].indice[0] = s;
        C[i][q1-1].indice[1] = b;

        C[i][q1-1].valChannel[0] = k1;
        C[i][q1-1].valChannel[1] = k2;
        C[i][q1-1].valChannel[2] = k3;
    }

    for(j = 0; j < q1-1; j++) { //#On affiche les q-1 fragments de la dernière ligne de la partition
        
        //#Même chose que précédemment pour les pixels de bordures basse
        s = rand() % (rp+p-1)+1; 
        b = rand() % q;
        pt.x = s;
        pt.y = b;

        while(contains( tab, pt, nbreClusters) == 1) {
            s = rand() % (rp+p-1)+1;
            b = rand() % q;
            pt.x = s;
            pt.y = b;
        }
        //printf("j = %d\n",j);
        tab[i+j] = pt;
        k1 = slicing( img.imgBlue, (p1-1)*p, img.height, j*q, (j+1)*q )[s][b]; 
        k2 = slicing( img.imgGreen, (p1-1)*p, img.height, j*q, (j+1)*q )[s][b]; 
        k3 = slicing( img.imgRed, (p1-1)*p, img.height, j*q, (j+1)*q )[s][b]; 
    
        C[p1-1][j].indice[0] = s;
        C[p1-1][j].indice[1] = b;

        C[p1-1][j].valChannel[0] = k1;
        C[p1-1][j].valChannel[1] = k2;
        C[p1-1][j].valChannel[2] = k3;
    }

    //#Même chose que précédemment, pour le pixel du coin bas droit
    s = rand() % (rp+p-1)+1; 
    b = rand() % (rp+p-1)+1;
    pt.x = s;
    pt.y = b;

    while(contains( tab, pt, nbreClusters) == 1) {
        s = rand() % (rp+p-1)+1;
        b = rand() % (rp+p-1)+1;
        pt.x = s;
        pt.y = b;
    }
    //printf("Hors boucle\n");
    tab[i+j] = pt;
    k1 = slicing( img.imgBlue, (p1-1)*p, img.height, (q1-1)*q, img.width )[s][b]; 
    k2 = slicing( img.imgGreen, (p1-1)*p, img.height, (q1-1)*q, img.width )[s][b]; 
    k3 = slicing( img.imgRed, (p1-1)*p, img.height, (q1-1)*q, img.width )[s][b];

    C[p1-1][q1-1].indice[0] = s;
    C[p1-1][q1-1].indice[1] = b;

    C[p1-1][q1-1].valChannel[0] = k1;
    C[p1-1][q1-1].valChannel[1] = k2;
    C[p1-1][q1-1].valChannel[2] = k3;

    int indexActuel = i+j+1;
    int co = q1-1;

    for(k = 0; k < p1-1; k++) {

        for(l = 0; l < q1-1; l++) { //#On affiche les (p1-1)*(q1-1) premiers fragments

            //#Même chose pour les pixels du milieu
            s = rand() % p;
            b = rand() % q;
            pt.x = s;
            pt.y = b;

            while(contains( tab, pt, nbreClusters) == 1) {
                s = rand() % p;
                b = rand() % q;
                pt.x = s;
                pt.y = b;
            }
            //printf("k = %d - l = %d\n", k, l);
            tab[indexActuel + ( (k*co) + l )] = pt;

            k1 = slicing( img.imgBlue, k*p, (k+1)*p, l*q, (l+1)*q )[s][b];
            k2 = slicing( img.imgGreen, k*p, (k+1)*p, l*q, (l+1)*q )[s][b];
            k3 = slicing( img.imgRed, k*p, (k+1)*p, l*q, (l+1)*q )[s][b];

            C[k][l].indice[0] = s;
            C[k][l].indice[1] = b;

            C[k][l].valChannel[0] = k1;
            C[k][l].valChannel[1] = k2;
            C[k][l].valChannel[2] = k3;
        }
    }

    centroid* C1 = (centroid *)malloc(sizeof(centroid) * (p1*q1));
    int kk = 0;

    for(int s = 0; s < p1; s++) {
        for(int m = 0; m < q1; m++) {
            C1[kk] = C[s][m];
            kk +=1;
        }
    }
    free(tab);
    return C1;
}


cluster generateClusterEmpty( char *imageFile, int row, int col, int k ) {

    // char * fullName;
    // char t[20];
    // size_t fullSize;
    // fullSize = strlen( "clusterVide.txt" ) + 1 +  strlen( "ent->d_name" ) + 1;
    // fullName = (char *) malloc( fullSize );
    // strcpy( fullName, "clusterVide" );
    // sprintf(t, "%d", k);
    // strcat( fullName, t );
    // strcat( fullName, ".txt" );

    centroid* clusterVide = (centroid *)calloc((row*col), sizeof(centroid));
    cluster clust;
    
    for(int i = 0; i < row; i++) { //#Parcours de la longueur du cluster

        for(int j = 0; j < col; j++) { //#pour chaque composante de pixel de couleur 

            clusterVide[i*col+j].indice = (int *)calloc(2, sizeof(int));//#On initialise par une valeur dont on sait qu'elle n'apparait pas dans une image réelle, c'est d'ailleur la raison pour laquelle, le dtype est "float32"
            
            if( clusterVide[i*col+j].indice == NULL ) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            clusterVide[i*col+j].indice[0] = i;
            clusterVide[i*col+j].indice[1] = j;

            clusterVide[i*col+j].valChannel = (int *)calloc(3, sizeof(int));

            if( clusterVide[i*col+j].valChannel == NULL ) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            clusterVide[i*col+j].valChannel[0] = -1;
            clusterVide[i*col+j].valChannel[1] = -1;
            clusterVide[i*col+j].valChannel[2] = -1;

        }
    }

    clust.clust = clusterVide;
    //printf("clusterVide\n");
    // serializeCluster2( fullName, clust, row, col );

    return clust;
}


cluster searchAndAssign(int i0, int j0, cluster clusterVide, int row, int col, int* valChannel) {
    
    for(int i = 0; i < row; i++) {

        if( clusterVide.clust[i].indice[0] == i0 ) {

            for(int j = 0; j < col; j++) {

                if( clusterVide.clust[i].indice[1] == j0 ) {
                    
                    clusterVide.clust[i].valChannel = valChannel;

                    return clusterVide;
                }
            }

        }
    }

    return clusterVide;
}


cluster unClusCentroide(centroid* Centroids, cluster clusterVide, int i0, int j0, int k, int row, int col) { //#Cette fonction prend en paramètres l'ensemble de tous les centroïdes, un cluster vide, les index des centroïdes et l'identifiant du centroïde, puis va nous renvoyer le cluster avec son centroïde positionné au bon endroit

    //serializeCluster2("clusterVide.txt", clusterVide, row, col );

    for(int i = 0; i < row*col; i++) {

        if( clusterVide.clust[i].indice[0] == i0 ) {

            if( clusterVide.clust[i].indice[1] == j0 ) {

                // printf("i = %d - j = %d\n",clusterVide.clust[i].indice[0], clusterVide.clust[i].indice[1]);
                // printf("i0 = %d - j0 = %d\n",i0, j0);
                // printf("Trouve\n");
                clusterVide.clust[i].valChannel = Centroids[k].valChannel;

            }
        }
    }    
    //     if( clusterVide.clust[i].indice[0] == i0 ) {

    //         for(int j = 0; j < col; j++) {

    //             if( clusterVide.clust[i].indice[1] == j0 ) {
                    
    //                 clusterVide.clust[i].valChannel = Centroids[k].valChannel;

    //                 return clusterVide;
    //             }
    //         }

    //     }
    // }

    //clusNonVide = searchAndAssign(i0, j0, clusterVide, row, col, Centroids[k].valChannel); //#On positionne le centroïde à la bonne position
    // printf("i0 = %d - j0 = %d\n",i0, j0);
    // printf("pas trouvé\n");
    // free(fullName);
    return clusterVide;  //#Et on renvoie ce cluster contenant désormais son centre
}


paramInit InitializeCluster(char* imageFile, int row, int col, int nbreClusters) { //#Ici, on définit une fonction qui va initialiser un cluster. Elle prend en paramètres le chemin d'accès à l'image à clusteriser et l'identifiant du cluster
    
    centroid* C =  centroide( imageFile, row, col ); //#On renvoie les différents centres de chaque cluster de l'image prise en paramètre
    serializeCentroid( "centroid.txt", C, nbreClusters );
    
    float **Centres = allocation_dynamiqueFloat(nbreClusters, 3);

    for(int k = 0; k < nbreClusters; k++) {

        for( int j = 0; j < 3; j++) {

            Centres[k][j] = C[k].valChannel[j];  //#On extrait les index des centres pour chaque cluster
        }
    }

    //cluster Aff = generateClusterEmpty( imageFile, row, col );    //#On génère un cluster vide qui va nous permettre d'initialiser notre ensemble de clusters

    cluster *lesClusters = (cluster *)calloc(nbreClusters, sizeof(cluster));
    

    if (lesClusters == NULL) {
        fprintf(stderr, "Erreur: Impossible d'allouer le tableau de clusters.\n");
        exit(EXIT_FAILURE);
    }

    // //#On initialise nos différents clusters
    // for(int i = 0; i < nbreClusters; i++) {
    //     lesClusters[i] = Aff;
    // }
    int i0, j0;
    for(int k = 0; k < nbreClusters; k++) {
        
        //#On extrait les index de la position du centroïde k
        i0 = C[k].indice[0]; 
        j0 = C[k].indice[1];
        
        lesClusters[k] = unClusCentroide(C, generateClusterEmpty( imageFile, row, col, k ), i0, j0, k, row, col); //#Puis le cluster est généré avec son centroïde correspondant
        //serializeCluster("InitialiazCluster.txt", lesClusters[k] , row, col);
    }
    
    paramInit init;
    init.centres = Centres;
    init.lesClusters = lesClusters;
    return init; //#On renvoie alors une liste contenant l'ID du cluster et le cluster initialisé..
}


float somme( float *tableau, int n ) {

    if (tableau == NULL) {
        fprintf(stderr, "Erreur : le tableau est nul.\n");
        exit(EXIT_FAILURE);
    }

    float s = 0.0;
    
    for( int i = 0; i < n; i++ ) {

        s += tableau[i];
    }
    return s;
}


float distance(int* a, float *b) {    //#Fonction calculant la distance euclidienne entre deux points de l'image

    if (a == NULL || b == NULL) {
        fprintf(stderr, "Erreur : l'un des points est nul.\n");
        exit(EXIT_FAILURE);
    }

    return sqrt( pow( ( a[0] - b[0] ), 2 ) + pow( ( a[1] - b[1] ), 2 ) + pow( ( a[2] - b[2] ), 2) );
}


int nbreDifferentsDeMoinsUn(cluster clust, int row, int col) {

    if (clust.clust == NULL) {
        fprintf(stderr, "Erreur : le tableau de pixels est nul.\n");
        exit(EXIT_FAILURE);
    }

    int nbre = 0;

    for(int i = 0; i < row*col; i++) {

            if ( 
                ( clust.clust[i].valChannel[0] != -1 ) 
                &&
                ( clust.clust[i].valChannel[1] != -1 )
                && 
                ( clust.clust[i].valChannel[2] != -1 ) 
            
            ){

                  nbre += 1;
            }
    }

    return nbre;
}


float* DetecteNMoinsUn(cluster clust, int row, int col) {  //#cette fonction nous donne la possibilité de faire la somme des vecteurs présents dans le cluster passé en paramètre

    if (clust.clust == NULL) {
        fprintf(stderr, "Erreur : le tableau de pixels est nul.\n");
        exit(EXIT_FAILURE);
    }

    float *c = (float *)calloc(3, sizeof(float));

    for(int i = 0; i < row*col; i++) {   //#On prend un cluster, puis on va sommer tous les points "connus" de ce cluster

            if ( 
                ( clust.clust[i].valChannel[0] != -1 ) 
                &&
                ( clust.clust[i].valChannel[1] != -1 )
                && 
                ( clust.clust[i].valChannel[2] != -1 ) 
            
            ){
                //#Si le vecteur se trouve dans le cluster, on calcule par cumulation les composantes du vecteur résultant
                c[0] += clust.clust[i].valChannel[0];    
                c[1] += clust.clust[i].valChannel[1];
                c[2] += clust.clust[i].valChannel[2];
            }
    }

    return c;
}


int* minimEtIndice(float *tab, int taille) {    //#On veut savoir le plus petit élément dans un tableau et la position occupée(son indice)
    
    if (tab == NULL) {
        fprintf(stderr, "Erreur : le tableau est nul.\n");
        exit(EXIT_FAILURE);
    }

    int minim = tab[0];  
    int indice = 0;  

    if (taille != 1) {

        for(int i = 0; i < taille - 1; i++) {

            if( tab[indice] >= tab[i+1] ) {

                minim = tab[i+1];
                indice = i+1;
            }
        }
    }

    int *tabl = (int *)calloc(2, sizeof(int));
    
    if (tabl == NULL) {
        fprintf(stderr, "Erreur : le tableau de pixels est nul.\n");
        exit(EXIT_FAILURE);
    }

    tabl[0] = indice;
    tabl[1] = minim;

    return tabl; 
}


int* minimEtIndiceMatrice(float **tab, int row, int col) {

    if (tab == NULL) {
        fprintf(stderr, "Erreur : la matrice est nulle.\n");
        exit(EXIT_FAILURE);
    }
    if (row == 0 || col == 0) {
        fprintf(stderr, "Erreur : la matrice a une taille nulle.\n");
        exit(EXIT_FAILURE);
    }

    int indiceRow = 0;
    int indiceCol = 0;
    int valMin = tab[0][0];

    for(int i = 0; i < row; i++) {

        int *val = minimEtIndice( tab[i], col);

        if( val[1] < valMin ) {

            valMin = val[1];
            indiceRow = i;
            indiceCol = val[0];
        }
    }

    int *tabl = (int *)calloc(3, sizeof(int));
    if (tabl == NULL) {
        fprintf(stderr, "Erreur : impossible d'allouer de la mémoire pour le tableau.\n");
        exit(EXIT_FAILURE);
    }

    tabl[0] = indiceRow;
    tabl[1] = indiceCol;
    tabl[2] = valMin;

    return tabl;
}


float* moyenne3(float *Vecteur, int Nbre) {  //#Cette fonction nous permet de faire une finition de calcule de la moyenne des vecteurs

    if(Vecteur == NULL) {
        fprintf(stderr, "Erreur : Vecteur est null");
        exit(EXIT_FAILURE);
    }

    float *tabl = (float *)calloc(3, sizeof(float));
    if(tabl == NULL) {
        fprintf(stderr, "Erreur : Echec de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    tabl[0] = Vecteur[0]/Nbre;
    tabl[1] = Vecteur[1]/Nbre;
    tabl[2] = Vecteur[2]/Nbre;

    return tabl;
}
    

float* rassemble(float **Collection, int row, int col) {

    if (Collection == NULL) {
        fprintf(stderr, "Erreur : la collection est nulle.\n");
        exit(EXIT_FAILURE);
    }
    if (row == 0 || col == 0) {
        fprintf(stderr, "Erreur : la collection a une taille nulle.\n");
        exit(EXIT_FAILURE);
    }

    float *Collections = (float *)calloc((row*col), sizeof(float));

    for(int i = 0; i < row; i++) { //#on recupère l'ensemble des moyennes distances minimales par rapport au cluster calculée à partir de  de l'image

        for(int j = 0; j < col; j++) {

            Collections[i*col+j] = Collection[i][j];
        }
    }

    return Collections;
}


paramInit clusteriser(char* imageFile, int row, int col, int nbreClusters) { //# cette fonction va nous permettre de clusteruser une image en K clusters

    if(nbreClusters <= 0) {
        fprintf(stderr, "Error : Le nombre de clusters doit être supérieur à zéro");
        exit(EXIT_FAILURE);
    }
    float *distances = (float *)calloc(nbreClusters, sizeof(float));

    paramInit clusters = InitializeCluster(imageFile, row, col, nbreClusters);    //#Ici, nous initialisons aléatoirement nos clusters
    int MaxIter = 20;
    int Iter = 0;
    float **centres = clusters.centres;
    ImagePix image = imread( imageFile, row, col );
    int N = row * col;     //#Il s'agit ici du nombre total de points de notre dataset. Nous nous en servirons pour calculer la fonction de coût

    float **cluster = allocation_dynamiqueFloat(row, col);      //#On se sert de ce tableau à deux dimensions pour remplir notre tableau de coût
      
    float seuil = 0.01;     //#On se donne un certain seuil de convergence de notre fonction de coût
    float Cost = 500000.0;   //#On suppose que le coût initial est de 3.0
    float Cost0 = 0.0;

    float *tableauCost = (float *)calloc(N, sizeof(float));
    //#Et on initialise notre tableau de coûts à 0.0
    for(int i = 0; i < N; i++) {
        tableauCost[i] = 0.0;
    }

    while ( abs( Cost - Cost0 ) >= seuil && Iter < MaxIter ) {

        for(int i0 = 0; i0 < nbreClusters; i0++) {     //Pour chaque cluster

            int nbre = nbreDifferentsDeMoinsUn(clusters.lesClusters[i0], row, col);     //#On calcule le nombre de pixels faisant déjà partie du cluster considéré
            float* vecteur = DetecteNMoinsUn(clusters.lesClusters[i0], row, col);     //#On fait la somme des pixels qui on été affectés au cluster en question  
            centres[i0] = moyenne3(vecteur, nbre);      //#On recalcule le centre du cluster, qui est en fait ici considéré comme la moyenne des pixels comptés dans le cluster
        }

        printf("le nouveau centre est calcule pour iteration Iter = %d\n", Iter);

        for(int i = 0; i < row; i++) {       //#Pour chaque pixel de l'image dont le chemin d'accès à été passé en paramètre

            for(int j = 0; j < col; j++) {

                for(int k = 0; k < nbreClusters; k++) {

                    distances[k] = distance(image.img[(i*col)+j].rgb, centres[k]);      //#On recalcule les distances entre chaque centre de cluster et chaque pixel
                }
                
                int* minimIndice = minimEtIndice(distances, nbreClusters);      //#On récupère la distance qui à été la plus petite
                
                int indice = minimIndice[0];

                clusters.lesClusters[indice].clust[(i*col)+j].valChannel = image.img[(i*col)+j].rgb;    //#Puis on range au bon endroit du cluster la valeur du pixel    
                cluster[i][j] = minimIndice[1];   //#On range enfin la valeur dans un tableau qui va nous servir par la suite à étudier le critère de continuité pour la recherche de nos clusters     
            }
        }

        tableauCost = rassemble(cluster, row, col);        //#Ce tableau subit donc une tranformation pour changer de dimension
        
        Cost0 = Cost;
        
        Cost = somme( tableauCost, N );    //#On calcule ainsi la fonction de coût, puis on revérifie si ce coût satisfait notre critère d'arrêt
        
        Iter += 1;
    }

    paramInit param;
    param.centres = centres;
    param.lesClusters = clusters.lesClusters;

    return param;
}


void serializeCluster(char* file, cluster clust , int row, int col) {

    FILE* fichier = NULL;

	fichier = fopen(file, "a+");

	if(fichier != NULL) {

        for(int i = 0; i < row; i++) {

            for(int j = 0; j < col; j++) {

                fprintf(fichier,"%d\n%d\n%d\n",clust.clust[(i*j)+col].valChannel[0],clust.clust[(i*j)+col].valChannel[1],clust.clust[(i*j)+col].valChannel[2]);
            }
        }
    
        fclose(fichier);
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
}


void serializeCluster2( char* file, cluster clust, int row, int col ) {
    
    FILE* fichier = NULL;

	fichier = fopen(file, "w+");

	if(fichier != NULL) {

        for(int i = 0; i < row; i++) {

            for(int j = 0; j < col; j++) {

                fprintf(fichier,"(%d,%d) (%d-%d-%d)\n",clust.clust[(i*col)+j].indice[0], clust.clust[(i*col)+j].indice[1], clust.clust[(i*col)+j].valChannel[0], clust.clust[(i*col)+j].valChannel[1], clust.clust[(i*col)+j].valChannel[2]);
            }
        }
        fclose(fichier);
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
}


void serializeCentroid( char* file, centroid* c, int nbreClusters ) {
    
    FILE* fichier = NULL;

	fichier = fopen(file, "a+");

	if(fichier != NULL) {

        for(int i = 0; i < nbreClusters; i++) {

            fprintf(fichier,"(%d,%d) (%d-%d-%d)\n",c[i].indice[0],c[i].indice[1],c[i].valChannel[0],c[i].valChannel[1],c[i].valChannel[2]);
        }
        fclose(fichier);
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
}


void serializeHisto( char *file, int*** histo, int nbreL, int nbreCol, int nbreR ) {

    FILE* fichier = NULL;

	fichier = fopen(file, "w+");

	if(fichier != NULL) {

        for(int i = 0; i < nbreL; i++) {

            for(int j = 0; j < nbreCol; j++) {

                for(int k = 0; k < nbreR; k++) {
                    
                    fprintf(fichier,"%d ",histo[i][j][k]);
                }
                fprintf(fichier, "\n");
            }
           
        }
        fclose(fichier);
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }

}


void serialize2DArray( char *file, float** array, int nbreL,int nbreCol ) {
    FILE* fichier = NULL;
	fichier = fopen(file, "w+");
    if(fichier != NULL) {
        for(int i = 0; i < nbreL; i++) {
            for(int j = 0; j < nbreCol; j++) {
                fprintf(fichier,"%f ",array[i][j]);
            }
            fprintf(fichier, "\n");
        }
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
}


void transform( char *file, int*** array, int nbreL, int nbreCol, int nbreR ) {

    FILE* fichier = NULL;
	fichier = fopen(file, "w+");

    if(fichier != NULL) {
        
        //  Red
        for(int i = 0; i < nbreL; i++) {

            for(int j = 0; j < nbreR; j++) {
                fprintf(fichier,"%d ",array[i][2][j]);
            }
            fprintf(fichier, "\n");
        }

         //  Green
        for(int i = 0; i < nbreL; i++) {

            for(int j = 0; j < nbreR; j++) {
                fprintf(fichier,"%d ",array[i][1][j]);
            }
            fprintf(fichier, "\n");
        }

         //  Blue
        for(int i = 0; i < nbreL; i++) {

            for(int j = 0; j < nbreR; j++) {
                fprintf(fichier,"%d ",array[i][0][j]);
            }
            fprintf(fichier, "\n");
        }
    }
    else {
        fprintf(stderr, "Erreur : Echec de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }
}

Image clusterToImage( cluster clust, int row, int col ) {

    Image img;
    img.deep = 3;
    img.height = row;
    img.width = col;

    int** redChannelValues = NULL;
    int** greenChannelValues = NULL;
    int** blueChannelValues = NULL; 

    redChannelValues = allocation_dynamique(row, col);
    greenChannelValues = allocation_dynamique(row, col);
    blueChannelValues = allocation_dynamique(row, col);

    for(int i = 0; i < row; i++) {
       
       for(int j = 0; j < col; j ++) {

            redChannelValues[i][j] = clust.clust[(i*col)+j].valChannel[0];
            greenChannelValues[i][j] = clust.clust[(i*col)+j].valChannel[1];
            blueChannelValues[i][j] = clust.clust[(i*col)+j].valChannel[2];
       }
    }

    img.imgRed = redChannelValues;
    img.imgGreen = greenChannelValues;
    img.imgBlue = blueChannelValues;

    return img;
}

int*** transf(int*** collection, int nbreL, int nbreCol, int nbreR) {

    int** R = allocation_dynamique(nbreL, nbreR);
    int** V = allocation_dynamique(nbreL, nbreR);
    int** B = allocation_dynamique(nbreL, nbreR);

    for(int k = 0; k < nbreL; k++) {

        for(int j = 0; j < nbreCol; j++) {

            if( j % 3 == 0 ) {

                for(int i = 0; i < nbreR; i++) {

                    B[k][i] = collection[k][j][i];
                }
            }
            else if ( j % 3 == 1 ) {

                for(int i = 0; i < nbreR; i++) {
                    
                    V[k][i] = collection[k][j][i];
                }
            }
            else {

                for(int i = 0; i < nbreR; i++) {
                    
                    R[k][i] = collection[k][j][i];
                }
            }
        }
    }

    int*** result = allocateDynamiq( nbreCol, nbreL, nbreR );

    for(int i = 0; i < nbreCol; i++) {

        if( i % 3 == 0 ) {

            result[i] = R;
        }
        else if ( i % 3 == 1 ) {

            result[i] = V;
        }
        else {

            result[i] = B;
        }
    }
    return result;
}

int*** generateHisto(cluster *clust, int nbreL, int nbreCol, int nbreR, int nbreClusters, int row, int col) {
    int** R = allocation_dynamique(nbreL, nbreR);
    int** G = allocation_dynamique(nbreL, nbreR);
    int** B = allocation_dynamique(nbreL, nbreR);

    for( int c = 0; c < nbreClusters; c++) {
        Image img = clusterToImage( clust[c], row, col);
        for(int k = 0; k < 256; k++) {
            for(int i = 0; i < row; i++) {   
                for(int j = 0; j < col;  j++) {
                    if ( img.imgRed[i][j] == k ) {
                        R[c][k] += 1;
                    }
                    if ( img.imgGreen[i][j] == k ) {
                        G[c][k] += 1;
                    }
                     if ( img.imgBlue[i][j] == k ) {
                        B[c][k] += 1;
                    }
                }
            }
        }
    }
    
    int*** LesC_Histog5 = allocateDynamiq( nbreCol, nbreL, nbreR );
    LesC_Histog5[0] = R;
    LesC_Histog5[1] = G;
    LesC_Histog5[2] = B;

    serializeHisto( "imageHisto.txt", LesC_Histog5, nbreCol, nbreL, nbreR );

    return LesC_Histog5;
}

int*** generatePKCHisto(cluster *clust, int nbreL, int nbreCol, int nbreR, int nbreClusters, int row, int col) {

    generateHisto(clust, nbreL, nbreCol, nbreR, nbreClusters, row, col);

    int*** LesC_Histog5 = allocateDynamiq( nbreL, nbreCol, nbreR );      //# shape vaut 51 3 256

    for(int c = 0; c < nbreClusters; c++) {      //#51

        int *PB = (int *)calloc(256, sizeof(int));
        int *PV = (int *)calloc(256, sizeof(int));
        int *PR = (int *)calloc(256, sizeof(int));

        Image img = clusterToImage( clust[c], row, col );
        
        for(int k = 0; k < 256; k++) {

            for(int i = 0; i < row; i++) {      //#On affiche les (h)*(l) pixels de chaque couleur du cluster

                for(int j = 0; j < col;  j++) {

                    if ( img.imgBlue[i][j] == k ) {
                        PB[k] += 1;
                    }
                    if ( img.imgGreen[i][j] == k ) {
                        PV[k] += 1;
                    }    
                    if ( img.imgRed[i][j] == k ) {
                        PR[k] += 1;
                    }
                }
            }
            
        }

        LesC_Histog5[c][0] = PB;
        LesC_Histog5[c][1] = PV;
        LesC_Histog5[c][2] = PR;

        free(PB);
        free(PV);
        free(PR);
    }
    serializeHisto( "imageHisto3.txt", LesC_Histog5, nbreL, nbreCol, nbreR );
    // Transformation

    // int** R = allocation_dynamique(nbreL, nbreR);
    // int** V = allocation_dynamique(nbreL, nbreR);
    // int** B = allocation_dynamique(nbreL, nbreR);

    // for(int k = 0; k < nbreL; k++) {

    //     for(int j = 0; j < nbreCol; j++) {

    //         if( j % 3 == 0 ) {

    //             for(int i = 0; i < nbreR; i++) {

    //                 B[k][i] = LesC_Histog5[k][j][i];
    //             }
    //         }
    //         else if ( j % 3 == 1 ) {

    //             for(int i = 0; i < nbreR; i++) {
                    
    //                 V[k][i] = LesC_Histog5[k][j][i];
    //             }
    //         }
    //         else {

    //             for(int i = 0; i < nbreR; i++) {
                    
    //                 R[k][i] = LesC_Histog5[k][j][i];
    //             }
    //         }
    //     }
    // }
    transform( "imageHisto2.txt", LesC_Histog5,  nbreL,  nbreCol,  nbreR );
    // //int*** result = allocateDynamiq( nbreCol, nbreL, nbreR );
    // int*** result = (int***)malloc(nbreCol * sizeof(int***));

    // for(int i = 0; i < nbreCol; i++) {

    //     if( i % 3 == 0 ) {

    //         result[i] = R;
    //     }
    //     else if ( i % 3 == 1 ) {

    //         result[i] = V;
    //     }
    //     else {

    //         result[i] = B;
    //     }
    // }



    
    // //int ***histos = transf(LesC_Histog5, nbreL, nbreCol, nbreR);
    // //int ***histosNorm = Normalization(histos, nbreCol, nbreL, nbreR );

    // serializeHisto( "imageHisto2.txt", result, nbreCol, nbreL, nbreR );
    // //serializeHisto( "imageHistoNorm.txt", histosNorm, nbreCol, nbreL, nbreR);
    // // VersChaineM(histosNorm, 50, nbreCol, nbreL, nbreR );
    
    return LesC_Histog5;  
}


