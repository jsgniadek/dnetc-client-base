; this was generated by gcc272 -O6 -unroll-loops
; the nasm output is op-for-op identical to the original gas output.
;
; This code is slightly faster than that obtained
; from gcc295 (-02 -mcpu=i[3|6]86 -unroll-loops)
; if your compiler can generate faster code please send it to me
; for disassembly.
;
; $Id: ogr.asm,v 1.1.2.1 2000/08/16 02:32:29 cyp Exp $

               global ogr_get_dispatch_table, _ogr_get_dispatch_table
               extern ogr_choose_dat

%ifdef __OMF__  ; Watcom/OS/2 and Borland
[SECTION _TEXT CLASS=CODE USE32 FLAT PUBLIC ALIGN=16]
%else
[SECTION .text]
%endif

%define offset
%define ptr
%define memset my_memset

%macro calign 1
  %assign sz  0
  %if %1 > 0
    %%szx equ ($ - $$)
    %assign sz (%%szx & (%1 - 1))
    %if sz != 0
      %assign sz %1 - sz
    %endif
  %endif
  %assign edinext 0
  %rep %1
    %assign edinext 0 ; always esi only for gas compatibility
    %if sz >= 7
      db 0x8D,0xB4,0x26,0x00,0x00,0x00,0x00  ; lea       esi,[esi]
      %assign sz sz-7
      %assign edinext 1
    %elif sz >= 6 && edinext != 0
      db 0x8d,0xBf,0x00,0x00,0x00,0x00       ; lea       edi,[edi]
      %assign edinext 0
      %assign sz sz-6
    %elif sz >= 6
      db 0x8D,0xB6,0x00,0x00,0x00,0x00       ; lea       esi,[esi]
      %assign edinext 1
      %assign sz sz-6
    %elif sz >= 4   
      db 0x8D,0x74,0x26,0x00                 ; lea       esi,[esi]
      %assign sz sz-4
      %assign edinext 1
    %elif sz >= 3 && edinext != 0
      db 0x8d,0x7f,0x00                      ; lea       edi,[edi]
      %assign sz sz-3
      %assign edinext 0
    %elif sz >= 3
      db 0x8D,0x76,0x00                      ; lea       esi,[esi] 
      %assign sz sz-3
      %assign edinext 1
    %elif sz >= 2 && edinext != 0
      db 0x8d,0x3f                           ; lea       edi,[edi]
      %assign sz sz-2
      %assign edinext 0
    %elif sz >= 2
      ;db 0x8D,0x36                          ; gas 2.7: lea esi,[esi]     
      mov esi,esi                            ; gas 2.9: mov esi,esi   
      %assign sz sz-2
      %assign edinext 1
    %elif sz >= 1
      nop
      %assign sz sz-1
    %else 
      %exitrep
    %endif
  %endrep  
%endmacro

%macro CALIGN 1  ; gcc 272 alignment is always 4 bytes
   calign 4
%endmacro

%macro gas_xxx 3 ;
  %ifnum %3
    %if %3 < 0x80
       %1 %2,BYTE %3
    %else 
      %assign j (%3 & 0xff)
      %assign m (%3 - j)
      %if m == 0 && j < 0x80
        %1 %2,BYTE j
      %elif m != 0xffffff00
        %1 %2,%3
      %elif j > 0x80
        %assign j (0 - j)
        %1 %2,BYTE -j
      %else 
        %1 %2,BYTE j
      %endif
    %endif
  %else
    %1 %2,%3
  %endif
%endmacro

%macro gas_sub 2 ;
  gas_xxx sub,%1,%2
%endmacro
%macro gas_add 2 ;
  gas_xxx add,%1,%2
%endmacro
%macro gas_cmp 2 ;
  gas_xxx cmp,%1,%2
%endmacro
%macro gas_and 2 ;
  gas_xxx and,%1,%2
%endmacro
%macro gas_sar 2
  %ifnum %2
    %ifidni %1,eax
      db 0xc1,0xf8,%2
    %elifidni %1,edx
      db 0xc1,0xfa,%2   
    %else
      sar %1,%2
    %endif
  %else
    sar %1,%2
  %endif
%endmacro
%macro gas_push 1+
  %ifnum %1
    %if %1 < 0x80
      push BYTE %1
    %else 
      push dword %1
    %endif
  %else
    push %1
  %endif
%endmacro

init_load_choose:
    gas_cmp       byte ptr [ogr_choose_dat+0x2],0x0c
    jne       X$1
    xor       eax,eax
    ret       
