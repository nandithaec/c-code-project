Disassembly Listing for sample_array
Generated From:

---  /usr/hitech/picc/9.82/sources/wmul.c  --------------------------------------------------------------
1:             unsigned int
2:             __wmul(unsigned int multiplier, unsigned int multiplicand)
3:             {
4:             	unsigned int	product = 0;
0391  1283     BCF STATUS, 0x5
0392  0190     CLRF product
0393  0191     CLRF 0x11
0394  2B95     GOTO 0x395
5:             
6:             	do {
7:             		if(multiplier & 1)
0395  1C0C     BTFSS multiplier, 0x0
0396  2B98     GOTO 0x398
0397  2B99     GOTO 0x399
0398  2B9F     GOTO 0x39F
8:             			product += multiplicand;
0399  080E     MOVF multiplicand, W
039A  0790     ADDWF product, F
039B  1803     BTFSC STATUS, 0x0
039C  0A91     INCF 0x11, F
039D  080F     MOVF 0xF, W
039E  0791     ADDWF 0x11, F
9:             		multiplicand <<= 1;
039F  3001     MOVLW 0x1
03A0  1003     BCF STATUS, 0x0
03A1  0D8E     RLF multiplicand, F
03A2  0D8F     RLF 0xF, F
03A3  3EFF     ADDLW 0xFF
03A4  1D03     BTFSS STATUS, 0x2
03A5  2BA0     GOTO 0x3A0
10:            		multiplier >>= 1;
03A6  3001     MOVLW 0x1
03A7  1003     BCF STATUS, 0x0
03A8  0C8D     RRF 0xD, F
03A9  0C8C     RRF multiplier, F
03AA  3EFF     ADDLW 0xFF
03AB  1D03     BTFSS STATUS, 0x2
03AC  2BA7     GOTO 0x3A7
11:            	} while(multiplier != 0);
03AD  080D     MOVF 0xD, W
03AE  040C     IORWF multiplier, W
03AF  1D03     BTFSS STATUS, 0x2
03B0  2BB2     GOTO 0x3B2
03B1  2BB3     GOTO 0x3B3
03B2  2B95     GOTO 0x395
03B3  2BB4     GOTO 0x3B4
12:            	return product;
03B4  0811     MOVF 0x11, W
03B5  018D     CLRF 0xD
03B6  078D     ADDWF 0xD, F
03B7  0810     MOVF product, W
03B8  018C     CLRF multiplier
03B9  078C     ADDWF multiplier, F
03BA  2BBB     GOTO 0x3BB
13:            }
03BB  0008     RETURN
---  /home/nanditha/MPLABXProjects/sample_project.X/dist/default/production/sample_array.X/array_ex.c  --
1:             /*
2:              * File:   array_ex.c
3:              * Author: nanditha
4:              *
5:              * Created on 13 March, 2012, 6:38 PM
6:              */
7:             
8:             
9:             #include "pic.h"
10:            
11:            int main(void)
12:            {
13:               // int i=4, a=0;
14:                //a=i*2;
15:            
16:                int i=0, a=1;
03BC  1283     BCF STATUS, 0x5
03BD  0194     CLRF i
03BE  0195     CLRF 0x15
03BF  3001     MOVLW 0x1
03C0  0092     MOVWF a
03C1  3000     MOVLW 0x0
03C2  0093     MOVWF 0x13
17:            
18:                for(i=1;i<5;i++)
03C3  3001     MOVLW 0x1
03C4  0094     MOVWF i
03C5  3000     MOVLW 0x0
03C6  0095     MOVWF 0x15
03C7  0815     MOVF 0x15, W
03C8  3A80     XORLW 0x80
03C9  00CF     MOVWF 0x4F
03CA  3080     MOVLW 0x80
03CB  024F     SUBWF 0x4F, W
03CC  1D03     BTFSS STATUS, 0x2
03CD  2BD0     GOTO 0x3D0
03CE  3005     MOVLW 0x5
03CF  0214     SUBWF i, W
03D0  1C03     BTFSS STATUS, 0x0
03D1  2BD3     GOTO 0x3D3
03D2  2BD4     GOTO 0x3D4
03D3  2BD6     GOTO 0x3D6
03D4  2BFF     GOTO 0x3FF
03D5  2BFF     GOTO 0x3FF
03EB  3001     MOVLW 0x1
03EC  0794     ADDWF i, F
03ED  1803     BTFSC STATUS, 0x0
03EE  0A95     INCF 0x15, F
03EF  3000     MOVLW 0x0
03F0  0795     ADDWF 0x15, F
03F1  0815     MOVF 0x15, W
03F2  3A80     XORLW 0x80
03F3  00CF     MOVWF 0x4F
03F4  3080     MOVLW 0x80
03F5  024F     SUBWF 0x4F, W
03F6  1D03     BTFSS STATUS, 0x2
03F7  2BFA     GOTO 0x3FA
03F8  3005     MOVLW 0x5
03F9  0214     SUBWF i, W
03FA  1C03     BTFSS STATUS, 0x0
03FB  2BFD     GOTO 0x3FD
03FC  2BFE     GOTO 0x3FE
03FD  2BD6     GOTO 0x3D6
03FE  2BFF     GOTO 0x3FF
19:                    a=a*i;
03D6  1283     BCF STATUS, 0x5
03D7  0813     MOVF 0x13, W
03D8  018D     CLRF 0xD
03D9  078D     ADDWF 0xD, F
03DA  0812     MOVF a, W
03DB  018C     CLRF multiplier
03DC  078C     ADDWF multiplier, F
03DD  0815     MOVF 0x15, W
03DE  018F     CLRF 0xF
03DF  078F     ADDWF 0xF, F
03E0  0814     MOVF i, W
03E1  018E     CLRF multiplicand
03E2  078E     ADDWF multiplicand, F
03E3  2391     CALL 0x391
03E4  1283     BCF STATUS, 0x5
03E5  080D     MOVF 0xD, W
03E6  0193     CLRF 0x13
03E7  0793     ADDWF 0x13, F
03E8  080C     MOVF multiplier, W
03E9  0192     CLRF a
03EA  0792     ADDWF a, F
20:                return 0; 
21:            }

