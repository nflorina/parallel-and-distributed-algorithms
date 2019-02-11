                  Tema #1 Antialiasing si micro renderer
                        Nastasoiu Florina 335CA

                    Part 1. Super Sampling Anti Aliasing

    Pentru prima parte am populat structura imagine cu: culoare, latime,
lungime, valoarea maxima si 4 matrici de unsigned char (una folosita in
cazul imaginilor alb-negru, celelate 3 folosite in cazul imaginilor rgb,
fiecare pentru o culoare).
    Functia readData() citeste randurile fisierului consecutiv, folosind 
fread() si populeaza fie prima matrice, fie celelalte trei, in functie
de valoarea P5/P6 de pe primul rand.
    Functia writeData() scrie in fisier fiecare rand din fiecare matrice.
Poate fi una singura(alb-negru) sau 3 matrice(RGB), caz in care se vor 
scrie intercalat, respectand ordinea initiala.
    Functia resize() scaleaza intreaga imagine. Se redimensioneaza latimea
si lungimea a.i. sa fie multipli de resize_factor, apoi micsoreaza de 
resize_factor ori. La crearea thread-urilor, functia pthread_create() primeste
argument functia threadFunction(). Aceasta creeaza practic matricea din out.
    threadFunction() paralelizeaza calculul noilor valori din matricea de out
obtinute prin parcurgerea matricei din input si calcularea unei singure valori
pt fiecare bloc de dimensiune resize_factor x resize_factor. Calcului valorilor
se realizeaza in 4 foruri, paralelizandu-se primul. 'start' si 'end' din 
primul for se obtin in functie de thread_id, num_threads si inaltimea matricei:

                    Part 2. Micro renderer

    Pentru a doua parte am populat structura imagine cu: dimensiune, valoare 
maxima si o matrice de unsigned char(cazul alb-negru).
    Functia initialize() initializeaza valorile pentru dimensiune si valoarea
maxima si aloca matricea in functie de prima.
    Functia render() creeaza thread-urile. Acestea folosesc Functia
threadFunction() care calculeaza valoarea matricei finale. Paralelizarea
calcului pe matrice se realizeaza dupa primul for, utilizand noi valori pentru
'start' si 'end'. Acestea se obtin in functie de thread_id, resolution si
num_threads. Practic se calculeaza fiecare punct (x,y) din imagine in functie
de dimensiunea unui pixel, adica dimensiune_totala/resolution. Avand in vedere
ca avem nevoie de centrul pixelului, se aduna jumatati de pixel la dimensiuni
de blocuri intregi. Folosind formula distantei de la un punct la o dreapta,
adica abs(a*x0 + b*y0 + c)/sqrt(a^2 + b^2), stabilim culoarea unui pixel.
Daca distanta aceasta se incadreaza in 3 cm, valoarea pixelului va fi 0, adica
alb, iar in caz contrar 255, adica negru.
    Functia writeData() scrie matricea finala in fisier.
                                                                                                                  
