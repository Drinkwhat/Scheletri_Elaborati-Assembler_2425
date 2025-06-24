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
		LEA ESI, testo          // ESI = puntatore al testo
		LEA EDI, risultato      // EDI = puntatore al risultato
		LEA EBX, chiave         // EBX = puntatore alla chiave
		MOV EDX, EBX            // EDX = puntatore corrente nella chiave
		XOR ECX, ECX            // ECX = contatore 

	ciclo_principale:
		MOV AL, [ESI]           // Carica carattere corrente del testo
		TEST AL, AL             // Controlla se è il carattere terminatore (0)
		JZ fine                 // Se sì, termina

		MOV AH, [EDX]           // Carica carattere corrente della chiave
		TEST AH, AH             // Controlla se siamo alla fine della chiave
		JNZ usa_carattere       // Se no, usa il carattere
		
		// Se siamo alla fine della chiave, ricomincia dall'inizio
		MOV EDX, EBX            // Ripristina EDX all'inizio della chiave
		MOV AH, [EDX]           // Carica il primo carattere della chiave

	usa_carattere:
		CMP decifra, 0          // Controlla se dobbiamo cifrare o decifrare
		JE cifra                // Se decifra = 0, cifra
		
		// Decifra: sottrai il valore della chiave
		SUB AL, AH
		JMP memorizza
		
	cifra:
		// Cifra: aggiungi il valore della chiave
		ADD AL, AH
		
	memorizza:
		MOV [EDI], AL           // Memorizza il risultato
		INC ESI                 // Avanza nel testo
		INC EDI                 // Avanza nel risultato
		INC EDX                 // Avanza nella chiave
		JMP ciclo_principale    // Continua il ciclo

	fine:
		MOV BYTE PTR [EDI], 0   // Aggiungi il terminatore di stringa
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


