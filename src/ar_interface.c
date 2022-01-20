#include "why.h"
#include "declarations.h"

const ArInterface RationalArInterface = {RationalZeroWRAP,
                                        RationalOneWRAP,
                                        RationalNegateWRAP,
                                        RationalInvWRAP,
                                        RationalAddWRAP,
                                        RationalMultWRAP,
                                        RationalDivWRAP,
                                        RationalEqualsWRAP,
                                        RationalIsZeroWRAP};

const ArInterface FloatArInterface =    {FloatZeroWRAP,
                                        FloatOneWRAP,
                                        FloatNegateWRAP,
                                        FloatInvWRAP,
                                        FloatAddWRAP,
                                        FloatMultWRAP,
                                        RationalDivWRAP,
                                        FloatEqualsWRAP,
                                        FloatIsZeroWRAP};