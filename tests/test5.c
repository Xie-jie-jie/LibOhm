#include <stdio.h>
#define LIBOHM_C
#include "libohm.h"
#include <math.h>

/* Test 5 - Transformer */

int main() {
    const OmFlt RL = 0.1;                   /* inductor winding resistance */
    const OmFlt R = 1000;                   /* load resistance */
    const OmFlt AM = 100;                   /* input voltage amplitude */
    const OmFlt F = 50;                     /* frequency */
    const OmFlt K = 1;                      /* coefficient of coupling */
    const OmFlt LP = 100;                   /* primary inductance */
    const OmFlt LS = 25;                    /* secondary inductance */
    const OmFlt DT = 5e-6;                  /* time step */
    int i, j, k;
    FILE* p = fopen("test5.csv", "w");
    OmCir* cr = OmCreate(2, 4, 2, DT);
    OmFlt m = K * sqrt(LP * LS);
    OmBran(cr, 1, 1, 0, OMTYP_X1);
    OmAddV(cr, 1, 0);
    OmAddX(cr, 1, RL);
    OmBran(cr, 2, 1, 0, OMTYP_X3);
    OmAddL(cr, 2, LP, 0);
    OmAddM(cr, 2, 3, m, 0);
    OmBran(cr, 3, 2, 0, OMTYP_X3);
    OmAddL(cr, 3, LS, 0);
    OmAddM(cr, 3, 2, m, 0);
    OmBran(cr, 4, 2, 0, OMTYP_Y1);
    OmAddY(cr, 4, 1.0/R);
    OmMetV(cr, 1, 1, 0);
    OmMetA(cr, 2, 3);
    OmStamp(cr);
    for (i=0; i <= 9999; ++i) {
        OmFlt vt = AM * sin(2*3.1415926*F*i*DT);
        OmSetQs(cr, 1, vt);
        OmUpdCr(cr);
        OmUpdMt(cr);
        fprintf(p, "%lf,%lf,%lf,%lf,%lf,%lf\n", 
            i*DT,               /* time */
            vt,                 /* input voltage */
            OmGetMt(cr, 1),     /* primary inductor voltage */
            -OmGetXc(cr, 1),    /* primary inductor current */
            OmGetXc(cr, 4),     /* secondary inductor voltage */
            -OmGetMt(cr, 2)     /* secondary inductor current */
        );
    }
    fclose(p);
    OmDelete(cr);
    return 0;
}