X$1:
    mov       eax,0xfffffffd
    ret       
    

    CALIGN 32
found_one:
    gas_sub       esp,0x00000084
    gas_push      ebp
    gas_push      edi
    gas_push      esi
    gas_push      ebx
    mov       ebx,dword ptr [esp+0x98]
    mov       ebx,dword ptr [ebx+0x8]
    mov       dword ptr [esp+0x18],ebx
    mov       ebx,dword ptr [esp+0x98]
    mov       ebx,dword ptr [ebx+0xc]
    mov       dword ptr [esp+0x14],ebx
    gas_push      0x00000078
    gas_push      0x00000000
    lea       eax,[esp+0x24]
    gas_push      eax
    call      near ptr memset
    mov       edi,0x00000001
    gas_add       esp,0x0000000c
X$2:
    gas_cmp       dword ptr [esp+0x14],edi
    jle       X$7
    mov       ebx,dword ptr [esp+0x98]
    mov       ebp,dword ptr [ebx+edi*4+0x20]
    xor       esi,esi
    CALIGN 4
X$3:
    gas_cmp       esi,edi
    jge       X$6
    mov       edx,ebp
    mov       ebx,dword ptr [esp+0x98]
    gas_sub       edx,dword ptr [ebx+esi*4+0x20]
    lea       eax,[edx+edx]
    gas_cmp       dword ptr [esp+0x18],eax
    jl        X$5
    gas_cmp       edx,0x00000040
    jle       X$6
    gas_add       edx,0xffffffc0
    mov       ebx,edx
    gas_sar       ebx,0x00000003
    mov       dword ptr [esp+0x10],ebx
    mov       ecx,edx
    gas_and       ecx,0x00000007
    mov       ebx,0x00000001
    shl       ebx,cl
    mov       ecx,ebx
    mov       ebx,dword ptr [esp+0x10]
    mov       dl,byte ptr [esp+ebx+0x1c]
    movsx     eax,dl
    test      ecx,eax
    je        X$4
    xor       eax,eax
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000084
    ret       
    CALIGN 4
X$4:
    or        dl,cl
    mov       ebx,dword ptr [esp+0x10]
    mov       byte ptr [esp+ebx+0x1c],dl
X$5:
    inc       esi
    jmp       short X$3
    CALIGN 4
X$6:
    inc       edi
    jmp       short X$2
    CALIGN 4
X$7:
    mov       eax,0x00000001
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000084
    ret       


ogr_init:
    call      near ptr init_load_choose
    test      eax,eax
    jne       X$8
    xor       eax,eax
    ret       
X$8:
    ret       
    CALIGN 4

ogr_create:
    gas_sub       esp,0x00000018
    gas_push      ebp
    gas_push      edi
    gas_push      esi
    gas_push      ebx
    mov       ecx,dword ptr [esp+0x2c]
    mov       dword ptr [esp+0x24],ecx
    test      ecx,ecx
    je        X$9
    gas_cmp       dword ptr [esp+0x30],0x0000001c
    je        X$10
X$9:
    mov       eax,0xfffffffd
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000018
    ret       
    CALIGN 4
X$10:
    gas_cmp       dword ptr [esp+0x38],0x00000c0f
    jbe       X$9
    mov       edi,dword ptr [esp+0x34]
    test      edi,edi
    jne       X$11
    mov       eax,0xffffffff
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000018
    ret       

    CALIGN 4
X$11:
    gas_push      0x00000c10
    gas_push      0x00000000
    gas_push      edi
    call      near ptr memset
    mov       ebx,dword ptr [esp+0x30]
    movzx     eax,word ptr [ebx]
    mov       dword ptr [edi+0xc],eax
    dec       eax
    mov       dword ptr [edi+0x10],eax
    mov       eax,dword ptr [edi+0xc]
    gas_add       esp,0x0000000c
    gas_cmp       eax,0x0000001d
    ja        X$9
    mov       eax,dword ptr [eax*4+OGR+0xfffffffc]
    mov       dword ptr [edi+0x8],eax
    mov       eax,dword ptr [edi+0xc]
    inc       eax
    mov       edx,eax
    gas_sar       edx,0x00000001
    lea       eax,[edx-0x1]
    mov       dword ptr [edi+0x18],eax
    mov       dword ptr [edi+0x1c],eax
    test      byte ptr [edi+0xc],0x01
    jne       X$12
    mov       dword ptr [edi+0x1c],edx
