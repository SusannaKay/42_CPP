# CPP09 - STL e container

La STL si basa su 3 elementi principali:

- container
- iteratori
- algoritmi

Container ed iteratori li abbiamo visti nel modulo precedente. Ora si tratta di mettere insieme tutto e fare le scelte giuste per ogni esercizio.

Il vincolo fondamentale del modulo è: ogni tipologia di container va usata una sola volta in tutto il modulo.

## Algoritmi STL

La STL fornisce algoritmi generici come ad es:

- `find`
- `sort`
- `lower_bound`
- `binary_search`
- `merge`
- `copy`
- `for_each`

Lavorano sugli iteratori, non direttamente sui container.

Esempio:

```cpp
std::sort(v.begin(), v.end());
```

`sort` non sa davvero cosa sia `v`. Lui riceve un inizio e una fine, cioè due iteratori, e lavora su quel range.

Questa è una dei concetti più importanti della STL:

container = dove stanno i dati
iteratori = come ci muoviamo sui dati
algoritmi = cosa facciamo sui dati

### Nota sugli iteratori

Non tutti gli iteratori sono uguali.

Alcuni container permettono accesso random, altri no.

Esempio:

- `vector` permette `v[i]`
- `deque` permette `d[i]`
- `list` non permette `l[i]`
- `map` si attraversa con iteratori, ma non ha accesso per indice

Questa cosa influenza gli algoritmi che possiamo usare.

Per esempio `std::sort` richiede random access iterator, quindi funziona con `vector` e `deque`, ma non con `list`.

## Complessità temporale

La complessità temporale indica come cresce il tempo richiesto per effettuare una operazione al crescere del numero degli elementi.

Il costo non è in termini di secondi reali, perché quelli variano da pc a pc, ma indica come scala l'algoritmo scelto.

### O-Notation

Immaginandoci:

- 100 elementi
- 1000 elementi
- 10000 elementi

quanto peggiora il costo?

### O(1) - costo costante

```text
1 elemento       -> 1 operazione
1000 elementi    -> 1 operazione
100000 elementi  -> 1 operazione
```

L'esempio migliore è accedere ad un elemento in un `vector` tramite indice.

```cpp
v[50]
```

Il costo non dipende da quanti elementi ci sono nel vector, perché la memoria è contigua. Internamente succede qualcosa tipo:

```text
base_address + index * sizeof(type)
```

e vai diretto alla posizione di memoria.

### O(n) - costo lineare

Il costo cresce insieme al numero di elementi.

Esempio: cercare un valore in una lista o in un vector non ordinato.

Per trovare il valore, devo guardare gli elementi uno alla volta, fino ad arrivare al nodo desiderato.

| Elementi | Operazioni |
| -------- | ---------- |
| 10       | 10         |
| 100      | 100        |
| 1000     | 1000       |

### O(log n) - ogni operazione dimezza il problema

Tipico di ricerche su dati ordinati, tipo binary search, oppure di container come `map` e `set`.

È un po' come cercare una parola nel dizionario: non parti dalla prima pagina, tagli il problema a metà ogni volta.

| Elementi  | Operazioni |
| --------- | ---------- |
| 8         | 3          |
| 1024      | 10         |
| 1 milione | circa 20   |

### O(n log n) - sorting efficiente

È la complessità tipica degli algoritmi di sorting efficienti, come `std::sort`.

Non è costante, non è lineare, ma cresce molto meglio di un doppio ciclo.

### O(n²) - costo quadratico

Un esempio classico è il bubble sort: per ogni elemento, scorro tutti gli altri.

| Elementi | Operazioni |
| -------- | ---------- |
| 10       | 100        |
| 1000     | 1.000.000  |


### Come si calcola il costo?

Si contano le operazioni elementari, tipo:

- confronti
- assegnazioni
- accessi memoria
- dereference
- incrementi

Esempi:

- ciclo `for` singolo su n elementi -> O(n)
- doppio loop -> O(n²)
- binary search -> O(log n)
- sorting efficiente -> O(n log n)

| Complessità | Significato        |
| ----------- | ------------------ |
| O(1)        | costante           |
| O(log n)    | cresce lentamente  |
| O(n)        | cresce linearmente |
| O(n log n)  | sorting efficiente |
| O(n²)       | cresce molto male  |

Tutto questo serve per scegliere il container giusto ed avere performance buone.

In questo modulo dobbiamo tenere in considerazione alcune operazioni e capirne la complessità.

