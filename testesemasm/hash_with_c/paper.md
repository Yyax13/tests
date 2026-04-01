First of all we need to get the way that gcc is allocating the struct:

```c
typedef struct {
    unsigned char *b;
    size_t len;
    
} bytes_t;
```

For doing that, we will create a simple file:

```c
#include <stddef.h>
typedef struct {
    unsigned char *b;
    size_t len;
    
} bytes_t;

bytes_t createBytes(unsigned char *b, size_t l) {
    bytes_t o;
    o.b = b;
    o.len = l;
    return o;
    
};

int main() {    
    return 0;
};
```

And compile with:

```bash
gcc app.c -o app.o
```

After that, let's use objdump to get asm function:

```bash
$ objdump -d -M amd64 app.o

app.o: formato do arquivo elf64-x86-64


Desmontagem da seção .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Desmontagem da seção .plt:

0000000000001020 <.plt>:
    1020:	ff 35 a2 2f 00 00    	push   0x2fa2(%rip)        # 3fc8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 a3 2f 00 00 	bnd jmp *0x2fa3(%rip)        # 3fd0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)

Desmontagem da seção .plt.got:

0000000000001030 <__cxa_finalize@plt>:
    1030:	f3 0f 1e fa          	endbr64 
    1034:	f2 ff 25 bd 2f 00 00 	bnd jmp *0x2fbd(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    103b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Desmontagem da seção .text:

0000000000001040 <_start>:
    1040:	f3 0f 1e fa          	endbr64 
    1044:	31 ed                	xor    %ebp,%ebp
    1046:	49 89 d1             	mov    %rdx,%r9
    1049:	5e                   	pop    %rsi
    104a:	48 89 e2             	mov    %rsp,%rdx
    104d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1051:	50                   	push   %rax
    1052:	54                   	push   %rsp
    1053:	45 31 c0             	xor    %r8d,%r8d
    1056:	31 c9                	xor    %ecx,%ecx
    1058:	48 8d 3d f4 00 00 00 	lea    0xf4(%rip),%rdi        # 1153 <main>
    105f:	ff 15 73 2f 00 00    	call   *0x2f73(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    1065:	f4                   	hlt    
    1066:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    106d:	00 00 00 

0000000000001070 <deregister_tm_clones>:
    1070:	48 8d 3d 99 2f 00 00 	lea    0x2f99(%rip),%rdi        # 4010 <__TMC_END__>
    1077:	48 8d 05 92 2f 00 00 	lea    0x2f92(%rip),%rax        # 4010 <__TMC_END__>
    107e:	48 39 f8             	cmp    %rdi,%rax
    1081:	74 15                	je     1098 <deregister_tm_clones+0x28>
    1083:	48 8b 05 56 2f 00 00 	mov    0x2f56(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    108a:	48 85 c0             	test   %rax,%rax
    108d:	74 09                	je     1098 <deregister_tm_clones+0x28>
    108f:	ff e0                	jmp    *%rax
    1091:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1098:	c3                   	ret    
    1099:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010a0 <register_tm_clones>:
    10a0:	48 8d 3d 69 2f 00 00 	lea    0x2f69(%rip),%rdi        # 4010 <__TMC_END__>
    10a7:	48 8d 35 62 2f 00 00 	lea    0x2f62(%rip),%rsi        # 4010 <__TMC_END__>
    10ae:	48 29 fe             	sub    %rdi,%rsi
    10b1:	48 89 f0             	mov    %rsi,%rax
    10b4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10b8:	48 c1 f8 03          	sar    $0x3,%rax
    10bc:	48 01 c6             	add    %rax,%rsi
    10bf:	48 d1 fe             	sar    %rsi
    10c2:	74 14                	je     10d8 <register_tm_clones+0x38>
    10c4:	48 8b 05 25 2f 00 00 	mov    0x2f25(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    10cb:	48 85 c0             	test   %rax,%rax
    10ce:	74 08                	je     10d8 <register_tm_clones+0x38>
    10d0:	ff e0                	jmp    *%rax
    10d2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10d8:	c3                   	ret    
    10d9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010e0 <__do_global_dtors_aux>:
    10e0:	f3 0f 1e fa          	endbr64 
    10e4:	80 3d 25 2f 00 00 00 	cmpb   $0x0,0x2f25(%rip)        # 4010 <__TMC_END__>
    10eb:	75 2b                	jne    1118 <__do_global_dtors_aux+0x38>
    10ed:	55                   	push   %rbp
    10ee:	48 83 3d 02 2f 00 00 	cmpq   $0x0,0x2f02(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    10f5:	00 
    10f6:	48 89 e5             	mov    %rsp,%rbp
    10f9:	74 0c                	je     1107 <__do_global_dtors_aux+0x27>
    10fb:	48 8b 3d 06 2f 00 00 	mov    0x2f06(%rip),%rdi        # 4008 <__dso_handle>
    1102:	e8 29 ff ff ff       	call   1030 <__cxa_finalize@plt>
    1107:	e8 64 ff ff ff       	call   1070 <deregister_tm_clones>
    110c:	c6 05 fd 2e 00 00 01 	movb   $0x1,0x2efd(%rip)        # 4010 <__TMC_END__>
    1113:	5d                   	pop    %rbp
    1114:	c3                   	ret    
    1115:	0f 1f 00             	nopl   (%rax)
    1118:	c3                   	ret    
    1119:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001120 <frame_dummy>:
    1120:	f3 0f 1e fa          	endbr64 
    1124:	e9 77 ff ff ff       	jmp    10a0 <register_tm_clones>

0000000000001129 <createBytes>:
    1129:	f3 0f 1e fa          	endbr64 
    112d:	55                   	push   %rbp
    112e:	48 89 e5             	mov    %rsp,%rbp
    1131:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    1135:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    1139:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    113d:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    1141:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    1145:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1149:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    114d:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    1151:	5d                   	pop    %rbp
    1152:	c3                   	ret    

0000000000001153 <main>:
    1153:	f3 0f 1e fa          	endbr64 
    1157:	55                   	push   %rbp
    1158:	48 89 e5             	mov    %rsp,%rbp
    115b:	b8 00 00 00 00       	mov    $0x0,%eax
    1160:	5d                   	pop    %rbp
    1161:	c3                   	ret    

Desmontagem da seção .fini:

0000000000001164 <_fini>:
    1164:	f3 0f 1e fa          	endbr64 
    1168:	48 83 ec 08          	sub    $0x8,%rsp
    116c:	48 83 c4 08          	add    $0x8,%rsp
    1170:	c3                   	ret    
```

