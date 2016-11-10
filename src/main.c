#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <assert.h>

#define YES_OR_NO(value) (value != 0 ? "Yes" : "No")


struct empleado
{
	char *nombre;
	int salario;
	int ID;
};
/**
 * PrintChar
 * ---------
 * Mapping function used to print one character element in a vector.
 * The file pointer is passed as the client data, so that it can be
 * used to print to any FILE *.
 */

static void PrintChar(void *elem, void *fp)
{
  fprintf((FILE *)fp, "%c", *(char *)elem);
  fflush((FILE *)fp);
}


static int CompareChar(const void *elemA, const void *elemB)
{
  return (*(char *)elemA - *(char *)elemB);
}

static void TestAppend(vector *alphabet)
{
  char ch;
  int i;

  for (ch = 'A'; ch <= 'Z'; ch++)   //  Start with letters of alphabet
    VectorAppend(alphabet, &ch);
  fprintf(stdout, "First, here is the alphabet: ");
  VectorMap(alphabet, PrintChar, stdout);

  for (i = 0; i < 10; i++) {	    // Append digit characters
    ch = '0' + i;                   // convert int to ASCII digit character
    VectorAppend(alphabet, &ch);
  }
  fprintf(stdout, "\nAfter append digits: ");
  VectorMap(alphabet, PrintChar, stdout);
}


static void TestSearch(vector *v, char ch)
{
  int foundSorted, foundNot;

  foundSorted = VectorSearch(v, &ch, CompareChar, 0, true); // Test sorted
  foundNot = VectorSearch(v, &ch, CompareChar, 0, false);   // Not sorted
  fprintf(stdout,"\nFound '%c' in sorted array? %s. How about unsorted? %s.",
	  ch, YES_OR_NO((foundSorted != -1)),
	  YES_OR_NO((foundNot != -1)));
}


static void TestSortSearch(vector *alphabet)
{
  VectorSort(alphabet, CompareChar);	 // Sort into order again
  fprintf(stdout, "\nAfter sorting: ");
  VectorMap(alphabet, PrintChar, stdout);
  TestSearch(alphabet, 'J');	// Test searching capabilities
  TestSearch(alphabet, '$');
}


static void TestAt(vector *alphabet)
{
  int i;

  for (i = 0; i < VectorLength(alphabet); i += 2) { // Lowercase every other
    char *elem = (char *) VectorNth(alphabet, i);
    *elem = tolower(*elem);
  }

  fprintf(stdout, "\nAfter lowercase every other letter: ");
  VectorMap(alphabet, PrintChar, stdout);
}


static void TestInsertDelete(vector *alphabet)
{
  char ch = '-';
  int i;

  for (i = 3; i < VectorLength(alphabet); i += 4) // Insert dash every 4th char
    VectorInsert(alphabet, &ch, i);
  fprintf(stdout, "\nAfter insert dashes: ");
  VectorMap(alphabet, PrintChar, stdout);

  for (i = 3; i < VectorLength(alphabet); i += 3) // Delete every 4th char
    VectorDelete(alphabet, i);
  fprintf(stdout, "\nAfter deleting dashes: ");
  VectorMap(alphabet, PrintChar, stdout);

  ch = '!';
  VectorInsert(alphabet, &ch, VectorLength(alphabet));
  VectorDelete(alphabet, VectorLength(alphabet) - 1);
  fprintf(stdout, "\nAfter adding and deleting to very end: ");
  VectorMap(alphabet, PrintChar, stdout);
}


static void TestReplace(vector *alphabet)
{
  int found = 0;
  char toFind = 's', toReplace = '*';

  while (found < VectorLength(alphabet)) {
    found = VectorSearch(alphabet, &toFind, CompareChar, found, false);
    if (found == -1) break;
    VectorReplace(alphabet, &toReplace, found);
  }

  fprintf(stdout, "\nAfter changing all %c to %c: ", toFind, toReplace);
  VectorMap(alphabet, PrintChar, stdout);
}


