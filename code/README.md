#   Metode strojnog učenja u predviđanju najniže svojstvene vrijednosti Laplaceovog operatora

Dokumentacija programa i funkcija nalazi se u izvornom k&ocirc;du. Imena poddirektorija i datoteka više-manje su sugestivna. Dodatna dokumentacija možda će biti dodana u ovu datoteku ili u neku drugu datoteku čitkiju od izvornog k&ocirc;da.

## Organizacija poddirektorija [*data*](data)

Poddirektorij [*data*](data) sadrži podatke koji su korišteni u programskim kodovima i/ili koji služe za vizualizaciju u samom radu.

### [*data/auxiliary_coordinates.tsv*](data/auxiliary_coordinates.tsv) i [*data/auxiliary_eigenvalues.tsv*](data/auxiliary_eigenvalues.tsv)

Datoteka [*data/auxiliary_coordinates.tsv*](data/auxiliary_coordinates.tsv) sadrži koordinate vrhova trokuta čiji su vrhovi (0.5, 0), *V*, (-0.5, 0), gdje je *V* = (*x*, *y*) tako da je 0 < *y* &le; 0.5 sqrt(3) i *x* = *p* (-0.5 + sqrt(1 - *y*<sup>2</sup>)) za *p* iz {0, 0.25, 0.5, 0.75, 1}. Datoteka [*data/auxiliary_eigenvalues.tsv*](data/auxiliary_eigenvalues.tsv) sadrži najmanje svojstvene vrijednosti Laplaceovog operatora na tim trokutima.

### [*data/arrays*](data/arrays)

[*NumPy*](https://numpy.org/) polja:

1.  [*data/arrays/x.npy*](data/arrays/x.npy),
2.  [*data/arrays/y.npy*](data/arrays/y.npy),
3.  [*data/arrays/X.npy*](data/arrays/X.npy),
4.  [*data/arrays/Y.npy*](data/arrays/Y.npy),
5.  [*data/arrays/L.npy*](data/arrays/L.npy),
6.  [*data/arrays/l_quantiles.npy*](data/arrays/l_quantiles.npy),
7.  [*data/arrays/l_log10_quantiles.npy*](data/arrays/l_log10_quantiles.npy).

Polja *x* i *y* sadrže sve distinktne koordinatne vrijednosti varijabilnih vrhova dobivenih pokretanjem programa čiji je izvorni kod [*generators/triangles_generator.c*](generators/triangles_generator.c) s `./generate 1001 out.tsv`, gdje je *out.tsv* lokacija izabrane izlazne datoteke, a za polja *X*, *Y* vrijedi

```Python
import numpy as np

x = np.load('data/arrays/x.npy')
y = np.load('data/arrays/y.npy')

X, Y = np.meshgrid(x, y, indexing = 'ij')
```

Nadalje, za svake *i*, *j* je *L*[*i*, *j*] najmanja svojstvena vrijednost Laplaceovog operatora na trokutu s vrhovima (0.5, 0), (*x*[*i*], *y*[*j*]), (-0.5, 0).

Polja [*data/arrays/l_quantiles.npy*](data/arrays/l_quantiles.npy) i [*data/arrays/l_log10_quantiles.npy*](data/arrays/l_log10_quantiles.npy) kvantilne su granice 5 %-tnih kvantila redom svojstvenih vrijednosti i logaritmiranih (s bazom 10) svojstvenih vrijednosti iz datoteke [*data/auxiliary_eigenvalues.tsv*](data/auxiliary_eigenvalues.tsv).

### [*data/numerical*](data/numerical) i [*data/imagination*](data/imagination)

Poddirektorij [*data/numerical*](data/numerical) sadrži podatke o trokutima koji su bili korišteni u modelima s numeričkim ulazom, a poddirektorij [*data/imagination*](data/imagination) podatke o trokutima koji su bili korišteni u modelima s vizualnim ulazom. Oba poddirektorija imaju poddirektorije:

1.  *training* &ndash; trokuti u skupu podataka za treniranje),
2.  *validation* &ndash; trokuti u skupu podataka za validaciju,
3.  *test* &ndash; trokuti u skupu podataka za testiranje).

Svaki od tih poddirektorija sadrži datoteke:

1.  *coordinates.tsv* &ndash; koordinate vrhova trokuta,
2.  *characteristics.tsv* &ndash; koordinate karakteristične točke trokuta,
3.  *descriptions.tsv* &ndash; duljine stranica i veličine vanjskih kutova,
4.  *sorted_descriptions.tsv* &ndash; duljine stranica poredane silazno i veličine vanjskih kutova poredane uzlazno,
5.  *singular_values.tsv* &ndash; singularne vrijednosti duljina stranica i singularne vrijednosti vanjskih kutova,
6.  *eigenvalues.tsv* &ndash; svojstvene vrijednosti Laplaceovog operatora na tim trokutima&mdash;ciljna varijabla.
