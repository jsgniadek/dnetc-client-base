#include "problem.h"
extern "C" int crunch_lintilla_604e(RC5UnitWork *work, unsigned long itr);

// $Id: crunch_lintilla_296-604.ppc.c,v 1.2 2000/07/11 01:47:59 mfeiri Exp $
//
// modified version of crunch_lintilla_296.ppc.c - optimized for 604e (original
// lintilla is still faster on 603e, at least). Optimized by Roberto Ragusa
// (robertoragusa@technologist.com)

// offsets into the RC5UnitWork structure
#define plain_hi	0     // plaintext, already mixed with IV
#define plain_lo	4
#define cypher_hi	8    // target cyphertext
#define cypher_lo	12
#define L0_hi		16       // key, changes with every unit * PIPELINE_COUNT
#define L0_lo		20

// Local_Frame struct: stack frame and local variables
#define frame_new	0	// save SP
#define save_CR		4	// save CR
#define save_RTOC	20	// save RTOC

// local variables
//#define param1, 24	// 24(SP) pass parameters here
//#define param2, 28	// 28(SP) ...
#define count	32
#define P_0		36
#define P_1		40
#define C_0		44
#define C_1		48
#define L0_0	52
#define L0_1	56
#define Sr_0	60
#define Sr_1	64
#define con0	68
#define con1	72
#define con2	76
#define S0_3	80
#define S0_4	84
#define S0_5	88
#define S0_6	92
#define S0_7	96
#define S0_8	100
#define S0_9	104
#define S0_10	108
#define S0_11	112
#define S0_12	116
#define S0_13	120
#define S0_14	124
#define S0_15	128
#define S0_16	132
#define S0_17	136
#define S0_18	140
#define S0_19	144
#define S0_20	148
#define S0_21	152
#define S0_22	156
#define S0_23	160
#define S0_24	164
#define S0_25	168
#define Ss_2	172
#define Ss_3	176
#define Ss_4	180
#define Ss_5	184
#define Ss_6	188
#define Ss_7	192
#define Ss_8	196
#define Ss_9	200
#define Ss_10	204
#define Ss_11	208
#define Ss_12	212
#define Ss_13	216
#define Ss_14	220

// old stack frame
   // -76+frame(SP)	saved GPR13-31
#define frame_old 300	// 0+frame(SP)	old SP pointed here
   // 4+frame(SP)	save CR
   // 8+frame(SP)	save LR
   // 12+frame(SP)	reserved
   // 16+frame(SP)	reserved
   // 20+frame(SP)	callers RTOC
// calling parameter storage
#define work_ptr 324	// 24+frame(SP)
#define iterations 328	// 28+frame(SP)

// register aliases

#define tmp		r0
#define Sr2		r2
#define Sr3		r3
#define Sr4		r4
#define Sr5		r5
#define Sr6		r6
#define Sr7		r7
#define Sr8		r8
#define Sr9		r9
#define Sr10	r10
#define Sr11	r11
#define Sr12	r12
#define Sr13	r13
#define Sr14	r14
#define tmp2	r14
#define Sr15	r15
#define Sr16	r16
#define Sr17	r17
#define Sr18	r18
#define Sr19	r19
#define Sr20	r20
#define Sr21	r21
#define Sr22	r22
#define Sr23	r23
#define Sr24	r24
#define Sr25	r25	// Note register overloading
#define At		r25
#define Lr0		r26
#define Lr1		r27
#define Lt0		r28
#define Ls0		r28
#define Lt1		r29
#define Ls1		r29
#define Sr0		r30
#define Ax		r30
#define Qr		r30
#define Sr1		r31
#define S0r		r31
#define Bx		r31