static void SimpleTest()
{
  fprintf(stdout, " ------------------------- Starting the basic test...\n");
  vector alphabet;
  VectorNew(&alphabet, sizeof(char), NULL, 4);
  TestAppend(&alphabet);
  TestSortSearch(&alphabet);
  TestAt(&alphabet);
  TestInsertDelete(&alphabet);
  TestReplace(&alphabet);
  VectorDispose(&alphabet);
}


static void InsertPermutationOfNumbers(vector *numbers, long n, long d)
{
  long k;
  long residue;
  fprintf(stdout, "Generating all of the numbers between 0 and %ld (using some number theory). ", d - 1);
  fflush(stdout); // force echo to the screen...

  for (k = 0; k < d; k++) {
    residue = (long) (((long long)k * (long long) n) % d);
    VectorAppend(numbers, &residue);
  }

  assert(VectorLength(numbers) == d);
  fprintf(stdout, "[All done]\n");
  fflush(stdout);
}


static int LongCompare(const void *vp1, const void *vp2)
{
  return (*(const long *)vp1) - (*(const long *)vp2);
}


static void SortPermutation(vector *vectorToSort)
{
  long residue, embeddedLong;
  vector *sortedVector;
  fprintf(stdout, "Sorting all of those numbers. ");
  fflush(stdout);
  VectorSort(vectorToSort, LongCompare);
  fprintf(stdout, "[Done]\n");
  fflush(stdout);
  fprintf(stdout, "Confirming everything was properly sorted. ");
  fflush(stdout);
  sortedVector = vectorToSort; // need better name now that it's sorted...
  for (residue = 0; residue < VectorLength(sortedVector); residue++) {
    embeddedLong = *(const long *) VectorNth(sortedVector, residue);
    assert(embeddedLong == residue);
  }
  fprintf(stdout, "[Yep, it's sorted]\n");
  fflush(stdout);
}

static void DeleteEverythingVerySlowly(vector *numbers)
{
  long largestOriginalNumber;
  fprintf(stdout, "Erasing everything in the vector by repeatedly deleting the 100th-to-last remaining element (be patient).\n");
  fflush(stdout);
  largestOriginalNumber = *(long *)VectorNth(numbers, VectorLength(numbers) - 1);
  while (VectorLength(numbers) >= 100) {
    VectorDelete(numbers, VectorLength(numbers) - 100);
    assert(largestOriginalNumber == *(long *)VectorNth(numbers, VectorLength(numbers) -1));
  }
  fprintf(stdout, "\t[Okay, almost done... deleting the last 100 elements... ");
  fflush(stdout);
  while (VectorLength(numbers) > 0) VectorDelete(numbers, 0);
  fprintf(stdout, "and we're all done... whew!]\n");
  fflush(stdout);
}


static const long kLargePrime = 1398269;
static const long kEvenLargerPrime = 3021377;
static void ChallengingTest()
{
  vector lotsOfNumbers;
  fprintf(stdout, "\n\n------------------------- Starting the more advanced tests...\n");
  VectorNew(&lotsOfNumbers, sizeof(long), NULL, 4);
  InsertPermutationOfNumbers(&lotsOfNumbers, kLargePrime, kEvenLargerPrime);
  SortPermutation(&lotsOfNumbers);
  DeleteEverythingVerySlowly(&lotsOfNumbers);
  VectorDispose(&lotsOfNumbers);
}


static void FreeString(void *elemAddr)
{
  char *s = *(char **) elemAddr;
  free(s);
}


static void PrintString(void *elemAddr, void *auxData)
{
  char *word = *(char **)elemAddr;
  FILE *fp = (FILE *) auxData;
  fprintf(fp, "\t%s\n", word);
}