| Operazione           | vector | deque | stack | map      |
| -------------------- | ------ | ----- | ----- | -------- |
| accesso random       | O(1)   | O(1)  | no    | no       |
| push_back            | O(1)*  | O(1)  | no    | no       |
| push_front           | O(n)   | O(1)  | no    | no       |
| top                  | no     | no    | O(1)  | no       |
| ricerca generica     | O(n)   | O(n)  | no    | O(log n) |
| inserimento ordinato | O(n)   | O(n)  | no    | O(log n) |

`vector` ha push_back ammortizzato O(1), cioè di solito è O(1), ma quando deve riallocare memoria può costare di più.

---

# Scelta dei container per il modulo

Prima di entrare nei singoli esercizi, conviene fare una panoramica.

## ex00

Abbiamo un database in CSV con:

```text
data,exchange_rate
```

Poi abbiamo un input nel formato:

```text
data | valore
```

Per ogni riga dobbiamo:

1. validare la data
2. validare il valore
3. trovare il rate corretto
4. moltiplicare valore * rate
5. stampare il risultato

Se la data non è presente nel database, dobbiamo usare la data precedente più vicina.

Ci serve quindi un contenitore:

- chiave -> valore
- ordinato
- con ricerca efficiente
- senza bisogno di chiavi duplicate

La scelta naturale è `std::map`.

## ex01

L'esercizio richiede di usare la Reverse Polish Notation.

In questa notazione gli operatori vengono dopo gli operandi.

Esempio:

```text
3 + 2
```

diventa:

```text
3 2 +
```

Il concetto di stack è implicito in questo metodo, perché ogni volta che trovo un operatore devo prendere gli ultimi due valori disponibili.

Quindi userò `std::stack` per l'ex01.

## ex02

In questo esercizio dobbiamo usare l'algoritmo Ford-Johnson / merge-insert sort.

L'algoritmo, semplificato, fa questo:

1. crea coppie
2. ordina internamente ogni coppia
3. ordina i valori più grandi delle coppie, in ricorsione con lo stesso algoritmo
4. costruisce una main chain
5. tiene da parte i valori pending
6. inserisce i pending nella main chain con binary search
7. usa Jacobsthal per ottimizzare l'ordine degli inserimenti

Il dettaglio di ogni passaggio è nella sezione dell'ex02.

L'algoritmo deve essere applicato su due container diversi, così possiamo confrontare le performance.

Per questo esercizio ha senso usare:

- `std::vector`
- `std::deque`

Entrambi permettono accesso tramite indice e supportano `lower_bound` e inserimenti nel range.

`vector` usa memoria contigua.

`deque` usa memoria a blocchi.

Quindi sono simili come interfaccia, ma diversi internamente: perfetto per confrontarli.

---

# Ex00 - Bitcoin Exchange

## Obiettivo

Abbiamo un database con date e rate del bitcoin.

Per ogni riga di input dobbiamo stampare:

```text
data => valore = valore * exchange_rate
```

Se la data esatta non c'è, bisogna usare la data precedente più vicina presente nel database.

## Perché std::map

`std::map` è un container associativo ordinato.

Esempio:

```cpp
std::map<std::string, double> prices;
prices["2024-01-01"] = 42.0;
```

Nel nostro caso:

```cpp
std::map<std::string, double> prices;
```

La chiave è la data in formato stringa `YYYY-MM-DD`.

Il valore è il rate.

Funziona bene perché il formato `YYYY-MM-DD` è confrontabile anche come stringa.

Esempio:

```text
2020-01-01 < 2021-01-01 < 2021-05-10
```

Quindi la map resta ordinata cronologicamente senza dover convertire le date in un tipo speciale.

## lower_bound

`lower_bound(key)` restituisce un iteratore al primo elemento con chiave non minore di `key`.

Tradotto:

- se la data esiste, mi dà quella data
- se la data non esiste, mi dà la prima data successiva

Ma a noi serve la data precedente più vicina.

Quindi:

```cpp
it = map.lower_bound(date);
```

Poi:

- se `it` punta alla data esatta, ok
- se `it` punta ad una data successiva, torno indietro di uno
- se `it == begin()` e la data non coincide, significa che non ho una data precedente valida

Esempio:

```text
DB:
2020-01-01
2020-01-10
2020-01-20

Input:
2020-01-15
```

`lower_bound("2020-01-15")` restituisce `2020-01-20`.

Ma noi dobbiamo usare `2020-01-10`, quindi facciamo `--it`.

## Struttura del programma

- parse database
- salvataggio in `map<data, valore>`
- process input file
  - parse riga
  - validazione input
  - ricerca rate corretto
  - moltiplicazione
  - stampa

## Spiegazione funzioni

### main.cpp

Il main:

- valida il numero di argomenti
- apre il file input
- se l'apertura fallisce, esce
- crea l'oggetto `bitExchange`
- chiama la funzione che processa e stampa i risultati

