/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#include <stdio.h>  // fprintf(), NULL
#include <stdlib.h> // exit(), EXIT_SUCCESS
#include <dlfcn.h>

#include "test.h" // PASS(), FAIL(), XPASS(), XFAIL()


static void trySO(const char* path)
{
	void* handle = dlopen(path, RTLD_LAZY);
	if ( handle == NULL ) {
		FAIL("dlopen(\"%s\") failed with: %s", path, dlerror());
		exit(0);
	}
	
	void* sym = dlsym(handle, "foo");
	if ( sym == NULL ) {
		FAIL("dlsym(handle, \"foo\") failed");
		exit(0);
	}
	
	int result = dlclose(handle);
	if ( result != 0 ) {
		if ( result == 1 ) {
			// panther dyld returns 1 if you try to dlclose() a dylib
			XFAIL("dlclose(handle) returned %d", result);
		}
		else {
			FAIL("dlclose(handle) returned %d", result);
			exit(0);
		}
	}

}



int main()
{
	trySO("test.bundle");
	trySO("test.dylib");
  
	PASS("dlopen-in-initializer");
	return EXIT_SUCCESS;
}
