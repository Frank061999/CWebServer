//
//  HashMap.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/1/24.
//

#include "HashMap.h"
#include "string.h"
#include <stdlib.h>

#define BUCKET_SIZE 127


unsigned long long hash(char* string, size_t strLen){
    const unsigned long long fnvPrime = 1099511628211ULL;
    unsigned long long hash = 14695981039346656037ULL;
    for (int i = 0; i < strLen; i++) {
        hash *= fnvPrime;
        hash ^= (unsigned char)string[i];
    }
    return hash;
}

struct KeyValue {
    char key[KEY_SIZE];
    char* value;
};

typedef struct HashMap {
    size_t bucketSize;
    struct KeyValue bucket[];
} HashMap;


HashMap* CreateHashMap(size_t bucketSize){
    HashMap* hm = (HashMap*) calloc(1, sizeof(HashMap) + sizeof(struct KeyValue) * bucketSize);
    hm->bucketSize = bucketSize;
    return hm;
}

size_t hashMapSize(void){
    return sizeof(HashMap);
}

int HashMapAppend(HashMap* hashMap, char* key, size_t keyLen, char* value, size_t valueLen){
    if(keyLen >= KEY_SIZE || valueLen >= VALUE_SIZE) return -1;
    unsigned long long slot = hash(key, keyLen) % hashMap->bucketSize;
    int slotsVisited = 0;
CheckIfEmptySlot:
    if (hashMap->bucket[slot].key[0] =='\0') {
        strncpy(hashMap->bucket[slot].key, key, keyLen);
        hashMap->bucket[slot].value = (char*) calloc(1, valueLen + 1);
        strncpy(hashMap->bucket[slot].value, value, valueLen);
        hashMap->bucket[slot].key[keyLen] = '\0';
        hashMap->bucket[slot].value[valueLen] = '\0';
        return 0;
    }
    else {
        if (strncmp(key, hashMap->bucket[slot].key, keyLen) == 0) return 1; // Duplicate Key (No updating here)
        else {
            if(slotsVisited == hashMap->bucketSize) return 2;
            if(slot == hashMap->bucketSize - 1){
                slot = 0;
                slotsVisited++;
                goto CheckIfEmptySlot;
            } else {
                slot++;
                slotsVisited++;
                goto CheckIfEmptySlot;
            }
        }
    }
}

void FreeHashMap(HashMap* hm){
    for (int i = 0; i < hm->bucketSize; i++) {
        free(hm->bucket[i].value);
    }
    free(hm);
    return;
}


void PrintHashMap(HashMap* hashMap) {
    for (size_t i = 0; i < hashMap->bucketSize; i++) {
        if (hashMap->bucket[i].value != NULL) {
            printf("Bucket %zu: Key = '%s', Value = '%s'\n", i, hashMap->bucket[i].key, hashMap->bucket[i].value);
        }
    }
}
