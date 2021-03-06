#include <stdlib.h>
#include "linkedList.h"
#include <stdio.h>
#include <string.h>


LinkedList createList(void){
  LinkedList list;
  list.first = NULL;
  list.last = NULL;
  list.total_ele = 0;
  return list;
}

int add_to_list(LinkedList *list,void *ele){
  Element *e = (Element *)malloc(sizeof(Element));
  e->value = ele;
  e->next = NULL;
  if(list->total_ele == 0){
    list->first = list->last = e;
  }
  else{
    list->last->next = e;
    list->last = e;
  }
  list->total_ele += 1;
  return list->total_ele;
}

void *get_first_element(LinkedList list){
  return list.first->value;
}

void *get_last_element(LinkedList list){
  return list.last->value;
}

void forEach(LinkedList list, ElementProcessor e){
  for (size_t i = 0; i < list.total_ele; i++) {
    e(list.first->value);
    list.first = list.first->next;
  }
}

void *getElementAt(LinkedList list, int index){
  if(index >= list.total_ele || index < 0){return NULL;}
  Element *val = list.first;
  int counter = 0;
  while(counter != index){
      val = val->next;
      counter += 1;
  }
  return val->value;
}

int indexOf(LinkedList list, void *val){
  Element *list_ele = list.first;
  for (size_t i = 0; i < list.total_ele; i++) {
    int comparedVal = memcmp(list_ele->value,val,sizeof(void *));
    if(comparedVal == 0){return i;}
    list_ele = list_ele->next;
  }
  return -1;
}

//------------deleteElementAt-------------------------------//

void delete_first_ele(LinkedList *list,void **deleted_Value){
  list->total_ele -= 1;
  *deleted_Value = list->first->value;
  free(list->first);
  list->first = list->first->next;
}

void delete_last_ele(LinkedList *list,Element *list_ele){
  free(list_ele->next);
  list_ele->next = NULL;
  list->last = list_ele;
}

void *deleteElementAt(LinkedList *list, int index){
  void *deleted_Value = NULL;
  Element *list_ele = list->first;
  if(index < 0 || index > list->total_ele-1){
    return deleted_Value;
  }
  if(index == 0){
    delete_first_ele(list,&deleted_Value);
  }
  for (size_t i = 0; i < list->total_ele; i++) {
    if(i == index-1){
      deleted_Value = list_ele->next->value;
      if(index == list->total_ele-1){
        delete_last_ele(list,list_ele);
      }
      else{
        free(list_ele->next);
        list_ele->next = list_ele->next->next;
      }
      list->total_ele -= 1;
    }
    list_ele = list_ele->next;
  }
  return deleted_Value;
}

//--------------------------------------------------------------//

int asArray(LinkedList list, void **array, int maxElements){
  int counter = 0;
  Element *list_ele = list.first;
  for (size_t i = 0; i < maxElements; i++) {
    if(array[counter]){
      void *val = (void *)calloc(1,sizeof(void *));
      if(list_ele){
        val = list_ele->value;
        list_ele = list_ele->next;
      }
      array[counter] = val;
      counter++;
    }
  }
  return counter;
}

LinkedList filter(LinkedList list , filterFn refiner, void *ele ){
  LinkedList refinedList = createList();
  Element *list_ele = list.first;
  for (size_t i = 0; i < list.total_ele; i++) {
    if(refiner(list_ele->value,ele)){
      add_to_list(&refinedList,list_ele->value);
    }
    list_ele = list_ele->next;
  }
  return refinedList;
}

LinkedList reverse(LinkedList list){
  LinkedList reversedList = createList();
  for (size_t i = list.total_ele ; i > 0 ; i--) {
    add_to_list(&reversedList,getElementAt(list,i-1));
  }
  return reversedList;
}

LinkedList map(LinkedList list, mapFn mapper , void *ele){
  Element *list_ele = list.first;
  LinkedList mappedList = createList();
  for (size_t i = 0; i < list.total_ele; i++) {
    void *val = (void *)malloc(sizeof(void *));
    mapper(ele,list_ele->value,val);
    add_to_list(&mappedList,val);
    list_ele = list_ele->next;
  }
  return mappedList;
}

void *reduce(LinkedList list, reduceFn reducer , void *hint, void *initialValue){
  void *previousValue = (void *)malloc(sizeof(void *));
  previousValue = initialValue;
  Element *list_ele = list.first;
  for (size_t i = 0; i < list.total_ele; i++) {
    previousValue = reducer(hint,previousValue,list_ele->value);
    list_ele = list_ele->next;
  }
  return previousValue;
}
