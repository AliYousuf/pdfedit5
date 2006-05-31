/** @file
 NullPointerException
 - class for exception raised when attempting to call some wrapper function
 methods, while the objest wrapped inside is a NULL pointer, meaning that the
 operation would result in null pointer dereference (and program crash), if
 they would be executed.
 @author Martin Petricek
*/

#include <qstring.h>
#include <qobject.h>
#include "nullpointerexception.h"

namespace gui {

/**
 Default constructor of exception
 @param className Name of class, in which this exception occured
 @param methodName Name of method, in which this exception occured
 */
NullPointerException::NullPointerException(const QString &className,const QString &methodName){
 _class=className;
 _method=methodName;
}

/**
 Return human-readable exception message
 @return exception message
*/
QString NullPointerException::message() {
 return QObject::tr("Null pointer access in ")+_class+"."+_method;
}

/**
 Return class in which this exception was thrown
 @return class name
*/
QString NullPointerException::exceptionClassName() {
 return _class;
}

/**
 Return method in which this exception was thrown
 @return method name
*/
QString NullPointerException::exceptionMethod() {
 return _method;
}

/** default destructor */
NullPointerException::~NullPointerException() {
}

} // namespace gui