We just want the function definition:

```yaml
0000000000001129 <createBytes>:
    1129:	f3 0f 1e fa          	endbr64 
    112d:	55                   	push   %rbp
    112e:	48 89 e5             	mov    %rsp,%rbp
    1131:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    1135:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    1139:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    113d:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    1141:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    1145:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1149:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    114d:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    1151:	5d                   	pop    %rbp
    1152:	c3                   	ret    
```

As we can see, the function is returning the data in `rax` and `rdx`, so we will return following that pattern.

As our function is designed to interact directly with bytes and don't handle strings, we need a way to print the format, for that we will use that function:

```c
void bytesf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
            
        }
        
        if (*(p + 1) == 'b') {
            bytes_t *bytes = va_arg(args, bytes_t*);
            if (*(p + 2) == 'h') {
                printf("[%#x", bytes->b[0]);
                for (int i = 1; i < bytes->len; i++) {
                    printf(", %#x", bytes->b[i]);
                    
                }
                
                printf("]");
                fflush(stdout);
                p += 2;
                continue;
                
            }
            
            
            printf("[%x", bytes->b[0]);
            for (int i = 1; i < bytes->len; i++) {
                printf(", %x", bytes->b[i]);
                
            }
            
            printf("]");
            fflush(stdout);
            p++;
            continue;
            
        } else if (*(p + 1) == 'p') {
            void *ptr = va_arg(args, void*);
            printf("%p", ptr);
            fflush(stdout);
            p++;
            continue;
            
        } else if (*(p + 1) == '%') {
            printf("%%");
            fflush(stdout);
            p++;
            continue;
            
        }
        
    }
    
}
```

