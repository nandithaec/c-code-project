Disassembly Listing for bubble_sort
Generated From:

---  /home/nanditha/MPLABXProjects/sample_project.X/dist/default/production/bubble_sort.X/bubble_sort.c  
1:             /*
2:              * File:   bubble_sort.c
3:              * Author: nanditha
4:              *
5:              * Created on 24 May, 2012, 9:45 AM
6:              */
7:             
8:             
9:             #include "pic.h"
10:            
11:            int main(void) {
12:            
13:                int array[10], n, c, d, swap;
14:            
15:             // printf("Enter number of elements\n");
16:             // scanf("%d", &n);
17:            
18:             // printf("Enter %d integers\n", n);
19:            
20:             // for (c = 0; c < n; c++)
21:             //   scanf("%d", &array[c]);
22:            	array[0]=34;
0346  3022     MOVLW 0x22
0347  1283     BCF STATUS, 0x5
0348  0093     MOVWF array
0349  3000     MOVLW 0x0
034A  0094     MOVWF 0x14
23:            	array[1]=3;
034B  3003     MOVLW 0x3
034C  0095     MOVWF 0x15
034D  3000     MOVLW 0x0
034E  0096     MOVWF 0x16
24:            	array[2]=5;
034F  3005     MOVLW 0x5
0350  0097     MOVWF 0x17
0351  3000     MOVLW 0x0
0352  0098     MOVWF 0x18
25:            	array[3]=47;
0353  302F     MOVLW 0x2F
0354  0099     MOVWF 0x19
0355  3000     MOVLW 0x0
0356  009A     MOVWF 0x1A
26:            	array[4]=9;
0357  3009     MOVLW 0x9
0358  009B     MOVWF 0x1B
0359  3000     MOVLW 0x0
035A  009C     MOVWF 0x1C
27:            
28:            
29:            for (c = 0 ; c < ( n - 1 ); c++)
035B  01AB     CLRF c
035C  01AC     CLRF 0x2C
035D  2BE8     GOTO 0x3E8
03E0  3001     MOVLW 0x1
03E1  1283     BCF STATUS, 0x5
03E2  07AB     ADDWF c, F
03E3  1803     BTFSC STATUS, 0x0
03E4  0AAC     INCF 0x2C, F
03E5  3000     MOVLW 0x0
03E6  07AC     ADDWF 0x2C, F
03E7  2BE8     GOTO 0x3E8
03E8  0829     MOVF n, W
03E9  3EFF     ADDLW 0xFF
03EA  008C     MOVWF 0xC
03EB  082A     MOVF 0x2A, W
03EC  1803     BTFSC STATUS, 0x0
03ED  3E01     ADDLW 0x1
03EE  3EFF     ADDLW 0xFF
03EF  008D     MOVWF 0xD
03F0  082C     MOVF 0x2C, W
03F1  3A80     XORLW 0x80
03F2  008E     MOVWF 0xE
03F3  080D     MOVF 0xD, W
03F4  3A80     XORLW 0x80
03F5  020E     SUBWF 0xE, W
03F6  1D03     BTFSS STATUS, 0x2
03F7  2BFA     GOTO 0x3FA
03F8  080C     MOVF 0xC, W
03F9  022B     SUBWF c, W
03FA  1C03     BTFSS STATUS, 0x0
03FB  2BFD     GOTO 0x3FD
03FC  2BFE     GOTO 0x3FE
03FD  2B5E     GOTO 0x35E
03FE  2BFF     GOTO 0x3FF
30:              {
31:                for (d = 0 ; d < n - c - 1; d++)
035E  1283     BCF STATUS, 0x5
035F  01AD     CLRF d
0360  01AE     CLRF 0x2E
0361  2BBC     GOTO 0x3BC
03B4  3001     MOVLW 0x1
03B5  1283     BCF STATUS, 0x5
03B6  07AD     ADDWF d, F
03B7  1803     BTFSC STATUS, 0x0
03B8  0AAE     INCF 0x2E, F
03B9  3000     MOVLW 0x0
03BA  07AE     ADDWF 0x2E, F
03BB  2BBC     GOTO 0x3BC
03BC  082B     MOVF c, W
03BD  3E01     ADDLW 0x1
03BE  008C     MOVWF 0xC
03BF  082C     MOVF 0x2C, W
03C0  1803     BTFSC STATUS, 0x0
03C1  3E01     ADDLW 0x1
03C2  3E00     ADDLW 0x0
03C3  008D     MOVWF 0xD
03C4  098C     COMF 0xC, F
03C5  098D     COMF 0xD, F
03C6  0A8C     INCF 0xC, F
03C7  1903     BTFSC STATUS, 0x2
03C8  0A8D     INCF 0xD, F
03C9  0829     MOVF n, W
03CA  070C     ADDWF 0xC, W
03CB  008E     MOVWF 0xE
03CC  082A     MOVF 0x2A, W
03CD  1803     BTFSC STATUS, 0x0
03CE  0A2A     INCF 0x2A, W
03CF  070D     ADDWF 0xD, W
03D0  008F     MOVWF 0xF
03D1  082E     MOVF 0x2E, W
03D2  3A80     XORLW 0x80
03D3  0090     MOVWF 0x10
03D4  080F     MOVF 0xF, W
03D5  3A80     XORLW 0x80
03D6  0210     SUBWF 0x10, W
03D7  1D03     BTFSS STATUS, 0x2
03D8  2BDB     GOTO 0x3DB
03D9  080E     MOVF 0xE, W
03DA  022D     SUBWF d, W
03DB  1C03     BTFSS STATUS, 0x0
03DC  2BDE     GOTO 0x3DE
03DD  2BDF     GOTO 0x3DF
03DE  2B62     GOTO 0x362
03DF  2BE0     GOTO 0x3E0
32:                {
33:                  if (array[d] > array[d+1]) /* For decreasing order use < */
0362  1283     BCF STATUS, 0x5
0363  082D     MOVF d, W
0364  008C     MOVWF 0xC
0365  070C     ADDWF 0xC, W
0366  3E13     ADDLW 0x13
0367  0084     MOVWF FSR
0368  1383     BCF STATUS, 0x7
0369  0800     MOVF INDF, W
036A  008D     MOVWF 0xD
036B  0A84     INCF FSR, F
036C  0800     MOVF INDF, W
036D  008E     MOVWF 0xE
036E  082D     MOVF d, W
036F  008F     MOVWF 0xF
0370  070F     ADDWF 0xF, W
0371  3E02     ADDLW 0x2
0372  3E13     ADDLW 0x13
0373  0084     MOVWF FSR
0374  0800     MOVF INDF, W
0375  0090     MOVWF 0x10
0376  0A84     INCF FSR, F
0377  0800     MOVF INDF, W
0378  0091     MOVWF 0x11
0379  0811     MOVF 0x11, W
037A  3A80     XORLW 0x80
037B  0092     MOVWF 0x12
037C  080E     MOVF 0xE, W
037D  3A80     XORLW 0x80
037E  0212     SUBWF 0x12, W
037F  1D03     BTFSS STATUS, 0x2
0380  2B83     GOTO 0x383
0381  080D     MOVF 0xD, W
0382  0210     SUBWF 0x10, W
0383  1803     BTFSC STATUS, 0x0
0384  2B86     GOTO 0x386
0385  2B87     GOTO 0x387
0386  2BB4     GOTO 0x3B4
34:                  {
35:                    swap       = array[d];
0387  1283     BCF STATUS, 0x5
0388  082D     MOVF d, W
0389  008C     MOVWF 0xC
038A  070C     ADDWF 0xC, W
038B  3E13     ADDLW 0x13
038C  0084     MOVWF FSR
038D  1383     BCF STATUS, 0x7
038E  0800     MOVF INDF, W
038F  00A7     MOVWF swap
0390  0A84     INCF FSR, F
0391  0800     MOVF INDF, W
0392  00A8     MOVWF 0x28
36:                    array[d]   = array[d+1];
0393  082D     MOVF d, W
0394  008C     MOVWF 0xC
0395  070C     ADDWF 0xC, W
0396  3E02     ADDLW 0x2
0397  3E13     ADDLW 0x13
0398  0084     MOVWF FSR
0399  0800     MOVF INDF, W
039A  008D     MOVWF 0xD
039B  0A84     INCF FSR, F
039C  0800     MOVF INDF, W
039D  008E     MOVWF 0xE
039E  082D     MOVF d, W
039F  008F     MOVWF 0xF
03A0  070F     ADDWF 0xF, W
03A1  3E13     ADDLW 0x13
03A2  0084     MOVWF FSR
03A3  080D     MOVF 0xD, W
03A4  0080     MOVWF INDF
03A5  0A84     INCF FSR, F
03A6  080E     MOVF 0xE, W
03A7  0080     MOVWF INDF
37:                    array[d+1] = swap;
03A8  082D     MOVF d, W
03A9  008C     MOVWF 0xC
03AA  070C     ADDWF 0xC, W
03AB  3E02     ADDLW 0x2
03AC  3E13     ADDLW 0x13
03AD  0084     MOVWF FSR
03AE  0827     MOVF swap, W
03AF  0080     MOVWF INDF
03B0  0A84     INCF FSR, F
03B1  0828     MOVF 0x28, W
03B2  0080     MOVWF INDF
03B3  2BB4     GOTO 0x3B4
38:                  }
39:                }
40:              }
41:            
42:             // printf("Sorted list in ascending order:\n");
43:            
44:             // for ( c = 0 ; c < n ; c++ )
45:             //    printf("%d\n", array[c]);
46:                    
47:                return 0;
48:            }

