#ifndef __PDFUTIL_H__
#define __PDFUTIL_H__

#include <iproperty.h>
class QString;
namespace pdfobjects {
class CPdf;
}

namespace util {

using namespace pdfobjects;

QString getTypeName(PropertyType typ);
QString getTypeName(IProperty *obj);
IndiRef getRef(IProperty *ref);
bool isRefValid(CPdf *pdf,IndiRef ref);

} // namespace util

#endif