We already have a functional assembly way to djb2 hash values, i made it when studying assembly:

```nasm
BITS 64

section .data
    STRS db 'howo', 0
    STRL equ $ - STRS
    HASH dq 5381

section .text
    global _start                                       ; Set the entrypoint as the function _start

_start:                                                 ; main function
    xor rax, rax                                        ; rax = 0
    mov rax, [HASH]                                     ; rax = &HASH
    
    xor rsi, rsi                                        ; rsi = 0
    mov rsi, STRS                                       ; rsi = STRS

    jmp djb2_loop                                       ; goto djb2_loop

djb2_loop:
                                                        ; bl is the least significant byte of rbx
    mov bl, [rsi]                                       ; bl = &rsi
    cmp bl, 0x00                                        ; if (bl)
    je end                                              ;   goto end;

    mov rcx, rax                                        ; Copy rax to rcx because we need actual value after the fisrt change
    shl rax, 5                                          ; rax <<= 5
    add rax, rcx                                        ; rax += rcx
    add rax, rbx                                        ; rax += rbx
                                                        ; rbx is the current byte of STRS set in mov bl, [rsi] as bl is rbx lower byte
    
    inc rsi                                             ; rsi = rsi[i + 1]
    jmp djb2_loop                                       ; goto next iteration

end:
    jmp exit_0

exit_0:                                                 ; Exit with status code 0
    xor rdi, rdi                                        ; rdi = 0
    xor rax, rax                                        ; rax = 0
    mov rax, 0x3c                                       ; rax = 60
    
    syscall                                             ; call syscall exit
```

We just need to adapt: we can't set the source in .data section and we don't need `_start`, `end` and `exit_0` functions.

To develop the function, first we need to know how assembly pass `bytes_t` values to C:

```c
#include <stddef.h>

typedef struct {
    unsigned char *b;
    size_t len;
    
} bytes_t;

bytes_t ret(bytes_t bytes) {
    bytes_t out;
    out.b = bytes.b;
    out.len = bytes.len;
    
    return out;
    
};

int main() {
    bytes_t bytes = {
        .b = (unsigned char*)"abc123",
        .len = 7
        
    };
    
    ret(bytes);
    return 0;
    
};
```

And get it's definition in `objdump`:

```
0000000000001129 <ret>:
    1129:	f3 0f 1e fa          	endbr64 
    112d:	55                   	push   %rbp
    112e:	48 89 e5             	mov    %rsp,%rbp
    1131:	48 89 f8             	mov    %rdi,%rax
    1134:	48 89 f1             	mov    %rsi,%rcx
    1137:	48 89 ca             	mov    %rcx,%rdx
    113a:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    113e:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
    1142:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    1146:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    114a:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    114e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1152:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    1156:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    115a:	5d                   	pop    %rbp
    115b:	c3                   	ret    
```

And some code from gdb:

