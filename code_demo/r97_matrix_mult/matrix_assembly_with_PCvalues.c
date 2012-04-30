Disassembly Listing for matrix
Generated From:

---  /usr/hitech/picc/9.82/sources/wmul.c  --------------------------------------------------------------
1:             unsigned int
2:             __wmul(unsigned int multiplier, unsigned int multiplicand)
3:             {
4:             	unsigned int	product = 0;
0274  1283     BCF STATUS, 0x5
0275  0190     CLRF product
0276  0191     CLRF 0x11
0277  2A78     GOTO 0x278
5:             
6:             	do {
7:             		if(multiplier & 1)
0278  1C0C     BTFSS multiplier, 0x0
0279  2A7B     GOTO 0x27B
027A  2A7C     GOTO 0x27C
027B  2A82     GOTO 0x282
8:             			product += multiplicand;
027C  080E     MOVF multiplicand, W
027D  0790     ADDWF product, F
027E  1803     BTFSC STATUS, 0x0
027F  0A91     INCF 0x11, F
0280  080F     MOVF 0xF, W
0281  0791     ADDWF 0x11, F
9:             		multiplicand <<= 1;
0282  3001     MOVLW 0x1
0283  1003     BCF STATUS, 0x0
0284  0D8E     RLF multiplicand, F
0285  0D8F     RLF 0xF, F
0286  3EFF     ADDLW 0xFF
0287  1D03     BTFSS STATUS, 0x2
0288  2A83     GOTO 0x283
10:            		multiplier >>= 1;
0289  3001     MOVLW 0x1
028A  1003     BCF STATUS, 0x0
028B  0C8D     RRF 0xD, F
028C  0C8C     RRF multiplier, F
028D  3EFF     ADDLW 0xFF
028E  1D03     BTFSS STATUS, 0x2
028F  2A8A     GOTO 0x28A
11:            	} while(multiplier != 0);
0290  080D     MOVF 0xD, W
0291  040C     IORWF multiplier, W
0292  1D03     BTFSS STATUS, 0x2
0293  2A95     GOTO 0x295
0294  2A96     GOTO 0x296
0295  2A78     GOTO 0x278
0296  2A97     GOTO 0x297
12:            	return product;
0297  0811     MOVF 0x11, W
0298  018D     CLRF 0xD
0299  078D     ADDWF 0xD, F
029A  0810     MOVF product, W
029B  018C     CLRF multiplier
029C  078C     ADDWF multiplier, F
029D  2A9E     GOTO 0x29E
13:            }
029E  0008     RETURN
---  /home/nanditha/MPLABXProjects/sample_project.X/dist/default/production/matrix.X/matrix_mult.c  -----
1:             /*
2:              * File:   matrix_mult.c
3:              * Author: nanditha
4:              *
5:              * Created on 15 March, 2012, 12:31 PM
6:              */
7:             
8:             
9:             #include "pic.h"
10:            //#include<conio.h>
11:            #define MATRIX_SIZE 2
12:            
13:            
14:            int main(void) {
15:            int m1[2][2],i,j,k,m2[2][2],mult[2][2],r1,c1,r2,c2;
16:                  
17:               // printf("Enter number of rows and columns of first matrix (less than 10)\n");
18:               //scanf("%d%d",&r1,&c1);
19:               //printf("Enter number of rows and columns of second matrix (less than 10)\n");
20:               //scanf("%d%d",&r2,&c2);
21:            
22:            	//Square matrix
23:            	r1= MATRIX_SIZE;
029F  3002     MOVLW 0x2
02A0  1283     BCF STATUS, 0x5
02A1  00BC     MOVWF r1
02A2  3000     MOVLW 0x0
02A3  00BD     MOVWF 0x3D
24:            	c1= MATRIX_SIZE;
02A4  3002     MOVLW 0x2
02A5  00B6     MOVWF c1
02A6  3000     MOVLW 0x0
02A7  00B7     MOVWF 0x37
25:            	r2= MATRIX_SIZE;
02A8  3002     MOVLW 0x2
02A9  00B8     MOVWF r2
02AA  3000     MOVLW 0x0
02AB  00B9     MOVWF 0x39
26:            	c2= MATRIX_SIZE;
02AC  3002     MOVLW 0x2
02AD  00BA     MOVWF c2
02AE  3000     MOVLW 0x0
02AF  00BB     MOVWF 0x3B
27:            
28:            
29:                    for(i=0;i<r1;i++)
02B0  01C0     CLRF i
02B1  01C1     CLRF 0x41
02B2  2AED     GOTO 0x2ED
02E6  3001     MOVLW 0x1
02E7  1283     BCF STATUS, 0x5
02E8  07C0     ADDWF i, F
02E9  1803     BTFSC STATUS, 0x0
02EA  0AC1     INCF 0x41, F
02EB  3000     MOVLW 0x0
02EC  07C1     ADDWF 0x41, F
02ED  0841     MOVF 0x41, W
02EE  3A80     XORLW 0x80
02EF  0092     MOVWF 0x12
02F0  083D     MOVF 0x3D, W
02F1  3A80     XORLW 0x80
02F2  0212     SUBWF 0x12, W
02F3  1D03     BTFSS STATUS, 0x2
02F4  2AF7     GOTO 0x2F7
02F5  083C     MOVF r1, W
02F6  0240     SUBWF i, W
02F7  1C03     BTFSS STATUS, 0x0
02F8  2AFA     GOTO 0x2FA
02F9  2AFB     GOTO 0x2FB
02FA  2AB3     GOTO 0x2B3
02FB  2AFC     GOTO 0x2FC
30:                        for(j=0;j<c1;j++)
02B3  1283     BCF STATUS, 0x5
02B4  01C2     CLRF j
02B5  01C3     CLRF 0x43
02B6  2AD7     GOTO 0x2D7
02D0  3001     MOVLW 0x1
02D1  07C2     ADDWF j, F
02D2  1803     BTFSC STATUS, 0x0
02D3  0AC3     INCF 0x43, F
02D4  3000     MOVLW 0x0
02D5  07C3     ADDWF 0x43, F
02D6  2AD7     GOTO 0x2D7
02D7  0843     MOVF 0x43, W
02D8  3A80     XORLW 0x80
02D9  0092     MOVWF 0x12
02DA  0837     MOVF 0x37, W
02DB  3A80     XORLW 0x80
02DC  0212     SUBWF 0x12, W
02DD  1D03     BTFSS STATUS, 0x2
02DE  2AE1     GOTO 0x2E1
02DF  0836     MOVF c1, W
02E0  0242     SUBWF j, W
02E1  1C03     BTFSS STATUS, 0x0
02E2  2AE4     GOTO 0x2E4
02E3  2AE5     GOTO 0x2E5
02E4  2AB7     GOTO 0x2B7
02E5  2AE6     GOTO 0x2E6
31:                           //m1[i][j]= (j + 1) * (i + j + 1); //Some random number
32:                            m1[i][j]= 1; //Some random number
02B7  1283     BCF STATUS, 0x5
02B8  0840     MOVF i, W
02B9  0092     MOVWF 0x12
02BA  3002     MOVLW 0x2
02BB  1003     BCF STATUS, 0x0
02BC  0D92     RLF 0x12, F
02BD  3EFF     ADDLW 0xFF
02BE  1D03     BTFSS STATUS, 0x2
02BF  2ABB     GOTO 0x2BB
02C0  301E     MOVLW 0x1E
02C1  0712     ADDWF 0x12, W
02C2  0093     MOVWF 0x13
02C3  0842     MOVF j, W
02C4  0094     MOVWF 0x14
02C5  0714     ADDWF 0x14, W
02C6  0713     ADDWF 0x13, W
02C7  0095     MOVWF 0x15
02C8  0815     MOVF 0x15, W
02C9  0084     MOVWF FSR
02CA  3001     MOVLW 0x1
02CB  1383     BCF STATUS, 0x7
02CC  0080     MOVWF INDF
02CD  0A84     INCF FSR, F
02CE  3000     MOVLW 0x0
02CF  0080     MOVWF INDF
33:            /*     printf("First Matrix is :\n");
34:                    for(i=0;i<r1;i++)
35:                    {
36:                        for(j=0;j<c1;j++)
37:                            printf("%d\t",m1[i][j]);
38:                        printf("\n");
39:                    }*/
40:            
41:                    for(i=0;i<r2;i++)
02FC  1283     BCF STATUS, 0x5
02FD  01C0     CLRF i
02FE  01C1     CLRF 0x41
02FF  2B3A     GOTO 0x33A
0333  3001     MOVLW 0x1
0334  1283     BCF STATUS, 0x5
0335  07C0     ADDWF i, F
0336  1803     BTFSC STATUS, 0x0
0337  0AC1     INCF 0x41, F
0338  3000     MOVLW 0x0
0339  07C1     ADDWF 0x41, F
033A  0841     MOVF 0x41, W
033B  3A80     XORLW 0x80
033C  0092     MOVWF 0x12
033D  0839     MOVF 0x39, W
033E  3A80     XORLW 0x80
033F  0212     SUBWF 0x12, W
0340  1D03     BTFSS STATUS, 0x2
0341  2B44     GOTO 0x344
0342  0838     MOVF r2, W
0343  0240     SUBWF i, W
0344  1C03     BTFSS STATUS, 0x0
0345  2B47     GOTO 0x347
0346  2B48     GOTO 0x348
0347  2B00     GOTO 0x300
0348  2B49     GOTO 0x349
42:                        for(j=0;j<c2;j++)
0300  1283     BCF STATUS, 0x5
0301  01C2     CLRF j
0302  01C3     CLRF 0x43
0303  2B24     GOTO 0x324
031D  3001     MOVLW 0x1
031E  07C2     ADDWF j, F
031F  1803     BTFSC STATUS, 0x0
0320  0AC3     INCF 0x43, F
0321  3000     MOVLW 0x0
0322  07C3     ADDWF 0x43, F
0323  2B24     GOTO 0x324
0324  0843     MOVF 0x43, W
0325  3A80     XORLW 0x80
0326  0092     MOVWF 0x12
0327  083B     MOVF 0x3B, W
0328  3A80     XORLW 0x80
0329  0212     SUBWF 0x12, W
032A  1D03     BTFSS STATUS, 0x2
032B  2B2E     GOTO 0x32E
032C  083A     MOVF c2, W
032D  0242     SUBWF j, W
032E  1C03     BTFSS STATUS, 0x0
032F  2B31     GOTO 0x331
0330  2B32     GOTO 0x332
0331  2B04     GOTO 0x304
0332  2B33     GOTO 0x333
43:                             m2[i][j] = 1; //Some random number
0304  1283     BCF STATUS, 0x5
0305  0840     MOVF i, W
0306  0092     MOVWF 0x12
0307  3002     MOVLW 0x2
0308  1003     BCF STATUS, 0x0
0309  0D92     RLF 0x12, F
030A  3EFF     ADDLW 0xFF
030B  1D03     BTFSS STATUS, 0x2
030C  2B08     GOTO 0x308
030D  3026     MOVLW 0x26
030E  0712     ADDWF 0x12, W
030F  0093     MOVWF 0x13
0310  0842     MOVF j, W
0311  0094     MOVWF 0x14
0312  0714     ADDWF 0x14, W
0313  0713     ADDWF 0x13, W
0314  0095     MOVWF 0x15
0315  0815     MOVF 0x15, W
0316  0084     MOVWF FSR
0317  3001     MOVLW 0x1
0318  1383     BCF STATUS, 0x7
0319  0080     MOVWF INDF
031A  0A84     INCF FSR, F
031B  3000     MOVLW 0x0
031C  0080     MOVWF INDF
44:                    //m2[i][j] = (i + 2) * (i + j + 1); //Some random number
45:                  /*  printf("Second Matrix is:\n");
46:                    for(i=0;i<r2;i++)
47:                    {
48:                        for(j=0;j<c2;j++)
49:                            printf("%d\t",m2[i][j]);
50:                        printf("\n");
51:                    }
52:                   * */
53:                    //printf("Multiplication of the Matrices:\n");
54:                    for(i=0;i<r1;i++)
0349  1283     BCF STATUS, 0x5
034A  01C0     CLRF i
034B  01C1     CLRF 0x41
034C  2BF0     GOTO 0x3F0
03E9  3001     MOVLW 0x1
03EA  1283     BCF STATUS, 0x5
03EB  07C0     ADDWF i, F
03EC  1803     BTFSC STATUS, 0x0
03ED  0AC1     INCF 0x41, F
03EE  3000     MOVLW 0x0
03EF  07C1     ADDWF 0x41, F
03F0  0841     MOVF 0x41, W
03F1  3A80     XORLW 0x80
03F2  0092     MOVWF 0x12
03F3  083D     MOVF 0x3D, W
03F4  3A80     XORLW 0x80
03F5  0212     SUBWF 0x12, W
03F6  1D03     BTFSS STATUS, 0x2
03F7  2BFA     GOTO 0x3FA
03F8  083C     MOVF r1, W
03F9  0240     SUBWF i, W
03FA  1C03     BTFSS STATUS, 0x0
03FB  2BFD     GOTO 0x3FD
03FC  2BFE     GOTO 0x3FE
03FD  2B4D     GOTO 0x34D
03FE  2BFF     GOTO 0x3FF
55:                    {
56:                        for(j=0;j<c2;j++)
034D  1283     BCF STATUS, 0x5
034E  01C2     CLRF j
034F  01C3     CLRF 0x43
0350  2BDA     GOTO 0x3DA
03D3  3001     MOVLW 0x1
03D4  1283     BCF STATUS, 0x5
03D5  07C2     ADDWF j, F
03D6  1803     BTFSC STATUS, 0x0
03D7  0AC3     INCF 0x43, F
03D8  3000     MOVLW 0x0
03D9  07C3     ADDWF 0x43, F
03DA  0843     MOVF 0x43, W
03DB  3A80     XORLW 0x80
03DC  0092     MOVWF 0x12
03DD  083B     MOVF 0x3B, W
03DE  3A80     XORLW 0x80
03DF  0212     SUBWF 0x12, W
03E0  1D03     BTFSS STATUS, 0x2
03E1  2BE4     GOTO 0x3E4
03E2  083A     MOVF c2, W
03E3  0242     SUBWF j, W
03E4  1C03     BTFSS STATUS, 0x0
03E5  2BE7     GOTO 0x3E7
03E6  2BE8     GOTO 0x3E8
03E7  2B51     GOTO 0x351
03E8  2BE9     GOTO 0x3E9
57:                        {
58:                            mult[i][j]=0;
0351  1283     BCF STATUS, 0x5
0352  0840     MOVF i, W
0353  0092     MOVWF 0x12
0354  3002     MOVLW 0x2
0355  1003     BCF STATUS, 0x0
0356  0D92     RLF 0x12, F
0357  3EFF     ADDLW 0xFF
0358  1D03     BTFSS STATUS, 0x2
0359  2B55     GOTO 0x355
035A  302E     MOVLW 0x2E
035B  0712     ADDWF 0x12, W
035C  0093     MOVWF 0x13
035D  0842     MOVF j, W
035E  0094     MOVWF 0x14
035F  0714     ADDWF 0x14, W
0360  0713     ADDWF 0x13, W
0361  0095     MOVWF 0x15
0362  0815     MOVF 0x15, W
0363  0084     MOVWF FSR
0364  3000     MOVLW 0x0
0365  1383     BCF STATUS, 0x7
0366  0080     MOVWF INDF
0367  0A84     INCF FSR, F
0368  3000     MOVLW 0x0
0369  0080     MOVWF INDF
59:                            for(k=0;k<r1;k++)
036A  01BE     CLRF k
036B  01BF     CLRF 0x3F
036C  2BC4     GOTO 0x3C4
03BD  3001     MOVLW 0x1
03BE  07BE     ADDWF k, F
03BF  1803     BTFSC STATUS, 0x0
03C0  0ABF     INCF 0x3F, F
03C1  3000     MOVLW 0x0
03C2  07BF     ADDWF 0x3F, F
03C3  2BC4     GOTO 0x3C4
03C4  083F     MOVF 0x3F, W
03C5  3A80     XORLW 0x80
03C6  0092     MOVWF 0x12
03C7  083D     MOVF 0x3D, W
03C8  3A80     XORLW 0x80
03C9  0212     SUBWF 0x12, W
03CA  1D03     BTFSS STATUS, 0x2
03CB  2BCE     GOTO 0x3CE
03CC  083C     MOVF r1, W
03CD  023E     SUBWF k, W
03CE  1C03     BTFSS STATUS, 0x0
03CF  2BD1     GOTO 0x3D1
03D0  2BD2     GOTO 0x3D2
03D1  2B6D     GOTO 0x36D
03D2  2BD3     GOTO 0x3D3
60:                                mult[i][j]+=m1[i][k]*m2[k][j];
036D  1283     BCF STATUS, 0x5
036E  0840     MOVF i, W
036F  0092     MOVWF 0x12
0370  3002     MOVLW 0x2
0371  1003     BCF STATUS, 0x0
0372  0D92     RLF 0x12, F
0373  3EFF     ADDLW 0xFF
0374  1D03     BTFSS STATUS, 0x2
0375  2B71     GOTO 0x371
0376  301E     MOVLW 0x1E
0377  0712     ADDWF 0x12, W
0378  0093     MOVWF 0x13
0379  083E     MOVF k, W
037A  0094     MOVWF 0x14
037B  0714     ADDWF 0x14, W
037C  0713     ADDWF 0x13, W
037D  0095     MOVWF 0x15
037E  0815     MOVF 0x15, W
037F  0084     MOVWF FSR
0380  1383     BCF STATUS, 0x7
0381  0800     MOVF INDF, W
0382  008C     MOVWF multiplier
0383  0A84     INCF FSR, F
0384  0800     MOVF INDF, W
0385  008D     MOVWF 0xD
0386  083E     MOVF k, W
0387  0096     MOVWF 0x16
0388  3002     MOVLW 0x2
0389  1003     BCF STATUS, 0x0
038A  0D96     RLF 0x16, F
038B  3EFF     ADDLW 0xFF
038C  1D03     BTFSS STATUS, 0x2
038D  2B89     GOTO 0x389
038E  3026     MOVLW 0x26
038F  0716     ADDWF 0x16, W
0390  0097     MOVWF 0x17
0391  0842     MOVF j, W
0392  0098     MOVWF 0x18
0393  0718     ADDWF 0x18, W
0394  0717     ADDWF 0x17, W
0395  0099     MOVWF 0x19
0396  0819     MOVF 0x19, W
0397  0084     MOVWF FSR
0398  0800     MOVF INDF, W
0399  008E     MOVWF multiplicand
039A  0A84     INCF FSR, F
039B  0800     MOVF INDF, W
039C  008F     MOVWF 0xF
039D  2274     CALL 0x274
039E  1283     BCF STATUS, 0x5
039F  0840     MOVF i, W
03A0  009A     MOVWF 0x1A
03A1  3002     MOVLW 0x2
03A2  1003     BCF STATUS, 0x0
03A3  0D9A     RLF 0x1A, F
03A4  3EFF     ADDLW 0xFF
03A5  1D03     BTFSS STATUS, 0x2
03A6  2BA2     GOTO 0x3A2
03A7  302E     MOVLW 0x2E
03A8  071A     ADDWF 0x1A, W
03A9  009B     MOVWF 0x1B
03AA  0842     MOVF j, W
03AB  009C     MOVWF 0x1C
03AC  071C     ADDWF 0x1C, W
03AD  071B     ADDWF 0x1B, W
03AE  009D     MOVWF 0x1D
03AF  081D     MOVF 0x1D, W
03B0  0084     MOVWF FSR
03B1  080C     MOVF multiplier, W
03B2  1383     BCF STATUS, 0x7
03B3  0780     ADDWF INDF, F
03B4  0F84     INCFSZ FSR, F
03B5  0800     MOVF INDF, W
03B6  1803     BTFSC STATUS, 0x0
03B7  0A00     INCF INDF, W
03B8  00CF     MOVWF 0x4F
03B9  080D     MOVF 0xD, W
03BA  074F     ADDWF 0x4F, W
03BB  0080     MOVWF INDF
03BC  0384     DECF FSR, F
61:                      //      printf("%d\t",mult[i][j]);
62:                        }
63:                        //printf("\n");
64:                    }
65:            
66:            
67:                return 0;
68:            
69:                }

