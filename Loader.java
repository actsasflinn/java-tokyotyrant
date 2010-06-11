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


package tokyotyrant;

import java.util.*;
import java.io.*;
import java.net.*;



/**
 * Library loader.
 */
class Loader {
  //----------------------------------------------------------------
  // static variables
  //---------------------------------------------------------------
  static boolean loaded = false;
  //----------------------------------------------------------------
  // static methods
  //----------------------------------------------------------------
  /**
   * Load the native library.
   */
  static synchronized void load(){
    if(loaded) return;
    System.loadLibrary("jtokyotyrant");
    loaded = true;
  }
}



/* END OF FILE */
