
-	Approccio ricorsivo alle gerarchie del file system

-	Ogni problema si suddivide in due sottoproblemi risolti in un file comandi

-	Il primo file comandi, quello principale, si dovrà obbligatoriamente nominare FCP.sh
			questo risolverà il primo sotto-problema:
				-	controllare i parametri di invocazione
				-	preparazione per l invocazione del secondo file comandi
				-	invocazione secondo file comandi
				-	eventuali azioni finali

	CONTROLLO PRIMO FILE:
		-	controllo sul numero di parametri, se il numero dei parametri è errato si deve fare l exit
				ACHTUNG: si può avere un controllo stretto (-eq) oppure lasco (greater, less, greater-equal etc..)	
		
		-	è anche necessario adare a controllare che parametri sono, se sono richiesti numeri positivi, directory, file
				i parametri sono sempre passati come stringhe, dovremo quindi andare a controllare cosa rappresentano
				ACHTUNG: i valori di exit devono sempre avere valori di errore crescenti
		-	settare in modo opportuno la variabile PATH per poi farne l EXPORT
				molto spesso andremo a fare
					PATH=`pwd`:$PATH	->	questo va a inserire in path il valore della directory corrente, siccome
											solitamente FCP.sh e FCR.sh sono contenuti proprio lì
											(l export viene fatto per i sistemi datati per la bourne shell)

		-	infine viene chiamato il secondo file comandi ricorsivo, ovvero FRP.sh, il quale farà il vero e proprio lavoro
			richiesto dal problema
		-	possono essere anche presenti azioni da svolgere a termine ricorsione


	SECONDO FILE:
		-	se una dir è traversabile (test -d $dir -a -x $dir)	