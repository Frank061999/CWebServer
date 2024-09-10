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
#define VALUE_SIZE 32


size_t hashMapSize(void);
typedef struct HashMap HashMap;
char* HashMapGet(HashMap* hashMap, const char* key);
void FreeHashMap(HashMap* hm);
HashMap* CreateHashMap(size_t bucketSize);
int HashMapAppend(HashMap* hashMap, char* key, size_t keyLen, char* value, size_t valueLen);

void PrintHashMap(HashMap* hashMap);



#endif /* HashMap_h */