```
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x7               
$rbx   : 0x0               
$rcx   : 0x0000555555557df8  →  0x00005555555550e0  →  <__do_global_dtors_aux+0000> endbr64 
$rdx   : 0x0000555555556004  →  0x0000333231636261 ("abc123"?)
$rsp   : 0x00007fffffffd9a8  →  0x000055555555518e  →  <main+0032> mov eax, 0x0
$rbp   : 0x00007fffffffd9c0  →  0x0000000000000001
$rsi   : 0x7               
$rdi   : 0x0000555555556004  →  0x0000333231636261 ("abc123"?)
$rip   : 0x0000555555555129  →  <ret+0000> endbr64 
$r8    : 0x00007ffff7e1bf10  →  0x0000000000000004
$r9    : 0x00007ffff7fc9040  →  <_dl_fini+0000> endbr64 
$r10   : 0x00007ffff7fc3908  →  0x000d00120000000e
$r11   : 0x00007ffff7fde660  →  <_dl_audit_preinit+0000> endbr64 
$r12   : 0x00007fffffffdad8  →  0x00007fffffffde9b  →  "/home/yyax/Documentos/testesemasm/hash_with_c/t.o"
$r13   : 0x000055555555515c  →  <main+0000> endbr64 
$r14   : 0x0000555555557df8  →  0x00005555555550e0  →  <__do_global_dtors_aux+0000> endbr64 
$r15   : 0x00007ffff7ffd040  →  0x00007ffff7ffe2e0  →  0x0000555555554000  →   jg 0x555555554047
$eflags: [zero carry parity adjust sign trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x33 $ss: 0x2b $ds: 0x00 $es: 0x00 $fs: 0x00 $gs: 0x00 
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x00007fffffffd9a8│+0x0000: 0x000055555555518e  →  <main+0032> mov eax, 0x0	 ← $rsp
0x00007fffffffd9b0│+0x0008: 0x0000555555556004  →  0x0000333231636261 ("abc123"?)
0x00007fffffffd9b8│+0x0010: 0x0000000000000007
0x00007fffffffd9c0│+0x0018: 0x0000000000000001	 ← $rbp
0x00007fffffffd9c8│+0x0020: 0x00007ffff7c29d90  →  <__libc_start_call_main+0080> mov edi, eax
0x00007fffffffd9d0│+0x0028: 0x0000000000000000
0x00007fffffffd9d8│+0x0030: 0x000055555555515c  →  <main+0000> endbr64 
0x00007fffffffd9e0│+0x0038: 0x0000000100000000
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
   0x555555555119 <__do_global_dtors_aux+0039> nop    DWORD PTR [rax+0x0]
   0x555555555120 <frame_dummy+0000> endbr64 
   0x555555555124 <frame_dummy+0004> jmp    0x5555555550a0 <register_tm_clones>
 → 0x555555555129 <ret+0000>       endbr64 
   0x55555555512d <ret+0004>       push   rbp
   0x55555555512e <ret+0005>       mov    rbp, rsp
   0x555555555131 <ret+0008>       mov    rax, rdi
   0x555555555134 <ret+000b>       mov    rcx, rsi
   0x555555555137 <ret+000e>       mov    rdx, rcx
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "t.o", stopped 0x555555555129 in ret (), reason: BREAKPOINT
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x555555555129 → ret()
[#1] 0x55555555518e → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef➤  disas
Dump of assembler code for function ret:
=> 0x0000555555555129 <+0>:	    endbr64 
   0x000055555555512d <+4>:	    push   rbp
   0x000055555555512e <+5>:	    mov    rbp,rsp
   0x0000555555555131 <+8>:	    mov    rax,rdi
   0x0000555555555134 <+11>:	mov    rcx,rsi
   0x0000555555555137 <+14>:	mov    rdx,rcx
   0x000055555555513a <+17>:	mov    QWORD PTR [rbp-0x20],rax
   0x000055555555513e <+21>:	mov    QWORD PTR [rbp-0x18],rdx
   0x0000555555555142 <+25>:	mov    rax,QWORD PTR [rbp-0x20]
   0x0000555555555146 <+29>:	mov    QWORD PTR [rbp-0x10],rax
   0x000055555555514a <+33>:	mov    rax,QWORD PTR [rbp-0x18]
   0x000055555555514e <+37>:	mov    QWORD PTR [rbp-0x8],rax
   0x0000555555555152 <+41>:	mov    rax,QWORD PTR [rbp-0x10]
   0x0000555555555156 <+45>:	mov    rdx,QWORD PTR [rbp-0x8]
   0x000055555555515a <+49>:	pop    rbp
   0x000055555555515b <+50>:	ret    
End of assembler dump.
gef➤  
```

We can determinate that the correct way to parse `bytes_t` in asm is checking `rsi` for `.len` and `rdi` for `.b`
