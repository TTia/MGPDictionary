# MGPDictionary
Progetto didattico, sviluppato per il corso di MGP -Modular and Generic Programming- dei professori Davide Ancona e Giovanni Lagorio durante l'a.a. 2013/2014 presso la facoltà di Informatica - DIBRIS - UNIGE.
### Obiettivi del progetto
L'obiettivo del progetto è l'approfondimento delle meccaniche del C++, con particolare attenzione alla Standard Template Library e alla libreria di Boost -versione 1.55-, per l'implementazione di degli iteratori e l'uso dei concept.
#### L'interfaccia
Nonostante all'interno della libreria sviluppata sia assente un'interfaccia comune per le implementazioni dei dizionari, le tre operazioni standard, *insert*, *search* e *delete*, condividono gli stessi prototipi.

L'operazione di insert, descritta qui dal prototipo: *bool insert(const Key, const Value&, Value* = nullptr);*, permette l'inserimento di nuovi elementi all'interno del dizionario. Il metodo fornisce anche le funzionalità di *update*: combinando il valore di ritorno booleano ed il terzo parametro opzionale, è possibile ottenere il valore precedente l'aggiornamento del dizionario. Ovviamente, l'area di memoria puntata dal terzo parametro deve essere allocata e scrivibile.
Utilizzando il terzo parametro, il metodo restituisce *true* se all'indirizzo è presente il valore sovrascritto; *false* altrimenti.

L'operazione di rimozione di un elemento è descritta dal prototipo *bool del(const Key, Value* = nullptr);*. Per rimuovere un elemento presente all'interno del dizionario, è necessario indicare la chiave ad esso associata. Come per l'operazione di *insert*, è disponibile una funzionalità opzionale: in particolare l'utente può ottenere il valore associato alla chiave prima della rimozione. Utilizzando il secondo parametro, il metodo restituisce *true* se all'indirizzo è presente il valore rimosso; *false* altrimenti.

L'ultima primitiva è la ricerca, descritta dal protipo *iterator search(const Key);*. Data una chiave, è restituito un iteratore: nel caso in cui un elemento sia associato con la chiave fornita sia presente nel dizionario, l'iteratore farà riferimento alla corretta coppia *(Chiave, Valore)*; altrimenti il puntatore coinciderà con il terminatore *end()*.

#### Comparazione con le interfacce in {Java, C#, C++, Python}
Per la definizione delle primitive è stato preso spunto dalle implementazioni dalle librerie standard dei più comuni linguaggi di programmazione. Osservando la libreria *map* di C++, ed in particolare dalla versione C++11, è stato scelto di implementare l'operazione di *search* sfruttando gli iteratori come tipo di ritorno. A differenza dell'operazione *map::find*, che espone esclusivamente iteratori sui valori, le implementazioni dei dizionari presenti in questo progetto restituiscono iteratori sulle coppie (Chiave, Valore).
Fra i diversi linguaggi osservati, Python offre i costrutti più "user-friendly" e meglio integrati nella sintassi: prendendo spunto da questo fatto è stato introdotto l'operatore *[ ]* per semplificare l'accesso alla libreria, similarmente anche al comportamento dei dizionari di C#.
Da Java invece è esportata la duttilità dei metodi di *put* e *remove*, che rispettivamente supportano anche le operazioni di *update* e *pop*. Riassumendo, per le implementazioni presenti in questa libreria è stata mostrata particolare attenzione alla semplicità e comodità d'uso, cercando di soddisfare le abitudini e le necessità di diverse tipologie di utenti.

