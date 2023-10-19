# Sviluppo
Di seguito idee iniziali sullo sviluppo.

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
- Non permettere all'utente di posizionare domino che non possono essere posizionati
- Tieni traccia del punteggio e mostralo all'utente sempre.
- Fai sì che la view sia stabile:
Quando si posizionano i tasselli dalla mano al tavolo,
anziché spostare tutto di conseguenza, logga un accapo.
In questo modo la riga in cui l'utente inserisce i dati non si muove.
- Feedback: Ad ogni input dell'utente, il sistema deve loggare un feedback.
Il feedback potrà essere:
  - Domino `[x|y]` posizionato sul tavolo.
  - Il domino `[x|y]` non si può posizionare ora.
  - Messaggio di help nel caso di input non testuali.
- Funzione ricorsiva dovrà essere in grado di calcolare
tutte le possibili soluzioni dalla situazione attuale e consigliare l'utente per quella con maggior punteggio.
  - Si potrebbe aggiungere un * a fianco della mossa consigliata.
  - Deve essere possibile abilitare questa opzione.
- Il gioco deve finire se non vi sono mosse ammesse.
- Mostrare i domino non selezionabili in quel momento in grigetto. Il domino consigliato in verde.