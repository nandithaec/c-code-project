Disassembly Listing for matrix
Generated From:

---  /usr/hitech/picc/9.82/sources/wmul.c  --------------------------------------------------------------
1:             unsigned int
2:             __wmul(unsigned int multiplier, unsigned int multiplicand)
3:             {
4:             	unsigned int	product = 0;
02EE  1283     BCF STATUS, 0x5
02EF  0190     CLRF product
02F0  0191     CLRF 0x11
02F1  2AF2     GOTO 0x2F2
5:             
6:             	do {
7:             		if(multiplier & 1)
02F2  1C0C     BTFSS multiplier, 0x0
02F3  2AF5     GOTO 0x2F5
02F4  2AF6     GOTO 0x2F6
02F5  2AFC     GOTO 0x2FC
8:             			product += multiplicand;
02F6  080E     MOVF multiplicand, W
02F7  0790     ADDWF product, F
02F8  1803     BTFSC STATUS, 0x0
02F9  0A91     INCF 0x11, F
02FA  080F     MOVF 0xF, W
02FB  0791     ADDWF 0x11, F
9:             		multiplicand <<= 1;
02FC  3001     MOVLW 0x1
02FD  1003     BCF STATUS, 0x0
02FE  0D8E     RLF multiplicand, F
02FF  0D8F     RLF 0xF, F
0300  3EFF     ADDLW 0xFF
0301  1D03     BTFSS STATUS, 0x2
0302  2AFD     GOTO 0x2FD
10:            		multiplier >>= 1;
0303  3001     MOVLW 0x1
0304  1003     BCF STATUS, 0x0
0305  0C8D     RRF 0xD, F
0306  0C8C     RRF multiplier, F
0307  3EFF     ADDLW 0xFF
0308  1D03     BTFSS STATUS, 0x2
0309  2B04     GOTO 0x304
11:            	} while(multiplier != 0);
030A  080D     MOVF 0xD, W
030B  040C     IORWF multiplier, W
030C  1D03     BTFSS STATUS, 0x2
030D  2B0F     GOTO 0x30F
030E  2B10     GOTO 0x310
030F  2AF2     GOTO 0x2F2
0310  2B11     GOTO 0x311
12:            	return product;
0311  0811     MOVF 0x11, W
0312  018D     CLRF 0xD
0313  078D     ADDWF 0xD, F
0314  0810     MOVF product, W
0315  018C     CLRF multiplier
0316  078C     ADDWF multiplier, F
0317  2B18     GOTO 0x318
13:            }
0318  0008     RETURN
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
0319  3002     MOVLW 0x2
031A  1283     BCF STATUS, 0x5
031B  00BC     MOVWF r1
031C  3000     MOVLW 0x0
031D  00BD     MOVWF 0x3D
24:            	c1= MATRIX_SIZE;
031E  3002     MOVLW 0x2
031F  009E     MOVWF c1
0320  3000     MOVLW 0x0
0321  009F     MOVWF 0x1F
25:            	r2= MATRIX_SIZE;
0322  3002     MOVLW 0x2
0323  00A0     MOVWF r2
0324  3000     MOVLW 0x0
0325  00A1     MOVWF 0x21
26:            	c2= MATRIX_SIZE;
0326  3002     MOVLW 0x2
0327  00AA     MOVWF c2
0328  3000     MOVLW 0x0
0329  00AB     MOVWF 0x2B
27:            
28:            
29:                //    for(i=0;i<r1;i++)
30:                  //      for(j=0;j<c1;j++)
31:                           
32:                    //        m1[i][j]= 1; //Some random number
33:            
34:                    m1[0][0]=1;
032A  3001     MOVLW 0x1
032B  00AC     MOVWF m1
032C  3000     MOVLW 0x0
032D  00AD     MOVWF 0x2D
35:                    m1[0][1]=2;
032E  3002     MOVLW 0x2
032F  00AE     MOVWF 0x2E
0330  3000     MOVLW 0x0
0331  00AF     MOVWF 0x2F
36:                    m1[1][0]=3;
0332  3003     MOVLW 0x3
0333  00B0     MOVWF 0x30
0334  3000     MOVLW 0x0
0335  00B1     MOVWF 0x31
37:                    m1[1][1]=4;
0336  3004     MOVLW 0x4
0337  00B2     MOVWF 0x32
0338  3000     MOVLW 0x0
0339  00B3     MOVWF 0x33
38:                    //m1[i][j]= (j + 1) * (i + j + 1); //Some random number
39:            /*     printf("First Matrix is :\n");
40:                    for(i=0;i<r1;i++)
41:                    {
42:                        for(j=0;j<c1;j++)
43:                            printf("%d\t",m1[i][j]);
44:                        printf("\n");
45:                    }*/
46:            
47:                  //  for(i=0;i<r2;i++)
48:                    //    for(j=0;j<c2;j++)
49:                      //       m2[i][j] = 3; //Some random number
50:                    m2[0][0]=3;
033A  3003     MOVLW 0x3
033B  00B4     MOVWF m2
033C  3000     MOVLW 0x0
033D  00B5     MOVWF 0x35
51:                    m2[0][1]=4;
033E  3004     MOVLW 0x4
033F  00B6     MOVWF 0x36
0340  3000     MOVLW 0x0
0341  00B7     MOVWF 0x37
52:                    m2[1][0]=5;
0342  3005     MOVLW 0x5
0343  00B8     MOVWF 0x38
0344  3000     MOVLW 0x0
0345  00B9     MOVWF 0x39
53:                    m2[1][1]=6;
0346  3006     MOVLW 0x6
0347  00BA     MOVWF 0x3A
0348  3000     MOVLW 0x0
0349  00BB     MOVWF 0x3B
54:                    //m2[i][j] = (i + 2) * (i + j + 1); //Some random number
55:                  /*  printf("Second Matrix is:\n");
56:                    for(i=0;i<r2;i++)
57:                    {
58:                        for(j=0;j<c2;j++)
59:                            printf("%d\t",m2[i][j]);
60:                        printf("\n");
61:                    }
62:                   * */
63:                    //printf("Multiplication of the Matrices:\n");
64:                    for(i=0;i<r1;i++)
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
65:                    {
66:                        for(j=0;j<c2;j++)
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
03DD  082B     MOVF 0x2B, W
03DE  3A80     XORLW 0x80
03DF  0212     SUBWF 0x12, W
03E0  1D03     BTFSS STATUS, 0x2
03E1  2BE4     GOTO 0x3E4
03E2  082A     MOVF c2, W
03E3  0242     SUBWF j, W
03E4  1C03     BTFSS STATUS, 0x0
03E5  2BE7     GOTO 0x3E7
03E6  2BE8     GOTO 0x3E8
03E7  2B51     GOTO 0x351
03E8  2BE9     GOTO 0x3E9
67:                        {
68:                            mult[i][j]=0;
0351  1283     BCF STATUS, 0x5
0352  0840     MOVF i, W
0353  0092     MOVWF 0x12
0354  3002     MOVLW 0x2
0355  1003     BCF STATUS, 0x0
0356  0D92     RLF 0x12, F
0357  3EFF     ADDLW 0xFF
0358  1D03     BTFSS STATUS, 0x2
0359  2B55     GOTO 0x355
035A  3022     MOVLW 0x22
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
69:                            for(k=0;k<r1;k++)
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
70:                                mult[i][j]+=m1[i][k]*m2[k][j];
036D  1283     BCF STATUS, 0x5
036E  0840     MOVF i, W
036F  0092     MOVWF 0x12
0370  3002     MOVLW 0x2
0371  1003     BCF STATUS, 0x0
0372  0D92     RLF 0x12, F
0373  3EFF     ADDLW 0xFF
0374  1D03     BTFSS STATUS, 0x2
0375  2B71     GOTO 0x371
0376  302C     MOVLW 0x2C
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
038E  3034     MOVLW 0x34
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
039D  22EE     CALL 0x2EE
039E  1283     BCF STATUS, 0x5
039F  0840     MOVF i, W
03A0  009A     MOVWF 0x1A
03A1  3002     MOVLW 0x2
03A2  1003     BCF STATUS, 0x0
03A3  0D9A     RLF 0x1A, F
03A4  3EFF     ADDLW 0xFF
03A5  1D03     BTFSS STATUS, 0x2
03A6  2BA2     GOTO 0x3A2
03A7  3022     MOVLW 0x22
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
71:                      //      printf("%d\t",mult[i][j]);
72:                        }
73:                        //printf("\n");
74:                    }
75:            
76:            
77:                return 0;
78:            
79:                }

