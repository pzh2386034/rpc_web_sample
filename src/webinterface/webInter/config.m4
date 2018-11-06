dnl $Id$
dnl config.m4 for extension webInter

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(webInter, for webInter support,
dnl Make sure that the comment is aligned:
dnl [  --with-webInter             Include webInter support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(webInter, whether to enable webInter support,
dnl Make sure that the comment is aligned:
[  --enable-webInter           Enable webInter support])

if test "$PHP_WEBINTER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-webInter -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/webInter.h"  # you most likely want to change this
  dnl if test -r $PHP_WEBINTER/$SEARCH_FOR; then # path given as parameter
  dnl   WEBINTER_DIR=$PHP_WEBINTER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for webInter files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WEBINTER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WEBINTER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the webInter distribution])
  dnl fi

  dnl # --with-webInter -> add include path
  dnl PHP_ADD_INCLUDE($WEBINTER_DIR/include)

  dnl # --with-webInter -> check for lib and symbol presence
  dnl LIBNAME=webInter # you may want to change this
  dnl LIBSYMBOL=webInter # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WEBINTER_DIR/$PHP_LIBDIR, WEBINTER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WEBINTERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong webInter lib version or lib not found])
  dnl ],[
  dnl   -L$WEBINTER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WEBINTER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(webInter, webInter.c, $ext_shared)
fi
