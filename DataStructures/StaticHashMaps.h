//
//  StaticHashMaps.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/5/24.
//

#ifndef StaticHashMaps_h
#define StaticHashMaps_h

#include <stdio.h>
const char* GetMime(const char* key);
const int isValidHttpMethod(char* method);
extern char defaultMime[];
#endif /* StaticHashMaps_h */
