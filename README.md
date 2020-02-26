#   Metode strojnog učenja u predviđanju najniže svojstvene vrijednosti Laplaceovog operatora

*Diplomski rad*

**Autor: Davor Penzar** <br>
**Voditelj rada: prof. dr. sc. Luka Grubišić**

[***Matematički odsjek***](https://www.math.pmf.unizg.hr/hr) <br>
[***Prirodoslovno-matematički fakultet***](https://www.pmf.unizg.hr/) <br>
[***Sveučilište u Zagrebu***](http://www.unizg.hr/)

*Zagreb, 2020.*

##  Opis repozitorija

Materijali mog diplomskog rada u nastajanju.

##  Sadržaj datoteke [*README.md*](README.md)

1.  [Sadržaj repozitorija](#sadr%C5%BEaj-repozitorija)
2.  [Sažetak rada](#sa%C5%BEetak-rada)
    1.  [Sažetak](#sa%C5%BEetak)
    2.  [Summary](#summary)
3.  [Uočeni problemi](#uo%C4%8Deni-problemi)
4.  [Literatura](#literatura)

##  Sadržaj repozitorija

1.  [**doc**](doc/) &ndash; literatura,
2.  [**code**](code/) &ndash; izvorni k&ocirc;dovi programa,
3.  [**latex**](latex/) &ndash; izvorni *LaTeX* k&ocirc;d diplomskog rada.

##  Sažetak rada

### Sažetak

Proučavajući teorijski svojstvene vrijednosti i svojstvene funkcije Laplaceovog operatora uočena su obilježja kojima se njihovo računanje može pojednostaviti. Prvenstveno, one ne ovise o afinim izometričnim transformacijama skupova (osim što se domena svojstvenih funkcija, očito, izometrično transformira). Također, skaliranjem skupa svojstvene se vrijednosti skaliraju multiplikativnim inverzom kvadrata skalara, dok se svojstvene funkcije komponiraju zdesna sa skalarom. Nadalje, one neprekidno ovise o domeni i svojstvene su vrijednosti padajuće s obzirom na relaciju inkluzije. Iz svega se navedenog zaključeno je da se određene domene u svrhu numeričkog računanja svojstvenih vrijednosti i svojstvenih funkcija Laplaceovog operatora (to jest, najmanje svojstvene vrijednosti u slučaju ovog rada) mogu aproksimirati *jednostavnijim*, *dovoljno sličnim* i normaliziranim domenama.

Kao jedan tip jednostavnijih domena koje mogu poprimiti *razne* oblike odabrani su poligoni. Zbog opaženih obilježja svojstvenih vrijednosti i svojstvenih funkcija Laplaceovog operatora, u svrhu njihova računanja modelom strojnog učenja bilo je poželjno poligone opisati karakterizacijama koje su dovoljno diskriminirajuće, ali i robusne na određene transformacije. U radu su ponuđene nude tri različite karakterizacije poligona, a u praktičnom nastavku rada proučavani su samo trokuti kao prototipi poligona.

Za svaku od karakterizacija, nakon proučavanja literature i eksploratorne analize izrađenog skupa podataka, odabrana je jedna pogodna metoda strojnog učenja za početni problem. Tako su konstruirani dva modela linearne regresije, neuronska mreža i konvolucijska neuronska mreža. Nakon treniranja modela njihove su uspješnosti ispitane na testnim skupovima podataka i modeli su uspoređeni s obzirom na uspješnosti i brzine. Rezultati su u većini slučajeva bili obećavajući, ali generalizabilnost modela na složenije poligone nije potvrđena jer model koji se najjednostavnije može generalizirati&mdash;konvolucijska neuronska mreža&mdash;luči najlošije rezultate.

### Summary

Studying the theory behind the eigenvalues and the eigenfunctions of Dirichlet Laplacian it was observed that they possess some characteristics that allow us to simplify calculations to find them. First of all, they are independent of affine isometric transformations of sets (except that the domain of the eigenfunctions is obviously isometrically transformed). Also, scaling a set scales its eigenvalues by the factor of the square of the multiplicative inverse of the scaling factor, and the resulting eigenfunctions are compositions of the original eigenfunctions on the left and the scaling factor on the right. Furthermore, they depend continuously on the domain and the eigenvalues are monotonically decreasing in regard to the order of inclusion of sets. Considering all this it was then concluded that some domains can be approximated by other, *simpler* yet *similar enough*, normalised domains to numerically find the eigenvalues and the eigenfunctions of Dirichelt Laplacian (i. e. the minimal eigenvalue, as is the case in this paper) on them.

Polygons were then chosen as an example of *simple* domains with *many* different shapes. Because of the observed characteristics of the eigenvalues and the eigenfunctions, to predict them on polygons using a machine learning model, polygons should be characterised in a way that discriminates them but is also resistant to specific transformations. In this paper three distinct characterisations were proposed, and then in the following part of the paper, the practical part, only triangles were observed as they can be considered prototypes of polygons.

After studying the sources and conducting exploratory analysis of the generated dataset, a suitable machine learning method was chosen for each of the characterisations to solve the original problem. Subsequently two linear regression models, a neural network, and a convolutional neural network were designed. After the training their success was measured on testing datasets and they were compared for their successfulness and time consumption. In most cases the results were promising, although generalisability of the models to more complex polygons was not confirmed since the model that could be generalised most easily&mdash;the convolutional neural network&mdash;yeilds the worst results.

##  Uočeni problemi

1.  U definiciji poligona (definicija 2.1.) potrebno je dodati uvjet **Int *P* &ne; &empty;** da bi ostatak teksta ostao smislen i/ili istinit. Alternativno, ali suboptimalno, ostatak teksta treba mijenjati tako da bude u skladu s definicijom.
2.  U teoremu o karakterizaciji poligona pravim vrhovima (teorem 2.6.) u točki 1. potrebno je ***S***, kao oznaku/naziv skupa (poligona), zamijeniti s ***P***. Ponovo, moguće je ostatak teksta teorema zamijeniti tako da je poligon označen sa ***S***, a ne s ***P***, ali logičnija je prva navedena zamjena.

##  Literatura

1.  C. Audet, J. E. Dennis, Jr. *Mesh Adaptive Direct Search Algorithms for Constrained Optimization*, SIAM Journal on Optimization 17.1 (2006), 188&ndash;217.
2.  C. Blatter, Rigorous Definition of Positive Curve in &#8477;<sup>*n*</sup>, 2012, dostupno na [`https://math.stackexchange.com/questions/218849/rigorous-definition-of-positive-orientation-of-curve-in-mathbbrn`](https://math.stackexchange.com/questions/218849/rigorous-definition-of-positive-orientation-of-curve-in-mathbbrn).
3.  R. Courant, D. Hilbert, *Methods of Mathematical Physics*, sv. 1, Interscience Publishers, Inc., New York, 1953.
4.  R. Donoso Benguria, Dirichlet Eigenvalue, Enyclopedia of Mathematics, dostupno na [`https://www.encyclopediaofmath.org/index.php/Dirichlet_eigenvalue`](https://www.encyclopediaofmath.org/index.php/Dirichlet_eigenvalue).
5.  K. Eidolon, G. Oman, *A Short Proof of the Bolzano Weierstrass Theorem*, The College Mathematics Journal 48.4 (2017), 288&ndash;289.
6.  Z. Fattah, M. Berrada, *Eigenvalues of Dirichlet Laplacian within the Class of Open Sets with Constant Diameter*, 2018, arXiv: [`1801.04133 [math.SP]`](https://arxiv.org/abs/1801.04133).
7.  *FreeFEM Documentation*. *Release 4.2.1*, 4. izdanje, 2019, dostupno na [`https://doc.freefem.org/introduction/index.html`](https://doc.freefem.org/introduction/index.html).
8.  G. Harris, C. Martin, *The Roots of a Polynomial Vary Continuously as a Function of the Coefficients*, Proceedings of the American Mathematical Society 100.2 (1987), 390&ndash;392.
9.  R. S. Laugesen, B. Siudeja, *Dirichlet Eigenvalue Sums on Triangles Are Minimal for Equilaterals*, Communications in Analysis and Geometry 19.5 (2011), 855&ndash;885.
10.  K. Mills, M. Spanner, I. Tamblyn, *Deep Learning and the Schr&ouml;dinger Equation*, 2017, arXiv: [`1702.01361 [cond-mat.mtrl-sci]`](https://arxiv.org/abs/1702.01361).
11. Multiphysics Encyclopedia, Eigenfrequncy Analysis, COMSOL Multiphysics, 2018, dostupno na [`https://www.comsol.com/multiphysics/eigenfrequency-analysis`](https://www.comsol.com/multiphysics/eigenfrequency-analysis).
12. B. Nica, *The Mazur-Ulam Theorem*, Expositiones Mathematicae 30.4 (2013), 397&ndash;398.
13. M. Reuter, F.-E. Wolter, M. Shenton, M. Niethammer, *Laplace-Beltrami Eigenvalues and Topological Features of Eigenfunctions for Statistical Shape Analysis*, Computer-Aided Design 41.10 (2009), 739&ndash;755.
14. B. Siudeja, *Isoperimetric Inequalities for Eigenvalues of Triangles*, Indiana University Mathematics Journal 59.3 (2010), 1097&ndash;1120.
15. H. Tverberg, *A Proof of the Jordan Curve Theorem*, Bulletin of the London Mathematical Society 12.1 (1980), 34&ndash;38.
16. E. W. Weisstein, Curve Orientation, Wolfram MathWorld, dostupno na [`http://mathworld.wolfram.com/CurveOrientation.html`](http://mathworld.wolfram.com/CurveOrientation.html).
17. E. W. Weisstein, Polygon, Wolfram MathWorld, dostupno na [`http://mathworld.wolfram.com/Polygon.html`](http://mathworld.wolfram.com/Polygon.html).
18. M. Žepić, *Latinsko-hrvatski rječnik* (Lj. Tomljenović-Biškupić, ur.), 11. izdanje, Školska knjiga, Zagreb, 1991.
