# LibOhm
LibOhm is a lightweight real-time circuit simulation library.  
-------------------------------------------------------------------------------
Features:  
- C89/C90 standard compatible  
- No other dependency  
-------------------------------------------------------------------------------
Type of Branch: (10)  
00  | OMTYP_UN | Branch type is unknown  
01  | OMTYP_X0 | Branch can contain X/E/H  
02  | OMTYP_X1 | Branch can contain X/E/H/V  
03  | OMTYP_X2 | Branch can contain X/E/H/V/L or X/E/H/V/Q  
04  | OMTYP_X3 | Branch can contain X/E/H/V/L/M or X/E/H/V/Q/A  
05  | OMTYP_Y0 | Branch can contain Y/F/G  
06  | OMTYP_Y1 | Branch can contain Y/F/G/I  
07  | OMTYP_Y2 | Branch can contain Y/F/G/I/C or Y/F/G/I/P  
08  | OMTYP_Y3 | Branch can contain Y/F/G/I/C/N or Y/F/G/I/P/B  
09  | OMTYP_SW | Branch can contain Y/F/G/I/S  
Method of ODE: (2)  
+x  | OMMTD_TR | Trapezoidal rule  
-x  | OMMTD_BE | Back Euler rule  
For example: -3 means YC branch with TR rule  
Branch Element Group: (17)  
|  X   Y  | Resistor  
|  V   I  | Independent Source  
| E H G F | Linear Controlled Source  
| L M C N | Differential Controlled Source  
| Q A P B | Integral Controlled Source  
|    S    | Switch  
1. B/U is 1-based  
2. node is 1-based, node 0 is always ground  
-------------------------------------------------------------------------------
OmCir Members: (31)  
|============ General Info ===========|  
| No | Type  | Name   | Size  | Init  |  
| 00   OmInt   numN      1      (0)   | Number of nodes (excluding GND)  
| 01   OmInt   numB      1      (0)   | Number of branches  
| 02   OmInt   numM      1      (0)   | Number of meters  
| 03   OmInt   numX      1      (0)   | Number of X-type branches  
| 04   OmInt   numC      1      (0)   | Number of branches after cutting  
| 05   OmFlt   timStp    1      (1.0) | Simulation time step  
|============= Setup Info ============|  
| No | Type  | Name   | Size  | Init  |  
| 06   OmInt   vecBn1   [b]     (-1)  | Node 1 of branch (0-based) (-1 GND)  
| 07   OmInt   vecBn2   [b]     (-1)  | Node 2 of branch (0-based) (-1 GND)  
| 08   OmInt   vecMn1   [m]     (-1)  | Node 1 / branch of meter (0-based) (-1 GND)  
| 09   OmInt   vecMn2   [m]     (-1)  | Node 2 of meter (0-based) (-1 GND -2 ammeter)  
| 10   OmFlt   matPa    [b,b]   (0.0) | Associated source update matrix  
| 11   OmFlt   matPb    [b,b]   (0.0) | Branch conductance / resistor matrix  
|============= Reset Info ============|  
| No | Type  | Name   | Size  | Init  |  
| 12   OmInt   vecBtm   [b]     (0)   | Type and method of branch  
| 13   OmInt   vecLut   [b]     (1)   | Lookup table for branches (0-based) (0...-x is X-type, 1 is Y-type)  
| 14   OmFlt   vecW1c   [b]     (0.0) | Weight of Xc (closed)  
| 15   OmFlt   vecW2c   [b]     (0.0) | Weight of Qa (closed)  
| 16   OmFlt   vecW1o   [b]     (0.0) | Weight of Xc (open)  
| 17   OmFlt   vecW2o   [b]     (0.0) | Weight of Qa (open)  
| 18   OmFlt   vecQa0   [b]     (0.0) | Initial value of Qa  
| 19   OmFlt   vecQs0   [b]     (0.0) | Initial value of Qs  
|============ Runtime Info ===========|  
| No | Type  | Name   | Size  | Init  |  
| 20   OmFlt   matC     [c,c]   (x)   | Matrix C to calculate Xc  
| 21   OmFlt   matD     [m,c]   (x)   | Matrix D to calculate Xm  
| 22   OmFlt   vecW1m   [c]     (x)   | Vector W1 used in UpdCr()  
| 23   OmFlt   vecW2m   [c]     (x)   | Vector W2 used in UpdCr()  
| 24   OmFlt   vecW1s   [c]     (x)   | Vector W1 used in UpdSw()  
| 25   OmFlt   vecW2s   [c]     (x)   | Vector W2 used in UpdSw()  
| 26   OmFlt   vecQa    [c]     (x)   | Vector Qa, associated source  
| 27   OmFlt   vecQs    [c]     (x)   | Vector Qs, independent source  
| 28   OmFlt   vecQtp   [c]     (x)   | Vector Qtp = Qs + Qa  
| 29   OmFlt   vecXm    [m]     (x)   | Vector Xm, value measured by meters  
| 30   OmFlt   vecXc    [c]     (x)   | Vector Xc, used for updating Qa  
-------------------------------------------------------------------------------
API List: Functions (36)  
|======================== API Functions (36) =========================================================|  
| No | Ret   | Name    | Parameters                                                                   |  
| 00   void    OmDelete  (OmCir* cr)                                                                  |  
| 01   OmCir*  OmCreate  (OmInt n, OmInt b, OmInt m, OmFlt stp)                                       |  
| 02   void    OmStamp   (OmCir* cr)                                                                  |  
| 03   void    OmReset   (OmCir* cr)                                                                  |  
| 04   void    OmUpdSw   (OmCir* cr)                                                                  |  
| 05   void    OmUpdCr   (OmCir* cr)                                                                  |  
| 06   void    OmUpdMt   (OmCir* cr)                                                                  |  
| 07   OmFlt   OmGetMt   (OmCir* cr, OmInt mt)                                                        |  
| 08   OmFlt   OmGetXc   (OmCir* cr, OmInt br)                                                        |  
| 09   void    OmSetQs   (OmCir* cr, OmInt br, OmInt x)                                               |  
| 10   void    OmSetSw   (OmCir* cr, OmInt bs, OmInt s)                                               |  
| 11   void    OmBran    (OmCir* cr, OmInt br, OmInt n1, OmInt n2, OmInt tm)                          |  
| 12   void    OmMetV    (OmCir* cr, OmInt mt, OmInt n1, OmInt n2)                                    |  
| 13   void    OmMetA    (OmCir* cr, OmInt mt, OmInt br)                                              |  
| 14   void    OmAddX    (OmCir* cr, OmInt bx, OmFlt res)                                             |  
| 15   void    OmAddY    (OmCir* cr, OmInt by, OmFlt con)                                             |  
| 16   void    OmAddV    (OmCir* cr, OmInt bx, OmFlt vol)                                             |  
| 17   void    OmAddI    (OmCir* cr, OmInt by, OmFlt cur)                                             |  
| 18   void    OmAddL    (OmCir* cr, OmInt bx, OmFlt ind, OmFlt i0)                                   |  
| 19   void    OmAddC    (OmCir* cr, OmInt by, OmFlt cap, OmFlt v0)                                   |  
| 20   void    OmAddQ    (OmCir* cr, OmInt bx, OmFlt rpc, OmFlt v0)                                   |  
| 21   void    OmAddP    (OmCir* cr, OmInt by, OmFlt rpi, OmFlt i0)                                   |  
| 22   void    OmAddE    (OmCir* cr, OmInt bx, OmInt cy, OmFlt k)                                     |  
| 23   void    OmAddH    (OmCir* cr, OmInt bx, OmInt cx, OmFlt k)                                     |  
| 24   void    OmAddF    (OmCir* cr, OmInt by, OmInt cx, OmFlt k)                                     |  
| 25   void    OmAddG    (OmCir* cr, OmInt by, OmInt cy, OmFlt k)                                     |  
| 26   void    OmAddM    (OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt ic0)                          |  
| 27   void    OmAddN    (OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt vc0)                          |  
| 28   void    OmAddA    (OmCir* cr, OmInt bx, OmInt cx, OmFlt k, OmFlt v0)                           |  
| 29   void    OmAddB    (OmCir* cr, OmInt by, OmInt cy, OmFlt k, OmFlt i0)                           |  
| 30   void    OmAddS    (OmCir* cr, OmInt bs, OmFlt k1, OmFlt k2, OmFlt ysw, OmFlt ron)              |  
| 31   void    OmMatInv  (OmInt m, OmFlt* a)                                                          |  
| 32   void    OmMatMul  (OmInt m, OmFlt* c, OmFlt* a, OmFlt* b)                                      |  
| 33   void    OmVecMul  (OmInt m, OmInt n, OmFlt* y, OmFlt* a, OmFlt* x)                             |  
| 34   void    OmVecAdd  (OmInt m, OmFlt* z, OmFlt* x, OmFlt* y)                                      |  
| 35   void    OmVecFma  (OmInt m, OmFlt* y, OmFlt* w1, OmFlt* x, OmFlt* w2)                          |  
-------------------------------------------------------------------------------