Nel codice il controllo è:

```cpp
if (argc != 2)
```

Quindi il programma accetta un solo argomento: il file input.

Il database invece non viene passato da terminale, ma viene aperto internamente come:

```cpp
file.open("data.csv");
```

Quindi quando eseguo il programma devo essere nella cartella giusta, cioè dove `data.csv` è raggiungibile con quel path relativo.

### BitcoinExchange.cpp

Il costruttore chiama subito `parseDb()`.

`parseDb()`:

- apre `data.csv`
- legge e valida la prima riga/header
- legge il file riga per riga
- cerca il separatore `,`
- crea una substring per la data
- crea una substring per il valore
- converte il valore con `strtod`
- controlla che la conversione sia andata bene
- valida la data
- se tutto è ok, inserisce nella map

La map usata nel codice si chiama:

```cpp
std::map<std::string, double> prices;
```

Quindi il nome mentale è proprio: data -> prezzo/rate.

### strtod ed end pointer

In ex00 uso `strtod` per convertire il valore da stringa a double.

Il punto importante è l'`end pointer`:

```cpp
char *end;
value = strtod(valueStr.c_str(), &end);
```

Se `*end != '\0'`, vuol dire che la conversione non ha consumato tutta la stringa.

Esempio:

```text
42abc
```

`strtod` riesce a leggere `42`, ma si ferma su `a`.

Quindi non posso considerarlo un numero valido.

### errno ed ERANGE

Nel processing dell'input viene anche azzerato `errno` prima della conversione:

```cpp
errno = 0;
value = strtod(valueStr.c_str(), &end);
```

Poi viene controllato:

```cpp
errno != ERANGE
```

Questo serve per intercettare valori fuori range durante la conversione numerica.

In pratica:

- `*end != '\0'` -> la stringa contiene roba non numerica
- `errno == ERANGE` -> il numero è fuori range per la conversione
- `value < 0` -> numero negativo
- `value > 1000` -> numero troppo grande per il subject

La validazione della data controlla:

- formato `YYYY-MM-DD`
- lunghezza corretta
- `-` nelle posizioni giuste
- anno valido
- mese valido
- giorno valido
- anni bisestili
- anno tra 2009 e 2022, cioè il range coperto da `data.csv`

Nel codice la data viene spezzata così:

```cpp
year = strtol(date.substr(0, 4).c_str(), &end, 10);
month = strtol(date.substr(5, 2).c_str(), &end, 10);
day = strtol(date.substr(8, 2).c_str(), &end, 10);
```

Anche qui viene controllato `*end`, perché pure l'anno/mese/giorno devono essere numeri puliti.

Per i giorni del mese viene usato un array:

```cpp
int dayMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
```

Così posso controllare che, ad esempio, febbraio non abbia 31 giorni.

Se l'anno è bisestile, febbraio passa a 29 giorni. Lo decide `checkLeap()`:

```cpp
static bool checkLeap(long year)
{
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
```

Regola: bisestile se divisibile per 4, tranne i secoli, che sono bisestili solo se divisibili per 400. Quindi 2000 sì, 1900 no.

Nella prima versione avevo scritto la condizione al contrario (`return false` se divisibile per 4), quindi `2012-02-29` veniva rifiutata e `2013-02-29` accettata. Classico errore che non vedi finché non testi proprio quella data.

Nel programma la data resta stringa perché così è più semplice usarla come chiave nella map.

Quando poi processiamo l'input:

- controlliamo l'header
- cerchiamo il divisore ` | `
- separiamo data e valore
- convertiamo il valore
- controlliamo errori tipo numero negativo o troppo grande
- cerchiamo il rate corretto con `lower_bound`
- stampiamo il risultato

Nel codice il limite del subject viene gestito così:

```cpp
if (value > 1000)
    std::cout << "Error: too large a number." << std::endl;
```

Quindi i valori validi sono positivi e non maggiori di 1000.

### Eccezioni custom

Nel `.hpp` ci sono varie eccezioni custom:

- `CouldNotOpen`
- `InvalidHeader`
- `InvalidLine`
- `InvalidYear`
- `InvalidMonth`
- `InvalidDay`
- `InvalidDate`

Servono per distinguere mentalmente i diversi punti in cui il parsing può fallire.

Nel processing dell'input però, quando fallisce la validazione della data, il programma stampa in modo uniforme:

```text
Error: bad input => data
```

Quindi le eccezioni sono dettagliate nel codice, ma l'output verso il subject resta semplice.

## Error handling ex00

Casi da gestire:

