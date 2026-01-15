/*
 * LibOhm: A Header-Only Ciruit Simulation Library
 * Copyright (C) 2026 H.J.Xie
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/**
* @file
* @brief        LibOhm Header File.
* @details      LibOhm is a lightweight real-time circuit simulation library.
* @author       H.J.Xie
* @date         2026/01/15
* @version      v1.0
*/

/*===========================================================================*/
#ifndef LIBOHM_H                        /*| Include Guards allow including  |*/
#define LIBOHM_H                        /*| this header file multiple times.|*/
/*===========================================================================*/

/*====================== Part 1. Dependency =================================*/

#include <stdlib.h>                     /** Function Used: malloc(), free()  */

/*====================== Part 2. Macro Defination ===========================*/

#define OMMALLOC(x) malloc(x)           /** Memory allocation function       */
#define OMFREE(x)   free(x)             /** Memory free function             */
#define OMABS(x)    ((x)<0?-(x):(x))    /** Absolute value function          */
#define OMTYP_UN    0                   /** Branch type is unknown           */
#define OMTYP_X0    1                   /** Branch contains X/E/H            */
#define OMTYP_X1    2                   /** Branch contains X/E/H/V          */
#define OMTYP_X2    3                   /** Branch contains X/E/H/V/L/Q      */
#define OMTYP_X3    4                   /** Branch contains X/E/H/V/L/M/Q/A  */
#define OMTYP_Y0    5                   /** Branch contains Y/F/G            */
#define OMTYP_Y1    6                   /** Branch contains Y/F/G/I          */
#define OMTYP_Y2    7                   /** Branch contains Y/F/G/I/C/P      */
#define OMTYP_Y3    8                   /** Branch contains Y/F/G/I/C/N/P/B  */
#define OMTYP_SW    9                   /** Branch contains Y/F/G/I/S        */

/*====================== Part 3. Type Defination ============================*/

typedef int    OmInt;                   /** Integer Type that LibOhm uses    */
typedef double OmFlt;                   /** Float Type that LibOhm uses      */
typedef struct OmCir {                  /** Circuit Information Structure    */
    /*======== Group 0: General Information =================================*/
    OmInt  numN   ;                     /** Number of nodes (excluding GND)  */
    OmInt  numB   ;                     /** Number of branches               */
    OmInt  numM   ;                     /** Number of meters                 */
    OmInt  numX   ;                     /** Number of X-type branches        */
    OmInt  numC   ;                     /** Number of branches after cutting */
    OmFlt  timStp ;                     /** Simulation time step             */
    /*======== Group 1: Setup Information ===================================*/
    OmInt* vecBn1 ;                     /** Node 1 of branch           [b]-1 */
    OmInt* vecBn2 ;                     /** Node 2 of branch           [b]-1 */
    OmInt* vecMn1 ;                     /** Node 1 of meter            [m]-1 */
    OmInt* vecMn2 ;                     /** Node 2 of meter            [m]-1 */
    OmFlt* matPa  ;                     /** Source update matrix      [b,b]0 */
    OmFlt* matPb  ;                     /** Branch conductance matrix [b,b]0 */
    /*======== Group 2: Reset Information ===================================*/
    OmInt* vecBtm ;                     /** Type and method of branch  [b]0  */
    OmInt* vecLut ;                     /** Lookup table for branches  [b]0  */
    OmFlt* vecW1c ;                     /** Weight of Xc   (closed)    [b]0  */
    OmFlt* vecW2c ;                     /** Weight of Qa   (closed)    [b]0  */
    OmFlt* vecW1o ;                     /** Weight of Xc   (open)      [b]0  */
    OmFlt* vecW2o ;                     /** Weight of Qa   (open)      [b]0  */
    OmFlt* vecQa0 ;                     /** Initial value of Qa        [b]0  */
    OmFlt* vecQs0 ;                     /** Initial value of Qs        [b]0  */
    /*======== Group 3: Runtime Information =================================*/
    OmFlt* matC   ;                     /** Matrix to calculate Xc    [c,c]x */
    OmFlt* matD   ;                     /** Matrix to calculate Xm    [m,c]x */
    OmFlt* vecW1m ;                     /** Weight of Xc in UpdCr()    [c]x  */
    OmFlt* vecW2m ;                     /** Weight of Qa in UpdCr()    [c]x  */
    OmFlt* vecW1s ;                     /** Weight of Xc in UpdSw()    [c]x  */
    OmFlt* vecW2s ;                     /** Weight of Qa in UpdSw()    [c]x  */
    OmFlt* vecQa  ;                     /** Associated source vector   [c]x  */
    OmFlt* vecQs  ;                     /** Independent source vector  [c]x  */
    OmFlt* vecQtp ;                     /** Vector Qtp = Qs + Qa       [c]x  */
    OmFlt* vecXm  ;                     /** Meter reading vector       [m]x  */
    OmFlt* vecXc  ;                     /** Source update vector       [c]x  */
} OmCir;

/*====================== Part 4. Function Declaration =======================*/

/**
 * @brief       [0] Free an OmCir pointer
 * @param       cr input OmCir pointer (can be NULL)
 */
void OmDelete(OmCir* cr);
/**
 * @brief       [1] Create new unstamped circuit
 * @param       n number of nodes (excepting GND) (must >= 0)
 * @param       b number of branches (must >= 0)
 * @param       m number of meters (must >= 0)
 * @param       stp time step (must > 0.0)
 * @retval      return valid pointer if succed, return NULL if failed
 * @note        Remember to use OmDelete() to free memory!
 */
OmCir* OmCreate(OmInt n, OmInt b, OmInt m, OmFlt stp);
/**
 * @brief       [2] Stamp circuit, get ready to run
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @note        do not stamp the same circuit more than once
 */
void OmStamp(OmCir* cr);
/**
 * @brief       [3] Reset stamped circuit to initial state
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 * @note        Switches are always set to open state after reset
 */
void OmReset(OmCir* cr);
/**
 * @brief       [4] Update switch associated source only
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 */
void OmUpdSw(OmCir* cr);
/**
 * @brief       [5] Update circuit (both switch and associated source)
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 */
void OmUpdCr(OmCir* cr);
/**
 * @brief       [6] Update meter readings
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 */
void OmUpdMt(OmCir* cr);
/**
 * @brief       [7] Get meter reading
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 * @param       mt meter index (1-based index, range: 1 to numM)
 * @retval      return meter reading value
 */
OmFlt OmGetMt(OmCir* cr, OmInt mt);
/**
 * @brief       [8] Get vector Xc
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 * @param       br branch index (1-based index, range: 1 to numB)
 * @retval      return Xc value
 * @note        for X0/Y0 branch, return 0.0, as they are always cut off
 * @note        for X1/X2 branch, return branch current
 * @note        for Y1/Y2/SW branch, return branch voltage
 */
OmFlt OmGetXc(OmCir* cr, OmInt br);
/**
 * @brief       [9] Set independent source vector Qs
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 * @param       br branch index (1-based index, range: 1 to numB)
 * @param       x new Qs value
 * @note        for X0/Y0 branch, do nothing
 * @note        for X1/X2/X3 branch, set voltage source
 * @note        for Y1/Y2/Y3/SW branch, set current source
 */
