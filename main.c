#include <stdio.h>
#include <stdlib.h>
#include "GeneHisto.h"
#include <time.h>
//#include <dirent.h>
#include <stdio.h>

int main() {
    char* nomImage = "image.ppm";
    char* nomFile = "clusterSerialize.txt";
    int nbreCulsters = 51;
    int row = 200;
    int col = 180;

    clock_t debut, fin;
    double temps;

    if (!fichierExiste(nomImage)) {
        printf("Erreur : le fichier %s n'existe pas.\n", nomImage);
        return -1;
    }
    else {
        printf("Le fichier : %s exite .\n", nomImage);
    }

    // Image img = split( nomImage, row, col );

    // for (int i = 0; i < row; i++) {
    //     for (int j = 0; j < col; j++)
    //     {
    //        printf("imgRed[%d][%d] = %d\t",i,j,img.imgRed[i][j]);
    //     }
    //     printf("\n");
    //     printf("i = %d\n", i);
    // }
    //debut = clock();
    //paramInit pram = InitializeCluster(nomImage, row, col, nbreCulsters);
    //fin = clock();
    // for(int i = 0; i < row; i++) {

    //     for(int j = 0; j < col; j++) {

    //         printf("%d\n%d\n%d\n",c.clust[(i*j)+col].valChannel[0],c.clust[(i*j)+col].valChannel[1],c.clust[(i*j)+col].valChannel[2]);
    //     }
    //     printf("i = %d\n", i);
    // }

    debut = clock();
    paramInit param =  clusteriser(nomImage, row, col, nbreCulsters);
     for(int i = 0; i < nbreCulsters; i++) {
        serializeCluster(nomFile, param.lesClusters[i] , row, col);
    }
    int*** Collection = generateHisto(param.lesClusters, nbreCulsters, 3, 256, nbreCulsters, row, col);
    int*** histoNorm = Normalization(Collection, 3, nbreCulsters, 256);
    int*** histosVers =  VersChaineM(histoNorm, 50, 3, nbreCulsters, 256);
    int*** hchapeu = Hchapeaux(histosVers, 3, nbreCulsters, 256);
    int*** Etats = transformerEtats(hchapeu, 3, nbreCulsters, 256);
    MarkovChains markovChains = markovChain(histosVers, Etats, 3, nbreCulsters, 256);

    printf("FIN !!!\n");
    fin = clock();

    // Affichage du nombre de clusters et des centres de cluster
    printf("Nombre de clusters : %d\n", nbreCulsters);

    float* cent = (float *)calloc(3, sizeof(float));

    for (int i = 0; i < 51; i++) {
        for (size_t j = 0; j < 3; j++)
        {
           cent[j] = param.centres[i][j];
        }
        
        printf("Centre de cluster %d : (%.3f, %.3f, %.3f)\n", i, cent[0], cent[1], cent[2]);
    }

    // for(int i = 0; i < nbreCulsters; i++) {
    //     serializeCluster(nomFile, param.lesClusters[i] , row, col);
    // }

    temps = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("\n\nTemps = %f secondes", temps);
    return 0;
}
    // printf("Main\n"); Systèmes Embarqués : système cyber-physique + réseaux de pétri
    // // Image img = split( argv[1], 200, 180 );
    // // printf("Width : %d\nHeight : %d\nDeep : %d", img.width, img.height, img.deep);
    // //readImage( argv[1], 200, 180 );
    // int*** PR = allocateDynamiq( 3 );

    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //        for(int k = 0; k < 3; k++){
    //         printf("PR[%d][%d][%d] = %d\t", i, j, k,PR[i][j][k]);
    //        }
    //        printf("\n");
    //     }
    //     printf("\n\n");
    // }

        // for(int i = 0; i < 51; i++) {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         printf("%f\t",param.centres[i][j]);
    //     }
    //     printf("\n");
    // }