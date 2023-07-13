typedef struct {
    int width;
    int height;
    int deep;
    int **imgRed;
    int **imgGreen;
    int **imgBlue;
} Image;

typedef struct {
    int* rgb;
} pixel;

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    pixel *img;
} ImagePix;

typedef struct {
    int ***PB;
    int ***PV;
    int ***PR;
} Collection;

typedef struct {
    int* indice;
    int* valChannel;
} centroid;

typedef struct {
    centroid* clust;
} cluster;

typedef struct {
    float** centres;
    cluster *lesClusters;
} paramInit;

typedef struct {
    int* symb;
    int* etats;
    int taille;
} MarkovChain;

typedef struct {
    MarkovChain* markovch;
} MarkovChains;

typedef struct {
    int** transiter0;
    int** transiter1;
} StatChain;

typedef struct {
    StatChain* statChainTab;
} StatChains;

int fichierExiste(const char* nomFichier);
int wholeDiv( int a, int b );
int **allocation_dynamique(int row, int col);
float **allocation_dynamiqueFloat(int row, int col);
centroid **allocationListCentroid( int row, int col );
int ***allocateDynamiq( int row, int col, int deep  );
float ***allocateDynamiqFloat( int row, int col, int deep );
void deallocate_dynamic_matrix(int **matrix, int row);
Image split( char *file, int row, int col );
ImagePix imread( char *file, int row, int col );
int **slicing( int **matrix, int startRow, int endRow, int startCol, int endCol );
int contains( point *tab, point p, int taille);
centroid* centroides( char *imageFile, int row, int col );
centroid* centroide( char *imageFile, int row, int col );
cluster generateClusterEmpty( char *imageFile, int row, int col, int k );
cluster searchAndAssign(int i0, int j0, cluster clusterVide, int row, int col, int* valChannel);
cluster unClusCentroide(centroid* Centroids, cluster clusterVide, int i0, int j0, int k, int row, int col);
paramInit InitializeCluster(char* imageFile, int row, int col, int nbreClusters);
float somme( float *tableau, int n );
float distance(int* a, float *b);
int nbreDifferentsDeMoinsUn(cluster clust, int row, int col);
float* DetecteNMoinsUn(cluster clust, int row, int col);
int* minimEtIndice(float *tab, int taille);
int* minimEtIndiceMatrice(float **tab, int row, int col);
float* moyenne3(float *Vecteur, int Nbre);
float* rassemble(float **Collection, int row, int col);
paramInit clusteriser(char* imageFile, int row, int col, int nbreClusters);
void serializeCluster(char* file, cluster clust , int row, int col);
void serializeCluster2( char* file, cluster clust, int row, int col );
void serializeCentroid( char* file, centroid* c, int nbreClusters );
void serializeHisto( char *file, int*** histo, int nbreL, int nbreCol, int nbreR );
void serialize2DArray( char *file, float** array, int nbreL,int nbreCol );
void serializeVersChaine( char *file, int*** histo, int nbreL, int nbreCol, int nbreR );
int*** generatePKCHisto( cluster *clust, int nbreL, int nbreCol, int nbreR, int nbreClusters, int row, int col );
int Max(int *tableau, int taille);
float Min(float *tableau, int taille);
int* table(int **EnsembleHistogrammes, int nbreLignes, int nbreCol);
float maximum(float a, float b);
float minimum(float a, float b);
int*** Normalization(int ***Collection, int nbreL, int nbreCol, int nbrDeep);
float** redefinition(int Nb);
int*** VersChaineM(int ***tabl, int Nb, int nbreL, int nbreCol, int nbreR);
int absolue(int x);
int len(int* tab, int taille);
int*** Hchapeaux(int ***Hnorm, int nbreL, int nbreCol, int nbreR);
int*** transformerEtats(int ***Hchapeau, int nbreL, int nbreCol, int nbreR);
void shape( char *name, int** matrix );
void readImage( char *file, int row, int col );
Image clusterToImage( cluster clust, int row, int col );
void transform( char *file, int*** array, int nbreL, int nbreCol, int nbreR );
int*** generateHisto(cluster *clust, int nbreL, int nbreCol, int nbreR, int nbreClusters, int row, int col);
MarkovChains markovChain(int ***versChaines, int ***Etats, int nbreL, int nbreCol, int nbreR);
StatChains statTransitions(int Nb, MarkovChains markovChains, int nbreL, int nbreCol, int nbreR);
float*** pseudoProba(int Nb, StatChains statChains, int nbreL, int nbreCol, int nbreR);
int observe(int j, int k, MarkovChain chaineM, int taille);
int observeAny(int j, MarkovChain chaineM, int taille);
StatChains statObservations(int Nb, MarkovChains markovChains, int nbreL, int nbreCol, int nbreR);