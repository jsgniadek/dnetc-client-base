/* s8_1.h v3.0 */
/* does function, links to next function */
/* TOADS not done */

/* $Log: s8_1.h,v $
/* Revision 1.1.1.1  1998/06/14 14:23:51  remi
/* Initial integration.
/* */


#ifdef MANUAL_REGISTER_ALLOCATION

    register INNER_LOOP_SLICE T1, T2, T3, T4, T5, T6, T7, T8;
    register INNER_LOOP_SLICE T9, T10, T11, T12, T13, T14, T15;

/* FREE */
#define x23	a1	/* GROSS, but ran out of registers */
#define x24	a1
#define x25	a1

/* FREE */
#define x15	T15
#define x17	T15
#define x18	T15
#define x19	T15
#define x20	T15

#define x4	T14
#define x5	T14
/* FREE */
#define Preload1_8 T14
/* FREE */
#define Preload3_8 T14

#define x16	T13
#define x33	T13
#define x34	T13

/* FREE */
#define x6	T12
#define x35	T12
#define x36	T12

#define x27	T11
#define x28	T11
#define x29	T11
#define x30	T11
#define x31	T11
#define x32	T11
#define x38	T11
#define x39	T11
#define x40	T11
#define x41	T11
#define x42	T11

/* FREE */
#define x12	T10
#define x13	T10
#define x14	T10
#define x37	T10
/* FREE */
#define Preload2_8 T10

/* FREE */
#define x8	T9
#define x9	T9
/* FREE */
#define t_a3	T9

/* FREE */
#define t_key3	T8
/* FREE but hold */
#define t_k3	T8

#define key6	T7
/* FREE but hold */
#define k6	T7
/* FREE */
#define x10	T7
#define x11	T7

#define x2	T6
/* FREE */
#define x45	T6
#define x46	T6
#define x47	T6
#define x48	T6
#define x49	T6
#define x50	T6

#define x43	T5
#define x44	T5
/* FREE */
#define Preload4_8 T5

#define x3	T4
#define x22	T4
/* FREE */
#define t_a5	T4

#define x1	T3
/* FREE */
#define t_key5	T3
/* FREE but hold */
#define t_k5	T3

/* FREE */
#define x7	T2
/* FREE */
#define t_key4	T2
/* FREE but hold */
#define t_k4	T2

#define key2	T1
/* FREE but hold */
#define k2	T1
/* FREE */
#define x21	T1
#define x26	T1
/* FREE */
#define t_key6	T1
/* FREE but hold */
#define t_k6	T1

#else /* MANUAL_REGISTER_ALLOCATION */

    INNER_LOOP_SLICE x1, x2, x3, x4, x5, x6, x7, x8;
    INNER_LOOP_SLICE x9, x10, x11, x12, x13, x14, x15, x16;
    INNER_LOOP_SLICE x17, x18, x19, x20, x21, x22, x23, x24;
    INNER_LOOP_SLICE x25, x26, x27, x28, x29, x30, x31, x32;
    INNER_LOOP_SLICE x33, x34, x35, x36, x37, x38, x39, x40;
    INNER_LOOP_SLICE x41, x42, x43, x44, x45, x46, x47, x48;
    INNER_LOOP_SLICE x49, x50;
    INNER_LOOP_SLICE Preload1_8, Preload2_8, Preload3_8, Preload4_8;

    INNER_LOOP_SLICE k2, k6;
    INNER_LOOP_SLICE t_a3, t_a5;
    INNER_LOOP_SLICE t_k3, t_k4, t_k5, t_k6;

    INNER_LOOP_SLICE *key2, *key6;
    INNER_LOOP_SLICE *t_key3, *t_key4, *t_key5, *t_key6;

#endif /* MANUAL_REGISTER_ALLOCATION */

