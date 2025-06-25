/********************************************************************************
 *                                                                              *
 *                   Architetture dei sistemi di Elaborazione                   *
 *                                                                              *
 ********************************************************************************

 Elaborato 1
 Descrizione:	Cifrare/decifrare il testo data la parola chiave. Nel testo cifrato
				il codice ASCII di ogni carattere � aumentato del codice ASCII del
				corrispondente carattere nella parola chiave sovrapposta (e ripetuta
				se necessario) al testo.

 ********************************************************************************/

#include <stdio.h>

void main()
{
	// Input
	char testo[] = "Questo e' un elaborato di ARC";
	char chiave[] = "Architetture";
	char decifra = 0; // 0 = cifra; 1 = decifra

	// Output
	char risultato[1024];

	__asm
	{
		lea esi, testo          // ESI = puntatore al testo
		lea edi, risultato      // EDI = puntatore al risultato
		lea ebx, chiave         // EBX = puntatore alla chiave
		mov edx, ebx            // EDX = puntatore corrente nella chiave
		xor ecx, ecx            // ECX = contatore (non necessario, ma per chiarezza)

	ciclo_principale:
		mov al, [esi]           // Carica carattere corrente del testo
		test al, al             // Controlla se è il carattere terminatore (0)
		jz fine                 // Se sì, termina

		mov ah, [edx]           // Carica carattere corrente della chiave
		test ah, ah             // Controlla se siamo alla fine della chiave
		jnz usa_carattere       // Se no, usa il carattere
		
		// Se siamo alla fine della chiave, ricomincia dall'inizio
		mov edx, ebx            // Ripristina EDX all'inizio della chiave
		mov ah, [edx]           // Carica il primo carattere della chiave

	usa_carattere:
		cmp decifra, 0          // Controlla se dobbiamo cifrare o decifrare
		je cifra                // Se decifra = 0, cifra
		
		// Decifra: sottrai il valore della chiave
		sub al, ah
		jmp memorizza
		
	cifra:
		// Cifra: aggiungi il valore della chiave
		add al, ah
		
	memorizza:
		mov [edi], al           // Memorizza il risultato
		inc esi                 // Avanza nel testo
		inc edi                 // Avanza nel risultato
		inc edx                 // Avanza nella chiave
		jmp ciclo_principale    // Continua il ciclo

	fine:
		mov byte ptr [edi], 0   // Aggiungi il terminatore di stringa
	}

	// Stampa su video
	printf("Risultato: |%s|\n", risultato);
	{ // N.B. Nella stringa precedente alcuni caratteri possono non essere stampabili: 
	  //    crea anche una stringa con tutti i caratteri come codici ascii esadecimali
		char* t = risultato;
		while (*t)
		{
			printf("\\x%02X", (unsigned char)(*t++));
		}
	}
}


