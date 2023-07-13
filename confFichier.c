#include <stdio.h>
#include <stdlib.h>
#include "GeneHisto.h"

//# Cette fonction prend en paramètre un tableau et renvoie en sortie l'élément maximal de celui-ci
int Max(int *tableau, int taille) {

    int max = tableau[0];

    for(int i = 1; i < taille; i++) {

        if( tableau[i] > max ) {
             max = tableau[i];
        }       
    }
    return max;
}
    

//#cette fonction prend en paramètre un tableau et renvoie en sortie l'élément minimal de celui-ci
float Min(float *tableau, int taille) {

    float min = tableau[0];

    for(int i = 1;  i < taille; i++) {

        if(tableau[i] < min) {
            min = tableau[i];
        }
    }
    return min;
}

//#cette fonction collecte les éléments max de chaque histogramme de l'ensemble
int* table(int **EnsembleHistogrammes, int nbreLignes, int nbreCol) {       

    int* T = (int *)calloc(nbreLignes, sizeof(int));

    for(int k = 0; k < nbreLignes; k++) {

         T[k] = Max(EnsembleHistogrammes[k], nbreCol);
    }
       
    return T;
}


float maximum(float a, float b) {

    return  (a > b) ? a : b;
}


float minimum(float a, float b) {

    return  (a > b) ? b : a;
}

//# Collection correspond à un ensemble d'histogrammes (par exemples les histogrammes des partitions d'une image)
int*** Normalization(int ***Collection, int nbreL, int nbreCol, int nbrDeep) {

    for(int k = 0; k < nbreL; k++) {

        int maxi = Max( table( Collection[k], nbreCol, nbrDeep ) , nbreCol);

        for(int i = 0; i < nbreCol; i++) {
           
            for(int j = 0; j < nbrDeep; j++) {

                Collection[k][i][j] = 100*(Collection[k][i][j])/maxi;
            }
        }
    }
    serializeHisto( "imageHistoNorm.txt", Collection, nbreL, nbreCol, nbrDeep);
    return Collection;
}


float** redefinition(int Nb) {

    float **T = allocation_dynamiqueFloat(Nb+1, 2);

    if ( Nb >= 1 ) {

        for(int k = 1; k < Nb+1; k++) {

            T[k][0] = 100*(k-1)/Nb;
            T[k][1] = 100*(k)/Nb;
        }
    }
    return T;
}


int*** VersChaineM(int ***tabl, int Nb, int nbreL, int nbreCol, int nbreR) {
    
    int*** F = allocateDynamiq( nbreL, nbreCol, nbreR+1 );
    //int** Fp = allocation_dynamique(3, (nbreR+1)*(Nb+1));
    float** L = redefinition( Nb );
    serialize2DArray("redefinition.txt", L, Nb+1, 2);
    for(int r = 0; r < nbreL; r++) {

        int **tableau = tabl[r];

        for(int l = 0; l < nbreCol; l++) {

            // int* T = (int*)calloc(nbreR+1, sizeof(int));
            int c = 1;

            for(int i = 0; i < nbreR; i++) {

                for(int j = 1; j < Nb+1; j++) {

                    if( (tableau[l][i] <= L[j][1]) && (tableau[l][i] > L[j][0]) ){
                        F[r][l][c] = j;
                        c++;
                        continue;
                    }
                }
            }
            // F[r][l] = T;
            // free(T);
        }
    }

    serializeHisto( "imageVersChaines.txt", F, nbreL, nbreCol, nbreR+1);
    return F;
}


int absolue(int x) {

    if( x >= 0 ) {
        return x;
    }
    else {
        return (-1) * x;
    }
}


int len(int* tab, int taille) {
    int s = 0;
    for(int i = 1; i < taille; i++) {
        if(tab[i] != 0) {
            s++;
        }
    }
    return s;
}


int*** Hchapeaux(int ***Hnorm, int nbreL, int nbreCol, int nbreR) {
   
    int*** hChapeu = allocateDynamiq( nbreL, nbreCol, nbreR );

    for(int r = 0; r < nbreL; r++) {

        int **tableau = Hnorm[r];

        for(int l = 0; l < nbreCol; l++) {

            if( len( tableau[l], nbreR) != 0 ) {

                // int* T = (int*)calloc(nbreR, sizeof(int));
                hChapeu[r][l][0] = tableau[l][1];
                int c = 1;

                for(int j = 0; j < nbreR-1; j++) {
                    
                    if( tableau[l][j+2] != 0 ) {
                        hChapeu[r][l][c] = tableau[l][j+2];
                        c++;
                    }
                }
                // hChapeu[r][l] = T;
                // free(T);
            }
        }
    }

    serializeHisto( "imageHChapeau.txt", hChapeu, nbreL, nbreCol, nbreR);
    return hChapeu;
}


