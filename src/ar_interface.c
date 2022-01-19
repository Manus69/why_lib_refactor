#include "why.h"
#include "declarations.h"

const ArInterface RationalArInterface = {RationalZeroWRAP,
                                        RationalOneWRAP,
                                        RationalAddWRAP,
                                        RationalMultWRAP};

const ArInterface FloatArInterface =    {FloatZeroWRAP,
                                        FloatOneWRAP,
                                        FloatAddWRAP,
                                        FloatMultWRAP};