- file non apribile
- header sbagliato
- riga senza separatore corretto
- data malformata
- data impossibile
- valore non numerico
- valore negativo
- valore troppo grande
- overflow / underflow numerico durante `strtod`
- data precedente non disponibile nel database

---

# Ex01 - RPN

## Obiettivo

Lo scopo del programma è prendere in input una espressione matematica in Reverse Polish Notation e calcolarne il risultato.

La notazione che siamo abituati ad usare è la infix:

```text
3 + 2
```

La reverse polish invece scrive:

```text
3 2 +
```

Nel caso ad esempio di:

```text
(3 + 4) * 2
```

scriviamo:

```text
3 4 + 2 *
```

Il lato positivo è che elimina completamente l'uso delle parentesi per dare precedenza alle operazioni.

Altro esempio:

```text
3 + (4 * 2)
```

in RPN diventa:

```text
3 4 2 * +
```

Prima calcolo `4 * 2`, poi sommo `3`.

## Perché stack

Una calcolatrice RPN deve usare lo stack.

L'algoritmo ha sempre bisogno degli ultimi due valori disponibili.

Quando leggo un numero:

```text
push nello stack
```

Quando leggo un operatore:

```text
prendo top
prendo il secondo valore
calcolo
pusho il risultato
```

## Ordine degli operandi

Questo è importante per non sbagliarsi con sottrazione e divisione.

Per una espressione:

```text
8 3 -
```

nello stack avrei:

```text
3  <- top
8
```

Quindi:

```text
right = top = 3
left = second = 8
op = -
```

Il calcolo è:

```text
left op right
8 - 3
```

Non il contrario.

## std::stack<int>

`stack` è un container adapter.

Questo significa che non è un container completo come `vector` o `deque`, ma una interfaccia limitata costruita sopra un altro container.

Di default, internamente usa `std::deque`.

Operazioni disponibili:

- `push()` aggiunge in cima
- `pop()` rimuove la cima
- `top()` legge il valore in cima
- `size()` restituisce la dimensione
- `empty()` controlla se è vuoto

Complessità: O(1).

Non posso iterare liberamente su uno stack, perché l'idea è proprio limitarlo al comportamento LIFO:

```text
Last In, First Out
```

cioè l'ultimo che entra è il primo che esce.

## std::stringstream

`std::stringstream` serve per parsare la stringa separandola automaticamente sugli spazi.

Esempio:

```cpp
std::stringstream ss(args);
std::string token;

while (ss >> token)
{
    // token contiene un pezzo alla volta
}
```

Spazi multipli vengono ignorati automaticamente.

Quindi:

```text
"3   4      +"
```

viene letto come:

```text
"3"
"4"
"+"
```

## Condizioni

Alla fine lo stack deve avere necessariamente size `== 1`.

Se è vuoto, manca qualcosa.

Se contiene più elementi, significa che c'erano troppi numeri o pochi operatori.

I numeri originali accettati dal subject sono a singola cifra.

Lo stack però può contenere anche risultati parziali, non solo numeri originali.

Esempio:

```text
9 9 *
```

I numeri originali sono una cifra, ma il risultato nello stack è `81`.

## Spiegazione funzioni

Il costruttore prende gli argomenti passati dal main come stringa e chiama subito:

```cpp
createStack(args);
```

Creiamo uno stream `ss` a partire da `args` e lo parsiamo buttando i token letti dentro una stringa temporanea.

Il ciclo:

```cpp
while (ss >> tmp)
```

continua finché lo stream non fallisce, cioè fino alla fine dell'argomento.

Il primo controllo è sulla lunghezza del token.

Da subject dobbiamo accettare solo:

- cifre a 1 digit
- operatori validi

Entrambi hanno lunghezza `== 1`.

Se non rientra in quel caso, lanciamo eccezione.

Poi:

- se è un numero, lo pusho nello stack
- se è un operatore, controllo che lo stack abbia almeno 2 elementi
- prendo `right`
- prendo `left`
- calcolo `left op right`
- pusho il risultato

Nel codice il riconoscimento del numero avviene con:

```cpp
if (isdigit(tmp[0]))
    _num.push(tmp[0] - '0');
```

`tmp[0] - '0'` serve per convertire il carattere numerico nel valore intero corrispondente.

Esempio:

```text
'7' - '0' = 7
```

Gli operatori validi sono controllati da `is_op()`:

```cpp
return (token == "+" || token == "-" || token == "/" || token == "*");
```

Quando trovo un operatore chiamo `solveOp()`.

`solveOp()`:

- controlla che lo stack abbia almeno due elementi
- salva il top in `num2`
- fa pop
- salva il nuovo top in `num1`
- fa pop
- calcola `calculate(num1, op, num2)`
- pusha il risultato