X$12:
    dec       dword ptr [edi+0x18]
    inc       dword ptr [edi+0x1c]
    mov       eax,dword ptr [edi+0x8]
    gas_add       eax,0xfffffffc
    gas_sar       eax,0x00000001
    mov       dword ptr [edi+0x14],eax
    test      byte ptr [edi+0xc],0x01
    jne       X$13
    mov       eax,dword ptr [edi+0x8]
    gas_add       eax,0xfffffff9
    gas_sar       eax,0x00000001
    mov       dword ptr [edi+0x14],eax
X$13:
    mov       dword ptr [edi+0xc8],0x00000001
    mov       ecx,dword ptr [esp+0x24]
    mov       ecx,dword ptr [ecx+0x18]
    mov       dword ptr [esp+0x1c],ecx
    mov       ebx,dword ptr [esp+0x24]
    movzx     eax,word ptr [ebx+0x2]
    gas_cmp       ecx,eax
    jge       X$14
    mov       dword ptr [esp+0x1c],eax
X$14:
    gas_cmp       dword ptr [esp+0x1c],0x0000000a
    jg        near X$9
    lea       ecx,[edi+0x118]
    mov       dword ptr [esp+0x14],ecx
    mov       dword ptr [esp+0x20],0x00000000
    mov       dword ptr [esp+0x18],edi
    CALIGN 4
X$15:
    mov       ebx,dword ptr [esp+0x1c]
    gas_cmp       dword ptr [esp+0x20],ebx
    jge       near X$26
    mov       edx,dword ptr [edi+0xc8]
    gas_cmp       dword ptr [edi+0x1c],edx
    jl        X$18
    mov       esi,dword ptr [edi+0x18]
    gas_cmp       edx,esi
    jg        X$16
    mov       eax,dword ptr [edi+0x10]
    gas_sub       eax,edx
    mov       edx,dword ptr [edi+0x8]
    gas_sub       edx,dword ptr [eax*4+OGR]
    mov       eax,dword ptr [edi+0x14]
    jmp       short X$17
X$16:
    mov       ecx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ecx+0x14]
    shr       eax,0x00000014
    mov       ebx,dword ptr [edi+0x10]
    gas_sub       ebx,edx
    mov       edx,ebx
    lea       eax,[eax+eax*2]
    movzx     edx,byte ptr [edx+eax*4+ogr_choose_dat+0x3]
    mov       eax,dword ptr [edi+0x8]
    mov       ecx,eax
    gas_sub       ecx,edx
    mov       edx,ecx
    gas_sub       eax,dword ptr [edi+esi*4+0x20]
    dec       eax
X$17:
    gas_cmp       edx,eax
    jle       X$19
    mov       edx,eax
    jmp       short X$19
    mov       esi,esi
X$18:
    mov       ebx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ebx+0x14]
    shr       eax,0x00000014
    mov       ecx,dword ptr [edi+0x10]
    gas_sub       ecx,edx
    mov       edx,ecx
    lea       eax,[eax+eax*2]
    movzx     eax,byte ptr [edx+eax*4+ogr_choose_dat+0x3]
    mov       edx,dword ptr [edi+0x8]
    gas_sub       edx,eax
X$19:
    mov       ebx,dword ptr [esp+0x14]
    mov       dword ptr [ebx+0x44],edx
    mov       ecx,dword ptr [esp+0x20]
    mov       ebx,dword ptr [esp+0x24]
    movzx     ebp,word ptr [ebx+ecx*2+0x4]
    mov       ebx,dword ptr [edi+ecx*4+0x20]
    gas_add       ebx,ebp
    mov       ecx,dword ptr [esp+0x18]
    mov       dword ptr [ecx+0x24],ebx
    mov       ecx,dword ptr [esp+0x14]
    gas_add       dword ptr [ecx+0x40],ebp
    mov       esi,ebp
    gas_cmp       ebp,0x0000001f
    jle       near X$23
    mov       eax,0x0000001f
    gas_sub       eax,ebp
    gas_and       eax,0x0000007f
    je        near X$22
    gas_cmp       eax,0x00000060
    jge       near X$21
    gas_cmp       eax,0x00000040
    jge       X$20
    gas_cmp       eax,0x0000001f
    jle       near X$22
    mov       eax,dword ptr [ecx+0x2c]
    mov       dword ptr [ecx+0x28],eax
    mov       eax,dword ptr [ecx+0x30]
    mov       dword ptr [ecx+0x2c],eax
    mov       eax,dword ptr [ecx+0x34]
    mov       dword ptr [ecx+0x30],eax
    mov       eax,dword ptr [ecx+0x38]
    mov       dword ptr [ecx+0x34],eax
    mov       dword ptr [ecx+0x38],0x00000000
    mov       eax,dword ptr [ecx+0xc]
    mov       dword ptr [ecx+0x10],eax
    mov       eax,dword ptr [ecx+0x8]
    mov       dword ptr [ecx+0xc],eax
    mov       eax,dword ptr [ecx+0x4]
    mov       dword ptr [ecx+0x8],eax
    mov       eax,dword ptr [ecx]
    mov       dword ptr [ecx+0x4],eax
    mov       dword ptr [ecx],0x00000000
    lea       esi,[ebp-0x20]
