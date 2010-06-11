/*************************************************************************************************
 * The test cases of the hash database API
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


package tokyotyrant;

import java.util.*;
import java.io.*;
import java.net.*;



/**
 * The test cases of the table database API.
 */
class TCRDBTest {
  //----------------------------------------------------------------
  // private constants
  //----------------------------------------------------------------
  private static final PrintStream STDOUT = System.out;
  private static final PrintStream STDERR = System.err;
  private static final Random RND = new Random();
  //----------------------------------------------------------------
  // public static methods
  //----------------------------------------------------------------
  /**
   * Main routine of the test command.
   */
  public static void main(String[] args){
    int rv = 0;
    if(args.length < 1) usage();
    if(args[0].equals("write")){
      rv = runwrite(args);
    } else if(args[0].equals("read")){
      rv = runread(args);
    } else if(args[0].equals("remove")){
      rv = runremove(args);
    } else if(args[0].equals("misc")){
      rv = runmisc(args);
    } else {
      usage();
    }
    System.gc();
    System.exit(rv);
  }
  //----------------------------------------------------------------
  // private static methods
  //----------------------------------------------------------------
  /* print the usage and exit */
  private static void usage(){
    STDERR.println("test cases of the hash database API");
    STDERR.println("");
    STDERR.println("synopsis:");
    STDERR.println("  java " + TCRDBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tchtest.rb write host rnum [opts]");
    STDERR.println("  tchtest.rb read host");
    STDERR.println("  tchtest.rb remove host");
    STDERR.println("  tchtest.rb misc host rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of hash database */
  private static void eprint(TCRDB tcrdb, String func){
    // String path = tcrdb.path();
    STDERR.println(TCRDBTest.class.getName() + ": " + //path + ": " +
                   func + ": " + tcrdb.errmsg());
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String host = null;
    String rstr = null;
    double timeout = 0.0;
    int opts = 0;
    int iflags = 0;
    int port = 1978;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(host == null && arg.startsWith("-")){
        if(arg.equals("-ip")){
          iflags |= 1 << 0;
        } else if(arg.equals("-is")){
          iflags |= 1 << 1;
        } else if(arg.equals("-in")){
          iflags |= 1 << 2;
        } else if(arg.equals("-it")){
          iflags |= 1 << 3;
        } else if(arg.equals("-if")){
          iflags |= 1 << 4;
        } else if(arg.equals("-ix")){
          iflags |= 1 << 5;
        } else {
          usage();
        }
      } else if(host == null){
        host = arg;
      } else if(rstr == null){
        rstr = arg;
      } else {
        usage();
      }
    }
    if(host == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int rv = procwrite(host, rnum, iflags, opts, port);
    return rv;
  }
  /* parse arguments of read command */
  private static int runread(String[] args){
    String host = null;
    int port = 1978;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(host == null){
        host = arg;
      } else {
        usage();
      }
    }
    if(host == null) usage();
    int rv = procread(host, port);
    return rv;
  }
  /* parse arguments of remove command */
  private static int runremove(String[] args){
    String host = null;
    int port = 1978;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(host == null){
        host = arg;
      } else {
        usage();
      }
    }
    if(host == null) usage();
    int rv = procremove(host, port);
    return rv;
  }
  /* parse arguments of misc command */
  private static int runmisc(String[] args){
    String host = null;
    String rstr = null;
    double timeout = 0.0;
    int opts = 0;
    int port = 1978;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(host == null){
        host = arg;
      } else if(rstr == null){
        rstr = arg;
      } else {
        usage();
      }
    }
    if(host == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int rv = procmisc(host, rnum, timeout, opts, port);
    return rv;
  }
  /* perform write command */
  private static int procwrite(String host, int rnum, int iflags, int opts, int port){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  host=" + host + "  rnum=" + rnum + "  iflags=" + iflags +
                   "  opts=" + opts + "  port=" + port);
    STDOUT.println("");
    double timeout = 0.0;
    boolean err = false;
    double stime = Util.time();
    TCRDB tcrdb = new TCRDB();
    if(!tcrdb.tune(timeout, opts)){
      eprint(tcrdb, "tune");
      err = true;
    }
    if(!tcrdb.open(host, port)){
      eprint(tcrdb, "open");
      err = true;
    }
    if((iflags & (1 << 0)) != 0 && !tcrdb.setindex("", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 1)) != 0 && !tcrdb.setindex("str", TCRDB.ITLEXICAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 2)) != 0 && !tcrdb.setindex("num", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 3)) != 0 && !tcrdb.setindex("type", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 4)) != 0 && !tcrdb.setindex("flag", TCRDB.ITTOKEN)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 5)) != 0 && !tcrdb.setindex("text", TCRDB.ITQGRAM)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      long id = tcrdb.genuid();
      Map cols = new HashMap();
      cols.put("str", new Long(id));
      cols.put("num", new Integer(RND.nextInt((int)id) + 1));
      cols.put("type", new Integer(RND.nextInt(32) + 1));
      StringBuffer sb = new StringBuffer();
      int num = RND.nextInt(5);
      int pt = 0;
      for(int j = 0; j < num; j++){
        pt += RND.nextInt(5) + 1;
        if(sb.length() > 0) sb.append(",");
        sb.append(pt);
      }
      if(sb.length() > 0){
        cols.put("flag", sb.toString());
        cols.put("text", sb.toString());
      }
      if(!tcrdb.put(new Long(id).toString().getBytes(), cols)){
        eprint(tcrdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(id, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tcrdb.rnum());
    STDOUT.println("size: " + tcrdb.dbsize());
    if(!tcrdb.close()){
      eprint(tcrdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform read command */
  private static int procread(String host, int port){
    STDOUT.println("<Reading Test>");
    STDOUT.println("  host=" + host + "  port=" + port);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    TCRDB tcrdb = new TCRDB();
    if(!tcrdb.open(host, port)){
      eprint(tcrdb, "open");
      err = true;
    }
    int rnum = (int)tcrdb.rnum();
    for(int i = 1; i <= rnum; i++){
      if(tcrdb.get(new Integer(i).toString()) == null){
        eprint(tcrdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tcrdb.rnum());
    STDOUT.println("size: " + tcrdb.dbsize());
    if(!tcrdb.close()){
      eprint(tcrdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform remove command */
  private static int procremove(String host, int port){
    STDOUT.println("<Removing Test>");
    STDOUT.println("  host=" + host + "  prot=" + port);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    TCRDB tcrdb = new TCRDB();
    if(!tcrdb.open(host, port)){
      eprint(tcrdb, "open");
      err = true;
    }
    int rnum = (int)tcrdb.rnum();
    for(int i = 1; i <= rnum; i++){
      if(!tcrdb.out(new Integer(i).toString())){
        eprint(tcrdb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tcrdb.rnum());
    STDOUT.println("size: " + tcrdb.dbsize());
    if(!tcrdb.close()){
      eprint(tcrdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform misc command */
  private static int procmisc(String host, int rnum, double timeout, int opts, int port){
    STDOUT.println("<Miscellaneous Test>");
    STDOUT.println("  host=" + host + "  rnum=" + rnum + " timeout=" + timeout + "  opts=" + opts + "  port=" + port);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    TCRDB tcrdb = new TCRDB();
    if(!tcrdb.tune(timeout, opts)){
      eprint(tcrdb, "tune");
      err = true;
    }
    if(!tcrdb.open(host, port)){
      eprint(tcrdb, "open");
      err = true;
    }
    if(!tcrdb.setindex("", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if(!tcrdb.setindex("str", TCRDB.ITLEXICAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if(!tcrdb.setindex("num", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if(!tcrdb.setindex("type", TCRDB.ITDECIMAL)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if(!tcrdb.setindex("flag", TCRDB.ITTOKEN)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    if(!tcrdb.setindex("text", TCRDB.ITQGRAM)){
      eprint(tcrdb, "setindex");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      long id = tcrdb.genuid();
      Map cols = new HashMap();
      cols.put("str", new Long(id));
      cols.put("num", new Integer(RND.nextInt((int)id) + 1));
      cols.put("type", new Integer(RND.nextInt(32) + 1));
      StringBuffer sb = new StringBuffer();
      int num = RND.nextInt(5);
      int pt = 0;
      for(int j = 0; j < num; j++){
        pt += RND.nextInt(5) + 1;
        if(sb.length() > 0) sb.append(",");
        sb.append(pt);
      }
      if(sb.length() > 0){
        cols.put("flag", sb.toString());
        cols.put("text", sb.toString());
      }
      if(!tcrdb.put(new Long(id).toString().getBytes(), cols)){
        eprint(tcrdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(id, 8, '0') + ")");
      }
    }
    STDOUT.println("reading:");
    for(int i = 1; i <= rnum; i++){
      if(tcrdb.get(new Integer(i).toString()) == null){
        eprint(tcrdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("removing:");
    for(int i = 1; i <= rnum; i++){
      if(RND.nextInt(2) == 0 && !tcrdb.out(new Integer(i).toString())){
        eprint(tcrdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("checking iterator:");
    if(!tcrdb.iterinit()){
      eprint(tcrdb, "iterinit");
      err = true;
    }
    int inum = 0;
    byte[] pkey;
    while((pkey = tcrdb.iternext()) != null){
      inum++;
      if(tcrdb.get(pkey) == null){
        eprint(tcrdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && inum % (rnum / 250) == 0){
        STDOUT.print('.');
        if(inum == rnum || inum % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
      }
    }
    if(rnum > 250) STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
    if(tcrdb.ecode() != TCRDB.ENOREC || inum != tcrdb.rnum()){
      eprint(tcrdb, "(validation)");
      err = true;
    }
    List keys = tcrdb.fwmkeys("1", 10);
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "i:" + RND.nextInt(rnum);
      if(RND.nextInt(2) == 0){
        if(tcrdb.addint(buf, 1) == Integer.MIN_VALUE){
          eprint(tcrdb, "addint");
          err = true;
          break;
        }
      } else {
        if(tcrdb.adddouble(buf, 1) == Double.NaN){
          eprint(tcrdb, "adddouble");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!tcrdb.sync()){
      eprint(tcrdb, "sync");
      err = true;
    }
    if(!tcrdb.optimize()){
      eprint(tcrdb, "optimize");
      err = true;
    }
/*
    String npath = path + "-tmp";
    if(!tcrdb.copy(npath)){
      eprint(tcrdb, "copy");
      err = true;
    }
    File file = new File(npath).getAbsoluteFile();
    File dir = new File(file.getParent());
    String[] children = dir.list();
    for(int i = 0; i < children.length; i++){
      String child = children[i];
      if(child.indexOf(npath) >= 0 && child.indexOf(".idx.") >= 0) new File(child).delete();
    }
    file.delete();
*/
    STDOUT.println("searching:");
    RDBQRY qry = new RDBQRY(tcrdb);
    String[] names = { "", "str", "num", "type", "flag", "text", "c1" };
    int ops[] = { RDBQRY.QCSTREQ, RDBQRY.QCSTRINC, RDBQRY.QCSTRBW, RDBQRY.QCSTREW,
                  RDBQRY.QCSTRAND, RDBQRY.QCSTROR, RDBQRY.QCSTROREQ, RDBQRY.QCSTRRX,
                  RDBQRY.QCNUMEQ, RDBQRY.QCNUMGT, RDBQRY.QCNUMGE, RDBQRY.QCNUMLT,
                  RDBQRY.QCNUMLE, RDBQRY.QCNUMBT, RDBQRY.QCNUMOREQ };
    int ftsops[] = { RDBQRY.QCFTSPH, RDBQRY.QCFTSAND, RDBQRY.QCFTSOR, RDBQRY.QCFTSEX };
    int types[] = { RDBQRY.QOSTRASC, RDBQRY.QOSTRDESC, RDBQRY.QONUMASC, RDBQRY.QONUMDESC };
    for(int i = 1; i <= rnum; i++){
      if(RND.nextInt(10) > 0) qry = new RDBQRY(tcrdb);
      int cnum = RND.nextInt(4);
      for(int j = 0; j < cnum; j++){
        String name = names[RND.nextInt(names.length)];
        int op = ops[RND.nextInt(ops.length)];
        if(RND.nextInt(10) == 0) op = ftsops[RND.nextInt(ftsops.length)];
        if(RND.nextInt(20) == 0) op |= RDBQRY.QCNEGATE;
        if(RND.nextInt(20) == 0) op |= RDBQRY.QCNOIDX;
        String expr = new Integer(RND.nextInt(i)).toString();
        if(RND.nextInt(10) == 0) expr += RND.nextInt(i);
        if(RND.nextInt(10) == 0) expr += RND.nextInt(i);
        qry.addcond(name, op, expr);
      }
      if(RND.nextInt(3) != 0){
        String name = names[RND.nextInt(names.length)];
        int type = types[RND.nextInt(types.length)];
        qry.setorder(name, type);
      }
      qry.setlimit(RND.nextInt(i), RND.nextInt(10));
      List res = qry.search();
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    qry = new RDBQRY(tcrdb);
    qry.addcond("", RDBQRY.QCSTRBW, "i:");
    qry.setorder("_num", RDBQRY.QONUMDESC);
    List ires = qry.search();
    int irnum = ires.size();
    int itnum = (int)tcrdb.rnum();

    
/*
    class MyProc implements RDBQRYPROC {
      int icnt = 0;
      public int proc(byte[] key, Map cols){
        icnt += 1;
        cols.put("icnt", new Integer(icnt).toString());
        return QPPUT;
      }
    }
    MyProc proc = new MyProc();
    if(!qry.proc(proc)){
      eprint(tcrdb, "qry::proc");
      err = true;
    }
*/
    //qry.addcond("icnt", RDBQRY.QCNUMGT, "0");
    qry.addcond("_num", RDBQRY.QCNUMGT, "1");
    RDBQRY[] qrys = { qry, qry };
    List mures = qry.metasearch(qrys, RDBQRY.MSUNION);
    if(mures.size() != irnum){
      eprint(tcrdb, "qry::metasearch");
      err = true;
    }
    List mires = qry.metasearch(qrys, RDBQRY.MSISECT);
    if(mires.size() != irnum){
      eprint(tcrdb, "qry::metasearch");
      err = true;
    }
    List mdres = qry.metasearch(qrys, RDBQRY.MSDIFF);
    if(mdres.size() != 0){
      eprint(tcrdb, "qry::metasearch");
      err = true;
    }
    if(!qry.searchout()){
      eprint(tcrdb, "qry::searchout");
      err = true;
    }
    if(tcrdb.rnum() != itnum - irnum){
      eprint(tcrdb, "(validation)");
      err = true;
    }
    qry = new RDBQRY(tcrdb);
    qry.addcond("text", RDBQRY.QCSTRBW, "1");
    qry.setlimit(100, 1);
    Iterator it = qry.search().iterator();
    while(it.hasNext()){
      Map cols = tcrdb.get((byte[])it.next());
      if(cols == null) {
        eprint(tcrdb, "get");
        err = true;
        break;
      }
    }
    if(!tcrdb.vanish()){
      eprint(tcrdb, "vanish");
      err = true;
    }
/*
    STDOUT.println("checking transaction commit:");
    if(!tcrdb.tranbegin()){
      eprint(tcrdb, "tranbegin");
      err = true;
    }
*/
    for(int i = 1; i <= rnum; i++){
      pkey = new Integer((RND.nextInt(rnum) + 1)).toString().getBytes();
      if(RND.nextInt(2) == 0){
        if(tcrdb.addint(pkey, 1) == Integer.MIN_VALUE){
          eprint(tcrdb, "addint");
          err = true;
          break;
        }
      } else {
        if(!tcrdb.out(pkey) && tcrdb.ecode() != TCRDB.ENOREC){
          eprint(tcrdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
/*
    if(!tcrdb.trancommit()){
      eprint(tcrdb, "trancommit");
      err = true;
    }
*/
/*
    STDOUT.println("checking transaction abort:");
*/
    long ornum = tcrdb.rnum();
    long ofsiz = tcrdb.dbsize();
/*
    if(!tcrdb.tranbegin()){
      eprint(tcrdb, "tranbegin");
      err = true;
    }
*/
    for(int i = 1; i <= rnum; i++){
      String pkstr = new Integer((RND.nextInt(rnum) + 1)).toString();
      if(RND.nextInt(2) == 0){
        if(tcrdb.addint(pkstr, 1) == Integer.MIN_VALUE){
          eprint(tcrdb, "addint");
          err = true;
          break;
        }
      } else {
        if(!tcrdb.out(pkstr) && tcrdb.ecode() != TCRDB.ENOREC){
          eprint(tcrdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
/*
    if(!tcrdb.tranabort()){
      eprint(tcrdb, "tranabort");
      err = true;
    }
*/
    if(tcrdb.rnum() != ornum || tcrdb.dbsize() != ofsiz){
      eprint(tcrdb, "(validation)");
      err = true;
    }
    STDOUT.println("record number: " + tcrdb.rnum());
    STDOUT.println("size: " + tcrdb.dbsize());
    if(!tcrdb.close()){
      eprint(tcrdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  //----------------------------------------------------------------
  // constructors and finalizers
  //----------------------------------------------------------------
  /**
   * Dummy constructor.
   */
  private TCRDBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