La funzione `calculate()` contiene lo switch sui quattro operatori.

La divisione per zero viene intercettata lì:

```cpp
if (num2 == 0)
    throw RPN::InvalidInput();
```

Alla fine controllo che lo stack abbia size `== 1` e stampo il risultato.

## Error handling ex01

Casi da gestire:

- numero di argomenti sbagliato
- token sconosciuto, tipo `2 3 s +`
- numero con più cifre, se non ammesso dal subject
- operatore senza abbastanza operandi
- divisione per zero
- stack finale vuoto
- stack finale con più elementi

Nel main il programma accetta esattamente una stringa come argomento:

```cpp
if (argc == 2)
    RPN ss(argv[1]);
else
    std::cerr << "Invalid Input" << std::endl;
```

Quindi l'espressione va passata tra virgolette, tipo:

```bash
./RPN "3 4 + 2 *"
```

---

# Ex02 - PmergeMe

## Obiettivo

Dobbiamo creare un programma in grado di ordinare integer positivi usando l'algoritmo Ford-Johnson, chiamato anche merge-insert sort.

Se accade un errore, dobbiamo mostrarlo sullo standard error.

Da subject:

- usare almeno due container
- gestire almeno 3000 integer diversi
- implementare l'algoritmo per ogni container
- non usare direttamente una funzione generica per ordinare tutto l'input e basta

Output richiesto:

```text
Before: lista int non ordinati
After: lista int ordinati
Time to process a range of X elements with std::container1 : tempo
Time to process a range of X elements with std::container2 : tempo
```

Il formato del tempo lo scegliamo noi, ma la precisione deve permettere di vedere la differenza tra i due container usati.

I duplicati il subject dice che possiamo gestirli come vogliamo.

Nel mio caso li accetto: l'algoritmo funziona lo stesso e nell'output compaiono più volte, uno vicino all'altro.

## Vector e deque

Entrambi i container permettono accesso tramite indice e supportano `push_back()`.

### vector

`std::vector` usa memoria contigua.

Pro:

- accesso per indice molto veloce
- cache friendly
- ottimo per scorrere tanti elementi

Contro:

- inserire in mezzo costa O(n)
- se finisce la capacità interna deve riallocare

### deque

`std::deque` usa memoria a blocchi.

Pro:

- accesso per indice comunque O(1)
- efficiente sia in push_front che push_back
- non deve riallocare tutto come vector

Contro:

- meno cache friendly del vector
- internamente è più complesso

Per questo ex sono una buona coppia da confrontare: hanno interfaccia simile, ma memoria diversa.

Spoiler: con 3000 numeri di solito vince il vector. Gli inserimenti in mezzo costano O(n) per entrambi, ma spostare memoria contigua è più veloce che saltare tra blocchi.

## Ford-Johnson / Merge-insert sort

L'idea generale è ridurre il numero di confronti.

Non è l'algoritmo più semplice del mondo, infatti il subject è famoso proprio per far impazzire.

Il flusso mentale è:

1. creo coppie
2. in ogni coppia metto prima il piccolo e poi il grande
3. ordino i grandi **con lo stesso algoritmo**, cioè in ricorsione
4. costruisco una main chain con i grandi ordinati
5. tengo i piccoli come pending
6. inserisco i pending nella main chain usando binary search
7. uso Jacobsthal per scegliere in quale ordine inserire i pending

Il punto 3 è quello che fa la differenza.

Se i grandi li ordino con `std::sort`, l'output esce ordinato lo stesso, ma non è più Ford-Johnson: la parte "merge" dell'algoritmo è proprio questa ricorsione.

La prima versione del mio codice faceva esattamente questo errore. Funzionava, ma non era l'algoritmo richiesto.

## Esempio passo per passo

Input:

```text
8 3 7 1 9 2
```

### Step 1 - creo coppie

```text
(8,3)
(7,1)
(9,2)
```

Se i numeri sono dispari, l'ultimo rimane fuori. Lo chiamo straggler e lo inserisco alla fine insieme ai pending.

### Step 2 - ordino internamente le coppie

Metto il minore prima e il maggiore dopo.

```text
(3,8)
(1,7)
(2,9)
```

Qui faccio un confronto per ogni coppia.

### Step 3 - ordino i grandi in ricorsione

I grandi sono:

```text
8 7 9
```

Su questi richiamo lo stesso algoritmo: coppie, grandi, ricorsione, inserimento. Si scende finché resta un solo elemento, che è già ordinato per definizione.

Quando la ricorsione torna su, i grandi sono ordinati:

```text
7 8 9
```

