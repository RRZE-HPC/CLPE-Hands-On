# Dot Product

## Import project

You can find the code including timing and measurement harness in the `ce_project` directory.  
**Please keep in mind that this project will not work out of the box for the standard Compiler Explorer (CE) due to the requirement of a `perf_event_paranoid` level of `<= 0` for the linux kernel.**  
You can import the project into the [Compiler Explorer](https://godbolt.org/) or any locally running instance by doing the following steps:
1. Create a zip repository from the `ce_project` directory, e.g.: `zip -r proj.zip ce_project/`
2. In the Compiler Explorer, click "_Add..._" in the top menu bar and select "_Tree (IDE Mode)_"
3. In the Tree view, choose "_Project_" to open a drop down menu and select "_Select file_" and choose your newly created zip repo


## Code snippets
You can also build your own timing harness or simply just look at the loop bodies of the relevant code.
For this we provide here the code and compiler information for reproducing the tutorial Hands-On.


### Pure C loop:

```c
int NI, NK;
double phi [NI][NK];

// benchmark loop
for (int i=1; i<NI-1; ++i) {
    for (int k=1; k<NK-1; ++k) {
        phi[i][k] = 0.25 * (
            phi[i][k-1] + phi[i+1][k]
          + phi[i][k+1] + phi[i-1][k]
        );
    }
}
```

### ASM loop(s):


| Compiler | Flags |
|----------|-------|
| icc 2021.6.0 | `-Ofast -qopenmp-simd -xHost -qopt-zmm-usage=low -fargument-noalias -funroll-loops -fno-builtin` |


```assembly
..B1.72:                        # Preds ..B1.72 ..B1.71
  vmovsd    (%rsi,%r9), %xmm2                             #81.19
  incq      %rdx                                          #78.7
  vaddsd    8(%rsi,%r10), %xmm2, %xmm3                    #80.15
  vaddsd    16(%rsi,%r9), %xmm3, %xmm4                    #80.15
  vaddsd    %xmm1, %xmm4, %xmm1                           #80.15
  vmulsd    %xmm1, %xmm0, %xmm5                           #82.33
  vmovsd    %xmm5, 8(%rsi,%r9)                            #80.15
  vaddsd    (%rsi,%r10), %xmm5, %xmm6                     #81.33
  vaddsd    8(%rsi,%r11), %xmm6, %xmm7                    #82.19
  vaddsd    16(%rsi,%r10), %xmm7, %xmm8                   #82.33
  vmulsd    %xmm8, %xmm0, %xmm9                           #82.33
  vmovsd    %xmm9, 8(%rsi,%r10)                           #80.15
  vaddsd    (%rsi,%r11), %xmm9, %xmm10                    #81.33
  vaddsd    8(%rsi,%r8), %xmm10, %xmm11                   #82.19
  vaddsd    16(%rsi,%r11), %xmm11, %xmm12                 #82.33
  vmulsd    %xmm12, %xmm0, %xmm13                         #82.33
  vmovsd    %xmm13, 8(%rsi,%r11)                          #80.15
  vaddsd    (%rsi,%r8), %xmm13, %xmm14                    #81.33
  vaddsd    8(%rsi,%r14), %xmm14, %xmm15                  #82.19
  vaddsd    16(%rsi,%r8), %xmm15, %xmm16                  #82.33
  vmulsd    %xmm16, %xmm0, %xmm1                          #82.33
  vmovsd    %xmm1, 8(%rsi,%r8)                            #80.15
  addq      %r13, %rsi                                    #78.7
  cmpq      %r12, %rdx                                    #78.7
  jb        ..B1.72       # Prob 27%                      #78.7
```
