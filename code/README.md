#   Metode strojnog učenja u predviđanju najniže svojstvene vrijednosti Laplaceovog operatora

##  [include](include)

Vlastita zaglavlja u programskom jeziku *C*/*C++*.

##  [compile.sh](compile.sh)

Prevedi program napisan u programskom jeziku *C*/*C++* prevoditeljem [GCC](https://gcc.gnu.org/).

Moguće korištenje:
```bash
./compile.sh code.c -o program
```
gdje je *code.c* lokacija izvornog koda, a *program* lokacija rezultantnog izvršivog programa (prevedeni program).

Oznake koje se koriste pri prevođenju su:

* `-std=...` &ndash; standard jezika,
* `-pedantic-errors` &ndash; sve devijacije od standarda rezultiraju greškom,
* `-Wall` &ndash; ispiši sva upozorenja,
* `-O` &ndash; razina optimizacije 1,
* `-lm` &ndash; poveži matematičke funkcije (*math.h* ili *cmath*),
* `-pthread` &ndash; prevedi s funkcijama iz *pthread.h*.

##  [computer3.edp](computer3.edp)

[*FreeFEM*](https://freefem.org/) skripta za računanje svojstvenih vrijednosti Laplaceovog operatora na poligonima.

Korištenje:
```bash
FreeFem++ computer.edp [-i filein] [-o fileout]
```

Argumenti:

* `-i <filename>` lokacija ulazne datoteke (zadana vrijednost: *in.tsv*)
* `-o <filename>` lokacija izlazne datoteke (zadana vrijednost: *out.csv*)

Ostali argumenti se ignoriraju.
