#include <stdio.h>
#define LIBOHM_C
#include "libohm.h"

/* Test 4 - Boost circuit equivalent model */

int main() {
    const OmFlt RL = 1;                     /* inductor winding resistance */
    const OmFlt R = 100;                    /* load resistance */
    const OmFlt VG = 100;                   /* input voltage */
    int i, j, k;
    FILE* p = fopen("test4.csv", "w");
    for (i=0; i <= 100; ++ i) {
        OmFlt D = 0.01 * i;                 /* duty cycle */
        OmCir* cr = OmCreate(1, 2, 1, 5e-6);
        OmBran(cr, 1, 0, 0, OMTYP_X1);
        OmAddV(cr, 1, -VG);
        OmAddX(cr, 1, RL);
        OmAddE(cr, 1, 2, 1-D);
        OmBran(cr, 2, 1, 0, OMTYP_Y0);
        OmAddF(cr, 2, 1, D-1);
        OmAddY(cr, 2, 1.0/R);
        OmMetV(cr, 1, 1, 0);
        OmStamp(cr);
        OmUpdMt(cr);
        fprintf(p, "%lf,%lf\n", D, OmGetMt(cr, 1)/VG);
        printf("D=%lf, V/Vg=%lf\n", D, OmGetMt(cr, 1)/VG);
        OmDelete(cr);
    }
    fclose(p);
    return 0;
}