E di conseguenza anche le coppie, se le leggo seguendo i grandi:

```text
(1,7)
(3,8)
(2,9)
```

### Step 4 - costruisco la main chain

La main chain parte con:

- il piccolo della prima coppia
- tutti i grandi ordinati

Quindi:

```text
1 7 8 9
```

Perché inserisco subito il piccolo della prima coppia?

Perché se la prima coppia è `(1,7)`, so già che `1 < 7`, e dato che `7` è il più piccolo dei grandi, `1` può stare all'inizio della main chain senza fare nessun confronto.

### Step 5 - creo i pending

I pending sono i piccoli delle altre coppie, più lo straggler se c'è.

Nel nostro esempio:

```text
3 2
```

### Step 6 - inserisco i pending con lower_bound

Per ogni pending cerco la posizione corretta nella main chain:

```cpp
it = std::lower_bound(mainchain.begin(), bound, value, comp);
mainchain.insert(it, value);
```

`lower_bound` funziona perché la main chain è sempre ordinata.

Il dettaglio importante è `bound`: non cerco su tutta la main chain, ma solo fino al grande della sua coppia.

Esempio: `3` viene dalla coppia `(3,8)`. So già che `3 < 8`, quindi è inutile cercare dopo `8`.

```text
main chain: 1 7 8 9
cerco 3 solo qui: 1 7
```

Meno elementi nel range = meno confronti nella binary search.

Lo straggler invece non ha un grande, quindi per lui il range è tutta la main chain.

Risultato finale:

```text
1 2 3 7 8 9
```

## Duplicati e partner: perché lavoro sugli indici

Dopo la ricorsione ho i grandi ordinati, ma per ogni grande devo ritrovare il suo piccolo.

Se cerco per valore, con i duplicati è un casino: se ho due `8`, quale coppia è quale?

La soluzione che ho usato: l'algoritmo non sposta i valori, ma i loro **indici** nel container originale.

```text
val:  8 3 7 1 9 2
idx:  0 1 2 3 4 5
```

Gli indici sono unici anche se i valori si ripetono, quindi posso salvare il partner così:

```cpp
partner[big] = small;
```

e ritrovarlo dopo in O(1).

I confronti però li faccio sempre sui valori, con un comparatore:

```cpp
template <typename C>
struct CompareIdx
{
    const C *_val;

    bool operator()(size_t a, size_t b) const
    {
        return ((*_val)[a] < (*_val)[b]);
    }
};
```

È un functor: una struct con `operator()`, che si usa come se fosse una funzione. Lo passo a `lower_bound` come quarto parametro.

Alla fine trasformo gli indici ordinati nei valori ordinati e ho il risultato.

## Jacobsthal

Jacobsthal viene usato sui pending per ridurre il numero di confronti prima dell'inserimento, cambiandone l'ordine.

L'idea non è ordinare i valori dei pending.

L'idea è ordinare gli indici dei pending secondo la sequenza di Jacobsthal.

La formula è:

```text
J(n) = J(n - 1) + 2 * J(n - 2)
```

con:

```text
J(0) = 0
J(1) = 1
```

Calcolando qualche valore:

```text
J(2) = J(1) + 2 * J(0) = 1 + 2 * 0 = 1
J(3) = J(2) + 2 * J(1) = 1 + 2 * 1 = 3
J(4) = J(3) + 2 * J(2) = 3 + 2 * 1 = 5
J(5) = J(4) + 2 * J(3) = 5 + 2 * 3 = 11
```

Quindi la sequenza completa parte così:

```text
0 1 1 3 5 11 21 43 ...
```

### Che sono questi numeri?

Sono limiti di gruppi.

Per ragionare chiamo i piccoli `b1, b2, b3, ...` e i grandi `a1, a2, a3, ...`, nell'ordine della main chain. Quindi `b2` è il piccolo della coppia di `a2`.

`b1` è già nella main chain (step 4), quindi si parte da `b2`.

I gruppi sono:

```text
(1, 3]   -> b3 b2
(3, 5]   -> b5 b4
(5, 11]  -> b11 b10 b9 b8 b7 b6
(11, 21] -> b21 ... b12
```

Cioè ogni gruppo viene inserito al contrario, dal più alto al più basso.

Quindi l'ordine finale diventa:

```text
b3, b2, b5, b4, b11, b10, b9, b8, b7, b6, ...
```

Ovviamente se ho meno pending, mi fermo alla size reale.

Esempio: se ho `b2 b3 b4`, l'ordine sarà:

```text
b3, b2, b4
```

Perché `b5` non esiste.

### Perché serve?

