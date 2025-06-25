/********************************************************************************
 *                                                                              *
 *                   Architetture dei sistemi di Elaborazione                   *
 *                                                                              *
 ********************************************************************************

 Elaborato 3
 Descrizione:	Data una sequenza di bit, restituire la lunghezza minima e
				massima	delle sotto-sequenze di bit a 0.

 ********************************************************************************/

#include <stdio.h>

void main()
{
	// Input
	unsigned char vet[] = { 0xBC,0xFF,0x01 };	// Sequenza di bit
	unsigned short int len = 18;				// Lunghezza (in bit) della sequenza

	// Output
	short int minLung;		// Lunghezza minima sotto-sequenza di 0
	short int maxLung;		// Lunghezza massima sotto-sequenza di 0

	__asm
	{
    // Inizializzazione: imposta minLung e maxLung a -1 (nessuna sequenza trovata)
    MOV  WORD PTR [minLung], -1    // minLung = -1
    MOV  WORD PTR [maxLung], -1    // maxLung = -1

    // CX = indice del bit corrente (i), BX = lunghezza corrente sequenza di zeri
    XOR  CX, CX                    // CX = 0 (inizia dal primo bit)
    XOR  BX, BX                    // BX = 0 (nessuna sequenza di zeri attiva)

ciclo_iniziale:
    // Verifica se abbiamo analizzato tutti i bit
    CMP  CX, [len]                 // confronta indice corrente con lunghezza totale
    JAE  fine                      // se i >= len, termina il ciclo

    // Estrae il bit i-esimo usando modalità LSB-first:
    // 1. Calcola byteIndex = i / 8 (i >> 3)
    // 2. Calcola bitPos = i % 8 (i & 7)
    // 3. Carica il byte e estrae il bit
    PUSH CX                        // salva CX per calcolo byteIndex
    SHR  CX, 3                     // CX = i / 8 (indice del byte)
    MOV  DL, [vet + CX]           // DL = byte contenente il bit
    POP  CX                        // ripristina CX (indice bit originale)
    AND  CX, 7                     // CX = i % 8 (posizione bit nel byte)
    MOV  AL, DL                    // AL = byte da analizzare
    SHR  AL, CL                    // sposta bit desiderato in posizione LSB
    AND  AL, 1                     // isola solo il bit di interesse

    // Analizza il valore del bit estratto
    CMP  AL, 0                     // verifica se il bit è 0
    JNE  bit_uno                   // se bit = 1, gestisci fine sequenza

    // Il bit è 0: incrementa contatore sequenza di zeri
    INC  BX                        // BX++ (aumenta lunghezza sequenza)
    JMP  incrementa_i              // passa al bit successivo

bit_uno:
    // Il bit è 1: potenziale fine di una sequenza di zeri
    CMP  BX, 0                     // verifica se c'era una sequenza attiva
    JLE  incrementa_i              // se BX <= 0, nessuna sequenza da chiudere

    // Aggiorna minLung se necessario
    MOV  AX, [minLung]             // AX = valore corrente di minLung
    CMP  AX, -1                    // verifica se minLung è ancora non inizializzato
    JE   imposta_min               // se sì, imposta direttamente
    CMP  BX, AX                    // confronta lunghezza corrente con minimo
    JGE  salta_min                 // se BX >= minLung, non aggiornare
imposta_min:
    MOV  [minLung], BX             // minLung = BX (nuovo minimo)
salta_min:

    // Aggiorna maxLung se necessario
    MOV  DX, [maxLung]             // DX = valore corrente di maxLung
    CMP  DX, -1                    // verifica se maxLung è ancora non inizializzato
    JE   imposta_max               // se sì, imposta direttamente
    CMP  BX, DX                    // confronta lunghezza corrente con massimo
    JLE  reset_corrente            // se BX <= maxLung, non aggiornare
imposta_max:
    MOV  [maxLung], BX             // maxLung = BX (nuovo massimo)

reset_corrente:
    XOR  BX, BX                    // azzera contatore per prossima sequenza

incrementa_i:
    INC  CX                        // passa al bit successivo
    JMP  ciclo_iniziale            // continua il ciclo

fine:
    // Gestisce eventuale sequenza di zeri che termina a fine array
    CMP  BX, 0                     // verifica se c'è una sequenza aperta
    JLE  fine_elaborazione         // se no, termina

    // Aggiorna minLung per l'ultima sequenza
    MOV  AX, [minLung]             // AX = minLung corrente
    CMP  AX, -1                    // se non ancora inizializzato
    JE   imposta_min2              // imposta direttamente
    CMP  BX, AX                    // confronta con minimo corrente
    JGE  fine_min                  // se BX >= minLung, non aggiornare
imposta_min2:
    MOV  [minLung], BX             // aggiorna minLung
fine_min:

    // Aggiorna maxLung per l'ultima sequenza
    MOV  DX, [maxLung]             // DX = maxLung corrente
    CMP  DX, -1                    // se non ancora inizializzato
    JE   imposta_max2              // imposta direttamente
    CMP  BX, DX                    // confronta con massimo corrente
    JLE  fine_elaborazione         // se BX <= maxLung, termina
imposta_max2:
    MOV  [maxLung], BX             // aggiorna maxLung

fine_elaborazione:

}
	// Stampa su video
	printf("Lunghezza minima delle sotto-sequenze di 0: %d\n", minLung);
	printf("Lunghezza massima delle sotto-sequenze di 0: %d\n", maxLung);
}
