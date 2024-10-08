//
//  HashMap.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/1/24.
//

#ifndef HashMap_h
#define HashMap_h

#include <stdio.h>
#define KEY_SIZE 32
#define VALUE_SIZE 512


size_t hashMapSize(void);
typedef struct HashMap HashMap;
char* HashMapGet(HashMap* hashMap, const char* key);
void FreeHashMap(HashMap* hm);
HashMap* CreateHashMap(size_t bucketSize);
int HashMapAppend(HashMap* hashMap, char* key, char* value);
int HashMapContains(HashMap* hm, char* key, char* value);
void PrintHashMap(HashMap* hashMap);



#endif /* HashMap_h */