X$20:
    mov       ebx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    gas_add       esi,0xffffffe0
X$21:
    mov       ecx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ecx+0x2c]
    mov       dword ptr [ecx+0x28],eax
    mov       eax,dword ptr [ecx+0x30]
    mov       dword ptr [ecx+0x2c],eax
    mov       eax,dword ptr [ecx+0x34]
    mov       dword ptr [ecx+0x30],eax
    mov       eax,dword ptr [ecx+0x38]
    mov       dword ptr [ecx+0x34],eax
    mov       dword ptr [ecx+0x38],0x00000000
    mov       eax,dword ptr [ecx+0xc]
    mov       dword ptr [ecx+0x10],eax
    mov       eax,dword ptr [ecx+0x8]
    mov       dword ptr [ecx+0xc],eax
    mov       eax,dword ptr [ecx+0x4]
    mov       dword ptr [ecx+0x8],eax
    mov       eax,dword ptr [ecx]
    mov       dword ptr [ecx+0x4],eax
    mov       dword ptr [ecx],0x00000000
    gas_add       esi,0xffffffe0
    gas_cmp       esi,0x0000001f
    jle       near X$23
    CALIGN 4
X$22:
    mov       ebx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    gas_add       esi,0xffffff80
    gas_cmp       esi,0x0000001f
    jg        near X$22
X$23:
    test      esi,esi
    jle       near X$24
    mov       ecx,0x00000020
    gas_sub       ecx,esi
    mov       dword ptr [esp+0x10],ecx
    mov       ecx,dword ptr [esp+0x14]
    mov       edx,dword ptr [ecx+0x28]
    mov       ecx,esi
    shl       edx,cl
    mov       ecx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ecx+0x2c]
    mov       ecx,dword ptr [esp+0x10]
    shr       eax,cl
    or        edx,eax
    mov       ebx,dword ptr [esp+0x14]
    mov       dword ptr [ebx+0x28],edx
    mov       edx,dword ptr [ebx+0x2c]
    mov       ecx,esi
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x30]
    mov       ecx,dword ptr [esp+0x10]
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x2c],edx
    mov       edx,dword ptr [ebx+0x30]
    mov       ecx,esi
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x34]
    mov       ecx,dword ptr [esp+0x10]
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x30],edx
    mov       edx,dword ptr [ebx+0x34]
    mov       ecx,esi
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x38]
    mov       ecx,dword ptr [esp+0x10]
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x34],edx
    mov       ecx,esi
    shl       dword ptr [ebx+0x38],cl
    mov       edx,dword ptr [ebx+0x10]
    shr       edx,cl
    mov       eax,dword ptr [ebx+0xc]
    mov       ecx,dword ptr [esp+0x10]
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x10],edx
    mov       edx,dword ptr [ebx+0xc]
    mov       ecx,esi
    shr       edx,cl
    mov       eax,dword ptr [ebx+0x8]
    mov       ecx,dword ptr [esp+0x10]
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0xc],edx
    mov       edx,dword ptr [ebx+0x8]
    mov       ecx,esi
    shr       edx,cl
    mov       eax,dword ptr [ebx+0x4]
    mov       ecx,dword ptr [esp+0x10]
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x8],edx
    mov       edx,dword ptr [ebx+0x4]
    mov       ecx,esi
    shr       edx,cl
    mov       eax,dword ptr [ebx]
    mov       ecx,dword ptr [esp+0x10]
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x4],edx
    mov       ecx,esi
    shr       dword ptr [ebx],cl
X$24:
    mov       esi,dword ptr [esp+0x14]
    gas_add       esi,0x00000048
    mov       ebx,dword ptr [esp+0x14]
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x48],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x4c],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0x50],eax
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x54],eax
    mov       eax,dword ptr [ebx+0x10]
    mov       dword ptr [ebx+0x58],eax
    gas_cmp       ebp,0x000000a0
    jg        X$25
    lea       eax,[ebp-0x1]
    mov       edx,eax
    gas_sar       edx,0x00000005
    mov       ebx,eax
    gas_and       ebx,0x0000001f
    mov       eax,0x80000000
    mov       ecx,ebx
    shr       eax,cl
    or        dword ptr [esi+edx*4],eax