static void MemoryTest()
{
  int i;
  const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
  const int kNumQuestionWords = sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
  vector questionWords;
  char *questionWord;

  fprintf(stdout, "\n\n------------------------- Starting the memory tests...\n");
  fprintf(stdout, "Creating a vector designed to store dynamically allocated C-strings.\n");
  VectorNew(&questionWords, sizeof(char *), FreeString, kNumQuestionWords);
  fprintf(stdout, "Populating the char * vector with the question words.\n");
  for (i = 0; i < kNumQuestionWords; i++) {
    questionWord = malloc(strlen(kQuestionWords[i]) + 1);
    strcpy(questionWord, kQuestionWords[i]);
    VectorInsert(&questionWords, &questionWord, 0);  // why the ampersand? isn't questionWord already a pointer?
  }

  fprintf(stdout, "Mapping over the char * vector (ask yourself: why are char **'s passed to PrintString?!!)\n");
  VectorMap(&questionWords, PrintString, stdout);
  fprintf(stdout, "Finally, destroying the char * vector.\n");
  VectorDispose(&questionWords);
}

int emp_str_cmp(void *elem1, void *elem2)
{
	struct empleado *emp1=(struct empleado*)(elem1);
	struct empleado *emp2=(struct empleado*)(elem2);
	char *str1=emp1->nombre;
	char *str2=emp2->nombre;
	return strcmp(str1,str2);

}
int emp_int_cmp(void *elem1, void *elem2)
{
	struct empleado *emp1=(struct empleado*)(elem1);
	struct empleado *emp2=(struct empleado*)(elem2);
	int intemp1=emp1->salario;
	int intemp2=emp2->salario;
	return intemp1-intemp2;

}

void print_emp(struct empleado *empl)
{
	struct empleado emp=*empl;
	printf("Nombre: %s....ID: %d....Salario: %d \n",emp.nombre,emp.ID,emp.salario);
}
void struct_tests()
{
	struct empleado jorge,pedro,paco,juan,rodrigo;
	vector vector_emp;
	printf("Test de estructuras: Creando estructuras...\n");
	jorge.nombre=strdup("Jorge");
	jorge.salario=1000;
	jorge.ID=0;
	pedro.nombre=strdup("Pedro");
	pedro.salario=1300;
	pedro.ID=1;
	paco.nombre=strdup("Paco");;
	paco.salario=1050;
	paco.ID=2;
	juan.nombre=strdup("Juan");
	juan.salario=900;
	juan.ID=3;
	rodrigo.nombre=strdup("Rodrigo");
	rodrigo.salario=1100;
	juan.ID=4;
	printf("Estructuras creadas..\n");
	printf("Creando vector e insertando elementos..\n");
	VectorNew(&vector_emp, sizeof(struct empleado),
			  NULL, 3);
	VectorAppend(&vector_emp,&jorge);
	VectorAppend(&vector_emp,&pedro);
	VectorAppend(&vector_emp,&paco);
	VectorAppend(&vector_emp,&juan);
	VectorAppend(&vector_emp,&rodrigo);
	printf("Listo, ordenando de acuerdo al nombre.....\n");
	VectorSort(&vector_emp,emp_str_cmp);
	printf("Listo, lista ordenada. Imprimiendo.....\n");
	VectorMap(&vector_emp, print_emp,NULL);
	printf("Listo, borrando a Rodrigo y a Juan.....\n");
	VectorDelete(&vector_emp, VectorSearch(&vector_emp,&(rodrigo.nombre),emp_str_cmp,0,true));
	VectorDelete(&vector_emp, VectorSearch(&vector_emp,&(juan.nombre),emp_str_cmp,0,true));
	printf("Listo, ordenando de acuerdo al salario.....\n");
	VectorSort(&vector_emp,emp_int_cmp);
	printf("Listo, lista ordenada. Imprimiendo.....\n");
	VectorMap(&vector_emp, print_emp,NULL);

}


int main(int ignored, char **alsoIgnored)
{
  SimpleTest();
  ChallengingTest();
  MemoryTest();
  struct_tests();
  return 0;
}
