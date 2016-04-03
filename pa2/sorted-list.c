#include"sorted-list.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


/***************************************************
 *SLCreate receives function pointers to cf and df, then it allocates enough space on the heap for a struct of SortedList, !!warning!! sizeof(SortedListPtr)
 does not provide sufficient space then you set all of the allocates memory to 0 so we know that everything points to * "empty"
 **************************************************/
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	SortedListPtr tmp = malloc(sizeof(struct SortedList));
	memset(tmp,0,sizeof(struct SortedList));
	tmp->d = df;
	tmp->c = cf;
	return tmp;
}


/***************************************************
 traverse and free.
 **************************************************/
void SLDestroy(SortedListPtr list){
	Node* curr = list->head;
	Node* del = curr;
	while(curr !=0){
		del = curr;
		curr = curr->next;
		(list->d)(del->data);
		free(del);
	}
	free(list);
	return;

}


/***************************************************
 * Helper function for SLInsert, allocates space on heap for Node* and initializes values. n1->data points to newObj, next points to 0 or NULL,
 * and the amount of pointers pointing to this node is set to 1.
 **************************************************/
Node* create_node(void *newObj){

	Node* n1 = malloc(sizeof(Node));
	n1->data = newObj;
	n1->next = 0;
	n1->pointCount = 1;
	return n1;


}


/***************************************************
 *SLInsert is simple linked list insertion, it takes a list ptr, and void pointer to newObj. Node* tmp points to the node created in create_node helper function.
 * Four cases are covered in this function which are: head ==  NULL, if new node > head, new node belongs somewhere in the middle of the list, and new node 
 * belongs at the tail. Also, if something is a duplicate then it will immediately be freed upon detection.
 **************************************************/

int SLInsert(SortedListPtr list, void *newObj){
	Node* tmp = create_node(newObj);

	//base case if head == NULL
	if(list->head == 0){
		list->head = tmp;
		return 1;
	}


	//if new node is bigger than head
	int comp = list->c(list->head->data,tmp->data);
	if( comp < 0){
		tmp->next = list->head;
		list->head = tmp;
		return 1;

	}else if( comp  == 0){
		(list->d)(tmp->data);
		free(tmp);
		return 0;
	}
	//if node is somewhere other than first or null
	Node* curr = list->head->next;
	Node* prev = list->head;
	while(curr != 0){
		if(list->c(curr->data,tmp->data) < 0){
			tmp->next = curr;
			prev->next = tmp;
			return 1;
		}else if(list->c(curr->data,tmp->data) == 0){
			(list->d)(tmp->data);
			free(tmp);
			return 0;
		}else if(list->c(curr->data,tmp->data) > 0){
			prev = curr;
			curr = curr->next;
		}
	}
	//inserts at tail
	prev->next = tmp;
	return 1;


}


//need to delete***************************************************************************************
void print(SortedListPtr list){
	Node* curr = list->head;
	while(curr != 0){
		printf("%s  ", (char*)curr->data);
		curr = curr->next;
	}
	printf("\n");


}

/***************************************************
 * SLRemove takes list and void pointer to data, create Node* del to keep access to obj being removed. First case is if head is target and then if target is 
 * somewhere else. If pointCount == 0 then that means no iterator is pointing to it so we can remove and free memory immediately otherwise if it does not == 0 
 * then we simply remove it from list. 
 **************************************************/
int SLRemove(SortedListPtr list, void *newObj){
	Node* del;

	//if head is target
	if((list->c)(list->head->data,newObj)== 0){
		list->head->pointCount--;
		del = list->head;
		list->head = list->head->next;
		if(del->pointCount == 0){
			(list->d)(del->data);
			free(del);
		}
		return 1;
	}

	Node* curr = (Node*)list->head->next;
	Node* prev = list->head;

	while(curr != 0){
		if(list->c(curr->data,newObj)== 0){
			prev->next = curr->next;
			((Node*)prev->next)->pointCount++;
			curr->pointCount--;
			if(curr->pointCount == 0){
				((Node*)curr->next)->pointCount--;
				(list->d)(curr->data);
				free(curr);
				return 1;
			}
			return 1;
		}
		prev = curr;
		curr = curr->next;
	
	}

	return 0;
}

/***************************************************
 * SLCreateIterator takes list and allocates memory for struct SortedListIterator. Sets curr equal to the head of the list. If if head is not NULL then the 
 * pointCount to head is increased by one since that node has 1 more pointer to it.
 **************************************************/

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr tmp = malloc(1*sizeof(struct SortedListIterator));
	tmp->curr = list->head;
	if(tmp->curr != 0){
		list->head->pointCount++;
	}
	return tmp;
}

/***************************************************
 *SLDestroy takes iterator and has three cases: when the iterator points to nothing we simply free the allocated iterator memory,
 when the iterator points to something already removed from the sorted list and no other iterator points to it so the only pointer keeping access is the current
 iterator, and case 3 is when its pointCount is more than 1 we can simply free the iterator but not the data.
 **************************************************/

void SLDestroyIterator(SortedListIteratorPtr iter){
	if(iter->curr == NULL){
		free(iter);
		return;
	}
	//points to nothing
	if(iter->curr == 0){
		free(iter);
		return;
	}
	//points to something that is only accessible through this iterator
	if(iter->curr->pointCount == 1 ){
		free(iter->curr->data);
		free(iter->curr);
		free(iter);
		return;
	}
	//points to something currently in list or pointed by other iterator but you only want to get rid of iterator
	iter->curr = NULL;
	free(iter);

}

/***************************************************
 * SLNextItem takes an iterator, checks to see if curr is NULL, if not then moves node pointer to the next position. A temp node pointer is used to maintain access
 * to node. If tmp node pointCount is 0 then it has been removed from sorted list and no other iterator points to it so we can free the memory it takes up
 * otherwise we leave it alone. If the new node that iterator points to is not NULL then its pointCount increases by one and void* obj points to the new node's 
 * data. If new node is NULL then void pointer is NULL and it is returned.
 *
 **************************************************/
void* SLNextItem(SortedListIteratorPtr iter){
	void* obj;
	Node* tmp;
	if(iter->curr == 0){
		return NULL;
	}
	tmp = iter->curr;
	iter->curr = iter->curr->next;

	tmp->pointCount--;
	if(tmp->pointCount == 0){
		free(tmp->data);
		free(tmp);
	}
	if(iter->curr!=0){
		iter->curr->pointCount++;
		obj = iter->curr->data;
	}else{
		obj = 0;
		return obj ;
	}

	
	return obj;
}

/***************************************************
 *SLGetItem takes iterator and if current node is NULL then it is pointing to nothing so we return NULL, otherwise void* obj points to current node's data and 
 * returns obj.
 **************************************************/
void* SLGetItem(SortedListIteratorPtr iter){
	void* obj;
	if(iter->curr == 0){
		///iter is pointing to something null
		obj = 0;
		return obj;
	}
	obj = iter->curr->data;
	return obj;
}






