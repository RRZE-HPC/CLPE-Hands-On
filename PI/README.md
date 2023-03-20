# PI by integration

## Import project

You can find the code including timing and measurement harness in the `ce_project` directory.
** Please keep in mind that this project will not work out of the box for the standard Compiler Explorer (CE) due to the requirement of a `perf_event_paranoid` level of `<= 0` for the linux kernel. **
You can import the project into the [Compiler Explorer](https://godbolt.org/) or any locally running instance by doing the following steps:
1. Create a zip repository from the `ce_project` directory, e.g.: `zip -r proj.zip ce_project/`
2. In the Compiler Explorer, click "_Add..._" in the top menu bar and select "_Tree (IDE Mode)_"
3. In the Tree view, choose "_Project_" to open a drop down menu and select "_Select file_" and choose your newly created zip repo


## Code snippets
You can also build your own timing harness or simply just look at the loop bodies of the relevant code.
For this we provide here the code and compiler information for reproducing the tutorial Hands-On.


### Pure C loop:

```c
int N;
double delta_x = 1./N;
double x;

// benchmark loop
for(int i=0; i<N; ++i) {
  x   = (i + 0.5) * delta_x;
  s += (4.0 / (1.0 + x * x));
}
```

### ASM loop(s):


| Compiler | Flags |
|----------|-------|
| icc 2021.6.0 | `-Ofast -qopenmp-simd -qopt-zmm-usage=low -xHost -fargument-noalias -funroll-loops -fno-builtin` |


```assembly
..B1.13:                        # Preds ..B1.13 ..B1.12
  vpaddd    %xmm6, %xmm5, %xmm15                          #37.5
  addl      $16, %eax                                     #36.3
  vcvtdq2pd %xmm5, %ymm8                                  #37.12
  vpaddd    %xmm6, %xmm15, %xmm20                         #37.5
  vcvtdq2pd %xmm15, %ymm5                                 #37.12
  vpaddd    %xmm6, %xmm20, %xmm25                         #37.5
  vcvtdq2pd %xmm20, %ymm16                                #37.12
  vcvtdq2pd %xmm25, %ymm21                                #37.12
  vaddpd    %ymm8, %ymm3, %ymm9                           #37.16
  vaddpd    %ymm5, %ymm3, %ymm12                          #37.16
  vpaddd    %xmm6, %xmm25, %xmm5                          #37.5
  vaddpd    %ymm16, %ymm3, %ymm17                         #37.16
  vaddpd    %ymm21, %ymm3, %ymm22                         #37.16
  vmulpd    %ymm9, %ymm4, %ymm10                          #37.23
  vmulpd    %ymm12, %ymm4, %ymm13                         #37.23
  vmulpd    %ymm17, %ymm4, %ymm18                         #37.23
  vmulpd    %ymm22, %ymm4, %ymm23                         #37.23
  vfmadd213pd %ymm1, %ymm10, %ymm10                       #38.28
  vfmadd213pd %ymm1, %ymm13, %ymm13                       #38.28
  vfmadd213pd %ymm1, %ymm18, %ymm18                       #38.28
  vfmadd213pd %ymm1, %ymm23, %ymm23                       #38.28
  vdivpd    %ymm10, %ymm1, %ymm11                         #38.28
  vdivpd    %ymm13, %ymm1, %ymm14                         #38.28
  vdivpd    %ymm18, %ymm1, %ymm19                         #38.28
  vdivpd    %ymm23, %ymm1, %ymm24                         #38.28
  vfmadd231pd %ymm2, %ymm11, %ymm0                        #38.5
  vfmadd231pd %ymm2, %ymm14, %ymm7                        #38.5
  vfmadd231pd %ymm2, %ymm19, %ymm0                        #38.5
  vfmadd231pd %ymm2, %ymm24, %ymm7                        #38.5
  cmpl      %ecx, %eax                                    #36.3
  jb        ..B1.13       # Prob 82%                      #36.3
```

| Compiler | Flags |
|----------|-------|
| icc 2021.6.0 | `-Ofast -qopenmp-simd -qopt-zmm-usage=high -xHost -fargument-noalias -funroll-loops -fno-builtin` |


```assembly
..B1.13:                        # Preds ..B1.13 ..B1.12
  addl      $32, %eax                                     #36.3
  vpaddd    %ymm3, %ymm7, %ymm13                          #37.5
  vcvtdq2pd %ymm7, %zmm8                                  #37.12
  vaddpd    %zmm8, %zmm1, %zmm9                           #37.16
  vmulpd    %zmm9, %zmm2, %zmm10                          #37.23
  vfmadd213pd %zmm0, %zmm10, %zmm10                       #38.28
  vrcp14pd  %zmm10, %zmm12                                #38.28
  vfnmadd213pd .L_2il0floatpacket.6(%rip){1to8}, %zmm12, %zmm10 #38.28
  vfpclasspd $30, %zmm12, %k0                             #38.28
  vmulpd    %zmm10, %zmm10, %zmm11                        #38.28
  knotw     %k0, %k1                                      #38.28
  vfmadd213pd %zmm12, %zmm10, %zmm12{%k1}                 #38.28
  vfmadd213pd %zmm12, %zmm11, %zmm12{%k1}                 #38.28
  vfmadd231pd %zmm4, %zmm12, %zmm6                        #38.5
  vpaddd    %ymm3, %ymm13, %ymm19                         #37.5
  vcvtdq2pd %ymm13, %zmm14                                #37.12
  vaddpd    %zmm14, %zmm1, %zmm15                         #37.16
  vmulpd    %zmm15, %zmm2, %zmm16                         #37.23
  vfmadd213pd %zmm0, %zmm16, %zmm16                       #38.28
  vrcp14pd  %zmm16, %zmm18                                #38.28
  vfnmadd213pd .L_2il0floatpacket.6(%rip){1to8}, %zmm18, %zmm16 #38.28
  vfpclasspd $30, %zmm18, %k2                             #38.28
  vmulpd    %zmm16, %zmm16, %zmm17                        #38.28
  knotw     %k2, %k3                                      #38.28
  vfmadd213pd %zmm18, %zmm16, %zmm18{%k3}                 #38.28
  vfmadd213pd %zmm18, %zmm17, %zmm18{%k3}                 #38.28
  vfmadd231pd %zmm4, %zmm18, %zmm5                        #38.5
  vpaddd    %ymm3, %ymm19, %ymm25                         #37.5
  vcvtdq2pd %ymm19, %zmm20                                #37.12
  vaddpd    %zmm20, %zmm1, %zmm21                         #37.16
  vmulpd    %zmm21, %zmm2, %zmm22                         #37.23
  vfmadd213pd %zmm0, %zmm22, %zmm22                       #38.28
  vrcp14pd  %zmm22, %zmm24                                #38.28
  vfnmadd213pd .L_2il0floatpacket.6(%rip){1to8}, %zmm24, %zmm22 #38.28
  vfpclasspd $30, %zmm24, %k4                             #38.28
  vmulpd    %zmm22, %zmm22, %zmm23                        #38.28
  knotw     %k4, %k5                                      #38.28
  vfmadd213pd %zmm24, %zmm22, %zmm24{%k5}                 #38.28
  vfmadd213pd %zmm24, %zmm23, %zmm24{%k5}                 #38.28
  vfmadd231pd %zmm4, %zmm24, %zmm6                        #38.5
  vcvtdq2pd %ymm25, %zmm26                                #37.12
  vpaddd    %ymm3, %ymm25, %ymm7                          #37.5
  vaddpd    %zmm26, %zmm1, %zmm27                         #37.16
  vmulpd    %zmm27, %zmm2, %zmm28                         #37.23
  vfmadd213pd %zmm0, %zmm28, %zmm28                       #38.28
  vrcp14pd  %zmm28, %zmm30                                #38.28
  vfnmadd213pd .L_2il0floatpacket.6(%rip){1to8}, %zmm30, %zmm28 #38.28
  vfpclasspd $30, %zmm30, %k6                             #38.28
  vmulpd    %zmm28, %zmm28, %zmm29                        #38.28
  knotw     %k6, %k7                                      #38.28
  vfmadd213pd %zmm30, %zmm28, %zmm30{%k7}                 #38.28
  vfmadd213pd %zmm30, %zmm29, %zmm30{%k7}                 #38.28
  vfmadd231pd %zmm4, %zmm30, %zmm5                        #38.5
  cmpl      %ecx, %eax                                    #36.3
  jb        ..B1.13       # Prob 82%                      #36.3
```