X$25:
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x14]
    or        ecx,dword ptr [ebx+0x48]
    mov       dword ptr [ebx+0x5c],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x18]
    or        ecx,dword ptr [ebx+0x4c]
    mov       dword ptr [ebx+0x60],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x1c]
    or        ecx,dword ptr [ebx+0x50]
    mov       dword ptr [ebx+0x64],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x20]
    or        ecx,dword ptr [ebx+0x54]
    mov       dword ptr [ebx+0x68],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x24]
    or        ecx,dword ptr [ebx+0x58]
    mov       dword ptr [ebx+0x6c],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x28]
    or        ecx,dword ptr [ebx+0x5c]
    mov       dword ptr [ebx+0x70],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x2c]
    or        ecx,dword ptr [ebx+0x60]
    mov       dword ptr [ebx+0x74],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x30]
    or        ecx,dword ptr [ebx+0x64]
    mov       dword ptr [ebx+0x78],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x34]
    or        ecx,dword ptr [ebx+0x68]
    mov       dword ptr [ebx+0x7c],ecx
    mov       ebx,dword ptr [esp+0x14]
    mov       ecx,dword ptr [ebx+0x38]
    or        ecx,dword ptr [ebx+0x6c]
    mov       dword ptr [ebx+0x80],ecx
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [ebx+0x84],eax
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [ebx+0x88],eax
    mov       dword ptr [esp+0x14],esi
    inc       dword ptr [edi+0xc8]
    gas_add       dword ptr [esp+0x18],0x00000004
    inc       dword ptr [esp+0x20]
    jmp       near ptr X$15
    CALIGN 4
X$26:
    dec       dword ptr [edi+0xc8]
    mov       ebx,dword ptr [esp+0x24]
    movzx     ebx,word ptr [ebx+0x2]
    mov       dword ptr [edi+0xc4],ebx
    xor       eax,eax
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000018
    ret       
    CALIGN 4

ogr_cycle:
    gas_sub       esp,0x00000018
    gas_push      ebp
    gas_push      edi
    gas_push      esi
    gas_push      ebx
    mov       edi,dword ptr [esp+0x2c]
    mov       ebp,dword ptr [edi+0xc8]
    inc       ebp
    lea       eax,[ebp+ebp*8]
    lea       ebx,[edi+eax*8+0xd0]
    mov       dword ptr [esp+0x24],0x00000000
    mov       ecx,dword ptr [esp+0x30]
    mov       ecx,dword ptr [ecx]
    mov       dword ptr [esp+0x20],ecx
    mov       dword ptr [esp+0x1c],0x00000001
    CALIGN 4
X$27:
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [edi+ebp*4+0x1c],eax
    gas_cmp       dword ptr [edi+0x1c],ebp
    jl        X$29
    mov       esi,dword ptr [edi+0x18]
    mov       dword ptr [esp+0x10],esi
    gas_cmp       ebp,esi
    jg        X$28
    mov       ecx,dword ptr [esp+0x20]
    gas_cmp       dword ptr [esp+0x24],ecx
    jge       near X$39
    mov       eax,dword ptr [edi+0x10]
    gas_sub       eax,ebp
    mov       esi,dword ptr [edi+0x8]
    gas_sub       esi,dword ptr [eax*4+OGR]
    mov       dword ptr [esp+0x18],esi
    mov       eax,dword ptr [edi+0x14]
    gas_cmp       esi,eax
    jle       X$30
    mov       dword ptr [esp+0x18],eax
    jmp       short X$30
    CALIGN 4
X$28:
    mov       eax,dword ptr [ebx+0x14]
    shr       eax,0x00000014
    mov       edx,dword ptr [edi+0x10]
    gas_sub       edx,ebp
    lea       eax,[eax+eax*2]
    movzx     edx,byte ptr [edx+eax*4+ogr_choose_dat+0x3]
    mov       eax,dword ptr [edi+0x8]
    mov       ecx,eax
    gas_sub       ecx,edx
    mov       dword ptr [esp+0x18],ecx
    mov       esi,dword ptr [esp+0x10]
    gas_sub       eax,dword ptr [edi+esi*4+0x20]
    dec       eax
    gas_cmp       ecx,eax
    jle       X$30
    mov       dword ptr [esp+0x18],eax
    jmp       short X$30