/* upon entry here, needs a3, a1, a4, a5 valid */

	ASM_A_LOAD (key2, offsets->Key_Ptrs[OFFSET8 + 2]);
	ASM_F_XOR (gx9, gx6, gx8);		/*             M  */  /* F */

	ASM_XOR (x1, a3, a1);			/*                */
	ASM_AND_NOT (x2, a1, a3);		/*           K    */
	ASM_D_LOAD (a2, in[S82]);
	ASM_F_OR (gx42, g_a5, gx20);		/*    D           */  /* F */

	ASM_XOR (x3, x2, a4);			/*           K    */
	ASM_AND (x27, a5, x2);			/*      F         */
	ASM_D_LOAD (k2, ((INNER_LOOP_SLICE *)key2)[0]);
	ASM_F_AND_NOT (gx21, g_a2, gx20);	/*          J     */  /* F */

	ASM_OR (x4, a5, x3);			/*           K    */
	ASM_AND_NOT (x43, x1, a5);		/*  B             */
	ASM_A_LOAD (key6, offsets->Key_Ptrs[OFFSET8 + 6]);
	ASM_F_AND (gx22, g_a6, gx21);		/*          J     */  /* F */

	ASM_XOR (x5, x1, x4);			/*           K    */
	ASM_XOR (a2, a2, k2);
	ASM_D_LOAD (a6, in[S86]);
	ASM_F_XOR (gx17, gx14, gx16);		/*            L N */  /* F */

	ASM_AND_NOT (x6, x5, a1);		/*           K    */
	ASM_OR (x21, x5, a5);			/*         I      */
	ASM_D_LOAD (k6, ((INNER_LOOP_SLICE *)key6)[0]);	
	ASM_F_XOR (gx44, g_a2, gx15);		/* A              */  /* F */

	ASM_OR (x12, x6, a4);			/*          J     */
	ASM_XOR (x7, x6, a3);			/*           K    */
    ASM_F_D_LOAD (Preloadg_1, merge[D71]);	/*                */  /* F */
	ASM_F_XOR (g_a1, g_a1, g_k1);				      /* F */

	ASM_OR (x35, a2, x6);			/*    D           */
	ASM_XOR (a6, a6, k6);
    ASM_F_D_LOAD (Preloadg_2, merge[D72]);	/*                */  /* F */
	ASM_F_AND (gx18, g_a1, gx17);		/*            L   */  /* F */

	ASM_XOR (x13, x12, x1);			/*          J     */ /* SWAP */
	ASM_AND_NOT (x8, x7, a5);		/*           K    */
    ASM_F_D_LOAD (Preloadg_3, merge[D73]);	/*                */  /* F */
	ASM_F_XOR (gx19, gx9, gx18);		/*            LM  */  /* F */

	ASM_XOR (x9, a4, x8);			/*           K    */
	ASM_XOR (x14, x13, a5);			/*          J     */
    ASM_A_LOAD (t_key3, offsets->Key_Ptrs[OFFSET1 + 3]);	/* PIPELINE */
	ASM_F_XOR_NOT (gx23, gx9, gx22);	/*          J     */  /* F */

	ASM_AND_NOT (x10, a2, x9);		/*           K    */
	ASM_AND_NOT (x15, x3, x14);		/*          J     */
    ASM_A_LOAD (t_key5, offsets->Key_Ptrs[OFFSET1 + 5]);	/* PIPELINE */
	ASM_F_AND (gx48, g_a3, gx22);		/*   C       J    */  /* F */

	ASM_XOR (x11, x5, x10);			/*           K    */
	ASM_XOR (x16, x15, x7);			/*          J     */
    ASM_D_LOAD (t_k3, ((INNER_LOOP_SLICE *)t_key3)[0]);		/* PIPELINE */
	ASM_F_XOR (Preloadg_1, Preloadg_1, gx19); /*            L   */  /* F */

	ASM_AND_NOT (x17, a2, x16);		/*          J     */
	ASM_XOR (x22, x21, x3);			/*         I      */
    ASM_D_LOAD (t_k5, ((INNER_LOOP_SLICE *)t_key5)[0]);		/* PIPELINE */
	ASM_F_OR (gx34, g_a2, gx24);		/*      F         */  /* F */

	ASM_XOR (x18, x14, x17);		/*          J     */
/**/	ASM_AND (x26, a1, x21);			/*        H       */
    ASM_A_LOAD (t_key4, offsets->Key_Ptrs[OFFSET1 + 4]);	/* PIPELINE */
	ASM_F_XOR_NOT (gx35, gx34, gx19);	/*      F         */  /* F */

	ASM_OR (x19, a6, x18);			/*          J     */