asm int crunch_lintilla_604e(RC5UnitWork *work, unsigned long itr)
{
 stmw	r13,-76(SP)
 stwu	SP,frame_new-frame_old(SP)

 stw	r2,save_RTOC(SP)
 stw     r3,work_ptr(SP)		// work
 stw     r4,iterations(SP)	// initial iterations
 stw     r4,count(SP)		// remaining iterations

// Save work parameters on the local stack
 lwz	tmp,plain_hi(r3)
 stw	tmp,P_1(SP)
 lwz	tmp,plain_lo(r3)
 stw	tmp,P_0(SP)
 lwz	tmp,cypher_hi(r3)
 stw	tmp,C_1(SP)
 lwz	tmp,cypher_lo(r3)
 stw	tmp,C_0(SP)
 lwz	Lr1,L0_hi(r3)
 stw	Lr1,L0_1(SP)
 lwz	Lr0,L0_lo(r3)
 stw	Lr0,L0_0(SP)

start:
//Round 1 is rolled out of the loop to fill the pipe and setup constants.
//
 lis     S0r,0xB7E1
 lis     Qr,0x9E37
 addi    S0r,S0r,0x5163	//S0r = P//
 addi    Qr,Qr,0x79B9	//Qr = Q//

// Round 1.0 even
 rotlwi  At,S0r,3
 stw	At,Sr_0(SP)//		// Sr_0 = ...
 lwz	Lr0,L0_0(SP)
 add     Lr0,Lr0,At
 rotlw	Lr0,Lr0,At
 stw	Lr0,con0(SP)		// con0 = Lr_0
 add     S0r,S0r,Qr

// Round 1.1 odd
 add     tmp,S0r,At
 add     tmp,tmp,Lr0	
 rotlwi  At,tmp,3
 stw	At,Sr_1(SP)		// Sr_1 = rotl3(...)
 add     tmp,At,Lr0
 stw	tmp,con1(SP)		// con1 =  Sr_1 + Lr_0
 lwz	Lr1,L0_1(SP)
 add     Lr1,Lr1,tmp
 rotlw	Lr1,Lr1,tmp
 add     S0r,S0r,Qr

// Round 1.2 even
 add     tmp,S0r,At
 stw	tmp,con2(SP)		// con2 = S0_2 + Sr_1
 add     tmp,tmp,Lr1
 rotlwi  Sr2,tmp,3
 add     tmp,Sr2,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_3(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.3 odd
 add     tmp,S0r,Sr2
 add     tmp,tmp,Lr0	
 rotlwi  Sr3,tmp,3
 add     tmp,Sr3,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_4(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.4 even
 add     tmp,S0r,Sr3
 add     tmp,tmp,Lr1
 rotlwi  Sr4,tmp,3
 add     tmp,Sr4,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_5(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.5 odd
 add     tmp,S0r,Sr4
 add     tmp,tmp,Lr0	
 rotlwi  Sr5,tmp,3
 add     tmp,Sr5,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_6(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.6 even
 add     tmp,S0r,Sr5
 add     tmp,tmp,Lr1
 rotlwi  Sr6,tmp,3
 add     tmp,Sr6,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_7(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.7 odd
 add     tmp,S0r,Sr6
 add     tmp,tmp,Lr0	
 rotlwi  Sr7,tmp,3
 add     tmp,Sr7,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_8(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.8 even
 add     tmp,S0r,Sr7
 add     tmp,tmp,Lr1
 rotlwi  Sr8,tmp,3
 add     tmp,Sr8,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_9(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.9 odd
 add     tmp,S0r,Sr8
 add     tmp,tmp,Lr0	
 rotlwi  Sr9,tmp,3
 add     tmp,Sr9,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_10(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.10 even
 add     tmp,S0r,Sr9
 add     tmp,tmp,Lr1
 rotlwi  Sr10,tmp,3
 add     tmp,Sr10,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_11(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.11 odd
 add     tmp,S0r,Sr10
 add     tmp,tmp,Lr0	
 rotlwi  Sr11,tmp,3
 add     tmp,Sr11,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_12(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.12 even
 add     tmp,S0r,Sr11
 add     tmp,tmp,Lr1
 rotlwi  Sr12,tmp,3
 add     tmp,Sr12,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_13(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.13 odd
 add     tmp,S0r,Sr12
 add     tmp,tmp,Lr0	
 rotlwi  Sr13,tmp,3
 add     tmp,Sr13,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_14(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.14 even
 add     tmp,S0r,Sr13
 add     tmp,tmp,Lr1
 rotlwi  Sr14,tmp,3
 add     tmp,Sr14,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_15(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.15 odd
 add     tmp,S0r,Sr14
 add     tmp,tmp,Lr0	
 rotlwi  Sr15,tmp,3
 add     tmp,Sr15,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_16(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.16 even
 add     tmp,S0r,Sr15
 add     tmp,tmp,Lr1
 rotlwi  Sr16,tmp,3
 add     tmp,Sr16,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_17(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.17 odd
 add     tmp,S0r,Sr16
 add     tmp,tmp,Lr0	
 rotlwi  Sr17,tmp,3
 add     tmp,Sr17,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_18(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.18 even
 add     tmp,S0r,Sr17
 add     tmp,tmp,Lr1
 rotlwi  Sr18,tmp,3
 add     tmp,Sr18,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_19(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.19 odd
 add     tmp,S0r,Sr18
 add     tmp,tmp,Lr0	
 rotlwi  Sr19,tmp,3
 add     tmp,Sr19,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_20(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.20 even
 add     tmp,S0r,Sr19
 add     tmp,tmp,Lr1
 rotlwi  Sr20,tmp,3
 add     tmp,Sr20,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_21(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.21 odd
 add     tmp,S0r,Sr20
 add     tmp,tmp,Lr0	
 rotlwi  Sr21,tmp,3
 add     tmp,Sr21,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_22(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.22 even
 add     tmp,S0r,Sr21
 add     tmp,tmp,Lr1
 rotlwi  Sr22,tmp,3
 add     tmp,Sr22,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_23(SP)
 rotlw	Lr0,Lr0,tmp

// Round 1.23 odd
 add     tmp,S0r,Sr22
 add     tmp,tmp,Lr0	
 rotlwi  Sr23,tmp,3
 add     tmp,Sr23,Lr0
 add     S0r,S0r,Qr
 add     Lr1,Lr1,tmp
 stw	S0r,S0_24(SP)
 rotlw	Lr1,Lr1,tmp

// Round 1.24 even
 add     tmp,S0r,Sr23
 add     tmp,tmp,Lr1
 rotlwi  Sr24,tmp,3
 add     tmp,Sr24,Lr1
 add     S0r,S0r,Qr
 add     Lr0,Lr0,tmp
 stw	S0r,S0_25(SP)
 rotlw	Ls0,Lr0,tmp

// Round 1.25 odd
 add     tmp,S0r,Sr24
 add     tmp,tmp,Ls0	
 rotlwi  Sr25,tmp,3
 add     tmp,Sr25,Ls0
 add     Lr1,Lr1,tmp
 rotlw	Ls1,Lr1,tmp

reloop:

 lwz	Lr1,L0_1(SP)	// key.hi

// registers tmp, r26, r30, r31 are free
////	compute the inner loop count to next rollover
//	cnt = 255-((key1>>24)&0xff)
 srwi	tmp,Lr1,24	// shift right logical 24, mask 0xFF
 subfic	tmp,tmp,255
//	if cnt==0 // handle hi byte rollover
//	{
  cmpwi	tmp,0
  bne	label2
//		key1=rb(rb(key1)+1)
  li	r26,L0_1
  lwbrx	Lr1,SP,r26
  addic.	Lr1,Lr1,1
  stwbrx	Lr1,SP,r26
//		cnt = 256
  li	tmp,256
  lwz	Lr1,L0_1(SP)
  subis	Lr1,Lr1,256
//		if key1 == 0 // finish last key before rollover
//			cnt = 1
  bne	label2
  li	tmp,1
//	}

// limit loop to remaining count
label2:
 lwz	r30,count(SP)
 cmpw	tmp,r30
 ble	label3
 mr	tmp,r30
label3:	
 sub	r30,r30,tmp
 stw	r30,count(SP)
 mtctr	tmp

 lwz	Sr0,Sr_0(SP)
 lwz	Sr1,Sr_1(SP)

loop:

//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...			Sr25 ?	Lr1	Ls0	Ls1	Sr0	Sr1

 //2.0
 add     Sr0,Sr0,Sr25
   //1-0
 add     Sr0,Sr0,Ls1
   lwz	Lr0,con1(SP)	// const Lr_0 + Sr_1

 rotlwi  Sr0,Sr0,3
   addis	Lr1,Lr1,256

 add     tmp,Sr0,Ls1
   stw	Lr1,L0_1(SP)	// key.hi

 add     Ls0,tmp,Ls0
   //1-1
   add     Lr1,Lr1,Lr0

 rotlw	Ls0,Ls0,tmp

//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...		   Sr25	Lr0	Lr1	Ls0	Ls1	Ss0	Sr1

// Round 2.1 odd
 add     Sr1,Sr1,Sr0

 add     tmp,Sr1,Ls0
   rotlw	Lr1,Lr1,Lr0

 rotlwi  Sr1,tmp,3
 add	Ls1,Ls0,Ls1

 add     tmp,Sr1,Ls0		
 add     Ls1,Sr1,Ls1

 rotlw	Ls1,Ls1,tmp

//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...		   Sr25	---	key	Ls0	Ls1	Ss0	Ss1

// Round 2.2 even
 add     Sr2,Sr2,Sr1

 add     Sr2,Sr2,Ls1
   lwz	r26,con2(SP)	// const S0_2 + Sr_1

 rotlwi  Sr2,Sr2,3
 add	Ls0,Ls0,Ls1

 add     tmp,Sr2,Ls1
 add     Ls0,Sr2,Ls0

 rotlw	Ls0,Ls0,tmp


//	r0	r1	r2	r3	r4	...	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Ss2	Ss3	Sr4	...	   Sr25	c2	Lr1	Ls0	Ls1	Ss0	Ss1

// Round 2.3 odd
 add     Sr3,Sr3,Sr2
   //1.2
 add     Sr3,Sr3,Ls0
 stw	Sr2,Ss_2(SP)

 rotlwi  Sr3,Sr3,3
 stw	Sr3,Ss_3(SP)
   add     Sr2,r26,Lr1

 add     tmp,Sr3,Ls0	
   rotlwi  Sr2,Sr2,3

 add     Ls1,tmp,Ls1
   lwz	Lr0,con0(SP)	// const Lr_0

 rotlw	Ls1,Ls1,tmp


// Round 2.4 even
 add     Sr4,Sr4,Sr3

 add     Sr4,Sr4,Ls1

   add     tmp,Sr2,Lr1
   lwz     Sr3,S0_3(SP)

   add     Lr0,Lr0,tmp
 rotlwi  Sr4,Sr4,3

   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr4,Ls1

 add     Ls0,tmp,Ls0

 rotlw	Ls0,Ls0,tmp

// Round 2.5 odd		round 1.3 odd
 add     Sr5,Sr5,Sr4
 add     Sr5,Sr5,Ls0
   add     Sr3,Sr3,Sr2
 rotlwi  Sr5,Sr5,3
   add     Sr3,Sr3,Lr0
 add     tmp,Sr5,Ls0		
   rotlwi  Sr3,Sr3,3
 add     Ls1,tmp,Ls1
   stw	Sr4,Ss_4(SP)
   stw	Sr5,Ss_5(SP)
 rotlw	Ls1,Ls1,tmp

// Round 2.6 even
 add     Sr6,Sr6,Sr5
 add     Sr6,Sr6,Ls1
   add     tmp,Sr3,Lr0
 rotlwi  Sr6,Sr6,3
   add     Lr1,Lr1,tmp
   rotlw	Lr1,Lr1,tmp
 add     tmp,Sr6,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr4,S0_4(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.7 odd		round 1.4 even
 add     Sr7,Sr7,Sr6
 add     Sr7,Sr7,Ls0
   add     Sr4,Sr4,Sr3
 rotlwi  Sr7,Sr7,3
   add     Sr4,Sr4,Lr1
   rotlwi  Sr4,Sr4,3
 add     tmp,Sr7,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.8 even
 add     Sr8,Sr8,Sr7
 add     Sr8,Sr8,Ls1
   add     tmp,Sr4,Lr1
 rotlwi  Sr8,Sr8,3
   add     Lr0,Lr0,tmp
   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr8,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr5,S0_5(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.9 odd		round 1.5 odd
 add     Sr9,Sr9,Sr8
 add     Sr9,Sr9,Ls0
   add     Sr5,Sr5,Sr4
 rotlwi  Sr9,Sr9,3
   add     Sr5,Sr5,Lr0
   rotlwi  Sr5,Sr5,3
 add     tmp,Sr9,Ls0		
 add     Ls1,tmp,Ls1
   stw	Sr6,Ss_6(SP)
   stw	Sr7,Ss_7(SP)
   stw	Sr8,Ss_8(SP)
   stw	Sr9,Ss_9(SP)
 rotlw	Ls1,Ls1,tmp

// Round 2.10 even
 add     Sr10,Sr10,Sr9
 add     Sr10,Sr10,Ls1
   add     tmp,Sr5,Lr0
 rotlwi  Sr10,Sr10,3
   add     Lr1,Lr1,tmp
   rotlw	Lr1,Lr1,tmp
 add     tmp,Sr10,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr6,S0_6(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.11 odd	round 1.6 even
 add     Sr11,Sr11,Sr10
 add     Sr11,Sr11,Ls0
   add     Sr6,Sr6,Sr5
 rotlwi  Sr11,Sr11,3
   add     Sr6,Sr6,Lr1
   rotlwi  Sr6,Sr6,3
 add     tmp,Sr11,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.12 even
 add     Sr12,Sr12,Sr11
 add     Sr12,Sr12,Ls1
   add     tmp,Sr6,Lr1
 rotlwi  Sr12,Sr12,3
   add     Lr0,Lr0,tmp
   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr12,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr7,S0_7(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.13 odd	round 1.7 odd
 add     Sr13,Sr13,Sr12
 add     Sr13,Sr13,Ls0
   add     Sr7,Sr7,Sr6
 rotlwi  Sr13,Sr13,3
   add     Sr7,Sr7,Lr0
   rotlwi  Sr7,Sr7,3
 add     tmp,Sr13,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.14 even
 add     Sr14,Sr14,Sr13
 add     Sr14,Sr14,Ls1
   add     tmp,Sr7,Lr0
 rotlwi  Sr14,Sr14,3
   add     Lr1,Lr1,tmp
   rotlw	Lr1,Lr1,tmp
 add     tmp,Sr14,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr8,S0_8(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.15 odd	round 1.8 even
 add     Sr15,Sr15,Sr14
 add     Sr15,Sr15,Ls0
   add     Sr8,Sr8,Sr7
 rotlwi  Sr15,Sr15,3
   add     Sr8,Sr8,Lr1
   rotlwi  Sr8,Sr8,3
 add     tmp,Sr15,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.16 even
 add     Sr16,Sr16,Sr15
 add     Sr16,Sr16,Ls1
   add     tmp,Sr8,Lr1
 rotlwi  Sr16,Sr16,3
   add     Lr0,Lr0,tmp
   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr16,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr9,S0_9(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.17 odd	round 1.9 odd
 add     Sr17,Sr17,Sr16
 add     Sr17,Sr17,Ls0
   add     Sr9,Sr9,Sr8
 rotlwi  Sr17,Sr17,3
   add     Sr9,Sr9,Lr0
   rotlwi  Sr9,Sr9,3
 add     tmp,Sr17,Ls0		
 add     Ls1,tmp,Ls1
   stw	Sr10,Ss_10(SP)
   stw	Sr11,Ss_11(SP)
   stw	Sr12,Ss_12(SP)
   stw	Sr13,Ss_13(SP)
   stw	Sr14,Ss_14(SP)
 rotlw	Ls1,Ls1,tmp

// Round 2.18 even
 add     Sr18,Sr18,Sr17
 add     Sr18,Sr18,Ls1
   add     tmp,Sr9,Lr0
 rotlwi  Sr18,Sr18,3
   add     Lr1,Lr1,tmp
   rotlw	Lr1,Lr1,tmp
 add     tmp,Sr18,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr10,S0_10(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.19 odd	round 1.10 even
 add     Sr19,Sr19,Sr18
 add     Sr19,Sr19,Ls0
   add     Sr10,Sr10,Sr9
 rotlwi  Sr19,Sr19,3
   add     Sr10,Sr10,Lr1
   rotlwi  Sr10,Sr10,3
 add     tmp,Sr19,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.20 even
 add     Sr20,Sr20,Sr19
 add     Sr20,Sr20,Ls1
   add     tmp,Sr10,Lr1
 rotlwi  Sr20,Sr20,3
   add     Lr0,Lr0,tmp
   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr20,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr11,S0_11(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.21 odd	round 1.11 odd
 add     Sr21,Sr21,Sr20
 add     Sr21,Sr21,Ls0
   add     Sr11,Sr11,Sr10
 rotlwi  Sr21,Sr21,3
   add     Sr11,Sr11,Lr0
   rotlwi  Sr11,Sr11,3
 add     tmp,Sr21,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.22 even
 add     Sr22,Sr22,Sr21
 add     Sr22,Sr22,Ls1
   add     tmp,Sr11,Lr0
 rotlwi  Sr22,Sr22,3
   add     Lr1,Lr1,tmp
   rotlw	Lr1,Lr1,tmp
 add     tmp,Sr22,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr12,S0_12(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.23 odd	round 1.12 even
 add     Sr23,Sr23,Sr22
 add     Sr23,Sr23,Ls0
   add     Sr12,Sr12,Sr11
 rotlwi  Sr23,Sr23,3
   add     Sr12,Sr12,Lr1
   rotlwi  Sr12,Sr12,3
 add     tmp,Sr23,Ls0		
 add     Ls1,tmp,Ls1
 rotlw	Ls1,Ls1,tmp

// Round 2.24 even
 add     Sr24,Sr24,Sr23
 add     Sr24,Sr24,Ls1
   add     tmp,Sr12,Lr1
 rotlwi  Sr24,Sr24,3
   add     Lr0,Lr0,tmp
   rotlw	Lr0,Lr0,tmp
 add     tmp,Sr24,Ls1
 add     Ls0,tmp,Ls0
   lwz	Sr13,S0_13(SP)
 rotlw	Ls0,Ls0,tmp

// Round 2.25 odd	round 1.13 odd
 add     Sr25,Sr25,Sr24

 add     Sr25,Sr25,Ls0
   add     Sr13,Sr13,Sr12

 rotlwi  Sr25,Sr25,3
   add     Sr13,Sr13,Lr0

   rotlwi  Sr13,Sr13,3
 add     tmp,Sr25,Ls0	

 add     Ls1,tmp,Ls1

 rotlw	Ls1,Ls1,tmp

////	stw	Sr25,Ss_25(SP)	// Ss_25 will most often never get read

// Registers:
//	r0	r1	r2	...	r13	r14	...	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	...Sr13	Ss14...Ss25	Lr0	Lr1	Ls0	Ls1	Ss0	Ss1
//						tmp2	At			Lt0	Lt1	Ax	Bx

// Note register lineup Sr25->At, Ls0->Lt0, Ls1->Lt1, Sr0->Ax, Sr1->Bx

// Round 3.0 even
 add     At,Sr25,Sr0	// At is r25
 add     At,At,Ls1
   lwz	Ax,P_0(SP)
 rotlwi  At,At,3
   add     tmp2,Sr13,Lr0
 add     tmp,At,Ls1
   add     Lr1,Lr1,tmp2
 add	Lt0,tmp,Lt0
   rotlw	Lr1,Lr1,tmp2
 rotlw	Lt0,Lt0,tmp

// Round 3.1 odd
 add     tmp2,At,Sr1

 add     tmp2,tmp2,Lt0
   lwz	Bx,P_1(SP)

   add	Ax,Ax,At	//Ax_0
 rotlwi  At,tmp2,3

 add     tmp,At,Lt0
   lwz	tmp2,Ss_2(SP)

 add	Lt1,tmp,Lt1
//	addi	r10,r10,0	//--fill???--

 rotlw	Lt1,Lt1,tmp

// Round 3.2 even
 add     tmp2,At,tmp2

 add     tmp2,tmp2,Lt1
   add	Bx,Bx,At	//Bx_0

 rotlwi  At,tmp2,3
 addi	r10,r10,0	//--fill--

 add     tmp,At,Lt1
   lwz	tmp2,Ss_3(SP)

 add     Lt0,Lt0,tmp
   xor	Ax,Ax,Bx

 rotlw	Lt0,Lt0,tmp

// Round 3.3 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At	//Ax_2
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	tmp2,Ss_4(SP)
   xor	Bx,Bx,Ax
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp

// Round 3.4 even
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt1
   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt1
   lwz	tmp2,Ss_5(SP)
   xor	Ax,Ax,Bx
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp


// Round 3.5 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	tmp2,Ss_6(SP)
   xor	Bx,Bx,Ax
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp

// Round 3.6 even
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt1
   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt1
   lwz	tmp2,Ss_7(SP)
 add     Lt0,Lt0,tmp
   xor	Ax,Ax,Bx
 rotlw	Lt0,Lt0,tmp

// Round 3.7 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	tmp2,Ss_8(SP)
 add     Lt1,Lt1,tmp
   xor	Bx,Bx,Ax
 rotlw	Lt1,Lt1,tmp

// Round 3.8 even
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt1
   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt1
   lwz	tmp2,Ss_9(SP)
 add     Lt0,Lt0,tmp
   xor	Ax,Ax,Bx
 rotlw	Lt0,Lt0,tmp

// Round 3.9 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	tmp2,Ss_10(SP)
 add     Lt1,Lt1,tmp
   xor	Bx,Bx,Ax
 rotlw	Lt1,Lt1,tmp

// Round 3.10 even
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt1
   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt1
   lwz	tmp2,Ss_11(SP)
   xor	Ax,Ax,Bx
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp

// Round 3.11 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	tmp2,Ss_12(SP)
   xor	Bx,Bx,Ax
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp

// Round 3.12 even
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt1
   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlwi  At,tmp2,3
 add     tmp,At,Lt1
   lwz	tmp2,Ss_13(SP)
   xor	Ax,Ax,Bx
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp

// Round 3.13 odd
 add     tmp2,At,tmp2
 add     tmp2,tmp2,Lt0
   rotlw	Ax,Ax,Bx
   add	Ax,Ax,At	//Ax_12
 rotlwi  At,tmp2,3
 add     tmp,At,Lt0
   lwz	Sr14,Ss_14(SP)	//tmp2 is Sr14!
   xor	Bx,Bx,Ax
 add     Lt1,Lt1,tmp

   rotlw	Bx,Bx,Ax
   add	Bx,Bx,At
 rotlw	Lt1,Lt1,tmp

// Round 1.14/3.14 even
 add     At,At,Sr14
 lwz     Sr14,S0_14(SP)
 xor	Ax,Ax,Bx
 add     At,At,Lt1
 rotlwi  At,At,3
 add     Sr14,Sr14,Sr13
 add     Sr14,Sr14,Lr1
 add     tmp,At,Lt1
 rotlwi  Sr14,Sr14,3
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp
 add     tmp,Sr14,Lr1
 rotlw	Ax,Ax,Bx
 add     Lr0,Lr0,tmp
 rotlw	Lr0,Lr0,tmp
 add	Ax,Ax,At

// Round 1.15/3.15 odd
 add     At,At,Sr15
 lwz     Sr15,S0_15(SP)
 xor	Bx,Bx,Ax
 add     At,At,Lt0
 rotlwi  At,At,3
 add     Sr15,Sr15,Sr14
 add     Sr15,Sr15,Lr0
 add     tmp,At,Lt0
 rotlwi  Sr15,Sr15,3
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp
 add     tmp,Sr15,Lr0
 rotlw	Bx,Bx,Ax
 add     Lr1,Lr1,tmp
 rotlw	Lr1,Lr1,tmp
 add	Bx,Bx,At

// Round 1.16/3.16 even
 add     At,At,Sr16
 lwz     Sr16,S0_16(SP)
 xor	Ax,Ax,Bx
 add     At,At,Lt1
 rotlwi  At,At,3
 add     Sr16,Sr16,Sr15
 add     Sr16,Sr16,Lr1
 add     tmp,At,Lt1
 rotlwi  Sr16,Sr16,3
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp
 add     tmp,Sr16,Lr1
 rotlw	Ax,Ax,Bx
 add     Lr0,Lr0,tmp
 rotlw	Lr0,Lr0,tmp
 add	Ax,Ax,At

// Round 1.17/3.17 odd
 add     At,At,Sr17
 lwz     Sr17,S0_17(SP)
 xor	Bx,Bx,Ax
 add     At,At,Lt0
 rotlwi  At,At,3
 add     Sr17,Sr17,Sr16
 add     Sr17,Sr17,Lr0
 add     tmp,At,Lt0
 rotlwi  Sr17,Sr17,3
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp
 add     tmp,Sr17,Lr0
 rotlw	Bx,Bx,Ax
 add     Lr1,Lr1,tmp
 rotlw	Lr1,Lr1,tmp
 add	Bx,Bx,At

// Round 1.18/3.18 even
 add     At,At,Sr18
 lwz     Sr18,S0_18(SP)
 xor	Ax,Ax,Bx
 add     At,At,Lt1
 rotlwi  At,At,3
 add     Sr18,Sr18,Sr17
 add     Sr18,Sr18,Lr1
 add     tmp,At,Lt1
 rotlwi  Sr18,Sr18,3
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp
 add     tmp,Sr18,Lr1
 rotlw	Ax,Ax,Bx
 add     Lr0,Lr0,tmp
 rotlw	Lr0,Lr0,tmp
 add	Ax,Ax,At

// Round 1.19/3.19 odd
 add     At,At,Sr19
 lwz     Sr19,S0_19(SP)
 xor	Bx,Bx,Ax
 add     At,At,Lt0
 rotlwi  At,At,3
 add     Sr19,Sr19,Sr18
 add     Sr19,Sr19,Lr0
 add     tmp,At,Lt0
 rotlwi  Sr19,Sr19,3
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp
 add     tmp,Sr19,Lr0
 rotlw	Bx,Bx,Ax
 add     Lr1,Lr1,tmp
 rotlw	Lr1,Lr1,tmp
 add	Bx,Bx,At

// Round 1.20/3.20 even
 add     At,At,Sr20
 lwz     Sr20,S0_20(SP)
 xor	Ax,Ax,Bx
 add     At,At,Lt1
 rotlwi  At,At,3
 add     Sr20,Sr20,Sr19
 add     Sr20,Sr20,Lr1
 add     tmp,At,Lt1
 rotlwi  Sr20,Sr20,3
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp
 add     tmp,Sr20,Lr1
 rotlw	Ax,Ax,Bx
 add     Lr0,Lr0,tmp
 rotlw	Lr0,Lr0,tmp
 add	Ax,Ax,At

// Round 1.21/3.21 odd
 add     At,At,Sr21
 lwz     Sr21,S0_21(SP)
 xor	Bx,Bx,Ax
 add     At,At,Lt0
 rotlwi  At,At,3
 add     Sr21,Sr21,Sr20
 add     Sr21,Sr21,Lr0
 add     tmp,At,Lt0
 rotlwi  Sr21,Sr21,3
 add     Lt1,Lt1,tmp
 rotlw	Lt1,Lt1,tmp
 add     tmp,Sr21,Lr0
 rotlw	Bx,Bx,Ax
 add     Lr1,Lr1,tmp
 rotlw	Lr1,Lr1,tmp
 add	Bx,Bx,At

// Round 1.22/3.22 even
 add     At,At,Sr22
 lwz     Sr22,S0_22(SP)
 xor	Ax,Ax,Bx
 add     At,At,Lt1
 rotlwi  At,At,3
 add     Sr22,Sr22,Sr21
 add     Sr22,Sr22,Lr1
 add     tmp,At,Lt1
 rotlwi  Sr22,Sr22,3
 add     Lt0,Lt0,tmp
 rotlw	Lt0,Lt0,tmp
 add     tmp,Sr22,Lr1
 rotlw	Ax,Ax,Bx
 add     Lr0,Lr0,tmp
 rotlw	Lr0,Lr0,tmp
 add	Ax,Ax,At

// Round 1.23/3.23 odd
 add     At,At,Sr23
 lwz     Sr23,S0_23(SP)

 xor	Bx,Bx,Ax
 add     At,At,Lt0

 rotlwi  At,At,3
 add     Sr23,Sr23,Sr22

 add     Sr23,Sr23,Lr0
 add     tmp,At,Lt0

 rotlwi  Sr23,Sr23,3
 add     Lt1,Lt1,tmp

 rotlw	Lt1,Lt1,tmp
 add     tmp,Sr23,Lr0

 rotlw	Bx,Bx,Ax
 add     Lr1,Lr1,tmp

 rotlw	Lr1,Lr1,tmp
 add	Bx,Bx,At


//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...	Sr23		Lr0	Lr1	---	Lt1
//							Ss24 At					Ax	Bx
// Round 1.24/3.24 even
 add     At,At,Sr24
 lwz     Sr24,S0_24(SP)

 xor	Ax,Ax,Bx
 add     At,At,Lt1

 add     Sr24,Sr24,Sr23
 rotlwi  At,At,3

 add     Sr24,Sr24,Lr1
 rotlw	Ax,Ax,Bx

 rotlwi  Sr24,Sr24,3
 add	r31,Ax,At

 add     tmp,Sr24,Lr1
 lwz     Sr25,S0_25(SP)

 add     Ls0,Lr0,tmp		//switch to Ls0/Ls1 registers
 lwz	r29,C_0(SP)

 rotlw	Ls0,Ls0,tmp

//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...			Sr25---	Lr1	Ls0	C_0	---	tAx

// Round 1.25 odd
 add     Sr25,Sr25,Sr24
 add     Sr25,Sr25,Ls0
   lwz	Sr0,Sr_0(SP)
   cmplw	r29,r31
 rotlwi  Sr25,Sr25,3
 add     tmp,Sr25,Ls0
   lwz	Sr1,Sr_1(SP)
 add     Ls1,tmp,Lr1
   lwz	Lr1,L0_1(SP)	// key.hi
 rotlw	Ls1,Ls1,tmp

//	r0	r1	r2	r3	...	r23	r24	r25	r26	r27	r28	r29	r30	r31
//	tmp	SP	Sr2	Sr3	...			Sr25 ?	Lr1	Ls0	Ls1	Sr0	Sr1

 bdnzf	2,loop
//// free registers:
//// r0 (tmp), r26 (Lr0)
//// r27 (Lr1), r30 (Sr0), r31 (Sr1) can be reloaded

////registers available:
////	tmp=r0,Lr0=r26,Lr1=r27,r30,r31

 lwz	r30,count(SP)

//	if key found
 bne	label5
//		decrement key_hi (byte reversed)
 li	r26,L0_1
 lwbrx	Lr1,SP,r26
 subi	Lr1,Lr1,1
 stwbrx	Lr1,SP,r26	////also save in work?
 lwz	Lr1,L0_1(SP)

//		increment remaining count
//		add unused loop count
 mfctr	tmp
 add	r30,r30,tmp
 addi	r30,r30,1
//		goto exit
 b	exit
//
label5:	
//	if key_hi==0x0000
//		increment key_lo
 cmpwi	Lr1,0
 bne	label6
 li	r31,L0_0
 lwbrx	Lr0,SP,r31
 addi	Lr0,Lr0,1
 stwbrx	Lr0,SP,r31	//// also save in work record

label6:	
//	if remaining count == 0 goto exit
 cmpwi	r30,0
 beq	exit

//	if key_hi==0x0000
 cmpwi	Lr1,0// same as earlier
 beq	start	//// round 1 was invalid! (should never happen in real client)
//carry registers to reloop: Lr1,cnt
 b	reloop

////exit conditions:
////  r30 = remiaining count
////  Lr1 = key.hi

exit:
//// save the last key tested in the work record
 lwz	r4,work_ptr(SP)
//	WITH RC5UnitWork // base register r4
 lwz	Lr0,L0_0(SP)
 stw	Lr0,L0_lo(r4)
//	lwz	Lr1,L0_1(SP)
 stw	Lr1,L0_hi(r4)
//	ENDWITH
//// return count of keys tested
 lwz	r3,iterations(SP)
 sub	r3,r3,r30

 lwz	r2,save_RTOC(SP)
 addi	SP,SP,frame_old-frame_new
 lmw     r13,-76(SP)
 blr

//	ENDWITH
//	END
}