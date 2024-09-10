//
//  StaticHashMaps.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/5/24.
//

#include "StaticHashMaps.h"
#include "HashMap.h"
#include <string.h>

#define MIME_BUCKET_SIZE 43
#define HTTP_METHOD_BUCKET_SIZE 13

struct staticKeyValue {
    char key[8];
    char value[17]; // Not power of two :(
};
struct staticKeyValue httpMethods[HTTP_METHOD_BUCKET_SIZE] = {
    [0] = { "PATCH", "" },
    [1] = { "HEAD", "" },
    [2] = { "CONNECT", "" },
    [3] = { "POST", "" },
    [4] = { "DELETE", "" },
    [6] = { "PUT", "" },
    [10] = { "TRACE", "" },
    [11] = { "OPTIONS", "" },
    [12] = { "GET", "" },
};
const struct staticKeyValue mimeTypes[MIME_BUCKET_SIZE + 1] = {
    [0] = { "ogg", "audio/ogg" },
    [1] = { "otf", "font/otf" },
    [4] = { "png", "image/png" },
    [5] = { "woff2", "font/woff2" },
    [9] = { "woff", "font/woff" },
    [10] = { "gif", "image/gif" },
    [11] = { "gzip", "application/gzip" },
    [12] = { "css", "text/css" },
    [13] = { "txt", "text/plain" },
    [17] = { "webp", "image/webp" },
    [18] = { "svg", "image/svg+xml" },
    [20] = { "mp3", "audio/mpeg" },
    [21] = { "webm", "video/webm" },
    [24] = { "jpg", "image/jpeg" },
    [25] = { "mp4", "video/mp4" },
    [27] = { "js", "text/javascript" },
    [28] = { "xml", "text/xml" },
    [29] = { "ttf", "font/ttf" },
    [31] = { "html", "text/html" },
    [33] = { "pdf", "application/pdf" },
    [36] = { "zip", "application/zip" },
    [37] = { "json", "application/json" },
    [42] = { "wav", "audio/wav" },
};
char defaultMime[] = "application/octet-stream";
unsigned long long hash(const char* string, size_t strLen);
const char* GetMime(const char* key){
    unsigned long long slot = hash(key, strlen(key)) % MIME_BUCKET_SIZE;
    unsigned long long startSlot = slot;
    do{
        if (mimeTypes[slot].key[0] =='\0') return NULL;
        if(strcasecmp(key, mimeTypes[slot].key) == 0) return mimeTypes[slot].value;
        else {
            slot = (slot + 1) % MIME_BUCKET_SIZE;
        }
    }
    while (slot != startSlot);
    return defaultMime;
}
const int isValidHttpMethod(char* method){
    unsigned long long slot = hash(method, strlen(method)) % HTTP_METHOD_BUCKET_SIZE;
    unsigned long long startSlot = slot;
    do{
        if (httpMethods[slot].key[0] =='\0') return 0;
        if(strcasecmp(method, httpMethods[slot].key) == 0) return 1;
        else {
            slot = (slot + 1) % HTTP_METHOD_BUCKET_SIZE;
        }
    }
    while (slot != startSlot);
    return 0;
}

