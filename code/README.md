#   Metode strojnog učenja u predviđanju najniže svojstvene vrijednosti Laplaceovog operatora

Dokumentacija programa i funkcija nalazi se u izvornom k&ocirc;du. Imena poddirektorija i datoteka više-manje su sugestivna.

## Organizacija poddirektorija [*data*](data)

Poddirektorij [*data*](data) sadrži podatke koji su korišteni u programskim kodovima i/ili koji služe za  vizualizaciju u samom radu.

Datoteka [*data/auxiliary_coordinates.tsv*](data/auxiliary_coordinates.tsv) sadrži koordinate vrhova trokuta čiji su vrhovi (0.5, 0), *V*, (-0.5, 0), gdje je *V* = (*x*, *y*) tako da je 0 < *y* &le; 0.5 sqrt(3) i *x* = *p* (-0.5 + sqrt(1 - *y*<sup>2</sup>)) za *p* iz {0, 0.25, 0.5, 0.75, 1}; datoteka [*data/auxiliary_eigenvalues.tsv*](data/auxiliary_eigenvalues.tsv) sadrži najmanje svojstvene vrijednosti Laplaceovog operatora na tim trokutima.

Poddirektorij [*data/arrays*](data/arrays) sadrži [*NumPy*](https://numpy.org/) polja [*data/arrays/x.npy*](data/arrays/x.npy), [*data/arrays/y.npy*](data/arrays/y.npy), [*data/arrays/X.npy*](data/arrays/X.npy), [*data/arrays/Y.npy*](data/arrays/Y.npy) i [*data/arrays/L.npy*](data/arrays/L.npy), pri čemu polja *x* i *y* sadrže sve distinktne koordinatne vrijednosti varijabilnih vrhova dobivenih pokretanjem programa čiji je izvorni kod *generators/triangles_generator.c* s `./generate 1001 out.tsv`, gdje je *out.tsv* proizvoljna izlazna datoteka, a za polja *X*, *Y* vrijedi

```Python
import numpy as np

x = np.load('data/arrays/x.npy')
y = np.load('data/arrays/y.npy')

X, Y = np.meshgrid(x, y, indexing = 'ij')
```

Nadalje, za svake *i*, *j* je *L*[*i*, *j*] najmanja svojstvena vrijednost Laplaceovog operatora na trokutu s vrhovima (0.5, 0), (*x*[*i*], *y*[*j*]), (-0.5, 0).

Polja (*data/arrays/l_quantiles.npy*)[data/arrays/l_quantiles.npy] i (*data/arrays/l_log10_quantiles.npy*)[data/arrays/l_log10_quantiles.npy] redom su kvantilnegranice 5 %-tnih kvantila svojstvenih vrijednosti i logaritmiranih (s bazom 10) svojstvenih vrijednosti iz datoteke [*data/auxiliary_eigenvalues.tsv*](data/auxiliary_eigenvalues.tsv).

Poddirektorij [*data/numerical*](data/numerical) sadrži podatke o trokutima koji su bili korišteni u modelima s numeričkim ulazom, a poddirektorij [*data/imagination*](data/imagination) podatke o trokutima koji su bili korišteni u modelima s vizualnim ulazom. Oba poddirektorija imaju poddirektorije *training* (trokuti u skupu podataka za treniranje), *validation* (trokuti u skupu podataka za validaciju) i *test* (trokuti u skupu podataka za testiranje). Svaki od tih poddirektorija sadrži datoteke *coordinates.tsv* (koordinate vrhova trokuta), *characteristics.tsv* (koordinate karakteristične točke trokuta), *descriptions.tsv* (duljine stranica i veličine vanjskih kutova), *sorted_descriptions.tsv* (duljine stranica poredane silazno i veličine vanjskih kutova poredane uzlazno), *singular_values.tsv* (singularne vrijednosti duljina stranica i singularne vrijednosti vanjskih kutova) i *eigenvalues.tsv* (svojstvene vrijednosti Laplaceovog operatora na tim trokutima&mdash;ciljna varijabla).
