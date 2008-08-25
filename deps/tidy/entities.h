#ifndef __ENTITIES_H__
#define __ENTITIES_H__

/* entities.h -- recognize character entities

  (c) 1998-2006 (W3C) MIT, ERCIM, Keio University
  See tidy.h for the copyright notice.

  CVS Info :

    $Author: meeloo $ 
    $Date: 2008-02-27 17:58:54 $ 
    $Revision: 1.1 $ 

*/

#include "forward.h"

/* entity starting with "&" returns zero on error */
/* uint    EntityCode( ctmbstr name, uint versions ); */
ctmbstr TY_(EntityName)( uint charCode, uint versions );
Bool    TY_(EntityInfo)( ctmbstr name, Bool isXml, uint* code, uint* versions );

#endif /* __ENTITIES_H__ */