X$29:
    mov       eax,dword ptr [ebx+0x14]
    shr       eax,0x00000014
    mov       edx,dword ptr [edi+0x10]
    gas_sub       edx,ebp
    lea       eax,[eax+eax*2]
    movzx     eax,byte ptr [edx+eax*4+ogr_choose_dat+0x3]
    mov       ecx,dword ptr [edi+0x8]
    gas_sub       ecx,eax
    mov       dword ptr [esp+0x18],ecx
X$30:
    inc       dword ptr [esp+0x24]
X$31:
    mov       edx,dword ptr [ebx+0x28]
    gas_cmp       edx,0xfffffffd
    ja        near X$33
    not       edx
    mov       esi,0x00000021
    bsr       edx,edx
    je        X$32
    gas_sub       esi,edx
    dec       esi
X$32:
    mov       dword ptr [esp+0x14],esi
    mov       eax,esi
    gas_add       eax,dword ptr [ebx+0x40]
    mov       dword ptr [ebx+0x40],eax
    gas_cmp       dword ptr [esp+0x18],eax
    jl        near X$37
    mov       esi,0x00000020
    gas_sub       esi,dword ptr [esp+0x14]
    mov       edx,dword ptr [ebx+0x28]
    mov       ecx,dword ptr [esp+0x14]
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x2c]
    mov       ecx,esi
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x28],edx
    mov       edx,dword ptr [ebx+0x2c]
    mov       ecx,dword ptr [esp+0x14]
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x30]
    mov       ecx,esi
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x2c],edx
    mov       edx,dword ptr [ebx+0x30]
    mov       ecx,dword ptr [esp+0x14]
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x34]
    mov       ecx,esi
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x30],edx
    mov       edx,dword ptr [ebx+0x34]
    mov       ecx,dword ptr [esp+0x14]
    shl       edx,cl
    mov       eax,dword ptr [ebx+0x38]
    mov       ecx,esi
    shr       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x34],edx
    mov       ecx,dword ptr [esp+0x14]
    shl       dword ptr [ebx+0x38],cl
    mov       edx,dword ptr [ebx+0x10]
    shr       edx,cl
    mov       eax,dword ptr [ebx+0xc]
    mov       ecx,esi
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x10],edx
    mov       edx,dword ptr [ebx+0xc]
    mov       ecx,dword ptr [esp+0x14]
    shr       edx,cl
    mov       eax,dword ptr [ebx+0x8]
    mov       ecx,esi
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0xc],edx
    mov       edx,dword ptr [ebx+0x8]
    mov       ecx,dword ptr [esp+0x14]
    shr       edx,cl
    mov       eax,dword ptr [ebx+0x4]
    mov       ecx,esi
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x8],edx
    mov       edx,dword ptr [ebx+0x4]
    mov       ecx,dword ptr [esp+0x14]
    shr       edx,cl
    mov       eax,dword ptr [ebx]
    mov       ecx,esi
    shl       eax,cl
    or        edx,eax
    mov       dword ptr [ebx+0x4],edx
    mov       ecx,dword ptr [esp+0x14]
    shr       dword ptr [ebx],cl
    jmp       short X$34
    CALIGN 4
X$33:
    mov       eax,dword ptr [ebx+0x40]
    gas_add       eax,0x00000020
    mov       dword ptr [ebx+0x40],eax
    gas_cmp       dword ptr [esp+0x18],eax
    jl        near X$37
    mov       eax,dword ptr [ebx+0x2c]
    mov       dword ptr [ebx+0x28],eax
    mov       eax,dword ptr [ebx+0x30]
    mov       dword ptr [ebx+0x2c],eax
    mov       eax,dword ptr [ebx+0x34]
    mov       dword ptr [ebx+0x30],eax
    mov       eax,dword ptr [ebx+0x38]
    mov       dword ptr [ebx+0x34],eax
    mov       dword ptr [ebx+0x38],0x00000000
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x10],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0xc],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x8],eax
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x4],eax
    mov       dword ptr [ebx],0x00000000
    gas_cmp       edx,0xffffffff
    je        near X$31
X$34:
    gas_cmp       dword ptr [edi+0x10],ebp
    jne       X$35
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [edi+ebp*4+0x20],eax
    gas_push      edi
    call      near ptr found_one
    gas_add       esp,0x00000004
    test      eax,eax
    je        near X$31
    mov       dword ptr [esp+0x1c],0x00000002
    jmp       near ptr X$39
    mov       esi,esi