void OmSetQs(OmCir* cr, OmInt br, OmFlt x);
/**
 * @brief       [10] Set switch state
 * @param       cr input stamped OmCir pointer (cannot be NULL)
 * @param       br branch index (1-based index, range: 1 to numB)
 * @param       s new switch state (0: open, 1: closed)
 * @note        only valid for SW-type branch, else will cause error
 */
void OmSetSw(OmCir* cr, OmInt br, OmInt s);
/**
 * @brief       [11] Set branch configuration
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       br branch index (1-based index, range: 1 to numB)
 * @param       n1 node 1 index (1-based index, range: 1 to numN, 0 for GND)
 * @param       n2 node 2 index (1-based index, range: 1 to numN, 0 for GND)
 * @param       tm branch type and ode method
 * @note        do not configure one branch more than once
 * @note        tm: use OMTYP_* macros defined in this header file
 * @note        tm: use Trapezoidal rule by default
 * @note        tm: add negative sign to use Backward Euler rule
 */
void OmBran(OmCir* cr, OmInt br, OmInt n1, OmInt n2, OmInt tm);
/**
 * @brief       [12] Set voltmeter configuration
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       mt meter index (1-based index, range: 1 to numM)
 * @param       n1 node 1 index (1-based index, range: 1 to numN, 0 for GND)
 * @param       n2 node 2 index (1-based index, range: 1 to numN, 0 for GND)
 */
void OmMetV(OmCir* cr, OmInt mt, OmInt n1, OmInt n2);
/**
 * @brief       [13] Set ammeter configuration
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       mt meter index (1-based index, range: 1 to numM)
 * @param       br branch index (1-based index, range: 1 to numB)
 */
void OmMetA(OmCir* cr, OmInt mt, OmInt br);
/**
 * @brief       [14] Series connect resistor to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx branch index (1-based index, range: 1 to numB)
 * @param       res value of resistance to be added
 * @note        branch must be X0/X1/X2/X3-type
 * @note        u = res * i
 */
void OmAddX(OmCir* cr, OmInt bx, OmFlt res);
/**
 * @brief       [15] Parallel connect conductor to Y/SW-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by branch index (1-based index, range: 1 to numB)
 * @param       con value of conductance to be added
 * @note        branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        j = con * v
 */
void OmAddY(OmCir* cr, OmInt by, OmFlt con);
/**
 * @brief       [16] Series connect voltage source to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx branch index (1-based index, range: 1 to numB)
 * @param       vol value of voltage source to be added
 * @note        branch must be X1/X2/X3-type
 * @note        u = vol
 */
void OmAddV(OmCir* cr, OmInt bx, OmFlt vol);
/**
 * @brief       [17] Parallel connect current source to Y/SW-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by branch index (1-based index, range: 1 to numB)
 * @param       cur value of current source to be added
 * @note        branch must be Y1/Y2/Y3/SW-type
 * @note        j = cur
 */
void OmAddI(OmCir* cr, OmInt by, OmFlt cur);
/**
 * @brief       [18] Series connect inductor to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx branch index (1-based index, range: 1 to numB)
 * @param       ind value of inductance to be added
 * @param       i0 initial current
 * @note        branch must be X2/X3-type
 * @note        u = ind * di / dt
 */
void OmAddL(OmCir* cr, OmInt bx, OmFlt ind, OmFlt i0);
/**
 * @brief       [19] Parallel connect capacitor to Y-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by branch index (1-based index, range: 1 to numB)
 * @param       cap value of capacitance to be added
 * @param       v0 initial voltage
 * @note        branch must be Y2/Y3-type
 * @note        j = cap * dv / dt
 */
void OmAddC(OmCir* cr, OmInt by, OmFlt cap, OmFlt v0);
/**
 * @brief       [20] Series connect capacitor to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx branch index (1-based index, range: 1 to numB)
 * @param       rpc value of reciprocal of capacitance to be added
 * @param       v0 initial voltage
 * @note        branch must be X2/X3-type
 * @note        u = sum(rpc * i * dt)
 */
void OmAddQ(OmCir* cr, OmInt bx, OmFlt rpc, OmFlt v0);
/**
 * @brief       [21] Parallel connect inductor to Y-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by branch index (1-based index, range: 1 to numB)
 * @param       rpi value of reciprocal of inductance to be added
 * @param       i0 initial current
 * @note        branch must be Y2/Y3-type
 * @note        j = sum(rpi * v * dt)
 */
void OmAddP(OmCir* cr, OmInt by, OmFlt rpi, OmFlt i0);
/**
 * @brief       [22] Series connect linear VCVS to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx controlled branch index (1-based index, range: 1 to numB)
 * @param       cy controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @note        controlled branch must be X0/X1/X2/X3-type
 * @note        controlling branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        u = k * vc
 */
void OmAddE(OmCir* cr, OmInt bx, OmInt cy, OmFlt k);
/**
 * @brief       [23] Series connect linear CCVS to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx controlled branch index (1-based index, range: 1 to numB)
 * @param       cx controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @note        controlled branch must be X0/X1/X2/X3-type
 * @note        controlling branch must be X0/X1/X2/X3-type
 * @note        u = k * ic
 */
void OmAddH(OmCir* cr, OmInt bx, OmInt cx, OmFlt k);
/**
 * @brief       [24] Parallel connect linear CCCS to Y/SW-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by controlled branch index (1-based index, range: 1 to numB)
 * @param       cx controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @note        controlled branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        controlling branch must be X0/X1/X2/X3-type
 * @note        j = k * ic
 */
void OmAddF(OmCir* cr, OmInt by, OmInt cx, OmFlt k);
/**
 * @brief       [25] Parallel connect linear VCCS to Y/SW-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by controlled branch index (1-based index, range: 1 to numB)
 * @param       cy controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @note        controlled branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        controlling branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        j = k * vc
 */
void OmAddG(OmCir* cr, OmInt by, OmInt cy, OmFlt k);
/**
 * @brief       [26] Series connect differential CCVS to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx controlled branch index (1-based index, range: 1 to numB)
 * @param       cx controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @param       ic0 initial current of controlling branch
 * @note        controlled branch must be X3-type
 * @note        controlling branch must be X0/X1/X2/X3-type
 * @note        u = k * dic / dt
 */
void OmAddM(OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt ic0);
/**
 * @brief       [27] Parallel connect differential VCCS to Y-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by controlled branch index (1-based index, range: 1 to numB)
 * @param       cy controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @param       vc0 initial voltage of controlling branch
 * @note        controlled branch must be Y3-type
 * @note        controlling branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        j = k * dvc / dt
 */
void OmAddN(OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt vc0);
/**
 * @brief       [28] Series connect integral CCVS to X-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bx controlled branch index (1-based index, range: 1 to numB)
 * @param       cx controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @param       v0 initial voltage
 * @note        controlled branch must be X3-type
 * @note        controlling branch must be X0/X1/X2/X3-type
 * @note        u = sum(k * ic * dt)
 */
