; Taken from Loki_Utils
; http://www.icculus.org
;
; 1997-98 by H. Dietz and R. Fisher
; Bug fixes and SSE detection by Sam Lantinga
;
; Modified for distributed.net by Steven Nikkel, Nov 2003
; return u32

%ifndef __OMF__
  %ifdef OS2
    %define __OMF__
  %endif
%endif
                      
%ifdef __OMF__   ; Watcom+OS/2 or Borland+Win32
[SECTION _DATA CLASS=DATA USE32 PUBLIC ALIGN=16]
[SECTION _TEXT CLASS=CODE USE32 PUBLIC ALIGN=16]
%define __DATASECT__ [SECTION _DATA]
%define __CODESECT__ [SECTION _TEXT]
%else
%define __DATASECT__ [SECTION .data]
%define __CODESECT__ [SECTION .text]
%endif

global          x86features,_x86features

%define MMX             00000001h
%define CYRIX_MMX_PLUS  00000010h
%define AMD_MMX_PLUS    00000100h
%define 3DNOW		00001000h
%define 3DNOW_PLUS	00010000h
%define SSE		00100000h
%define SSE2		01000000h
%define SSE3		10000000h

__CODESECT__
_x86features:            
x86features: 

  push ebx
  push ecx
  push edx
  push esi

  mov esi, 0h

  ; See if CPUID instruction is supported ...
  ; ... Get copies of EFLAGS into eax and ecx
  pushf
  pop eax
  mov ecx, eax

  ; ... Toggle the ID bit in one copy and store
  ;     to the EFLAGS reg
  xor eax, 200000h
  push eax
  popf

  ; ... Get the (hopefully modified) EFLAGS
  pushf
  pop eax

  ; ... Compare and test result
  xor ecx, eax
  test ecx, 200000h
  jz NotSupported  ; Nothing supported

  ; Get standard CPUID information, and
  ; go to a specific vendor section
  mov eax, 0h
  cpuid

; Check for Intel
TryIntel:
  cmp ebx, 756E6547h
  jne TryAMD
  cmp edx, 49656E69h
  jne TryAMD
  cmp ecx, 6C65746Eh
  jne TryAMD
  jmp Intel

; Check for AMD
TryAMD:
  cmp ebx, 68747541h
  jne TryCyrix
  cmp edx, 69746E65h
  jne TryCyrix
  cmp ecx, 444D4163h
  jne TryCyrix
  jmp AMD

; Check for Cyrix
TryCyrix:
  cmp ebx, 69727943h
  jne TryExtended
  cmp edx, 736E4978h
  jne TryExtended
  cmp ecx, 64616574h
  jne TryExtended
  jmp Cyrix

AMD:
  ; See if extended CPUID is supported
  mov eax, 80000000h
  cpuid
  cmp eax, 80000000h
  jl Standard  ; Try standard CPUID instead

  ; Extended CPUID supported, so get extended features
  mov eax, 80000001h
  cpuid
  mov edx, eax

  test edx, 00400000h  ; Test for AMD Ext'd MMX
  jz Extended
  or esi, AMD_MMX_PLUS           ; AMD EMMX supported

  jmp Extended

Cyrix:
  ; See if extended CPUID is supported
  mov eax, 80000000h
  cpuid
  cmp eax, 80000000h
  jl Standard  ; Try standard CPUID instead

  test edx, 01000000h  ; Test for Cyrix Ext'd MMX
  jz Extended
  or esi, CYRIX_MMX_PLUS           ; Cyrix EMMX supported

  jmp Extended

TryExtended:
  ; See if extended CPUID is supported
  mov eax, 80000000h
  cpuid
  cmp eax, 80000000h
  jl Standard  ; Try standard CPUID instead

Extended:

  ; Extended CPUID supported, so get extended features
  mov eax, 80000001h
  cpuid
  mov edx, eax
  test edx, 00800000h   ; Test for MMX
  jz 3DNow
  or esi, MMX           ; MMX Supported
3DNow:
  test edx, 80000000h   ; Test for 3DNow!
  jz 3DNowPlus
  or esi, 3DNOW         ; 3DNow! also supported
3DNowPlus:
  test edx, 40000000    ; Test for 3DNow!+
  jz Standard
  or esi, 3DNOW_PLUS    ; 3DNow!+ also supported

Intel:
Standard:
  mov eax, 1h
  cpuid
MMX:
  test edx, 00800000h   ; Test for MMX
  jz SSE		; MMX Not supported
  or esi, MMX		; MMX Supported
SSE:
  test edx, 02000000h	; Test for SSE
  jz SSE2		; SSE Not supported
  or esi, SSE		; SSE Supported
SSE2:
  test edx, 04000000h   ; Test for SSE2
  jz SSE3               ; SSE2 Not supported
  or esi, SSE2          ; SSE2 Supported
SSE3:
  test ecx, 00000001h   ; Test for SSE3
  jz Return             ; SSE3 Not supported
  or esi, SSE3          ; SSE3 Supported
  jmp Return

; Nothing supported
NotSupported:
Return:
  mov eax, esi

  pop esi
  pop edx
  pop ecx
  pop ebx

  ret