X$35:
    lea       esi,[ebx+0x48]
    mov       dword ptr [esp+0x10],esi
    mov       edx,dword ptr [ebx+0x40]
    gas_sub       edx,dword ptr [ebx+0x3c]
    mov       eax,dword ptr [ebx]
    mov       dword ptr [ebx+0x48],eax
    mov       eax,dword ptr [ebx+0x4]
    mov       dword ptr [ebx+0x4c],eax
    mov       eax,dword ptr [ebx+0x8]
    mov       dword ptr [ebx+0x50],eax
    mov       eax,dword ptr [ebx+0xc]
    mov       dword ptr [ebx+0x54],eax
    mov       eax,dword ptr [ebx+0x10]
    mov       dword ptr [ebx+0x58],eax
    gas_cmp       edx,0x000000a0
    jg        X$36
    lea       eax,[edx-0x1]
    mov       edx,eax
    gas_sar       edx,0x00000005
    gas_and       eax,0x0000001f
    mov       dword ptr [esp+0x14],eax
    mov       eax,0x80000000
    mov       ecx,dword ptr [esp+0x14]
    shr       eax,cl
    or        dword ptr [esi+edx*4],eax
X$36:
    mov       esi,dword ptr [ebx+0x14]
    or        esi,dword ptr [ebx+0x48]
    mov       dword ptr [ebx+0x5c],esi
    mov       ecx,dword ptr [ebx+0x18]
    or        ecx,dword ptr [ebx+0x4c]
    mov       dword ptr [ebx+0x60],ecx
    mov       esi,dword ptr [ebx+0x1c]
    or        esi,dword ptr [ebx+0x50]
    mov       dword ptr [ebx+0x64],esi
    mov       ecx,dword ptr [ebx+0x20]
    or        ecx,dword ptr [ebx+0x54]
    mov       dword ptr [ebx+0x68],ecx
    mov       esi,dword ptr [ebx+0x24]
    or        esi,dword ptr [ebx+0x58]
    mov       dword ptr [ebx+0x6c],esi
    mov       ecx,dword ptr [ebx+0x28]
    or        ecx,dword ptr [ebx+0x5c]
    mov       dword ptr [ebx+0x70],ecx
    mov       esi,dword ptr [ebx+0x2c]
    or        esi,dword ptr [ebx+0x60]
    mov       dword ptr [ebx+0x74],esi
    mov       ecx,dword ptr [ebx+0x30]
    or        ecx,dword ptr [ebx+0x64]
    mov       dword ptr [ebx+0x78],ecx
    mov       esi,dword ptr [ebx+0x34]
    or        esi,dword ptr [ebx+0x68]
    mov       dword ptr [ebx+0x7c],esi
    mov       ecx,dword ptr [ebx+0x38]
    or        ecx,dword ptr [ebx+0x6c]
    mov       dword ptr [ebx+0x80],ecx
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [ebx+0x84],eax
    mov       eax,dword ptr [ebx+0x40]
    mov       dword ptr [ebx+0x88],eax
    mov       esi,dword ptr [esp+0x18]
    mov       dword ptr [ebx+0x44],esi
    mov       ebx,dword ptr [esp+0x10]
    inc       ebp
    jmp       near ptr X$27
    CALIGN 4
X$37:
    gas_add       ebx,0xffffffb8
    dec       ebp
    gas_cmp       dword ptr [edi+0xc4],ebp
    jge       X$38
    mov       ecx,dword ptr [ebx+0x44]
    mov       dword ptr [esp+0x18],ecx
    jmp       near ptr X$31
X$38:
    mov       dword ptr [esp+0x1c],0x00000000
X$39:
    fld       qword ptr [edi]
    fiadd     dword ptr [esp+0x24]
    fstp      qword ptr [edi]
    dec       ebp
    mov       dword ptr [edi+0xc8],ebp
    mov       ecx,dword ptr [esp+0x24]
    mov       esi,dword ptr [esp+0x30]
    mov       dword ptr [esi],ecx
    mov       eax,dword ptr [esp+0x1c]
    pop       ebx
    pop       esi
    pop       edi
    pop       ebp
    gas_add       esp,0x00000018
    ret       
    CALIGN 4
ogr_getresult:
    gas_push      ebx
    mov       edx,dword ptr [esp+0x8]
    mov       ecx,dword ptr [esp+0xc]
    gas_cmp       dword ptr [esp+0x10],0x0000001c
    je        X$40
    mov       eax,0xfffffffd
    pop       ebx
    ret       
    CALIGN 4