int*** transformerEtats(int ***Hchapeau, int nbreL, int nbreCol, int nbreR) {

    int*** hChapeu = allocateDynamiq( nbreL, nbreCol, nbreR );

    for(int r = 0; r < nbreL; r++) {

        int **tableau = Hchapeau[r];

        for(int l = 0; l < nbreCol; l++) {
            int taille = len( tableau[l], nbreR);
            if(  taille != 0 ) {
                //int* T = (int*)calloc(nbreR, sizeof(int));
                hChapeu[r][l][0] = tableau[l][0];

                for(int k = 1; k < taille+1; k++) {
                    
                    hChapeu[r][l][k] = absolue( tableau[l][k] - tableau[l][k-1] );
                }

                // hChapeu[r][l] = T;
                // free(T);
            }
        }
    }

    serializeHisto( "imageEtats.txt", hChapeu, nbreL, nbreCol, nbreR);
    return hChapeu;
}


MarkovChains markovChain(int ***versChaines, int ***Etats, int nbreL, int nbreCol, int nbreR) {

    MarkovChains markovChains;
    markovChains.markovch = (MarkovChain *)malloc((nbreL*nbreCol) * sizeof(MarkovChain));
    
    if (markovChains.markovch == NULL) {
        printf("Erreur d'allocation de mémoire pour la chaine de Markov\n");
        exit(1); // arrêt du programme avec code d'erreur 1
    }
    for(int i = 0; i < nbreL; i++) {

        for(int j = 0; j < nbreCol; j++) {

            markovChains.markovch[(i*nbreCol)+j].symb = versChaines[i][j];
            markovChains.markovch[(i*nbreCol)+j].etats = Etats[i][j];
            markovChains.markovch[(i*nbreCol)+j].taille = len(versChaines[i][j], nbreR);
        }
    }

    return markovChains;
}


int transit(int j, int k, int* etats, int taille) {
    int nb = 0;
    for(int l = 0; l < taille-1; l++) {

        if ((etats[l] == j) && (etats[l+1] == k)) {
            nb ++;
        }
    }
    return nb;
}


int transitToAny(int j, int* etats, int taille) {
    int nb = 0;
    for(int l = 0; l < taille-1; l++) {

        if (etats[l] == j) {
            nb ++;
        }
    }
    return nb;
}


StatChains statTransitions(int Nb, MarkovChains markovChains, int nbreL, int nbreCol, int nbreR) {

    int s = 0;
    int p0 = 0;
    int p1 = 0;

    int** tableauxBon = allocation_dynamique(nbreL*nbreCol*Nb+1*Nb+1, 2);
    
    int cpt = nbreCol;
    int i = 0;
    int temp = 0;
    int inc = 0;

    while ( inc < nbreL ) {
        for(int j = 0; j < Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {

                for(i = temp; i < cpt; i++) {
                    tableauxBon[s][0] = transit(j, k, markovChains.markovch[i].etats, markovChains.markovch[i].taille);
                    tableauxBon[s][1]  = transitToAny(j, markovChains.markovch[i].etats, markovChains.markovch[i].taille);
                    s++;
                }
            }
        }
        printf("Pour inc = %d, s = %d\n", inc, s);
        temp = cpt;
        cpt +=nbreCol;
        inc++;
    }

    StatChains statChains;
    statChains.statChainTab = (StatChain *)malloc((nbreL) * sizeof(StatChain));

    if (statChains.statChainTab == NULL) {
        printf("Erreur d'allocation de mémoire pour la chaine de Markov\n");
        exit(1); // arrêt du programme avec code d'erreur 1
    }

    inc = 0;
    s = 0;
    
    while ( inc < nbreL)
    {
        int** transiter0 = allocation_dynamique(Nb+1, Nb+1);
        int** transiter1 = allocation_dynamique(Nb+1, 1);

        for(int j = 0; j < Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {
                
                for(int v = 0; v < nbreCol; v++) {
                    p0 += tableauxBon[s][0];
                    p1 += tableauxBon[s][1];
                    s++;
                }

                transiter0[j][k] = p0;
                transiter1[j][0] = p1;

                p0 = 0;
                p1 = 0;

            }
        }
        statChains.statChainTab[inc].transiter0 = transiter0;
        statChains.statChainTab[inc].transiter1 = transiter1;

        deallocate_dynamic_matrix(transiter1, Nb+1);
        deallocate_dynamic_matrix(transiter0, Nb+1);

        inc++;
    }

    deallocate_dynamic_matrix(tableauxBon,nbreL*nbreCol*Nb+1*Nb+1);

    statChains;     
}


