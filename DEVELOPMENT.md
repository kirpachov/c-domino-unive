# Sviluppo
Di seguito idee iniziali sullo sviluppo.

## Compile and run
```bash
make -f makefile run
```

## Test
```bash
make -f makefile test
```

## Modalità
Devono esserci tre possibili modalità di gioco.
1. **Interattiva**: l'utente gioca con tessere assegnate a caso.
2. **AI**: una funzione risolve il gioco autonomamente con delle tessere assegnate a caso. Il numero di tessere 
3. **Challenge**: Specifiche nel documento.

## Layout
Bisogna permettere all'utente di visualizzare:
- Tavolo da gioco con tutti i domino che vi sono stati messi
- Punteggio attuale (in alto a dx)
- Domino che il giocatore ha in mano:
  - In grigio quelli che non si possono mettere.
  - In bianco quelli che si possono mettere invece.
  - In verde la mossa consigliata.

## Log
Dovrà essere presente un sistema di log.
Un file `game.log` verrà scritto mentre si gioca.

I log dovranno chiaramente avere un livello di logging,
che può essere impostato da configurazione.

Ciò permetterà:
- Scrivere codice "commentato", dove i commenti saranno i log stessi
- Debug semplificato nel momento in cui vi sono presenti bug

## TODO
- All'inizio della partita all'utente non deve essere chiesto il lato dove mettere la card, perché il tavolo è vuoto.
- Tieni traccia del punteggio e mostralo all'utente sempre.
- Feedback: Ad ogni input dell'utente, il sistema deve loggare un feedback.
Il feedback potrà essere:
  - Domino `[x|y]` posizionato sul tavolo.
  - Il domino `[x|y]` non si può posizionare ora.
  - Messaggio di help nel caso di input non testuali o invalidi.
- Funzione ricorsiva dovrà essere in grado di calcolare
tutte le possibili soluzioni dalla situazione attuale e consigliare l'utente per quella con maggior punteggio.
  - Si potrebbe aggiungere un * a fianco della mossa consigliata.
  - Deve essere possibile abilitare questa opzione.
- Il gioco deve finire se non vi sono mosse ammesse.
- Mostrare i domino non selezionabili in quel momento in grigetto. Il domino consigliato in verde.
- Leggersi l'universo dei domino da file o da configurazione runtime
- Github actions:
  - Build in ubuntu
  - Build in windows (se possibile)
  - Build in macos (se possibile)
### Testing
Al momento poco e niente è stato testato.
Bisognerebbe implementare i test per le funzionalità più importanti.

### non-challenge (5 nov 23)
- TODO: esci dal gioco con `CTRL+D`

c'è la funzione che interpreta la selezione dell'utente ma manca la possibilità di selezionare
il domino con una stringa di due caratteri tipo "5l", "5L", "5R" oppure "5R". bisognerà aggiungere
questa opzione.

Fatto ciò bisognerà processare la richiesta di selezione del domino dell'utente e fornire feedback,
positivo o negativo che sia.

Dopo di che:
- La funzione ricorsiva deve ricalcolare la miglior mossa in quel momento
- bisogna ricalcolare il punteggio attuale dell'utente
- printare lo schermo aggiornato
- verificare se è possibile continuare il gioco; se non è possibile bisognerà uscire.