Perché con quest'ordine ogni binary search lavora su un range di dimensione `2^k - 1` (3, 7, 15, ...), che è il caso migliore per la binary search: il range si dimezza pulito a ogni confronto, senza sprecarne uno.

Questo funziona solo se il range è limitato al grande della coppia (il `bound` di prima). Se cercassi su tutta la main chain, l'ordine Jacobsthal non servirebbe a niente.

Evito di approfondire troppo l'analisi matematica sennò finisco fra un paio di anni di capire questo esercizio, però il concetto utile per implementarlo è:

- creo la sequenza Jacobsthal
- genero gruppi di indici
- inserisco ogni gruppo al contrario
- ogni pending lo cerco solo fino al suo grande

### Come verificare che sia davvero Ford-Johnson

Il Ford-Johnson ha un numero massimo di confronti noto:

```text
F(n) = somma per k da 1 a n di ceil(log2(3k / 4))
```

Per verificarlo ho messo un contatore dentro il comparatore (solo in una copia di test, non nel codice consegnato) e ho confrontato con la formula:

| Elementi | Confronti fatti | Massimo teorico |
| -------- | --------------- | --------------- |
| 21       | 65              | 66              |
| 100      | 525             | 534             |
| 3000     | 30358           | 30546           |

Se il contatore supera il massimo teorico, qualcosa non va: o manca la ricorsione, o il range della binary search è sbagliato, o l'ordine Jacobsthal è sfasato.

## Funzione jacobVect / jacobDeq

Nel codice ho due versioni:

- `jacobVect`
- `jacobDeq`

Fanno la stessa cosa, ma una usa `std::vector<size_t>` e l'altra `std::deque<size_t>`.

Questo rispetta l'idea del subject: implementare il procedimento su entrambi i container.

Il procedimento è:

1. parto da `0` e `1`
2. genero la sequenza con la formula Jacobsthal finché supero la size
3. per ogni intervallo Jacobsthal, pusho gli indici al contrario
4. ritorno l'ordine finale

Esempio concettuale:

```text
Jacobsthal: 1 3 5
Ordine:     3 2 5 4
```

Gli indici ritornati sono quelli `b`, 1-based e con `b1` già inserito. Nel codice i pending partono da `b2` in posizione 0, quindi uso:

```cpp
size_t id = jacob[i] - 2;
```

Il `-2` è il classico punto dove ci si sbaglia: `-1` perché il container è 0-based, l'altro `-1` perché `b1` non sta nei pending.

## Ordine di chiamata delle funzioni

```text
main
│
├── valida argomenti -> vector<int> input
├── stampa Before
└── PmergeMe::process()
     │
     ├── processVector()       <- timer vector
     │    ├── copia input in vector<int> + indici
     │    └── mergeInsertVect()
     │         ├── coppie + partner
     │         ├── mergeInsertVect() sui grandi (ricorsione)
     │         ├── mainVector(): main chain + pending
     │         ├── jacobVect(): ordine di inserimento
     │         └── lower_bound limitato + insert
     │
     ├── processDeque()        <- timer deque
     │    └── stessa cosa con deque
     │
     └── stampa After e i due tempi
```

## Spiegazione funzioni

### Costruttore PmergeMe

Il costruttore riceve un `std::vector<int> input` e se lo salva così com'è in `_input`.

Non fa nessun lavoro: le coppie le crea ogni container per conto suo, dentro il timer. Così il tempo misurato include anche la gestione dei dati, come chiede il subject.

### processVector / processDeque

1. copia `_input` nel proprio container di valori
2. crea il container di indici `0 1 2 ... n-1`
3. chiama `mergeInsertVect()` (o `mergeInsertDeq()`)
4. trasforma gli indici ordinati nei valori e li ritorna

### mergeInsertVect / mergeInsertDeq

È la funzione ricorsiva vera e propria.

```cpp
if (idx.size() < 2)
    return;
```

Caso base: con 0 o 1 elemento non c'è niente da ordinare.

Poi:

- scorre gli indici a due a due, confronta i valori, salva il grande in `bigs` e il piccolo in `partner[big]`
- richiama se stessa su `bigs`
- costruisce main chain e pending con `mainVector()`
- aggiunge lo straggler ai pending se la size è dispari
- inserisce i pending in ordine Jacobsthal con `lower_bound` limitato al grande

Il limite lo trovo così:

```cpp
if (id + 1 < bigs.size())
    bound = std::find(mainchain.begin(), mainchain.end(), bigs[id + 1]);
else
    bound = mainchain.end();
```

`std::find` qui cerca un indice, non un valore, quindi non conta come confronto dell'algoritmo. Il ramo `else` è lo straggler.

### mainVector / mainDeque

