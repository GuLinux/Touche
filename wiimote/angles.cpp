#include "angles.h"

Q_DECLARE_METATYPE(WiimoteVector3)
class _RegisterBigDecimalMetatype {
public:
    _RegisterBigDecimalMetatype() { qRegisterMetaType<WiimoteVector3>(); }
};

static _RegisterBigDecimalMetatype _regWiimoteVector3;
