# Dot Product

## Import project

You can find the code including timing and measurement harness in the `ce_project` directory.
** Please keep in mind that this project will not work out of the box for the standard Compiler Explorer (CE) due to the requirement of a `perf_event_paranoid` level of `<= 0` for the linux kernel. **
You can import the project into the [Compiler Explorer](https://godbolt.org/) or any locally running instance by doing the following steps:
1. Create a zip repository from the `ce_project` directory, e.g.: `zip -r proj.zip ce_project/`
2. In the Compiler Explorer, click "Add..." in the top menu bar and select "Tree (IDE Mode)"
3. In the Tree view, choose "Project" to open a drop down menu and select "Select file" and choose your newly created zip repo


## Code snippets
You can also build your own timing harness or simply just look at the loop bodies of the relevant code.
For this we provide here the code and compiler information for reproducing the tutorial Hands-On.


### Pure C loop:

```c
int N;
double s = 0.0;
double *a, *b;

// benchmark loop
for (int i=0; i<N; ++i) {
  s = s + a[i] * b[i];
}
```

### ASM loop(s):

| Compiler | Flags |
|==========|=======|
| icc 2021.6.0 | `-Ofast -qopenmp-simd -xHost -qopt-zmm-usage=low -fargument-noalias -funroll-loops -fno-builtin` |

```assembly
..B1.49:                        # Preds ..B1.49 ..B1.48
  vmovupd   (%rbx,%rdi,8), %ymm6                          #53.24
  vmovupd   32(%rbx,%rdi,8), %ymm7                        #53.24
  vmovupd   64(%rbx,%rdi,8), %ymm8                        #53.24
  vmovupd   96(%rbx,%rdi,8), %ymm9                        #53.24
  vfmadd231pd (%r14,%rdi,8), %ymm6, %ymm2                 #53.24
  vfmadd231pd 32(%r14,%rdi,8), %ymm7, %ymm5               #53.24
  vfmadd231pd 64(%r14,%rdi,8), %ymm8, %ymm4               #53.24
  vfmadd231pd 96(%r14,%rdi,8), %ymm9, %ymm3               #53.24
  addq      $16, %rdi                                     #52.7
  cmpq      %rsi, %rdi                                    #52.7
  jb        ..B1.49       # Prob 82%                      #52.7
```
