/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class tokyotyrant_Util */

#ifndef _Included_tokyotyrant_Util
#define _Included_tokyotyrant_Util
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     tokyotyrant_Util
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_tokyotyrant_Util_init
  (JNIEnv *, jclass);

/*
 * Class:     tokyotyrant_Util
 * Method:    version
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tokyotyrant_Util_version
  (JNIEnv *, jclass);

/*
 * Class:     tokyotyrant_Util
 * Method:    packint
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_tokyotyrant_Util_packint
  (JNIEnv *, jclass, jint);

/*
 * Class:     tokyotyrant_Util
 * Method:    unpackint
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_tokyotyrant_Util_unpackint
  (JNIEnv *, jclass, jbyteArray);

/*
 * Class:     tokyotyrant_Util
 * Method:    packdouble
 * Signature: (D)[B
 */
JNIEXPORT jbyteArray JNICALL Java_tokyotyrant_Util_packdouble
  (JNIEnv *, jclass, jdouble);

/*
 * Class:     tokyotyrant_Util
 * Method:    unpackdouble
 * Signature: ([B)D
 */
JNIEXPORT jdouble JNICALL Java_tokyotyrant_Util_unpackdouble
  (JNIEnv *, jclass, jbyteArray);

/*
 * Class:     tokyotyrant_Util
 * Method:    system
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_tokyotyrant_Util_system
  (JNIEnv *, jclass, jstring);

/*
 * Class:     tokyotyrant_Util
 * Method:    chdir
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_tokyotyrant_Util_chdir
  (JNIEnv *, jclass, jstring);

/*
 * Class:     tokyotyrant_Util
 * Method:    getcwd
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tokyotyrant_Util_getcwd
  (JNIEnv *, jclass);

/*
 * Class:     tokyotyrant_Util
 * Method:    getpid
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_tokyotyrant_Util_getpid
  (JNIEnv *, jclass);

/*
 * Class:     tokyotyrant_Util
 * Method:    putenv
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_tokyotyrant_Util_putenv
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     tokyotyrant_Util
 * Method:    getenv
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tokyotyrant_Util_getenv
  (JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif
