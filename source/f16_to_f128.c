
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015 The Regents of the University of
California.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifdef HAVE_PLATFORM_H
#include "platform.h"
#endif
#if !defined(false)
#include <stdbool.h>
#endif
#if !defined(int32_t)
#include <stdint.h>             /* C99 standard integers */
#endif
#include "internals.h"
#include "specialize.h"
#include "softfloat.h"

float128_t f16_to_f128( float16_t a )
{
    union ui16_f16 uA;
    uint_fast16_t uiA;
    bool sign;
    int_fast8_t exp;
    uint_fast16_t frac;
    struct commonNaN commonNaN;
    struct uint128 uiZ;
    struct exp8_sig16 normExpSig;
    union ui128_f128 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA.f = a;
    uiA = uA.ui;
    sign = signF16UI( uiA );
    exp  = expF16UI( uiA );
    frac = fracF16UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( exp == 0x1F ) {
        if ( frac ) {
            softfloat_f16UIToCommonNaN( uiA, &commonNaN );
            uiZ = softfloat_commonNaNToF128UI( &commonNaN );
        } else {
            uiZ.v64 = packToF128UI64( sign, 0x7FFF, 0 );
            uiZ.v0  = 0;
        }
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( ! exp ) {
        if ( ! frac ) {
            uiZ.v64 = packToF128UI64( sign, 0, 0 );
            uiZ.v0  = 0;
            goto uiZ;
        }
        normExpSig = softfloat_normSubnormalF16Sig( frac );
        exp = normExpSig.exp - 1;
        frac = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiZ.v64 = packToF128UI64( sign, exp + 0x3FF0, (uint_fast64_t) frac<<38 );
    uiZ.v0  = 0;
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}
