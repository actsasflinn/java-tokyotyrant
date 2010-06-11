/*************************************************************************************************
 * Java binding of Tokyo Tyrant
 *                                                      Copyright (C) 2010 Flinn Mueller
 *                                                      Copyright (C) 2006-2009 Mikio Hirabayashi
 * This file is part of tokyotyrant-java
 * Tokyo Tyrant is free software; you can redistribute it and/or modify it under the terms of
 * the GNU Lesser General Public License as published by the Free Software Foundation; either
 * version 2.1 of the License or any later version.  Tokyo Tyrant is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with Tokyo
 * Tyrant; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 *************************************************************************************************/


#include "myconf.h"



/*************************************************************************************************
 * common settings
 *************************************************************************************************/


void throwexception(JNIEnv *env, const char *name, const char *message){
  jclass cls;
  cls = (*env)->FindClass(env, name);
  (*env)->ThrowNew(env, cls, message);
}


void throwoutmem(JNIEnv *env){
  throwexception(env, CLSEOUTMEM, "out of memory");
}


void throwillarg(JNIEnv *env){
  throwexception(env, CLSEILLARG, "illegal argument");
}



/* END OF FILE */
