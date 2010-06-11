/*************************************************************************************************
 * Java binding of Tokyo Tyrant
 *                                                      Copyright (C) 2010 Flinn Mueller
 *                                                      Copyright (C) 2006-2009 Mikio Hirabayashi
 * This file is part of java-tokyotyrant
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


#include "tokyotyrant_RDBQRY.h"
#include "myconf.h"


/* private global variables */
static jclass cls_tcrdbqry;
static jfieldID tcrdbqry_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* init */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_init
(JNIEnv *env, jclass cls){
  cls_tcrdbqry = cls;
  tcrdbqry_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* addcond */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_addcond
(JNIEnv *env, jobject self, jstring name, jint op, jstring expr){
  if(!name || !expr){
    throwillarg(env);
    return;
  }
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  jboolean ice;
  const char *texpr = (*env)->GetStringUTFChars(env, expr, &ice);
  tcrdbqryaddcond(qry, tname, op, texpr);
  if(ice) (*env)->ReleaseStringUTFChars(env, expr, texpr);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
}


/* setorder */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_setorder
(JNIEnv *env, jobject self, jstring name, jint type){
  if(!name){
    throwillarg(env);
    return;
  }
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  tcrdbqrysetorder(qry, tname, type);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
}


/* setlimit */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_setlimit
(JNIEnv *env, jobject self, jint max, jint skip){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  tcrdbqrysetlimit(qry, max, skip);
}


/* search */
JNIEXPORT jobject JNICALL Java_tokyotyrant_RDBQRY_search
(JNIEnv *env, jobject self){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  TCLIST *tkeys = tcrdbqrysearch(qry);
  jclass clslist = (*env)->FindClass(env, CLSARRAYLIST);
  jmethodID midinit = (*env)->GetMethodID(env, clslist, "<init>", "()V");
  jobject pkeys = (*env)->NewObject(env, clslist, midinit);
  jmethodID midadd = (*env)->GetMethodID(env, clslist, "add", "(L" CLSOBJECT ";)Z");
  for(int i = 0; i < tclistnum(tkeys); i++){
    int ksiz;
    const char *kbuf = tclistval(tkeys, i, &ksiz);
    jbyteArray pkey = (*env)->NewByteArray(env, ksiz);
    (*env)->SetByteArrayRegion(env, pkey, 0, ksiz, (jbyte *)kbuf);
    (*env)->CallVoidMethod(env, pkeys, midadd, pkey);
    (*env)->DeleteLocalRef(env, pkey);
  }
  tclistdel(tkeys);
  return pkeys;
}


/* searchout */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_RDBQRY_searchout
(JNIEnv *env, jobject self){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  return tcrdbqrysearchout(qry);
}


/* hint */
JNIEXPORT jstring JNICALL Java_tokyotyrant_RDBQRY_hint
(JNIEnv *env, jobject self){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  const char *hint = tcrdbqryhint(qry);
  jstring jhint;
  if(hint){
    jhint = (*env)->NewStringUTF(env, hint);
    if(!hint){
      throwoutmem(env);
      return NULL;
    }
  } else {
    jhint = NULL;
  }
  return jhint;
}


/* metasearch */
JNIEXPORT jobject JNICALL Java_tokyotyrant_RDBQRY_metasearch
(JNIEnv *env, jobject self, jobjectArray others, jint type){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  int onum = (*env)->GetArrayLength(env, others);
  RDBQRY *qrys[onum+1];
  int qnum = 0;
  qrys[qnum++] = qry;
  jclass clsqry = (*env)->GetObjectClass(env, self);
  for(int i = 0; i < onum; i++){
    jobject oqry = (*env)->GetObjectArrayElement(env, others, i);
    if((*env)->IsInstanceOf(env, oqry, clsqry))
      qrys[qnum++] = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, oqry, tcrdbqry_fid_ptr);
  }
  TCLIST *tkeys = tcrdbmetasearch(qrys, qnum, type);
  jclass clslist = (*env)->FindClass(env, CLSARRAYLIST);
  jmethodID midinit = (*env)->GetMethodID(env, clslist, "<init>", "()V");
  jobject pkeys = (*env)->NewObject(env, clslist, midinit);
  jmethodID midadd = (*env)->GetMethodID(env, clslist, "add", "(L" CLSOBJECT ";)Z");
  for(int i = 0; i < tclistnum(tkeys); i++){
    int ksiz;
    const char *kbuf = tclistval(tkeys, i, &ksiz);
    jbyteArray pkey = (*env)->NewByteArray(env, ksiz);
    (*env)->SetByteArrayRegion(env, pkey, 0, ksiz, (jbyte *)kbuf);
    (*env)->CallVoidMethod(env, pkeys, midadd, pkey);
    (*env)->DeleteLocalRef(env, pkey);
  }
  tclistdel(tkeys);
  return pkeys;
}


/* initialize */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_initialize
(JNIEnv *env, jobject self, jobject tcrdb){
  jclass clstcrdb = (*env)->GetObjectClass(env, tcrdb);
  jfieldID fidtcrdb = (*env)->GetFieldID(env, clstcrdb, "ptr", "J");
  TCRDB *tcrdbptr = (TCRDB *)(intptr_t)(*env)->GetLongField(env, tcrdb, fidtcrdb);
  RDBQRY *qry = tcrdbqrynew(tcrdbptr);
  (*env)->SetLongField(env, self, tcrdbqry_fid_ptr, (intptr_t)qry);
}


/* destruct */
JNIEXPORT void JNICALL Java_tokyotyrant_RDBQRY_destruct
(JNIEnv *env, jobject self){
  RDBQRY *qry = (RDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tcrdbqry_fid_ptr);
  tcrdbqrydel(qry);
}


/* END OF FILE */