void OmAddA(OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt v0);
/**
 * @brief       [29] Parallel connect integral VCCS to Y-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       by controlled branch index (1-based index, range: 1 to numB)
 * @param       cy controlling branch index (1-based index, range: 1 to numB)
 * @param       k gain of controlled source
 * @param       i0 initial current
 * @note        controlled branch must be Y3-type
 * @note        controlling branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        j = sum(k * vc * dt)
 */
void OmAddB(OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt i0);
/**
 * @brief       [30] Parallel connect switch to SW-type branch
 * @param       cr input unstamped OmCir pointer (cannot be NULL)
 * @param       bs switch branch index (1-based index, range: 1 to numB)
 * @param       k1 closed state coefficient
 * @param       k2 open state coefficient
 * @param       ysw switch conductance (cannot be 0)
 * @param       ron series connected on-resistance (can be 0)
 * @note        branch must be SW-type
 * @note        controlling branch must be Y0/Y1/Y2/Y3/SW-type
 * @note        ja(t+dt) = k1 * ysw * v(t) + i(t), ON
 * @note        ja(t+dt) = -ysw * v(t) + k2 * i(t), OFF
 * @note        if you know switch rated voltage (V) and rated current (I)
 * @note        try k1 = 1, k2 = 0.6569, ysw = 0.2929 * I / V
 */
void OmAddS(OmCir* cr, OmInt bs, OmFlt k1, OmFlt k2, OmFlt ysw, OmFlt ron);
/**
 * @brief       [31] Get inverse of input square matrix
 * @param       m square matrix row / column length (must >= 0)
 * @param       a input square matrix (cannot be NULL)
 */
void OmMatInv(OmInt m, OmFlt* a);
/**
 * @brief       [32] Square matrix-matrix multiplication, C = A @ B
 * @param       m square matrix row / column length (must >= 0)
 * @param       c input square matrix (cannot be NULL, cannot be A or B)
 * @param       a input square matrix (cannot be NULL)
 * @param       b input square matrix (cannot be NULL)
 */
void OmMatMul(OmInt m, OmFlt* c, OmFlt* a, OmFlt* b);
/**
 * @brief       [33] Matrix-vector multiplication, y = A @ x
 * @param       m number of rows of matrix A (must >= 0)
 * @param       n number of columns of matrix A (must >= 0)
 * @param       y output vector (cannot be NULL, cannot be x), size = m
 * @param       a input matrix (cannot be NULL), size = m*n
 * @param       x input vector (cannot be NULL), size = n
 */
void OmVecMul(OmInt m, OmInt n, OmFlt* y, OmFlt* a, OmFlt* x);
/**
 * @brief       [34] Vector addition, z = x + y
 * @param       m number of elements of vectors (must >= 0)
 * @param       z output vector (cannot be NULL, cannot be x or y)
 * @param       x input vector (cannot be NULL)
 * @param       y input vector (cannot be NULL)
 */
void OmVecAdd(OmInt m, OmFlt* z, OmFlt* x, OmFlt* y);
/**
 * @brief       [35] Vector fused multiply-add, y = w1 * x + w2 * y
 * @param       m number of elements of vectors (must >= 0)
 * @param       y output vector (cannot be NULL, cannot be x or w1, w2)
 * @param       w1 input vector (cannot be NULL)
 * @param       x input vector (cannot be NULL)
 * @param       w2 input vector (cannot be NULL)
 */
void OmVecFma(OmInt m, OmFlt* y, OmFlt* w1, OmFlt* x, OmFlt* w2);

/*===========================================================================*/
#ifdef LIBOHM_C                         /*| define this in exactly one file |*/
/*===========================================================================*/

/*====================== Part 5. Function Implementation ====================*/