X$40:
    mov       eax,dword ptr [edx+0xc]
    mov       word ptr [ecx],ax
    mov       eax,dword ptr [edx+0xc4]
    mov       word ptr [ecx+0x2],ax
    mov       bx,word ptr [edx+0x24]
    gas_sub       bx,word ptr [edx+0x20]
    mov       word ptr [ecx+0x4],bx
    mov       bx,word ptr [edx+0x28]
    gas_sub       bx,word ptr [edx+0x24]
    mov       word ptr [ecx+0x6],bx
    mov       bx,word ptr [edx+0x2c]
    gas_sub       bx,word ptr [edx+0x28]
    mov       word ptr [ecx+0x8],bx
    mov       bx,word ptr [edx+0x30]
    gas_sub       bx,word ptr [edx+0x2c]
    mov       word ptr [ecx+0xa],bx
    mov       bx,word ptr [edx+0x34]
    gas_sub       bx,word ptr [edx+0x30]
    mov       word ptr [ecx+0xc],bx
    mov       bx,word ptr [edx+0x38]
    gas_sub       bx,word ptr [edx+0x34]
    mov       word ptr [ecx+0xe],bx
    mov       bx,word ptr [edx+0x3c]
    gas_sub       bx,word ptr [edx+0x38]
    mov       word ptr [ecx+0x10],bx
    mov       bx,word ptr [edx+0x40]
    gas_sub       bx,word ptr [edx+0x3c]
    mov       word ptr [ecx+0x12],bx
    mov       bx,word ptr [edx+0x44]
    gas_sub       bx,word ptr [edx+0x40]
    mov       word ptr [ecx+0x14],bx
    mov       bx,word ptr [edx+0x48]
    gas_sub       bx,word ptr [edx+0x44]
    mov       word ptr [ecx+0x16],bx
    mov       eax,dword ptr [edx+0xc8]
    mov       dword ptr [ecx+0x18],eax
    gas_cmp       eax,0x0000000a
    jbe       X$41
    mov       dword ptr [ecx+0x18],0x0000000a
X$41:
    xor       eax,eax
    pop       ebx
    ret       
    CALIGN 4

    ;---------------- end of bit compatible area -------------

ogr_destroy:
ogr_cleanup:
    xor       eax,eax
    ret       

    align 32

_dispatch_table:
    dd offset ogr_init
    dd offset ogr_create
    dd offset ogr_cycle
    dd offset ogr_getresult
    dd offset ogr_destroy
    dd offset ogr_cleanup
    dd 0,0

    align 32

ogr_get_dispatch_table:
    mov       eax,offset _dispatch_table
    ret

    CALIGN 32
my_memset:
    gas_push       edi
    gas_push       ebx
    mov        edi, [esp+0xc]
    movzx eax, BYTE [esp+0x10] ;0f b6 44 24 10
    mov        ecx, [esp+0x14]
    gas_push       edi
    cld              
    gas_cmp        ecx, 15
    jle        short .here
    mov        ah,al
    mov        edx,eax
    shl        eax,16
    mov        ax,dx
    mov        edx,edi
    neg        edx
    gas_and        edx,3
    mov        ebx,ecx
    gas_sub        ebx,edx
    mov        ecx,edx
    repz       stosb
    mov        ecx,ebx
    shr        ecx,2
    repz       stosd
    mov        ecx,ebx
    gas_and        ecx,3
.here:
    repz stosb
    pop        eax
    pop        ebx
    pop        edi
    ret              

    ;-------------------------------------------------
    align 32
OGR:
    db	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
    db	0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
    db	0x0b, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00
    db	0x19, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00
    db	0x2c, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00
    db	0x48, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00
    db	0x6a, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00
    db	0x97, 0x00, 0x00, 0x00, 0xb1, 0x00, 0x00, 0x00
    db	0xc7, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00
    db	0xf6, 0x00, 0x00, 0x00, 0x1b, 0x01, 0x00, 0x00
    db	0x4d, 0x01, 0x00, 0x00, 0x64, 0x01, 0x00, 0x00
    db	0x74, 0x01, 0x00, 0x00, 0xa9, 0x01, 0x00, 0x00
    db	0xe0, 0x01, 0x00, 0x00, 0xec, 0x01, 0x00, 0x00
    db	0x29, 0x02, 0x00, 0x00, 0x49, 0x02, 0x00, 0x00
    db	0x6f, 0x02, 0x00, 0x00,    0,0,0,0
    db  0,0,0,0, 0,0,0,0
    ;-------------------------------------------------
