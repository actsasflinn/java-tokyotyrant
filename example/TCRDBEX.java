import tokyotyrant.*;
import java.util.*;

public class TCRDBEX {
  public static void main(String[] args){

    // create the object
    TCRDB tcrdb = new TCRDB();

    // open the database
    if(!tcrdb.open("127.0.0.1", 1978)){
      int ecode = tcrdb.ecode();
      System.err.println("open error: " + tcrdb.errmsg(ecode));
    }

    // store a record
    String pkey = new Long(tcrdb.genuid()).toString();
    Map cols = new HashMap();
    cols.put("name", "mikio");
    cols.put("age", "30");
    cols.put("lang", "ja,en,c");
    if(!tcrdb.put(pkey, cols)){
      int ecode = tcrdb.ecode();
      System.err.println("put error: " + tcrdb.errmsg(ecode));
    }

    // store another record
    cols = new HashMap();
    cols.put("name", "falcon");
    cols.put("age", "31");
    cols.put("lang", "ja");
    cols.put("skill", "cook,blog");
    if(!tcrdb.put("x12345", cols)){
      int ecode = tcrdb.ecode();
      System.err.println("put error: " + tcrdb.errmsg(ecode));
    }

    // search for records
    RDBQRY qry = new RDBQRY(tcrdb);
    qry.addcond("age", RDBQRY.QCNUMGE, "20");
    qry.addcond("lang", RDBQRY.QCSTROR, "ja,en");
    qry.setorder("name", RDBQRY.QOSTRASC);
    qry.setlimit(10, 0);
    List res = qry.search();
    Iterator it = res.iterator();
    while(it.hasNext()){
      String rkey = new String((byte[])it.next());
      Map rcols = tcrdb.get(rkey);
      System.out.println("name:" + rcols.get("name"));
    }

    // close the database
    if(!tcrdb.close()){
      int ecode = tcrdb.ecode();
      System.err.println("close error: " + tcrdb.errmsg(ecode));
    }

  }
}