/*========== OmDelete ================*//** Function [0]                     */
void OmDelete(OmCir* cr) {
    if (cr == NULL) return;             /* check if cr is null pointer       */
    cr->numN    = 0;
    cr->numB    = 0;
    cr->numM    = 0;
    cr->numX    = 0;
    cr->numC    = 0;
    cr->timStp  = 1.0;
    OMFREE(cr->vecBn1 ); cr->vecBn1 = NULL;
    OMFREE(cr->vecBn2 ); cr->vecBn2 = NULL;
    OMFREE(cr->vecMn1 ); cr->vecMn1 = NULL;
    OMFREE(cr->vecMn2 ); cr->vecMn2 = NULL;
    OMFREE(cr->matPa  ); cr->matPa  = NULL;
    OMFREE(cr->matPb  ); cr->matPb  = NULL;
    OMFREE(cr->vecBtm ); cr->vecBtm = NULL;
    OMFREE(cr->vecLut ); cr->vecLut = NULL;
    OMFREE(cr->vecW1c ); cr->vecW1c = NULL;
    OMFREE(cr->vecW2c ); cr->vecW2c = NULL;
    OMFREE(cr->vecW1o ); cr->vecW1o = NULL;
    OMFREE(cr->vecW2o ); cr->vecW2o = NULL;
    OMFREE(cr->vecQa0 ); cr->vecQa0 = NULL;
    OMFREE(cr->vecQs0 ); cr->vecQs0 = NULL;
    OMFREE(cr->matC   ); cr->matC   = NULL;
    OMFREE(cr->matD   ); cr->matD   = NULL;
    OMFREE(cr->vecW1m ); cr->vecW1m = NULL;
    OMFREE(cr->vecW2m ); cr->vecW2m = NULL;
    OMFREE(cr->vecW1s ); cr->vecW1s = NULL;
    OMFREE(cr->vecW2s ); cr->vecW2s = NULL;
    OMFREE(cr->vecQa  ); cr->vecQa  = NULL;
    OMFREE(cr->vecQs  ); cr->vecQs  = NULL;
    OMFREE(cr->vecQtp ); cr->vecQtp = NULL;
    OMFREE(cr->vecXm  ); cr->vecXm  = NULL;
    OMFREE(cr->vecXc  ); cr->vecXc  = NULL;
}
/*========== OmCreate ================*//** Function [1]                     */
OmCir* OmCreate(OmInt n, OmInt b, OmInt m, OmFlt stp) {
    OmInt  i, j;                        /* used in for-loop                  */
    OmCir* cir;                         /* new circuit to be created         */
    /*======== Step 0: Check parameter and Allocate struct ==================*/
    if (n < 0 || b < 0 || m < 0 || stp <= 0.0) return NULL;
    cir = (OmCir*)OMMALLOC(sizeof(OmCir));
    /*======== Step 1: Initialize and Allocate memory for Group 1 & 2 =======*/
    cir->numN   = n;
    cir->numB   = b;
    cir->numM   = m;
    cir->numX   = 0;
    cir->numC   = 0;
    cir->timStp = stp;
    cir->vecBn1 = (OmInt*)OMMALLOC(b * sizeof(OmInt));
    cir->vecBn2 = (OmInt*)OMMALLOC(b * sizeof(OmInt));
    cir->vecMn1 = (OmInt*)OMMALLOC(m * sizeof(OmInt));
    cir->vecMn2 = (OmInt*)OMMALLOC(m * sizeof(OmInt));
    cir->matPa  = (OmFlt*)OMMALLOC(b * b * sizeof(OmFlt));
    cir->matPb  = (OmFlt*)OMMALLOC(b * b * sizeof(OmFlt));
    cir->vecBtm = (OmInt*)OMMALLOC(b * sizeof(OmInt));
    cir->vecLut = (OmInt*)OMMALLOC(b * sizeof(OmInt));
    cir->vecW1c = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    cir->vecW2c = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    cir->vecW1o = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    cir->vecW2o = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    cir->vecQa0 = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    cir->vecQs0 = (OmFlt*)OMMALLOC(b * sizeof(OmFlt));
    /*======== Step 2: Initialize allocated struct pointer ==================*/
    for (i=0; i < b; ++i) {
        for (j=0; j < b; ++j) {
            cir->matPa[i*b+j] = 0.0;    /* fill Pa with zeros                */
            cir->matPb[i*b+j] = 0.0;    /* fill Pb with zeros                */
        }
        cir->vecBn1[i] = -1;            /* fill node1 of branch with GND(-1) */
        cir->vecBn2[i] = -1;            /* fill node2 of branch with GND(-1) */
        cir->vecBtm[i] = OMTYP_UN;      /* fill branch type with UN          */
        cir->vecLut[i] = 1;             /* fill lookup table with Y-type     */
        cir->vecW1c[i] = 0.0;           /* fill W1c with zeros               */
        cir->vecW2c[i] = 0.0;           /* fill W2c with zeros               */
        cir->vecW1o[i] = 0.0;           /* fill W1o with zeros               */
        cir->vecW2o[i] = 0.0;           /* fill W2o with zeros               */
        cir->vecQa0[i] = 0.0;           /* fill Qa0 with zeros               */
        cir->vecQs0[i] = 0.0;           /* fill Qs0 with zeros               */
    }
    for (i=0; i < m; ++i) {
        cir->vecMn1[i] = -1;            /* fill node1 of meter with GND(-1)  */
        cir->vecMn2[i] = -1;            /* fill node2 of meter with GND(-1)  */
    }
    return cir;
}
/*========== OmStamp =================*//** Function [2]                     */
void OmStamp(OmCir* cr) {
    OmInt n, b, m, x, c;                /* numN, numB, numM, numX, numC      */
    OmInt i, j;                         /* used in for-loop                  */
    OmInt ilut, jlut, idx, jdx;         /* used in lookup table              */
    OmInt btyp;                         /* branch type                       */
    OmInt n1, n2, nc1, nc2;             /* node and controlling node         */
    OmFlt k;                            /* used as factor / coefficient      */
    OmFlt* matPn;                       /* node conductance matrix [n+x,n+x] */
    OmFlt* matPtp;                      /* Ptp = (Pn^-1)(Tn)       [n+x,b]   */
    OmFlt* matTtp;                      /* Ttp = (Tb)(Ptp)         [b,b]     */
    OmFlt* matRtp;                      /* Rtp = (Pb)(Ttp) + I     [b,b]     */
    OmFlt* matCtp;                      /* Ctp = (Pa)(Ttp)         [b,b]     */
    OmFlt* matDtp;                      /* Dtp = (K)(Ptp,Rtp)      [m,b]     */
    /*======== Step 0: Get Number of nodes and branches =====================*/
    n = cr->numN;
    b = cr->numB;
    m = cr->numM;
    x = cr->numX;
    /*======== Step 1: Stamp Pb to Pn =======================================*/
    matPn  = (OmFlt*)OMMALLOC((n+x) * (n+x) * sizeof(OmFlt));
    matPtp = (OmFlt*)OMMALLOC((n+x) * b * sizeof(OmFlt));
    for (i=0; i < (n+x) * (n+x); ++i) matPn[i] = 0.0;
    for (i=0; i < (n+x) * b; ++i) matPtp[i] = 0.0;
    for (i=0; i < b; ++i) {
        ilut = cr->vecLut[i];           /* 0...-x is X-type, 1 is Y-type     */
        n1 = cr->vecBn1[i];             /* node1 (0-based index)             */
        n2 = cr->vecBn2[i];             /* node2 (0-based index)             */
        if (ilut > 0) {                 /* Branch i is Y-type branch         */
            for (j=0; j < b; ++j) {
                k = cr->matPb[i*b+j];
                jlut = cr->vecLut[j];   /* 0...-x is X-type, 1 is Y-type     */
                nc1 = cr->vecBn1[j];    /* controlling node1 (0-based index) */
                nc2 = cr->vecBn2[j];    /* controlling node2 (0-based index) */
                if (jlut > 0) {         /* Branch j is Y-type branch         */
                    if (n1 >= 0 && nc1 >= 0) matPn[n1*(n+x)+nc1] += k;
                    if (n1 >= 0 && nc2 >= 0) matPn[n1*(n+x)+nc2] -= k;
                    if (n2 >= 0 && nc1 >= 0) matPn[n2*(n+x)+nc1] -= k;
                    if (n2 >= 0 && nc2 >= 0) matPn[n2*(n+x)+nc2] += k;
                } else {                /* Branch j is X-type branch         */
                    if (n1 >= 0) matPn[n1*(n+x)+(n-jlut)] += k;
                    if (n2 >= 0) matPn[n2*(n+x)+(n-jlut)] -= k;
                }
            }
        } else {                        /* Branch i is X-type branch         */
            if (n1 >= 0) matPn[n1*(n+x)+(n-ilut)] += 1.0;
            if (n1 >= 0) matPn[(n-ilut)*(n+x)+n1] += 1.0;
            if (n2 >= 0) matPn[n2*(n+x)+(n-ilut)] -= 1.0;
            if (n2 >= 0) matPn[(n-ilut)*(n+x)+n2] -= 1.0;
            for (j=0; j < b; ++j) {
                k = cr->matPb[i*b+j];
                jlut = cr->vecLut[j];   /* 0...-x is X-type, 1 is Y-type     */
                nc1 = cr->vecBn1[j];    /* controlling node1 (0-based index) */
                nc2 = cr->vecBn2[j];    /* controlling node2 (0-based index) */
                if (jlut > 0) {         /* Branch j is Y-type branch         */
                    if (nc1 >= 0) matPn[(n-ilut)*(n+x)+nc1] -= k;
                    if (nc2 >= 0) matPn[(n-ilut)*(n+x)+nc2] += k;
                } else {                /* Branch j is X-type branch         */
                    matPn[(n-ilut)*(n+x)+(n-jlut)] -= k;
                }
            }
        }
    }
    OmMatInv(n+x, matPn);               /* calculate Pn^-1                   */
    for (i=0; i < n+x; ++i) {           /* calculate Ptp = (Pn^-1)(Tn)       */
        for (j=0; j < b; ++j) {
            jlut = cr->vecLut[j];       /* 0...-x is X-type, 1 is Y-type     */
            n1 = cr->vecBn1[j];         /* 0-based index                     */
            n2 = cr->vecBn2[j];         /* 0-based index                     */
            if (jlut > 0) {             /* Y-type branch                     */
                if (n1 >= 0) matPtp[i*b+j] -= matPn[i*(n+x)+n1];
                if (n2 >= 0) matPtp[i*b+j] += matPn[i*(n+x)+n2];
            } else {                    /* X-type branch                     */
                matPtp[i*b+j] += matPn[i*(n+x)+(n-jlut)];
            }
        }
    }
    OMFREE(matPn);                      /* matPn is not needed anymore       */
    /*======== Step 2: Calculate Ttp, Rtp, Ctp, Dtp =========================*/
    matTtp = (OmFlt*)OMMALLOC(b * b * sizeof(OmFlt));
    matRtp = (OmFlt*)OMMALLOC(b * b * sizeof(OmFlt));
    matCtp = (OmFlt*)OMMALLOC(b * b * sizeof(OmFlt));
    matDtp = (OmFlt*)OMMALLOC(m * b * sizeof(OmFlt));
    for (i=0; i < b * b; ++i) matTtp[i] = 0.0;
    for (i=0; i < m * b; ++i) matDtp[i] = 0.0;
    for (i=0; i < b; ++i) {
        ilut = cr->vecLut[i];
        if (ilut > 0) {                 /* Y-type branch                     */
            n1 = cr->vecBn1[i];         /* 0-based index                     */
            n2 = cr->vecBn2[i];         /* 0-based index                     */
            if (n1 >= 0) for (j=0; j < b; ++j) {
                matTtp[i*b+j] += matPtp[n1*b+j];
            }
            if (n2 >= 0) for (j=0; j < b; ++j) {
                matTtp[i*b+j] -= matPtp[n2*b+j];
            }
        } else {                        /* X-type branch                     */
            for (j=0; j < b; ++j) {
                matTtp[i*b+j] += matPtp[(n-ilut)*b+j];
            }
        }
    }
    OmMatMul(b, matCtp, cr->matPa, matTtp);
    OmMatMul(b, matRtp, cr->matPb, matTtp);
    for (i=0; i < b; ++i) matRtp[i*b+i] += 1.0;
    for (i=0; i < m; ++i) {             /* calculate Dtp = (K)(Ptp,Rtp)      */
        n1 = cr->vecMn1[i];             /* 0-based index                     */
        n2 = cr->vecMn2[i];             /* 0-based index                     */
        if (n2 < -1) {                  /* ammeter                           */
            ilut = cr->vecLut[n1];      /* use Rtp if Y-type, Ptp if X-type  */
            if (ilut > 0) for (j=0; j < b; ++j) {
                matDtp[i*b+j] += matRtp[n1*b+j];
            } 
            else for (j=0; j < b; ++j) {
                matDtp[i*b+j] += matPtp[(n-ilut)*b+j];
            }
        } else {                        /* voltmeter                         */
            if (n1 >= 0) for (j=0; j < b; ++j) {
                matDtp[i*b+j] += matPtp[n1*b+j];
            }
            if (n2 >= 0) for (j=0; j < b; ++j) {
                matDtp[i*b+j] -= matPtp[n2*b+j];
            }
        }
    }
    OMFREE(cr->vecBn1); cr->vecBn1 = NULL;
    OMFREE(cr->vecBn2); cr->vecBn2 = NULL;
    OMFREE(cr->vecMn1); cr->vecMn1 = NULL;
    OMFREE(cr->vecMn2); cr->vecMn2 = NULL;
    OMFREE(cr->matPa ); cr->matPa  = NULL;
    OMFREE(cr->matPb ); cr->matPb  = NULL;
    OMFREE(matPtp);
    OMFREE(matTtp);
    OMFREE(matRtp);
    /*======== Step 3: Count the number of kept branches ====================*/
    c = 0;
    for (i=0; i < b; ++i) {             /* detect X0 and Y0 branches         */
        btyp = OMABS(cr->vecBtm[i]);
        if (btyp == OMTYP_X0 || btyp == OMTYP_Y0) {
            cr->vecLut[i] = -1;         /* mark as cut branch                */
        } else {
            cr->vecLut[i] = c;          /* mark as kept branch               */
            c += 1;
        }
    }
    cr->numC   = c;                     /* set numC                          */
    /*======== Step 4: Simplify Ctp and Dtp (matrix cutting) ================*/
    cr->matC   = (OmFlt*)OMMALLOC(c * c * sizeof(OmFlt));
    cr->matD   = (OmFlt*)OMMALLOC(m * c * sizeof(OmFlt));
    for (i=0; i < m; ++i) {             /* build D matrix                    */
        jdx = 0;
        for (j=0; j < b; ++j) {
            jlut = cr->vecLut[j];
            if (jlut < 0) continue;     /* cut branch                        */
            cr->matD[i*c+jdx] = matDtp[i*b+j];
            jdx += 1;
        }
    }
    idx = 0;
    for (i=0; i < b; ++i) {             /* build C matrix                    */
        ilut = cr->vecLut[i];
        if (ilut < 0) continue;         /* cut branch                        */
        jdx = 0;
        for (j=0; j < b; ++j) {
            jlut = cr->vecLut[j];
            if (jlut < 0) continue;     /* cut branch                        */
            cr->matC[idx*c+jdx] = matCtp[i*b+j];
            jdx += 1;
        }
        idx += 1;
    }
    OMFREE(matCtp);
    OMFREE(matDtp);
    /*======== Step 4: Allocate memory for runtime vectors ==================*/
    cr->vecW1m = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecW2m = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecW1s = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecW2s = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecQa  = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecQs  = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecQtp = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    cr->vecXm  = (OmFlt*)OMMALLOC(m * sizeof(OmFlt));
    cr->vecXc  = (OmFlt*)OMMALLOC(c * sizeof(OmFlt));
    OmReset(cr);                        /* reset circuit to initial state    */
}
/*========== OmReset =================*//** Function [3]                     */
void OmReset(OmCir* cr) {
    OmInt b, c;                         /* numB, numC                        */
    OmInt i;                            /* used in for-loop                  */
    OmInt btyp, ilut;                   /* branch type and lut value         */
    b = cr->numB;
    c = cr->numC;
    for (i=0; i < b; ++i) {
        ilut = cr->vecLut[i];
        btyp = OMABS(cr->vecBtm[i]);
        if (ilut < 0) continue;         /* cut branch                        */
        cr->vecW1m[ilut] = cr->vecW1o[i];
        cr->vecW2m[ilut] = cr->vecW2o[i];
        cr->vecQa[ilut] = cr->vecQa0[i];
        cr->vecQs[ilut] = cr->vecQs0[i];
        if (btyp == OMTYP_SW) {         /* always reset switch to open state */
            cr->vecW1s[ilut] = cr->vecW1o[i];
            cr->vecW2s[ilut] = cr->vecW2o[i];
        } else {
            cr->vecW1s[ilut] = 0.0;
            cr->vecW2s[ilut] = 1.0;     /* keep other branch Qa unchanged    */
        }
    }
    OmVecAdd(c, cr->vecQtp, cr->vecQa, cr->vecQs);
}
/*========== OmUpdSw =================*//** Function [4]                     */
void OmUpdSw(OmCir* cr) {
    OmInt c;                            /* numC                              */
    c = cr->numC;
    OmVecAdd(c, cr->vecQtp, cr->vecQa, cr->vecQs);
    OmVecMul(c, c, cr->vecXc, cr->matC, cr->vecQtp);
    OmVecFma(c, cr->vecQa, cr->vecW1s, cr->vecXc, cr->vecW2s);
}
/*========== OmUpdCr =================*//** Function [5]                     */
void OmUpdCr(OmCir* cr) {
    OmInt c;                            /* numC                              */
    c = cr->numC;
    OmVecAdd(c, cr->vecQtp, cr->vecQa, cr->vecQs);
    OmVecMul(c, c, cr->vecXc, cr->matC, cr->vecQtp);
    OmVecFma(c, cr->vecQa, cr->vecW1m, cr->vecXc, cr->vecW2m);
}
/*========== OmUpdMt =================*//** Function [6]                     */
void OmUpdMt(OmCir* cr) {
    OmInt m, c;                         /* numM, numC                        */
    m = cr->numM;
    c = cr->numC;
    OmVecMul(m, c, cr->vecXm, cr->matD, cr->vecQtp);
}
/*========== OmGetMt =================*//** Function [7]                     */
OmFlt OmGetMt(OmCir* cr, OmInt mt) {
    return cr->vecXm[mt-1];
}
/*========== OmGetXc =================*//** Function [8]                     */
OmFlt OmGetXc(OmCir* cr, OmInt br) {
    OmInt ilut;                         /* lookup table value                */
    ilut = cr->vecLut[br-1];
    if (ilut < 0) return 0.0;           /* cut branch                        */
    return cr->vecXc[ilut];
}
/*========== OmSetQs =================*//** Function [9]                     */
void OmSetQs(OmCir* cr, OmInt br, OmFlt x) {
    OmInt ilut;                         /* lookup table value                */
    ilut = cr->vecLut[br-1];
    if (ilut < 0) return;               /* cut branch                        */
    cr->vecQs[ilut] = x;
}
/*========== OmSetSw =================*//** Function [10]                    */
void OmSetSw(OmCir* cr, OmInt br, OmInt s) {
    OmInt ilut;                         /* lookup table value                */
    ilut = cr->vecLut[br-1];
    if (ilut < 0) return;               /* cut branch                        */
    cr->vecW1m[ilut] = (s == 0) ? cr->vecW1o[br-1] : cr->vecW1c[br-1];
    cr->vecW2m[ilut] = (s == 0) ? cr->vecW2o[br-1] : cr->vecW2c[br-1];
    cr->vecW1s[ilut] = cr->vecW1m[ilut];
    cr->vecW2s[ilut] = cr->vecW2m[ilut];
}
/*========== OmBran ==================*//** Function [11]                    */
void OmBran(OmCir* cr, OmInt br, OmInt n1, OmInt n2, OmInt tm) {
    OmInt b;                            /* numB                              */
    OmInt btyp;                         /* branch type                       */
    cr->vecBn1[br-1] = n1 - 1;
    cr->vecBn2[br-1] = n2 - 1;
    cr->vecBtm[br-1] = tm;
    btyp = OMABS(tm);
    if (btyp >= OMTYP_Y0) {             /* Y/SW-type branch                  */
        cr->vecLut[br-1] = 1;
    } else {
        cr->vecLut[br-1] = -cr->numX;
        cr->numX += 1;
    }
    if (btyp != OMTYP_X3 && btyp != OMTYP_Y3) {
        cr->matPa[(br-1)*b+(br-1)] = 1.0;
    }                                   /* Xc is branch current or voltage   */
}
/*========== OmMetV ==================*//** Function [12]                    */
void OmMetV(OmCir* cr, OmInt mt, OmInt n1, OmInt n2) {
    cr->vecMn1[mt-1] = n1 - 1;
    cr->vecMn2[mt-1] = n2 - 1;
}
/*========== OmMetA ==================*//** Function [13]                    */
void OmMetA(OmCir* cr, OmInt mt, OmInt br) {
    cr->vecMn1[mt-1] = br - 1;
    cr->vecMn2[mt-1] = -2;
}
/*========== OmAddX ==================*//** Function [14]                    */
void OmAddX(OmCir* cr, OmInt bx, OmFlt res) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(bx-1)*b+(bx-1)] += res;
}
/*========== OmAddY ==================*//** Function [15]                    */
void OmAddY(OmCir* cr, OmInt by, OmFlt con) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(by-1)*b+(by-1)] += con;
}
/*========== OmAddV ==================*//** Function [16]                    */
void OmAddV(OmCir* cr, OmInt bx, OmFlt vol) {
    cr->vecQs0[bx-1] += vol;
}
/*========== OmAddI ==================*//** Function [17]                    */
void OmAddI(OmCir* cr, OmInt by, OmFlt cur) {
    cr->vecQs0[by-1] += cur;
}
/*========== OmAddL ==================*//** Function [18]                    */
void OmAddL(OmCir* cr, OmInt bx, OmFlt ind, OmFlt i0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[bx-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPb[(bx-1)*b+(bx-1)] += ind / stp;
        cr->vecQa0[bx-1] -= (ind * i0) / stp;
        cr->vecW2o[bx-1] = 0.0;
        if (btm == -OMTYP_X3) {         /* Level 3 branch                    */
            cr->matPa[(bx-1)*b+(bx-1)] += ind;
            cr->vecW1o[bx-1] = -1.0 / stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[bx-1] += (-1.0 * ind) / stp;
        }
    } else {                            /* trapezoidal rule                  */
        cr->matPb[(bx-1)*b+(bx-1)] += (2.0 * ind) / stp;
        cr->vecQa0[bx-1] -= (2.0 * ind * i0) / stp;
        cr->vecW2o[bx-1] = -1.0;
        if (btm == OMTYP_X3) {          /* Level 3 branch                    */
            cr->matPa[(bx-1)*b+(bx-1)] += ind;
            cr->vecW1o[bx-1] = -4.0 / stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[bx-1] += (-4.0 * ind) / stp;
        }
    }
}
/*========== OmAddC ==================*//** Function [19]                    */
void OmAddC(OmCir* cr, OmInt by, OmFlt cap, OmFlt v0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[by-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPb[(by-1)*b+(by-1)] += cap / stp;
        cr->vecQa0[by-1] -= (cap * v0) / stp;
        cr->vecW2o[by-1] = 0.0;
        if (btm == -OMTYP_Y3) {         /* Level 3 branch                    */
            cr->matPa[(by-1)*b+(by-1)] += cap;
            cr->vecW1o[by-1] = -1.0 / stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[by-1] += (-1.0 * cap) / stp;
        }
    } else {                            /* trapezoidal rule                  */
        cr->matPb[(by-1)*b+(by-1)] += (2.0 * cap) / stp;
        cr->vecQa0[by-1] -= (2.0 * cap * v0) / stp;
        cr->vecW2o[by-1] = -1.0;
        if (btm == OMTYP_Y3)  {         /* Level 3 branch                    */
            cr->matPa[(by-1)*b+(by-1)] += cap;
            cr->vecW1o[by-1] = -4.0 / stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[by-1] += (-4.0 * cap) / stp;
        }
    }
}
/*========== OmAddQ ==================*//** Function [20]                    */
void OmAddQ(OmCir* cr, OmInt bx, OmFlt rpc, OmFlt v0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[bx-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPb[(bx-1)*b+(bx-1)] += rpc * stp;
        cr->vecQa0[bx-1] += v0;
        cr->vecW2o[bx-1] = 1.0;
        if (btm == -OMTYP_X3) {         /* Level 3 branch                    */
            cr->matPa[(bx-1)*b+(bx-1)] += rpc;
            cr->vecW1o[bx-1] = stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[bx-1] += rpc * stp;
        }
    } else {                            /* trapezoidal rule                  */
        cr->matPb[(bx-1)*b+(bx-1)] += (rpc * stp) / 2.0;
        cr->vecQa0[bx-1] += v0;
        cr->vecW2o[bx-1] = 1.0;
        if (btm == OMTYP_X3) {          /* Level 3 branch                    */
            cr->matPa[(bx-1)*b+(bx-1)] += rpc;
            cr->vecW1o[bx-1] = stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[bx-1] += rpc * stp;
        }
    }
}
/*========== OmAddP ==================*//** Function [21]                    */
void OmAddP(OmCir* cr, OmInt by, OmFlt rpi, OmFlt i0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[by-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPb[(by-1)*b+(by-1)] += rpi * stp;
        cr->vecQa0[by-1] += i0;
        cr->vecW2o[by-1] = 1.0;
        if (btm == -OMTYP_Y3) {         /* Level 3 branch                    */
            cr->matPa[(by-1)*b+(by-1)] += rpi;
            cr->vecW1o[by-1] = stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[by-1] += rpi * stp;
        }
    } else {                            /* trapezoidal rule                  */
        cr->matPb[(by-1)*b+(by-1)] += (rpi * stp) / 2.0;
        cr->vecQa0[by-1] += i0;
        cr->vecW2o[by-1] = 1.0;
        if (btm == OMTYP_Y3) {          /* Level 3 branch                    */
            cr->matPa[(by-1)*b+(by-1)] += rpi;
            cr->vecW1o[by-1] = stp;
        } else {                        /* Level 2 branch                    */
            cr->vecW1o[by-1] += rpi * stp;
        }
    }
}
/*========== OmAddE ==================*//** Function [22]                    */
void OmAddE(OmCir* cr, OmInt bx, OmInt cy, OmFlt k) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(bx-1)*b+(cy-1)] += k;
}
/*========== OmAddH ==================*//** Function [23]                    */
void OmAddH(OmCir* cr, OmInt bx, OmInt cx, OmFlt k) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(bx-1)*b+(cx-1)] += k;
}
/*========== OmAddF ==================*//** Function [24]                    */
void OmAddF(OmCir* cr, OmInt by, OmInt cx, OmFlt k) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(by-1)*b+(cx-1)] += k;
}
/*========== OmAddG ==================*//** Function [25]                    */
void OmAddG(OmCir* cr, OmInt by, OmInt cy, OmFlt k) {
    OmInt b;                            /* numB                              */
    b = cr->numB;
    cr->matPb[(by-1)*b+(cy-1)] += k;
}
/*========== OmAddM ==================*//** Function [26]                    */
void OmAddM(OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt ic0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[bx-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPa[(bx-1)*b+(cx-1)] += k;
        cr->matPb[(bx-1)*b+(cx-1)] += k / stp;
        cr->vecQa0[bx-1] -= (k * ic0) / stp;
        cr->vecW1o[bx-1] = -1.0 / stp;
        cr->vecW2o[bx-1] = 0.0;
    } else {                            /* trapezoidal rule                  */
        cr->matPa[(bx-1)*b+(cx-1)] += k;
        cr->matPb[(bx-1)*b+(cx-1)] += (2.0 * k) / stp;
        cr->vecQa0[bx-1] -= (2.0 * k * ic0) / stp;
        cr->vecW1o[bx-1] = -4.0 / stp;
        cr->vecW2o[bx-1] = -1.0;
    }
}
/*========== OmAddN ==================*//** Function [27]                    */
void OmAddN(OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt vc0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[by-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPa[(by-1)*b+(cy-1)] += k;
        cr->matPb[(by-1)*b+(cy-1)] += k / stp;
        cr->vecQa0[by-1] -= (k * vc0) / stp;
        cr->vecW1o[by-1] = -1.0 / stp;
        cr->vecW2o[by-1] = 0.0;
    } else {                            /* trapezoidal rule                  */
        cr->matPa[(by-1)*b+(cy-1)] += k;
        cr->matPb[(by-1)*b+(cy-1)] += (2.0 * k) / stp;
        cr->vecQa0[by-1] -= (2.0 * k * vc0) / stp;
        cr->vecW1o[by-1] = -4.0 / stp;
        cr->vecW2o[by-1] = -1.0;
    }
}
/*========== OmAddA ==================*//** Function [28]                    */
void OmAddA(OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt v0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[bx-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPa[(bx-1)*b+(cx-1)] += k;
        cr->matPb[(bx-1)*b+(cx-1)] += k * stp;
        cr->vecQa0[bx-1] += v0;
        cr->vecW1o[bx-1] = stp;
        cr->vecW2o[bx-1] = 1.0;
    } else {                            /* trapezoidal rule                  */
        cr->matPa[(bx-1)*b+(cx-1)] += k;
        cr->matPb[(bx-1)*b+(cx-1)] += (k * stp) / 2.0;
        cr->vecQa0[bx-1] += v0;
        cr->vecW1o[bx-1] = stp;
        cr->vecW2o[bx-1] = 1.0;
    }
}
/*========== OmAddB ==================*//** Function [29]                    */
void OmAddB(OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt i0) {
    OmInt btm;                          /* branch type and ode method        */
    OmInt b;                            /* numB                              */
    OmFlt stp;                          /* timStp                            */
    btm = cr->vecBtm[by-1];
    b = cr->numB;
    stp = cr->timStp;
    if (btm < 0) {                      /* backward euler rule               */
        cr->matPa[(by-1)*b+(cy-1)] += k;
        cr->matPb[(by-1)*b+(cy-1)] += k * stp;
        cr->vecQa0[by-1] += i0;
        cr->vecW1o[by-1] = stp;
        cr->vecW2o[by-1] = 1.0;
    } else {                            /* trapezoidal rule                  */
        cr->matPa[(by-1)*b+(cy-1)] += k;
        cr->matPb[(by-1)*b+(cy-1)] += (k * stp) / 2.0;
        cr->vecQa0[by-1] += i0;
        cr->vecW1o[by-1] = stp;
        cr->vecW2o[by-1] = 1.0;
    }
}
/*========== OmAddS ==================*//** Function [30]                    */
void OmAddS(OmCir* cr, OmInt bs, OmFlt k1, OmFlt k2, OmFlt ysw, OmFlt ron) {
    OmInt b;                            /* numB                              */
    OmFlt tmp;                          /* temporary value                   */
    b = cr->numB;
    tmp = 1.0 + ysw * ron;
    cr->matPb[(bs-1)*b+(bs-1)] += ysw / tmp;
    cr->vecW1c[bs-1] = (k1 + 1.0) * ysw / (tmp * tmp);
    cr->vecW2c[bs-1] = (1.0 - k1 * ysw * ron) / tmp;
    cr->vecW1o[bs-1] = (k2 - 1.0) * ysw / (tmp * tmp);
    cr->vecW2o[bs-1] = (k2 + ysw * ron) / tmp;
}
/*========== OmMatInv ================*//** Function [31]                    */
void OmMatInv(OmInt m, OmFlt* a) {
    OmInt i, j, k;                      /* used in for-loop                  */
    OmFlt mv;                           /* max value of row                  */
    OmFlt cv;                           /* current value                     */
    OmFlt tmp;                          /* used for swap                     */
    OmInt* pm;                          /* permute vector [m]                */
    OmFlt* lu;                          /* LU matrix [m,m]                   */
    /*======== Step 0: Row permutation (swap diagonal zeros) ================*/
    pm = (OmInt*)OMMALLOC(m * sizeof(OmInt));
    lu = (OmFlt*)OMMALLOC(m * m * sizeof(OmFlt));
    for (i=0; i < m; ++i) {             /* initialize pm with row index      */
        pm[i] = i;
    }    
    for (j=0; j < m; ++j) {             /* sort rows by pivot element        */
        mv = 0.0;
        for (i=j; i < m; ++i) {
            cv = OMABS(a[pm[i]*m+j]);   /* cv = A[pm[i], j]                  */
            if (cv > mv) {              /* swap rows                         */
                mv = cv;
                tmp = pm[j];
                pm[j] = pm[i];
                pm[i] = tmp;
            }
        }
    }
    for (i=0; i < m; ++i) {               /* make matrix G with new row order  */
        for (j=0; j < m; ++j) { 
            lu[i*m+j] = a[pm[i]*m+j];
        }
    }
    /*======== Step 2: LU decomposition (save both L & U in lu) =============*/
    for (i=0; i < m; ++i) {             /* calculate U matrix                */
        for (j=i; j < m; ++j) {
            for (k=0; k < i; ++k) {     /* LU[i,j] -= LU[i,k] * LU[k,j]      */
                lu[i*m+j] -= lu[i*m+k] * lu[k*m+j]; 
            }
        }
        for (k=i+1; k < m; ++k) {       /* calculate L matrix                */
            for (j=0; j < i; ++j) {     /* LU[k,i] -= LU[k,j] * LU[j,i]      */
                lu[k*m+i] -= lu[k*m+j] * lu[j*m+i]; 
            }
            lu[k*m+i] /= lu[i*m+i];     /* LU[k,i] /= LU[i,i]                */
        }
    }
    /*======== Step 3: LU inversion (save both L^-1 & U^-1 in mat) ==========*/
    for (i=0; i < m; ++i) {             /* fill matrix with zeros            */
        for (j=0; j < m; ++j) {
            a[i*m+j] = 0.0;
        }
    }
    for (i=0; i < m; ++i) {             /* L inverse & U inverse             */
        a[i*m+i] = 1.0;                 /* L matrix inverse, omit diagonal   */           
        for (k=i+1; k < m; ++k) {       /* Inv[k,i] -= LU[k,j] * Inv[j,i]    */
            for (j=i; j <= k - 1; ++j) {
                a[k*m+i] -= lu[k*m+j] * a[j*m+i];
            }
        }
        a[i*m+i] = 1.0 / lu[i*m+i];     /* U matrix inverse                  */
        for (k=i-1; k >= 0; --k) {      /* Inv[k,i] -= LU[k,j] * Inv[j,i]    */ 
            for (j=k+1; j <= i; ++j) {
                a[k*m+i] -= lu[k*m+j] * a[j*m+i];
            }
            a[k*m+i] /= lu[k*m+k];
        }
    }
    /*======== Step 4: Calculate G^-1 = U^-1 * L^-1 (save in lu) ============*/
    for (i=0; i < m; ++i) {             /* set initial value                 */
        for (j=0; j < i; ++j) {
            lu[i*m+j] = 0.0;            /* G[i,j] = 0.0                      */
        }
        for (j=i; j < m; ++j) {
            lu[i*m+j] = a[i*m+j];       /* G[i,j] = Inv[i,j]                 */
        }
    }
    for (i=1; i < m; ++i) {             /* lower part product                */
        for (k=i; k < m; ++k) {         /* G[i,j] += Inv[i,k] * Inv[k,j]     */
            for (j=0; j < i; ++j) {
                lu[i*m+j] += a[i*m+k] * a[k*m+j];
            }       
        }
    }
    for (i=0; i < m; ++i) {             /* upper part product                */
        for (j=i; j < m; ++j) {         /* G[i,j] += Inv[i,k] * Inv[k,j]     */
            for (k=j+1; k < m; ++k) {
                lu[i*m+j] += a[i*m+k] * a[k*m+j];
            }
        }
    }
    /*======== Step 5: Permute column back (save in mat) ====================*/
    for (i=0; i < m; ++i) {
        for (j=0; j < m; ++j) {
            a[i*m+pm[j]] = lu[i*m+j];
        }
    }
    free(pm);
    free(lu);
}
/*========== OmMatMul ================*//** Function [32]                    */
void OmMatMul(OmInt m, OmFlt* c, OmFlt* a, OmFlt* b) {
    OmInt i, j, k;                      /* used in for-loop                  */
    OmFlt s;                            /* temporary value                   */
    for (i=0; i < m; ++i) {
        for (j=0; j < m; ++j) {
            c[i*m+j] = 0.0;
        }
    }
    for(i=0; i < m; ++i) {
        for(k=0; k < m; ++k) {
            s = a[i*m+k];
            for (j=0; j < m; ++j) {
                c[i*m+j] += s * b[k*m+j];
            }
        }
    }
}
/*========== OmVecMul ================*//** Function [33]                    */
void OmVecMul(OmInt m, OmInt n, OmFlt* y, OmFlt* a, OmFlt* x) {
    OmInt i, j;                         /* used in for-loop                  */
    OmFlt sum;                          /* used to store summation value     */
    for (i=0; i < m; ++i) {
        sum = 0.0;
        for (j=0; j < n; ++j) {
            sum += a[i*n+j] * x[j];
        }
        y[i] = sum;
    }
}
/*========== OmVecAdd ================*//** Function [34]                    */
void OmVecAdd(OmInt m, OmFlt* z, OmFlt* x, OmFlt* y) {
    OmInt i;                            /* used in for-loop                  */
    for (i=0; i < m; ++i) {
        z[i] = x[i] + y[i];
    }
}
/*========== OmVecFma ================*//** Function [35]                    */
void OmVecFma(OmInt m, OmFlt* y, OmFlt* w1, OmFlt* x, OmFlt* w2) {
    OmInt i;                            /* used in for-loop                  */
    for (i=0; i < m; ++i) {
        y[i] = w1[i] * x[i] + w2[i] * y[i];
    }
}

/*===========================================================================*/
#endif                                  /*| #ifdef LIBOHM_C                 |*/
/*===========================================================================*/

/*====================== Part 6. Undefine Macro =============================*/

#define OMFLG_NC
#define OMFLG_RI
#define OMFLG_RH
#define OMFLG_CB
#define OMFLG_CT
#define OMFLG_LB
#define OMFLG_LT
#define OMFLG_SW
#define OMFLG_KB
#define OMFLG_KT

/*===========================================================================*/
#endif                                  /*| #ifndef LIBOHM_H                |*/
/*===========================================================================*/