Disassembly Listing for sample_array
Generated From:

---  /usr/hitech/picc/9.82/sources/wmul.c  --------------------------------------------------------------
1:             unsigned int
2:             __wmul(unsigned int multiplier, unsigned int multiplicand)
3:             {
4:             	unsigned int	product = 0;
0388  1283     BCF STATUS, 0x5
0389  0190     CLRF product
038A  0191     CLRF 0x11
038B  2B8C     GOTO 0x38C
5:             
6:             	do {
7:             		if(multiplier & 1)
038C  1C0C     BTFSS multiplier, 0x0
038D  2B8F     GOTO 0x38F
038E  2B90     GOTO 0x390
038F  2B96     GOTO 0x396
8:             			product += multiplicand;
0390  080E     MOVF multiplicand, W
0391  0790     ADDWF product, F
0392  1803     BTFSC STATUS, 0x0
0393  0A91     INCF 0x11, F
0394  080F     MOVF 0xF, W
0395  0791     ADDWF 0x11, F
9:             		multiplicand <<= 1;
0396  3001     MOVLW 0x1
0397  1003     BCF STATUS, 0x0
0398  0D8E     RLF multiplicand, F
0399  0D8F     RLF 0xF, F
039A  3EFF     ADDLW 0xFF
039B  1D03     BTFSS STATUS, 0x2
039C  2B97     GOTO 0x397
10:            		multiplier >>= 1;
039D  3001     MOVLW 0x1
039E  1003     BCF STATUS, 0x0
039F  0C8D     RRF 0xD, F
03A0  0C8C     RRF multiplier, F
03A1  3EFF     ADDLW 0xFF
03A2  1D03     BTFSS STATUS, 0x2
03A3  2B9E     GOTO 0x39E
11:            	} while(multiplier != 0);
03A4  080D     MOVF 0xD, W
03A5  040C     IORWF multiplier, W
03A6  1D03     BTFSS STATUS, 0x2
03A7  2BA9     GOTO 0x3A9
03A8  2BAA     GOTO 0x3AA
03A9  2B8C     GOTO 0x38C
03AA  2BAB     GOTO 0x3AB
12:            	return product;
03AB  0811     MOVF 0x11, W
03AC  018D     CLRF 0xD
03AD  078D     ADDWF 0xD, F
03AE  0810     MOVF product, W
03AF  018C     CLRF multiplier
03B0  078C     ADDWF multiplier, F
03B1  2BB2     GOTO 0x3B2
13:            }
03B2  0008     RETURN
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
16:                int i, a[5];
17:                a[0]=1;
03B3  3001     MOVLW 0x1
03B4  1283     BCF STATUS, 0x5
03B5  0094     MOVWF a
03B6  3000     MOVLW 0x0
03B7  0095     MOVWF 0x15
18:                
19:            
20:                for(i=1;i<5;i++)
03B8  3001     MOVLW 0x1
03B9  009E     MOVWF i
03BA  3000     MOVLW 0x0
03BB  009F     MOVWF 0x1F
03BC  081F     MOVF 0x1F, W
03BD  3A80     XORLW 0x80
03BE  00CF     MOVWF 0x4F
03BF  3080     MOVLW 0x80
03C0  024F     SUBWF 0x4F, W
03C1  1D03     BTFSS STATUS, 0x2
03C2  2BC5     GOTO 0x3C5
03C3  3005     MOVLW 0x5
03C4  021E     SUBWF i, W
03C5  1C03     BTFSS STATUS, 0x0
03C6  2BC8     GOTO 0x3C8
03C7  2BC9     GOTO 0x3C9
03C8  2BCB     GOTO 0x3CB
03C9  2BFF     GOTO 0x3FF
03CA  2BFF     GOTO 0x3FF
03EB  3001     MOVLW 0x1
03EC  079E     ADDWF i, F
03ED  1803     BTFSC STATUS, 0x0
03EE  0A9F     INCF 0x1F, F
03EF  3000     MOVLW 0x0
03F0  079F     ADDWF 0x1F, F
03F1  081F     MOVF 0x1F, W
03F2  3A80     XORLW 0x80
03F3  00CF     MOVWF 0x4F
03F4  3080     MOVLW 0x80
03F5  024F     SUBWF 0x4F, W
03F6  1D03     BTFSS STATUS, 0x2
03F7  2BFA     GOTO 0x3FA
03F8  3005     MOVLW 0x5
03F9  021E     SUBWF i, W
03FA  1C03     BTFSS STATUS, 0x0
03FB  2BFD     GOTO 0x3FD
03FC  2BFE     GOTO 0x3FE
03FD  2BCB     GOTO 0x3CB
03FE  2BFF     GOTO 0x3FF
21:                    a[i]=a[i-1]*i;
03CB  1283     BCF STATUS, 0x5
03CC  081E     MOVF i, W
03CD  0092     MOVWF 0x12
03CE  0712     ADDWF 0x12, W
03CF  3EFE     ADDLW 0xFE
03D0  3E14     ADDLW 0x14
03D1  0084     MOVWF FSR
03D2  1383     BCF STATUS, 0x7
03D3  0800     MOVF INDF, W
03D4  008C     MOVWF multiplier
03D5  0A84     INCF FSR, F
03D6  0800     MOVF INDF, W
03D7  008D     MOVWF 0xD
03D8  081F     MOVF 0x1F, W
03D9  018F     CLRF 0xF
03DA  078F     ADDWF 0xF, F
03DB  081E     MOVF i, W
03DC  018E     CLRF multiplicand
03DD  078E     ADDWF multiplicand, F
03DE  2388     CALL 0x388
03DF  1283     BCF STATUS, 0x5
03E0  081E     MOVF i, W
03E1  0093     MOVWF 0x13
03E2  0713     ADDWF 0x13, W
03E3  3E14     ADDLW 0x14
03E4  0084     MOVWF FSR
03E5  080C     MOVF multiplier, W
03E6  1383     BCF STATUS, 0x7
03E7  0080     MOVWF INDF
03E8  0A84     INCF FSR, F
03E9  080D     MOVF 0xD, W
03EA  0080     MOVWF INDF
22:                return 0; 
23:            }