Costruisce main chain e pending:

```cpp
mainchain.push_back(partner[bigs[0]]);
for (size_t i = 0; i < bigs.size(); i++)
    mainchain.push_back(bigs[i]);
for (size_t i = 1; i < bigs.size(); i++)
    pending.push_back(partner[bigs[i]]);
```

Cioè: il piccolo della prima coppia, poi tutti i grandi, poi nei pending tutti gli altri piccoli.

## Timing

Per misurare il tempo uso `clock()`.

Il pattern è:

```cpp
start = clock();
// lavoro
elapsed = clock() - start;
```

Poi converto in microsecondi:

```cpp
(static_cast<double>(elapsed) / CLOCKS_PER_SEC) * 1000000.0
```

Il subject dice che il tempo deve includere sia il sort sia la gestione dei dati. Per questo dentro il timer c'è anche la copia dell'input nel container: se creassi le coppie nel costruttore, prima di far partire il timer, il tempo misurato sarebbe falsato.

Su input piccoli la differenza può essere rumorosa o quasi invisibile.

Su input grandi, tipo 3000 numeri, la differenza tra container si vede meglio.

## Error handling ex02

Casi da gestire:

- meno di due numeri in input
- lettere o simboli
- numeri negativi
- overflow di `int`
- stringhe tipo `12abc`
- stringa vuota `""`
- spazi o segno davanti, tipo `" 42"` o `"+42"`

Nel main uso `strtol` perché mi permette di controllare anche dove si ferma la conversione.

```cpp
char *end;
long num = std::strtol(argv[i], &end, 10);
```

Se `*end != '\0'`, significa che la stringa non era un numero pulito.

Esempio:

```text
42abc
```

`strtol` converte `42`, ma si ferma su `a`.

Quindi devo considerarlo errore.

Però `strtol` da solo non basta: accetta in silenzio la stringa vuota (la converte in `0` con `end` già su `'\0'`), gli spazi iniziali e il `+`.

Per questo prima controllo che la stringa sia fatta solo di cifre:

```cpp
static bool isNumber(const char *str)
{
    if (!str[0])
        return false;
    for (size_t i = 0; str[i]; i++)
    {
        if (!isdigit(static_cast<unsigned char>(str[i])))
            return false;
    }
    return true;
}
```

Controllo anche:

```cpp
num < 0 || num > INT_MAX
```

per accettare solo integer nel range di `int`.

Lo `0` lo accetto. Il subject dice "positive integers", quindi a rigore si potrebbe rifiutare: è una scelta, l'importante è saperla motivare in correzione.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- cos'è la STL
- cosa sono container, iteratori e algoritmi
- perché gli algoritmi lavorano su iteratori
- complessità O(1), O(n), O(log n), O(n log n), O(n²)
- perché `map` è adatta per ex00
- come funziona `lower_bound` su map
- perché `stack` è adatto per RPN
- differenza tra notazione infix e RPN
- ordine corretto degli operandi in RPN
- perché vector e deque sono adatti a ex02
- differenza tra memoria contigua e memoria a blocchi
- cos'è un functor e perché lo passo a `lower_bound`
- perché il Ford-Johnson ordina i grandi in ricorsione
- cos'è la main chain
- cosa sono i pending
- a cosa serve Jacobsthal
- come misurare il tempo di esecuzione

## Errori tipici da evitare

- usare lo stesso container in più esercizi quando il subject lo vieta
- pensare che `lower_bound` dia sempre la data precedente: dà la successiva non minore
- invertire left/right in RPN
- dimenticare il controllo divisione per zero
- usare `std::sort` direttamente sull'input finale in ex02 e basta
- usare `std::sort` per ordinare le coppie: l'output è giusto, ma non è Ford-Johnson
- fare la binary search su tutta la main chain invece che fino al grande della coppia
- dimenticare lo straggler quando gli elementi sono dispari
- sfasare gli indici Jacobsthal: sono 1-based e `b1` è già nella main chain
- ritrovare i partner per valore quando ci sono duplicati
- fidarsi di `strtol` senza controllare `end`, e senza scartare `""`, spazi e `+`
- far partire il timer dopo aver già preparato i dati
- stampare tempi con precisione troppo bassa
- invertire la condizione degli anni bisestili

## Mini riassunto mentale

```text
ex00 -> map
perché devo cercare una data ordinata e recuperare il valore associato

ex01 -> stack
perché RPN lavora sempre sugli ultimi due valori disponibili

ex02 -> vector + deque
perché devo confrontare due container indicizzabili applicando Ford-Johnson
coppie -> ricorsione sui grandi -> main chain -> pending in ordine Jacobsthal
```
