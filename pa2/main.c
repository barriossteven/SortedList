#include<stdio.h>
#include<stdlib.h>
#include"sorted-list.h"
#include<ctype.h>
#include<string.h>


int compareInt(void* a, void* b){
	double* aa = (double*)a;
	double* bb = (double*)b;
	if(*aa > *bb){
		return (1);
	}else if(*aa < *bb){
		return -1;
	}else {
		return 0;
	}

}

void destructInt(void* a){
	free(a);
	return;

}

int compareChar(void* a,void* b){
	char* aa = a;
	char* bb = b;

	return strcmp(aa,bb);
}

void destructChar(void* a){
	free(a);
	return;
}



int main(int argc, char** argv){

//***********create sorted list given functions and inserts ints in this case****
	SortedListPtr s1;
	s1 = SLCreate(compareInt, destructInt);

	double num;
	double *numptr;
	int status = 0;
	int i;
	for(i = 0; i<8; i++){
		scanf("%lf", &num);
		numptr = malloc(sizeof(double));
		*numptr = num;
		status = SLInsert(s1,numptr);
		if(status == 1){
			printf("insert success\n");
		}else if(status == 0){
			printf("duplicate\n");	
		}
	}

	print(s1);
	SortedListIteratorPtr i1;
	i1 = SLCreateIterator(s1);

	void* v = SLGetItem(i1);
	if(v != NULL){
		printf("this is v: %f\n", *(double*)v);
	}
	while(v != 0){
		//printf("list traversal\n");
		printf("%f ", *(double*)v);
		v = SLNextItem(i1);
	}
	
	/*
	print(s1);
	

	//needs to be ccreated after atleast one item in list is inserted
	SortedListIteratorPtr i1;
	i1 = SLCreateIterator(s1);

	void* v = SLGetItem(i1);
	if(v != NULL){
		printf("this is v: %d\n", *(int*)v);
	}
	
	SLRemove(s1,v);
	print(s1);
	printf("this is v after removal: %d\n", *(int*)v);

	v = SLNextItem(i1);
	if(v != NULL){
		printf("this is v: %d\n", *(int*)v);
	}


	print(s1);
	
	while(v != 0){
		//printf("list traversal\n");
		printf("%d ", *(int*)v);
		v = SLNextItem(i1);
	}

	SLDestroy(s1);
	SLDestroyIterator(i1);

*/

	return 0;





}

