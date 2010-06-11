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


#include "tokyotyrant_TCRDB.h"
#include "myconf.h"


/* private global variables */
static jclass cls_tcrdb;
static jfieldID tcrdb_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* errmsg */
JNIEXPORT jstring JNICALL Java_tokyotyrant_TCRDB_errmsg
(JNIEnv *env, jclass cls, jint ecode){
  jstring jmsg = (*env)->NewStringUTF(env, tcrdberrmsg(ecode));
  if(!jmsg){
    throwoutmem(env);
    return NULL;
  }
  return jmsg;
}


/* init */
JNIEXPORT void JNICALL Java_tokyotyrant_TCRDB_init
(JNIEnv *env, jclass cls){
  cls_tcrdb = cls;
  tcrdb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* ecode */
JNIEXPORT jint JNICALL Java_tokyotyrant_TCRDB_ecode
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbecode(tcrdb);
}


/* tune */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_tune
(JNIEnv *env, jobject self, jdouble timeout, jint opts){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbtune(tcrdb, timeout, opts);
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_open
(JNIEnv *env, jobject self, jstring host, jint port){
  if(!host){
    throwillarg(env);
    return false;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean icp;
  const char *thost = (*env)->GetStringUTFChars(env, host, &icp);
  if(!thost){
    throwoutmem(env);
    return false;
  }
  bool rv = tcrdbopen(tcrdb, thost, port);
  if(icp) (*env)->ReleaseStringUTFChars(env, host, thost);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_close
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbclose(tcrdb);
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_out
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return false;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  bool rv = tcrdbout(tcrdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* vsiz */
JNIEXPORT jint JNICALL Java_tokyotyrant_TCRDB_vsiz
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  int rv = tcrdbvsiz(tcrdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* iterinit */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_iterinit
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbiterinit(tcrdb);
}


/* iternext */
JNIEXPORT jbyteArray JNICALL Java_tokyotyrant_TCRDB_iternext
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  int vsiz;
  char *vbuf = tcrdbiternext(tcrdb, &vsiz);
  jbyteArray val;
  if(vbuf){
    val = (*env)->NewByteArray(env, vsiz);
    if(!val){
      throwoutmem(env);
      return NULL;
    }
    (*env)->SetByteArrayRegion(env, val, 0, vsiz, (jbyte *)vbuf);
    tcfree(vbuf);
  } else {
    val = NULL;
  }
  return val;
}


/* fwmkeys */
JNIEXPORT jobject JNICALL Java_tokyotyrant_TCRDB_fwmkeys
(JNIEnv *env, jobject self, jbyteArray prefix, jint max){
  if(!prefix){
    throwillarg(env);
    return NULL;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean icp;
  jbyte *pbuf = (*env)->GetByteArrayElements(env, prefix, &icp);
  if(!pbuf){
    throwoutmem(env);
    return NULL;
  }
  int psiz = (*env)->GetArrayLength(env, prefix);
  TCLIST *tkeys = tcrdbfwmkeys(tcrdb, pbuf, psiz, max);
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
  if(icp) (*env)->ReleaseByteArrayElements(env, prefix, pbuf, JNI_ABORT);
  return pkeys;
}


/* addint */
JNIEXPORT jint JNICALL Java_tokyotyrant_TCRDB_addint
(JNIEnv *env, jobject self, jbyteArray pkey, jint num){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  num = tcrdbaddint(tcrdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyotyrant_TCRDB_adddouble
(JNIEnv *env, jobject self, jbyteArray pkey, jdouble num){
  if(!pkey){
    throwillarg(env);
    return 0.0;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  num = tcrdbadddouble(tcrdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_sync
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbsync(tcrdb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_optimize
(JNIEnv *env, jobject self, jstring params){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean icp;
  const char *tparams = (*env)->GetStringUTFChars(env, params, &icp);
  if(!tparams){
    throwoutmem(env);
    return false;
  }
  bool rv = tcrdboptimize(tcrdb, tparams);
  if(icp) (*env)->ReleaseStringUTFChars(env, params, tparams);
  return rv;
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_vanish
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbvanish(tcrdb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcrdbcopy(tcrdb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* rnum */
JNIEXPORT jlong JNICALL Java_tokyotyrant_TCRDB_rnum
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbrnum(tcrdb);
}


/* dbsize */
JNIEXPORT jlong JNICALL Java_tokyotyrant_TCRDB_dbsize
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbsize(tcrdb);
}


/* setindex */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_setindex
(JNIEnv *env, jobject self, jstring name, jint type){
  if(!name){
    throwillarg(env);
    return false;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return false;
  }
  bool rv = tcrdbtblsetindex(tcrdb, tname, type);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return rv;
}


/* genuid */
JNIEXPORT jlong JNICALL Java_tokyotyrant_TCRDB_genuid
(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  return tcrdbtblgenuid(tcrdb);
}


/* initialize */
JNIEXPORT void JNICALL
Java_tokyotyrant_TCRDB_initialize(JNIEnv *env, jobject self){
  TCRDB *tcrdb = tcrdbnew();
  (*env)->SetLongField(env, self, tcrdb_fid_ptr, (intptr_t)tcrdb);
}


/* destruct */
JNIEXPORT void JNICALL
Java_tokyotyrant_TCRDB_destruct(JNIEnv *env, jobject self){
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  tcrdbdel(tcrdb);
}


/* putimpl */
JNIEXPORT jboolean JNICALL Java_tokyotyrant_TCRDB_putimpl
(JNIEnv *env, jobject self, jbyteArray pkey, jobjectArray cols, jint dmode){
  if(!pkey || !cols){
    throwillarg(env);
    return false;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  jsize cnum = (*env)->GetArrayLength(env, cols);
  TCMAP *tcols = tcmapnew2(cnum + 1);
  cnum--;
  for(int i = 0; i < cnum; i += 2){
    jobject name = (*env)->GetObjectArrayElement(env, cols, i);
    jboolean icn;
    jbyte *nbuf = (*env)->GetByteArrayElements(env, name, &icn);
    if(!nbuf){
      throwoutmem(env);
      return false;
    }
    int nsiz = (*env)->GetArrayLength(env, name);
    jobject val = (*env)->GetObjectArrayElement(env, cols, i + 1);
    jboolean icv;
    jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
    if(!vbuf){
      throwoutmem(env);
      return false;
    }
    int vsiz = (*env)->GetArrayLength(env, val);
    tcmapputkeep(tcols, nbuf, nsiz, vbuf, vsiz);
    if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
    if(icn) (*env)->ReleaseByteArrayElements(env, name, nbuf, JNI_ABORT);
  }
  bool rv;
  switch(dmode){
  case 0:
    rv = tcrdbtblput(tcrdb, kbuf, ksiz, tcols);
    break;
  case 1:
    rv = tcrdbtblputkeep(tcrdb, kbuf, ksiz, tcols);
    break;
  case 2:
    rv = tcrdbtblputcat(tcrdb, kbuf, ksiz, tcols);
    break;
  default:
    rv = false;
    break;
  }
  tcmapdel(tcols);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* getimpl */
JNIEXPORT jobjectArray JNICALL Java_tokyotyrant_TCRDB_getimpl
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCRDB *tcrdb = (TCRDB *)(intptr_t)(*env)->GetLongField(env, self, tcrdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  jobjectArray ary = NULL;
  TCMAP *tcols = tcrdbtblget(tcrdb, kbuf, ksiz);
  if(tcols){
    int anum = tcmaprnum(tcols) * 2;
    ary = (*env)->NewObjectArray(env, anum, (*env)->GetObjectClass(env, pkey), NULL);
    anum = 0;
    tcmapiterinit(tcols);
    const char *nbuf;
    int nsiz;
    while((nbuf = tcmapiternext(tcols, &nsiz)) != NULL){
      int vsiz;
      const char *vbuf = tcmapiterval(nbuf, &vsiz);
      jbyteArray nary = (*env)->NewByteArray(env, nsiz);
      if(!nary){
        throwoutmem(env);
        return NULL;
      }
      jbyteArray vary = (*env)->NewByteArray(env, vsiz);
      if(!vary){
        throwoutmem(env);
        return NULL;
      }
      (*env)->SetByteArrayRegion(env, nary, 0, nsiz, (jbyte *)nbuf);
      (*env)->SetByteArrayRegion(env, vary, 0, vsiz, (jbyte *)vbuf);
      (*env)->SetObjectArrayElement(env, ary, anum++, nary);
      (*env)->SetObjectArrayElement(env, ary, anum++, vary);
      (*env)->DeleteLocalRef(env, vary);
      (*env)->DeleteLocalRef(env, nary);
    }
    tcmapdel(tcols);
  }
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return ary;
}



/* END OF FILE */
