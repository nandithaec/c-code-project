Disassembly Listing for sample_array
Generated From:

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
16:                int i=0, a=0;
03CC  1283     BCF STATUS, 0x5
03CD  018E     CLRF i
03CE  018F     CLRF 0xF
03CF  018C     CLRF a
03D0  018D     CLRF 0xD
17:            
18:                for(i=0;i<5;i++)
03D1  018E     CLRF i
03D2  018F     CLRF 0xF
03D3  080F     MOVF 0xF, W
03D4  3A80     XORLW 0x80
03D5  00CF     MOVWF 0x4F
03D6  3080     MOVLW 0x80
03D7  024F     SUBWF 0x4F, W
03D8  1D03     BTFSS STATUS, 0x2
03D9  2BDC     GOTO 0x3DC
03DA  3005     MOVLW 0x5
03DB  020E     SUBWF i, W
03DC  1C03     BTFSS STATUS, 0x0
03DD  2BDF     GOTO 0x3DF
03DE  2BE0     GOTO 0x3E0
03DF  2BE2     GOTO 0x3E2
03E0  2BFF     GOTO 0x3FF
03E1  2BFF     GOTO 0x3FF
03EB  3001     MOVLW 0x1
03EC  078E     ADDWF i, F
03ED  1803     BTFSC STATUS, 0x0
03EE  0A8F     INCF 0xF, F
03EF  3000     MOVLW 0x0
03F0  078F     ADDWF 0xF, F
03F1  080F     MOVF 0xF, W
03F2  3A80     XORLW 0x80
03F3  00CF     MOVWF 0x4F
03F4  3080     MOVLW 0x80
03F5  024F     SUBWF 0x4F, W
03F6  1D03     BTFSS STATUS, 0x2
03F7  2BFA     GOTO 0x3FA
03F8  3005     MOVLW 0x5
03F9  020E     SUBWF i, W
03FA  1C03     BTFSS STATUS, 0x0
03FB  2BFD     GOTO 0x3FD
03FC  2BFE     GOTO 0x3FE
03FD  2BE2     GOTO 0x3E2
03FE  2BFF     GOTO 0x3FF
19:                    a=a+i;
03E2  1283     BCF STATUS, 0x5
03E3  080E     MOVF i, W
03E4  070C     ADDWF a, W
03E5  008C     MOVWF a
03E6  080F     MOVF 0xF, W
03E7  1803     BTFSC STATUS, 0x0
03E8  0A0F     INCF 0xF, W
03E9  070D     ADDWF 0xD, W
03EA  008D     MOVWF 0xD
20:                return 0; 
21:            }