/**/	ASM_AND_NOT (x23, x11, a4);		/*       G        */
	ASM_D_LOAD (Preload1_8, merge[D81]);	/*                */
	ASM_F_OR (gx36, g_a6, gx35);		/*      F         */  /* F */

	ASM_XOR_NOT (x20, x11, x19);		/*          JK    */
	ASM_XOR (x28, x27, x23);		/*      F         */
    ASM_D_LOAD (t_k4, ((INNER_LOOP_SLICE *)t_key4)[0]);		/* PIPELINE */
	ASM_F_XOR (gx27, g_a3, gx3);		/*         I      */  /* F */

	ASM_XOR (Preload1_8, Preload1_8, x20);	/*          J     */
	ASM_D_STORE (out[D81], Preload1_8);	/*          J     */
	ASM_AND (x29, a2, x28);			/*      F         */
	ASM_F_AND (gx28, gx27, g_a2);		/*         I      */  /* F */

	ASM_AND_NOT (x24, a2, x23);		/*       G        */
	ASM_XOR (x30, x26, x29);		/*      F H       */
    ASM_A_LOAD (t_key6, offsets->Key_Ptrs[OFFSET1 + 6]);	/* PIPELINE */
	ASM_F_AND_NOT (gx29, g_a6, gx28);	/*         I      */  /* F */

	ASM_XOR (x25, x22, x24);		/*       G I      */
	ASM_AND_NOT (x31, x30, a6);		/*      F         */
	ASM_D_LOAD (Preload3_8, merge[D83]);	/*                */
	ASM_F_OR (gx25, g_a3, gx3);		/*           K    */  /* F */

	ASM_XOR (x32, x25, x31);		/*      FG        */
	ASM_AND_NOT (x33, a3, x16);		/*    D           */
    ASM_D_LOAD (t_k6, ((INNER_LOOP_SLICE *)t_key6)[0]);		/* PIPELINE */
	ASM_F_XOR (gx26, gx24, gx25);		/*           K    */  /* F */

	ASM_XOR (Preload3_8, Preload3_8, x32);	/*      F         */
	ASM_D_STORE (out[D83], Preload3_8);	/*      F         */
	ASM_OR (x34, x9, x33);			/*    D           */
	ASM_F_AND_NOT (gx38, gx26, g_a3);	/*     E G        */  /* F */

	ASM_AND_NOT (x37, x2, x14);		/*     E          */
	ASM_OR (x38, x22, x32);			/*   C            */
    ASM_D_LOAD (t_a3, in[S13]);				/* PIPELINE */
	ASM_F_AND_NOT (gx45, gx24, gx44);	/* A              */  /* F */

	ASM_XOR (x45, a3, a5);			/* A              */
	ASM_AND_NOT (x39, a2, x38);		/*   C            */
    ASM_D_LOAD (t_a5, in[S15]);				/* PIPELINE */
	ASM_F_AND (gx46, g_a6, gx45);		/* A              */  /* F */

	ASM_XOR (x40, x37, x39);		/*   C E          */
	ASM_XOR (x46, x45, x37);		/* A              */
    ASM_F_D_LOAD (Preloadg_4, merge[D74]);	/*                */  /* F */
	ASM_F_XOR (gx30, gx26, gx29);		/*         I K    */  /* F */

	ASM_OR (x41, a6, x40);			/*   C            */
	ASM_AND_NOT (x47, x46, a2);		/* A              */
	ASM_D_LOAD (Preload2_8, merge[D82]);	/*                */
	ASM_F_XOR_NOT (gx33, gx7, gx30);	/*        H       */  /* F */

	ASM_XOR (x36, x34, x35);		/*    D           */
	ASM_OR (x44, x43, a4);			/*  B             */
    ASM_D_LOAD (a4, in[S14]);				/* PIPELINE */
	ASM_F_OR (gx31, g_a1, gx30);		/*         I      */  /* F */

	ASM_XOR_NOT (x42, x36, x41);		/*   CD           */
	ASM_XOR (x48, x44, x47);		/* AB             */
	ASM_D_LOAD (Preload4_8, merge[D84]);	/*                */
	ASM_F_XOR (gx32, gx23, gx31);		/*         IJ     */  /* F */

	ASM_XOR (Preload2_8, Preload2_8, x42);	/*   C            */
	ASM_D_STORE (out[D82], Preload2_8);	/*   C            */
	ASM_AND (x49, a6, x48);			/* A              */
	ASM_F_XOR (Preloadg_2, Preloadg_2, gx32); /*         I      */  /* F */

    ASM_XOR (a3, t_a3, t_k3);				/* PIPELINE */
	ASM_XOR_NOT (x50, x11, x49);		/* A              */
    ASM_D_LOAD (a6, in[S16]);				/* PIPELINE */
	ASM_F_XOR (gx43, gx42, gx33);		/*    D           */  /* F */

	ASM_XOR (Preload4_8, Preload4_8, x50);	/* A              */
	ASM_D_STORE (out[D84], Preload4_8);	/* A              */       
    ASM_XOR (a5, t_a5, t_k5);				/* PIPELINE */
	ASM_F_XOR (gx37, gx33, gx36);		/*      F H       */  /* F */

    ASM_XOR (a4, a4, t_k4);				/* PIPELINE */
    ASM_XOR (a6, a6, t_k6);				/* PIPELINE */

    ASM_COMMENT_END_INCLUDE (end_of_s8_1);

#include "s0.h"
/* end of s8_1.h  */
