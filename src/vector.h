#ifndef _vector_
#define _vector_

typedef enum {
  false, true
} bool;


typedef int (*VectorCompareFunction)(const void *elemAddr1, const void *elemAddr2);
typedef void (*VectorMapFunction)(void *elemAddr, void *auxData);
typedef void (*VectorFreeFunction)(void *elemAddr);

typedef struct {
	void *elems;
	int elemSize;
	int maxSize;
	int allocSize;
	void (*freefn)(void *);
	int size;
} vector;

/** 
 * Function: VectorNew
 * Usar: vector myFriends;
 *        VectorNew(&myFriends, sizeof(char *), StringFree, 10);
 * -------------------
 * 
 * initialAllocation especifica el tamanno inicial del vector,
 * tambien indica en cuanto se debe aumentar el vector cada vez que se necesite.
 * El tamanno reservado indica cuantos espacios estan disponibles, mientras que 
 * la logitud indica cuanto esta ocupado
 * Si este valor es menor que cero un assert debe ocurrir. Si es igual a cero el default valor de
 * 4 debe utilizarce
 */

void VectorNew(vector *v, int elemSize, VectorFreeFunction freefn, int initialAllocation);
void VectorDispose(vector *v);
int VectorLength(const vector *v);
void *VectorNth(const vector *v, int position);
					  
/**
 * Function: VectorInsert
 * ----------------------
 * Un assert debe fallar si n es menor que 0 mayour que la longitud 
 */

void VectorInsert(vector *v, const void *elemAddr, int position);
void VectorAppend(vector *v, const void *elemAddr);
void VectorReplace(vector *v, const void *elemAddr, int position);
void VectorDelete(vector *v, int position);
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchfn, int startIndex, bool isSorted);
void VectorSort(vector *v, VectorCompareFunction comparefn);
void VectorMap(vector *v, VectorMapFunction mapfn, void *auxData);

#endif