### Close Addressing
[![Close Addressing by MIT](https://developers.google.com/youtube/images/YouTube_logo_standard_white.png)](https://www.youtube.com/watch?v=0M_kIqhwbFo "Close Addressing video by MIT")

Lo schema classico di un dizionario con indirizzamento fisso prevede l'uso di un'array di bucket, ai quali sono associati sequenze di coppie (Chiave, Valore). Data una chiave, si effettua il calcolo dell'impronta all'interno del codominio [0, m), con *m* pari al numero di bucket della tabella, individuando la corrispondente sequenza sulla quale compiere l'operazione. Nel caso in cui la chiave ricercata sia già presente, si assume che la funzione di hashing della chiave abbia un comportamento deterministico, viene aggiornato il valore.
Per l'operazione di ricerca, è effettuata la ricerca lineare sulla lista di coppie, confrontando ad ogni iterazione la chiave fornita con l'attuale; nel caso in cui la ricerca dia esito positivo la coppia è fornita all'utente.
L'operazione di delete si comporta in maniera comparabile alla ricerca, effettuando però la rimozione della coppia dalla struttura dati.

Oltre alle funzionalità classiche, è stato anche prevista il re-hashing: raggiunto un certo load-factor, rappresentato dal rapporto *#elementi/#bucket*, è avviato il processo di ridimensionamento della tabella, che consiste nell'allocazione di un'altra struttura, in sostituzione della precedente, e della copia degli elementi presenti, previo ricalcolo dell'impronta in funzione della nuova dimensione.
Dato il costo elevato dell'operazione, è stato scelto di svolgerla in maniera incrementale: ad ogni nuovo inserimento e cancellazione viene eseguito una porzione di rehashing, di default pari a *0.1 * #bucket*. Il rehashing è compiuto in entrambe le direzioni, è possibile fissare il load-factor massimo e minimo ed anche la dimensione minima della tabella. L'operazione di search, così come l'accesso tramite l'operatore parentesi quadre, forza però la struttura a completare l'eventuale processo in atto.

E' prevista la possibilità di influenzare la velocità del processo ad hoc, tramite l'apposito metodo *forceRehash* e opzionalmente specificando la percentuale di elementi da ricollocare, di default il processo effettua il ricalcolo e lo spostamento di tutti gli elementi rimasti.

#### Metodi di Hashing
Sono forniti alcuni di metodi di hashing: tramite divisione, tramite divisione rivista da Knuth, tramite "moltiplicazione" ed il metodo universale. Per un'analisi delle proprietà, di eventuali difetti in termini di clustering di primo e secondo livello si rimanda al seguente link: [Mit - Open Addressing](http://www.cse.unt.edu/~tarau/teaching/AnAlgo/MIT_PDF_slides/MIT6_006F11_lec08.pdf "Mit - Open Addressing").

### Open Addressing
[![Open Addressing by MIT](https://developers.google.com/youtube/images/YouTube_logo_standard_white.png)](https://www.youtube.com/watch?v=rvdJDijO2Ro "Open Addressing video by MIT")

All'interno della libreria è presente anche un'implementazione di dizionario con indirizzamento aperto. Rispetto alla versione già analizzata i bucket non corrispondono a sequenza di coppie (Chiave, Valore), bensì puntano alla singola coppia corrispondente. Il nome *"Open Addressing"* è dato dall'algoritmo di probing che sta alla base dell'indirizzamento: le collisioni possibili nelle operazioni sono risolte iterando la funzione di normalizzazione in un range [0, m). Le funzioni di probing sono descritte da tre parametri: numero di bucket *#m*, chiave da normalizzare *k* e in intero *i* che indica l'iterazione attuale. Caratteristica dei metodi di probing è restituire risultati periodici su *km*.

La cancellazione effettuando il probing può incorrere nei seguenti casi: trova l'elemento con chiave corrispondente e lo elimina sostituendo la coppia con un puntatore ad una particolare coppia costante "DELETED", fornendo eventualmente l'elemento rimosso all'utente; trova un bucket "DELETED" e prosegue nell'iterazione; trova un bucket vuoto e ferma la ricerca.
L'operazione di insert effettua il probing fino a trovare un bucket vuoto o marcato come "DELETED" e inserisce la coppia -in via teorica, grazie al rehashing, non si dovrebbe andare incontro al riempimento della tabella-. L'inserimento di un valore associato ad una chiave già esistente, provoca l'aggiornamento della coppia.

La ricerca si comporta in maniera simile alla rimozione, con l'eccezione che restituisce un iteratore all'elemento richiesto se presente.

Le funzionalità di rehashing sono disponibili anche per questa implementazione.

#### Metodi di Probing (...e di Hashing)
Sono forniti alcuni metodi di probing: lineare, quadratico e tramite doppio hashing. Nuovamente, per un'analisi più accurata delle proprietà si rimanda alla seguente risorsa esterna: [Mit - Close Addressing](http://courses.csail.mit.edu/6.006/fall11/lectures/lecture10.pdf "Mit - Close Addressing").
Le classi che implementano le funzionalità di probing sfruttano i metodi di hashing elencati in precedenza, lasciando all'utente la possibilità di accoppiare a proprio piacimento i metodi di hashing e di probing. L'unico requisito per implementare nuovi metodi è l'overriding dell'operatore funzione con due o tre parametri, nel caso che si voglia implementare rispettivamente un metodo di hash o di probing.

### Gli iteratori e Boost
Per ciascun dizionario sono forniti tre iteratori bidirezionali, sulle coppie, chiavi e valori. Sfruttando la libreria di [Boost 1.55](http://www.boost.org/doc/libs/1_55_0 "Boost 1.55"), ed in particolare la classe generica *[iterator_facade](http://www.boost.org/doc/libs/1_55_0/libs/iterator/doc/facade-and-adaptor.html "iterator_facade")*, selezionado tale classe come classe padre del proprio iteratore è sufficiente implementare 4 operazioni essenziali {incremento, decremento, confronto e dereference} e configurare opportunamente i parametri di tipo della super-classe per derivare le altre funzionalità richieste dallo standard per un *BidirectionalIterator*.
Definito l'iteratore principale, che itera sulle coppie (Chiave, Valore), non è stato possibile implementare le altre due versioni sfruttando l'ereditarietà a causa dei parametri di tipo utilizzati, infatti uno dei parametri richiesti dalla classe *iterator_facade* è il tipo risultato dell'operazione di dereference; si è rivelato quindi necessario "decorare" la classe base con le funzionalità degli iteratori su chiavi e valori.

Ciascuna classe definisce i tre diversi iteratori, *::iterator*, *::iterator_key* ed *::iterator_value*, e fornisce i relativi metodi *begin{, _value, _end}()* e *end{, _value, _key}()* per iterare sugli elementi presenti. Data la necessità completare un'eventuale processo di rehashing, i metodi appena elencati non possono essere considerati *const*.

Tutti gli iteratori presenti nella libreria riconoscono le eventuali modifiche strutturali eseguite sulla struttura a cui si riferiscono: tramite un contatore di versione, esponendosi purtroppo alle stesse problematiche degli iteratori in Java 1.7, verificano eventuali cambiamenti sulla struttura dati corrispondente e invalidano ogni tentativo di operazione tramite l'iteratore stesso.

### Il layout delle classi
Oltre ai prototipi delle principali funzioni, entrambe le classi condividono i tipi *Table*, *Hash*, *Pair* -seppur corrispondenti a tipi differenti- e i tipi iteratori già discussi. E' stato possibile quindi collocare alcune operazioni in una classe comune. La libreria presenta la seguente struttura:

``` c++
template<typename Key, typename Value, typename Method>
class ChainingHashTable;
template<typename Key, typename Value, typename Method>
class ProbingHashTable;
template <typename Dictionary, typename Key, typename Value, typename Method>
class Core: public Dictionary;
template <typename Key, typename Value, typename Method = DivisionMethod>
class CloseAddressingDictionary: public Core<ChainingHashTable<Key, Value, Method>, Key, Value, Method>;
template <typename Key, typename Value, typename Method = LinearProbing>
class OpenAddressingDictionary: public Core<ProbingHashTable<Key, Value, Method>, Key, Value, Method>;
```

### I concept di Boost
### GTest
Il progetto è accompagnato da un numero di test importante: la maggior parte delle funzionalità è stata implementata, corretta e verificata parallelamente allo sviluppo di una suite di unit-test. La libreria sfruttata è GTest 1.7. Link

### Valgrind
Oltre alla correttezza funzionale verificata dai test, sono stati effettuati controlli con [ValGrind](http://valgrind.org/ "ValGrind"). per verificare la presenza di leak ed in generale l'uso non corretto della memoria. L'esecuzione dei quasi 90 casi di test tramite il tool non causa alcun problema di questo tipo.
