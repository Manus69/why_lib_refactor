#include "why.h"
#include "declarations.h"
#include "ar_interface.h"

const ArInterface RationalArInterface = {RationalZeroWRAP,
                                        RationalOneWRAP,
                                        RationalAddWRAP,
                                        RationalMultWRAP};