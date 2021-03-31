
	-	ps	->	Mostra i processi	(args: --full --long -e(mostra tutti i processi))
	-	man	-> Mostra il manuale riguardo un comando
	-	which	-> Mostra dove si trovi un comando (usually /bin)
	-	bash / sh	-> invoco altre shell o bash (args: -x)
	-	id	-> Mostra info riguardo l utente in uso corrente
	-	who / w	-> 'w' ci mostra qualche log in più
	-	ls	->	(args: -l -a -h -A(mostra tutto) -R(ricorsivo) -r(inverte alfabetico) 
				-t(ultima modifica) -d(info specifiche di un determinato file) -i (stampa anche l i-number, ovvero))
	-	touch	-> va a mettere al momento l ultima modifica, non serve quindi solo per creare un file dal nulla
	-	chmod, chown & chgrp (per questi ultimi due si deve essere superuser)
	-	ln
	-	mkdir
	-	pwd
	-	rm	->	Il primo obbiettivo è cancellare un link hw	(args: -i (da usare sempre, questa
				va ad eliminare l ultimo link, ovvero va a decrementare di uno il valore di link
				attaccati ad un file
	-	mv
	-	> \ <	->	Permette di andare a scrivere su un file l output (overwrite)
	-	>> \ <<	-> Invece di fare l overwrite fa l append
	-	more	->	Mostra il testo per parti, mentre cat lo mostra stampato integralmente in una volta
                    space (page skip), enter (line skip)
	-	cat		->	Stampa integralmente, spesso usato per casting
	-	sort	->	Fa' il sorting attraverso il casting, come 'sort < file' seguendo l ordine ASCII, prima spazi 			bianchi, poi maiuscole e infine minuscole in ordine alfabetico, per impostare questo tipo di 
				ordinamento scrivo "LC_ALL=C" + "export LC_ALL"	(args: -f (diventa case insensitive) -c (per check, ci dice se un file è in ordine alfabetico) -C (sempre un check con storage del valore
				in "echo $?", 1 se disordinato, 0 se sorted) -u (rimuove le righe doppie equivalenti))
				
				useful:
					"sort < unsorted_file > sorted_file"
	-	grep	->	(args: -n (mostra il numero d ordine delle linee) -i (diventa case insensitive)
					-v (riporta soltanto le linee che non contengono la string specificata) ) 
				
				subcommands:
					->	"grep string < textfile" (stampa tutte le linee in cui 'string' compare
					->	"grep ing < textfile" (si può usare anche con frazioni"
					->	"grep 'string ' < textfile" (single quote)
					->	"grep '^str' < textfile" (considera tutte le linee che iniziano per 'str')
					->	"grep 'ing$' < textfile" (considera tutte le linee che finiscono per 'ing')
						-> "grep '\.$' < textfile" (considera tutte le lInee terminano col punto usando l escape)
	-	rev		-> Inverte la stampa (use: rev < file)
	-	wc		->	{lines;	words; chars} con args -l (lines) -w (words) -c (chars), WARN: usare sempre la 						ridirezione con < altrimenti nello stdout avremo 'output + filename'
	-	head/tail	-> args( -{numberoflines}, senza l arg considerano come valore 10 ), stampano/considerano le 				prime (head) o ultime (tail) {numberoflines} linee (ex: head -5 < filename)
	-	tee			-> va a creare un file con scritto il risultato parizale del e nel processo di 						piping
	-	kill {PID}
    -   echo $  ->  Fa l echo riguardo l ultimo comando eseguito
    -   date    -> Locale del sistema
    -   diff    -> Riporta la differenza di linee tra due file ( '<' si riferisce al primo file e '>' al secondo )
    -   find
    -   env     -> Shows all environments vars
    -   eval    ->  Usato quando si assegna un comando come stringa ad una variabile
                        ex: x=ls, eval $x
    -   shift   -> con lo shift pediamo il parametro più piccolo di valore, $1->eliminato, $2->$1, 
                                            $3->$2 etc..
    -   set
    -   test    -> serve per valutare un espressione, ha valore 0 se success altrimenti != 0
                        (args: -f esistenza di un file, -d esistenza directory, -r se si ha il diritto
                                di lettura/scrittura su quel file con -x e -w)
                        
                        ex:
                            -   test stringa1 != stringa 2      SPAZIO OBBLGIATORIO (così non è  
                                                                            assegnamento)
                            -   test -z stringa                 Verifica se è nulla (senza -z verifica 
                                                                            che non è nulla)
                            -   test numero1 [-eq -ne -gt -ge -lt -le] numero2
                                                    
                                            
                            -   test
                            
                        ATTENZIONE: 
                            Test tra numeri si deve usare -eq, mentre per le stringhe ' = ' spaziato
    -   exit
    -   read {nome_var} -> spesso seguita da "if test $var = yes", legge da StdIn
