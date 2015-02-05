//
//  cs559core.h
//  cs559fltk
//
//  Created by Leon Chu on 11/24/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef cs559fltk_cs559core_h
#define cs559fltk_cs559core_h

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

template<typename T>
void SafeDeleteArray(T& ptr) {
  if (nullptr != ptr) {
    delete []ptr;
  }
}//end SafeDelete()

#define SAFE_DELETE(ptr) {if(nullptr!=ptr){delete ptr; ptr = nullptr;}};
#define SAFE_ARRAY_DELETE(ptr) {if(nullptr!=ptr){delete [] ptr; ptr = nullptr;}};

#endif
