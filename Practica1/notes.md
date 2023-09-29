# Ejercicio 1 - Notes

## Part 1

1. [DO HOME]
2. OKAY
3. 
    ggc version `11.3.0`

    simple2.c:26:21: optimized: loop vectorized using 32 byte vectors
    simple2.c:19:17: optimized: loop vectorized using 32 byte vectors
4. No output - not optimizing vectors.
5. 
```
10a11
>       xorl    %eax, %eax
13,18d13
<       movdqa  .LC0(%rip), %xmm2
<       movdqa  .LC1(%rip), %xmm4
<       movdqa  .LC2(%rip), %xmm3
<       movq    %rcx, %rax
<       movq    %rdx, %rsi
<       leaq    16384(%rcx), %rdi
20,35c15,23
<       movdqa  %xmm2, %xmm0
<       addq    $32, %rax
<       paddd   %xmm4, %xmm2
<       addq    $32, %rsi
<       cvtdq2pd        %xmm0, %xmm1
<       movaps  %xmm1, -32(%rax)
<       pshufd  $238, %xmm0, %xmm1
<       paddd   %xmm3, %xmm0
<       cvtdq2pd        %xmm1, %xmm1
<       movaps  %xmm1, -16(%rax)
<       cvtdq2pd        %xmm0, %xmm1
<       pshufd  $238, %xmm0, %xmm0
<       cvtdq2pd        %xmm0, %xmm0
<       movaps  %xmm1, -32(%rsi)
<       movaps  %xmm0, -16(%rsi)
<       cmpq    %rdi, %rax
---
>       pxor    %xmm0, %xmm0
>       leal    1(%rax), %esi
>       cvtsi2sdl       %eax, %xmm0
>       movsd   %xmm0, (%rcx,%rax,8)
>       pxor    %xmm0, %xmm0
>       cvtsi2sdl       %esi, %xmm0
>       movsd   %xmm0, (%rdx,%rax,8)
>       addq    $1, %rax
>       cmpq    $2048, %rax
38c26
<       movapd  .LC3(%rip), %xmm3
---
>       movsd   .LC0(%rip), %xmm2
45,50c33,36
<       movapd  (%rdx,%rax), %xmm0
<       mulpd   %xmm3, %xmm0
<       addpd   (%rcx,%rax), %xmm0
<       addq    $16, %rax
<       addsd   %xmm0, %xmm1
<       unpckhpd        %xmm0, %xmm0
---
>       movsd   (%rdx,%rax), %xmm0
>       mulsd   %xmm2, %xmm0
>       addsd   (%rcx,%rax), %xmm0
>       addq    $8, %rax
68,69c54,55
<       .section        .rodata.cst16,"aM",@progbits,16
<       .align 16
---
>       .section        .rodata.cst8,"aM",@progbits,8
>       .align 8
71,90d56
<       .long   0
<       .long   1
<       .long   2
<       .long   3
<       .align 16
< .LC1:
<       .long   4
<       .long   4
<       .long   4
<       .long   4
<       .align 16
< .LC2:
<       .long   1
<       .long   1
<       .long   1
<       .long   1
<       .align 16
< .LC3:
<       .long   -611603343
<       .long   1072693352
```

Ill probaby have to repeat this on my laptop but its not hard

## Part 2

## Part 3

Theres a "solution" in the ppt.