float*** pseudoProba(int Nb, StatChains statChains, int nbreL, int nbreCol, int nbreR) {

    
    float*** tableur = allocateDynamiqFloat(3, Nb+1, Nb+1);

    for(int i = 0; i < nbreL; i++) {

        for(int j = 0; j <  Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {

                tableur[i][j][k] = (statChains.statChainTab[i].transiter0[j][k]) / (statChains.statChainTab[i].transiter0[j][0]+1);
            }
        }
    }

    return tableur;
}


int observe(int j, int k, MarkovChain chaineM, int taille) {
    int s = 0;
    for(int i = 0;  i < taille; i++) {
        if ( (chaineM.symb[i+1] == k) && (chaineM.etats[i] == j) ) {
            s ++;
        }
    }
    return s;
}


int observeAny(int j, MarkovChain chaineM, int taille) {
    int s = 0;
    for(int i = 0;  i < taille; i++) {
        if (chaineM.etats[i] == j) {
            s++;
        }
    }
    return s;
}


StatChains statObservations(int Nb, MarkovChains markovChains, int nbreL, int nbreCol, int nbreR) {

    int s = 0 ;
    int p0 = 0; 
    int p1 = 0; 
    int** tableauxBon = allocation_dynamique(nbreL*nbreCol*Nb+1*Nb+1, 2);
    int** observer0 = allocation_dynamique(Nb+1, Nb+1);
    
    int cpt = nbreCol;
    int i = 0;
    int temp = 0;
    int inc = 0;

    while ( inc < nbreL ) {
        for(int j = 0; j < Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {

                for(i = temp; i < cpt; i++) {

                    tableauxBon[s][0] = observe(j, k, markovChains.markovch[i], markovChains.markovch[i].taille);
                    tableauxBon[s][1] = observeAny(j, markovChains.markovch[i], markovChains.markovch[i].taille);
                    s++;
                }
            }
        }
        temp = cpt;
        cpt +=nbreCol;
        inc++;
    }

    StatChains statChains;
    statChains.statChainTab = (StatChain *)malloc((nbreL) * sizeof(StatChain));

    if (statChains.statChainTab == NULL) {
        printf("Erreur d'allocation de mémoire pour la chaine de Markov\n");
        exit(1); // arrêt du programme avec code d'erreur 1
    }

    inc = 0;
    s = 0;
    
    while ( inc < nbreL)
    {
        int** observer00 = allocation_dynamique(Nb+1, Nb+1);
        int** observer1 = allocation_dynamique(Nb+1, 1);

        for(int j = 0; j < Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {
                
                for(int v = 0; v < nbreCol; v++) {
                    p0 += tableauxBon[s][0];
                    p1 += tableauxBon[s][1];
                    s++;
                }

                observer00[j][k] = p0;
                observer1[j][0] = p1;

                p0 = 0;
                p1 = 0;

            }
        }

        for(int j = 0; j < Nb+1; j++) {

            for(int k = 0; k < Nb+1; k++) {
                observer0[j][k] = observer00[k][j];
            }
        }
        
        statChains.statChainTab[inc].transiter0 = observer0;
        statChains.statChainTab[inc].transiter1 = observer1;     

        deallocate_dynamic_matrix(observer1, Nb+1);
        deallocate_dynamic_matrix(observer00, Nb+1);

        inc++;
    }

    deallocate_dynamic_matrix(tableauxBon,nbreL*nbreCol*Nb+1*Nb+1);

    statChains;
}


// void train_model(int number_iter) {

//     int* states = (int*)calloc(51, sizeof(int));
//     int* possible_observation = (int*)calloc(51, sizeof(int));
// }


