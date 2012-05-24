1283 //   BCF STATUS, 0x5
0190 //   CLRF product
0191 //   CLRF 0x11
2A78 //   GOTO 0x278     
1C0C //   BTFSS multiplier, 0x0
2A7B //   GOTO 0x27B
2A7C //   GOTO 0x27C
2A82 //   GOTO 0x282
080E //   MOVF multiplicand, W
0790 //   ADDWF product, F
1803 //   BTFSC STATUS, 0x0
0A91 //   INCF 0x11, F
080F //   MOVF 0xF, W
0791 //   ADDWF 0x11, F
3001 //   MOVLW 0x1
1003 //   BCF STATUS, 0x0
0D8E //   RLF multiplicand, F
0D8F //   RLF 0xF, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2A83 //   GOTO 0x283
3001 //   MOVLW 0x1
1003 //   BCF STATUS, 0x0
0C8D //   RRF 0xD, F
0C8C //   RRF multiplier, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2A8A //   GOTO 0x28A
080D //   MOVF 0xD, W
040C //   IORWF multiplier, W
1D03 //   BTFSS STATUS, 0x2
2A95 //   GOTO 0x295
2A96 //   GOTO 0x296
2A78 //   GOTO 0x278
2A97 //   GOTO 0x297
0811 //   MOVF 0x11, W
018D //   CLRF 0xD
078D //   ADDWF 0xD, F
0810 //   MOVF product, W
018C //   CLRF multiplier
078C //   ADDWF multiplier, F
2A9E //   GOTO 0x29E
0008 //   RETURN
3002 //   MOVLW 0x2
1283 //   BCF STATUS, 0x5
00BC //   MOVWF r1
3000 //   MOVLW 0x0
00BD //   MOVWF 0x3D
3002 //   MOVLW 0x2
00B6 //   MOVWF c1
3000 //   MOVLW 0x0
00B7 //   MOVWF 0x37
3002 //   MOVLW 0x2
00B8 //   MOVWF r2
3000 //   MOVLW 0x0
00B9 //   MOVWF 0x39
3002 //   MOVLW 0x2
00BA //   MOVWF c2
3000 //   MOVLW 0x0
00BB //   MOVWF 0x3B
01C0 //   CLRF i
01C1 //   CLRF 0x41
2AED //   GOTO 0x2ED
3001 //   MOVLW 0x1
1283 //   BCF STATUS, 0x5
07C0 //   ADDWF i, F
1803 //   BTFSC STATUS, 0x0
0AC1 //   INCF 0x41, F
3000 //   MOVLW 0x0
07C1 //   ADDWF 0x41, F
0841 //   MOVF 0x41, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
083D //   MOVF 0x3D, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2AF7 //   GOTO 0x2F7
083C //   MOVF r1, W
0240 //   SUBWF i, W
1C03 //   BTFSS STATUS, 0x0
2AFA //   GOTO 0x2FA
2AFB //   GOTO 0x2FB
2AB3 //   GOTO 0x2B3
2AFC //   GOTO 0x2FC
1283 //   BCF STATUS, 0x5
01C2 //   CLRF j
01C3 //   CLRF 0x43
2AD7 //   GOTO 0x2D7
3001 //   MOVLW 0x1
07C2 //   ADDWF j, F
1803 //   BTFSC STATUS, 0x0
0AC3 //   INCF 0x43, F
3000 //   MOVLW 0x0
07C3 //   ADDWF 0x43, F
2AD7 //   GOTO 0x2D7
0843 //   MOVF 0x43, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
0837 //   MOVF 0x37, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2AE1 //   GOTO 0x2E1
0836 //   MOVF c1, W
0242 //   SUBWF j, W
1C03 //   BTFSS STATUS, 0x0
2AE4 //   GOTO 0x2E4
2AE5 //   GOTO 0x2E5
2AB7 //   GOTO 0x2B7
2AE6 //   GOTO 0x2E6
1283 //   BCF STATUS, 0x5
0840 //   MOVF i, W
0092 //   MOVWF 0x12
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D92 //   RLF 0x12, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2ABB //   GOTO 0x2BB
301E //   MOVLW 0x1E
0712 //   ADDWF 0x12, W
0093 //   MOVWF 0x13
0842 //   MOVF j, W
0094 //   MOVWF 0x14
0714 //   ADDWF 0x14, W
0713 //   ADDWF 0x13, W
0095 //   MOVWF 0x15
0815 //   MOVF 0x15, W
0084 //   MOVWF FSR
3001 //   MOVLW 0x1
1383 //   BCF STATUS, 0x7
0080 //   MOVWF INDF
0A84 //   INCF FSR, F
3000 //   MOVLW 0x0
0080 //   MOVWF INDF
1283 //   BCF STATUS, 0x5
01C0 //   CLRF i
01C1 //   CLRF 0x41
2B3A //   GOTO 0x33A
3001 //   MOVLW 0x1
1283 //   BCF STATUS, 0x5
07C0 //   ADDWF i, F
1803 //   BTFSC STATUS, 0x0
0AC1 //   INCF 0x41, F
3000 //   MOVLW 0x0
07C1 //   ADDWF 0x41, F
0841 //   MOVF 0x41, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
0839 //   MOVF 0x39, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2B44 //   GOTO 0x344
0838 //   MOVF r2, W
0240 //   SUBWF i, W
1C03 //   BTFSS STATUS, 0x0
2B47 //   GOTO 0x347
2B48 //   GOTO 0x348
2B00 //   GOTO 0x300
2B49 //   GOTO 0x349
1283 //   BCF STATUS, 0x5
01C2 //   CLRF j
01C3 //   CLRF 0x43
2B24 //   GOTO 0x324
3001 //   MOVLW 0x1
07C2 //   ADDWF j, F
1803 //   BTFSC STATUS, 0x0
0AC3 //   INCF 0x43, F
3000 //   MOVLW 0x0
07C3 //   ADDWF 0x43, F
2B24 //   GOTO 0x324
0843 //   MOVF 0x43, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
083B //   MOVF 0x3B, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2B2E //   GOTO 0x32E
083A //   MOVF c2, W
0242 //   SUBWF j, W
1C03 //   BTFSS STATUS, 0x0
2B31 //   GOTO 0x331
2B32 //   GOTO 0x332
2B04 //   GOTO 0x304
2B33 //   GOTO 0x333
1283 //   BCF STATUS, 0x5
0840 //   MOVF i, W
0092 //   MOVWF 0x12
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D92 //   RLF 0x12, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2B08 //   GOTO 0x308
3026 //   MOVLW 0x26
0712 //   ADDWF 0x12, W
0093 //   MOVWF 0x13
0842 //   MOVF j, W
0094 //   MOVWF 0x14
0714 //   ADDWF 0x14, W
0713 //   ADDWF 0x13, W
0095 //   MOVWF 0x15
0815 //   MOVF 0x15, W
0084 //   MOVWF FSR
3003 //   MOVLW 0x3
1383 //   BCF STATUS, 0x7
0080 //   MOVWF INDF
0A84 //   INCF FSR, F
3000 //   MOVLW 0x0
0080 //   MOVWF INDF
1283 //   BCF STATUS, 0x5
01C0 //   CLRF i
01C1 //   CLRF 0x41
2BF0 //   GOTO 0x3F0
3001 //   MOVLW 0x1
1283 //   BCF STATUS, 0x5
07C0 //   ADDWF i, F
1803 //   BTFSC STATUS, 0x0
0AC1 //   INCF 0x41, F
3000 //   MOVLW 0x0
07C1 //   ADDWF 0x41, F
0841 //   MOVF 0x41, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
083D //   MOVF 0x3D, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2BFA //   GOTO 0x3FA
083C //   MOVF r1, W
0240 //   SUBWF i, W
1C03 //   BTFSS STATUS, 0x0
2BFD //   GOTO 0x3FD
2BFE //   GOTO 0x3FE
2B4D //   GOTO 0x34D
2BFF //   GOTO 0x3FF
1283 //   BCF STATUS, 0x5
01C2 //   CLRF j
01C3 //   CLRF 0x43
2BDA //   GOTO 0x3DA
3001 //   MOVLW 0x1
1283 //   BCF STATUS, 0x5
07C2 //   ADDWF j, F
1803 //   BTFSC STATUS, 0x0
0AC3 //   INCF 0x43, F
3000 //   MOVLW 0x0
07C3 //   ADDWF 0x43, F
0843 //   MOVF 0x43, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
083B //   MOVF 0x3B, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2BE4 //   GOTO 0x3E4
083A //   MOVF c2, W
0242 //   SUBWF j, W
1C03 //   BTFSS STATUS, 0x0
2BE7 //   GOTO 0x3E7
2BE8 //   GOTO 0x3E8
2B51 //   GOTO 0x351
2BE9 //   GOTO 0x3E9
1283 //   BCF STATUS, 0x5
0840 //   MOVF i, W
0092 //   MOVWF 0x12
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D92 //   RLF 0x12, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2B55 //   GOTO 0x355
302E //   MOVLW 0x2E
0712 //   ADDWF 0x12, W
0093 //   MOVWF 0x13
0842 //   MOVF j, W
0094 //   MOVWF 0x14
0714 //   ADDWF 0x14, W
0713 //   ADDWF 0x13, W
0095 //   MOVWF 0x15
0815 //   MOVF 0x15, W
0084 //   MOVWF FSR
3000 //   MOVLW 0x0
1383 //   BCF STATUS, 0x7
0080 //   MOVWF INDF
0A84 //   INCF FSR, F
3000 //   MOVLW 0x0
0080 //   MOVWF INDF
01BE //   CLRF k
01BF //   CLRF 0x3F
2BC4 //   GOTO 0x3C4
3001 //   MOVLW 0x1
07BE //   ADDWF k, F
1803 //   BTFSC STATUS, 0x0
0ABF //   INCF 0x3F, F
3000 //   MOVLW 0x0
07BF //   ADDWF 0x3F, F
2BC4 //   GOTO 0x3C4
083F //   MOVF 0x3F, W
3A80 //   XORLW 0x80
0092 //   MOVWF 0x12
083D //   MOVF 0x3D, W
3A80 //   XORLW 0x80
0212 //   SUBWF 0x12, W
1D03 //   BTFSS STATUS, 0x2
2BCE //   GOTO 0x3CE
083C //   MOVF r1, W
023E //   SUBWF k, W
1C03 //   BTFSS STATUS, 0x0
2BD1 //   GOTO 0x3D1
2BD2 //   GOTO 0x3D2
2B6D //   GOTO 0x36D
2BD3 //   GOTO 0x3D3
1283 //   BCF STATUS, 0x5
0840 //   MOVF i, W
0092 //   MOVWF 0x12
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D92 //   RLF 0x12, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2B71 //   GOTO 0x371
301E //   MOVLW 0x1E
0712 //   ADDWF 0x12, W
0093 //   MOVWF 0x13
083E //   MOVF k, W
0094 //   MOVWF 0x14
0714 //   ADDWF 0x14, W
0713 //   ADDWF 0x13, W
0095 //   MOVWF 0x15
0815 //   MOVF 0x15, W
0084 //   MOVWF FSR
1383 //   BCF STATUS, 0x7
0800 //   MOVF INDF, W
008C //   MOVWF multiplier
0A84 //   INCF FSR, F
0800 //   MOVF INDF, W
008D //   MOVWF 0xD
083E //   MOVF k, W
0096 //   MOVWF 0x16
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D96 //   RLF 0x16, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2B89 //   GOTO 0x389
3026 //   MOVLW 0x26
0716 //   ADDWF 0x16, W
0097 //   MOVWF 0x17
0842 //   MOVF j, W
0098 //   MOVWF 0x18
0718 //   ADDWF 0x18, W
0717 //   ADDWF 0x17, W
0099 //   MOVWF 0x19
0819 //   MOVF 0x19, W
0084 //   MOVWF FSR
0800 //   MOVF INDF, W
008E //   MOVWF multiplicand
0A84 //   INCF FSR, F
0800 //   MOVF INDF, W
008F //   MOVWF 0xF
2274 //   CALL 0x274
1283 //   BCF STATUS, 0x5
0840 //   MOVF i, W
009A //   MOVWF 0x1A
3002 //   MOVLW 0x2
1003 //   BCF STATUS, 0x0
0D9A //   RLF 0x1A, F
3EFF //   ADDLW 0xFF
1D03 //   BTFSS STATUS, 0x2
2BA2 //   GOTO 0x3A2
302E //   MOVLW 0x2E
071A //   ADDWF 0x1A, W
009B //   MOVWF 0x1B
0842 //   MOVF j, W
009C //   MOVWF 0x1C
071C //   ADDWF 0x1C, W
071B //   ADDWF 0x1B, W
009D //   MOVWF 0x1D
081D //   MOVF 0x1D, W
0084 //   MOVWF FSR
080C //   MOVF multiplier, W
1383 //   BCF STATUS, 0x7
0780 //   ADDWF INDF, F
0F84 //   INCFSZ FSR, F
0800 //   MOVF INDF, W
1803 //   BTFSC STATUS, 0x0
0A00 //   INCF INDF, W
00CF //   MOVWF 0x4F
080D //   MOVF 0xD, W
074F //   ADDWF 0x4F, W
0080 //   MOVWF INDF
0384 //   DECF FSR, F
