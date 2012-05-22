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
13:                int i=4, a=0;
03E8  3004     MOVLW 0x4
03E9  1283     BCF STATUS, 0x5
03EA  0090     MOVWF i
03EB  3000     MOVLW 0x0
03EC  0091     MOVWF 0x11
03ED  018E     CLRF a
03EE  018F     CLRF 0xF
14:                a=i*2;
03EF  0811     MOVF 0x11, W
03F0  008D     MOVWF 0xD
03F1  0810     MOVF i, W
03F2  008C     MOVWF 0xC
03F3  3001     MOVLW 0x1
03F4  00CF     MOVWF 0x4F
03F5  1003     BCF STATUS, 0x0
03F6  0D8C     RLF 0xC, F
03F7  0D8D     RLF 0xD, F
03F8  0BCF     DECFSZ 0x4F, F
03F9  2BF5     GOTO 0x3F5
03FA  080C     MOVF 0xC, W
03FB  008E     MOVWF a
03FC  080D     MOVF 0xD, W
03FD  008F     MOVWF 0xF
03FE  2BFF     GOTO 0x3FF
15:                return 0; 
16:            }

