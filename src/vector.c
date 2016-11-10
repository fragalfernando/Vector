#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
  if(v==NULL)
	 return;
  assert(initialAllocation>0);
  v->elemSize=elemSize;
  v->maxSize=initialAllocation;
  v->allocSize=initialAllocation;
  v->freefn=freeFn;
  v->size=0;
  v->elems=malloc(v->maxSize*elemSize);
  assert(v->elems!=NULL);
}

void VectorDispose(vector *v)
{
	int i;
	if(v==NULL)
	   return;
	if (v->freefn != NULL)
		for (i = 0; i < v->size; i++)
			v->freefn((char *) v->elems + i * v->elemSize);

	free(v->elems);
}

int VectorLength(const vector *v)
{
	if(v==NULL)
	   return -1;
	return v->size;
}

void *VectorNth(const vector *v, int position)
{
	if(v==NULL)
	   return NULL;
	assert(position>=0 && position<(v->size));
	return ((char*)v->elems+position*(v->elemSize));

}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
	if(v==NULL)
	   return;
	assert(position>=0 && position<(v->size));
	memcpy((char*)v->elems+v->elemSize*position,elemAddr,v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
	if(v==NULL)
	   return;
	assert(position>=0 && position<=(v->size));
	if(v->size==v->maxSize)
	{
	   v->maxSize +=v->allocSize;
	   v->elems=realloc(v->elems,v->maxSize*v->elemSize);
	}
	memmove( (char*) v->elems+(position+1)*v->elemSize, (char*) v->elems+(position)*v->elemSize
			,(v->elemSize)*(v->size-position));
	memcpy((char*)v->elems+v->elemSize*position,elemAddr,v->elemSize);
    v->size++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
  if(v==NULL)
     return;
  if(v->size==v->maxSize)
  {
	  v->maxSize +=v->allocSize;
	  v->elems=realloc(v->elems,v->maxSize*v->elemSize);
  }

  memcpy((char*)v->elems+v->elemSize*v->size,elemAddr,v->elemSize);
  v->size++;

}

void VectorDelete(vector *v, int position)
{
	if(v==NULL)
	  return;
	assert(position>=0 && position<(v->size));

	if(v->freefn!=NULL)
	   v->freefn((char*)v->elems+v->elemSize*position);

	memmove( (char*) v->elems+(position)*v->elemSize,
			 (char*) v->elems+(position+1)*v->elemSize
			,(v->elemSize)*(v->size-position));
	v->size--;

}

void VectorSort(vector *v, VectorCompareFunction compare)
{
	if(v==NULL)
	  return;
	qsort (v->elems, v->size, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
   int i;
   assert(v!=NULL && mapFn!=NULL);
   for(i=0;i<v->size;i++)
     mapFn((char*)v->elems+i*v->elemSize,auxData);
}

static const int kNotFound = -1;

int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
	  void* elemAddr;
	  int i;
	  assert(startIndex>=0 && startIndex<v->size);
	  if(isSorted)
	  {
		  elemAddr=bsearch(key,(char*)v->elems+startIndex*v->elemSize,
					   v->size, v->elemSize,
					   searchFn);
		  if(elemAddr!=NULL)
			  return (elemAddr-v->elems)/v->elemSize;
	  }
	  else
		for(i=startIndex;i<v->size;i++)
			if(searchFn(key,(char*)v->elems+i*v->elemSize)==0)
				return i;

      return kNotFound;
}
