#include <stdio.h>
#define LIBOHM_C
#include "libohm.h"
#include <math.h>

/* Test 6 - Switch Pulse */

int main() {
    const OmFlt VG = 100;                   /* input voltage */
    const OmFlt R = 1000;                   /* load resistance */
    const OmFlt F = 100e3;                  /* frequency */
    const OmFlt K1 = 1.0;                   /* closed state coefficient */
    const OmFlt K2 = 0.6569;                /* open state coefficient */
    const OmFlt YS = 0.2929 / 1000;         /* switch conductance */
    const OmFlt DT = 5e-6;                  /* time step */
    const OmFlt ST = 10;                    /* substep */
    int i, j, k;
    FILE* p = fopen("test6.csv", "w");
    OmCir* cr = OmCreate(1, 2, 0, 1e-6);
    OmFlt t = 0.0;
    OmInt s = 0;
    OmBran(cr, 1, 1, 0, OMTYP_X1);
    OmAddV(cr, 1, VG);
    OmAddX(cr, 1, R);
    OmBran(cr, 2, 1, 0, OMTYP_SW);
    OmAddS(cr, 2, K1, K2, YS, 0.0);
    OmStamp(cr);
    for (i=0; i < ST; ++i) OmUpdSw(cr);
    for (i=1; i <= 10000; ++i) {
        t += DT;
        if (t > 1.0 / F) {
            s = 1 - s;
            OmSetSw(cr, 2, s);
            for (j=0; j < ST; ++j) OmUpdSw(cr);
            t = 0.0;
        }
        OmUpdCr(cr);
        OmUpdMt(cr);
        fprintf(p, "%lf,%lf,%lf\n", 
            i * DT,             /* time */
            OmGetXc(cr, 2),     /* voltage */
            -OmGetXc(cr, 1)     /* current */
        );
    }
    fclose(p);
    OmDelete(cr);
    return 0;
}
