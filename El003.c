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

int main()
{
    // Input
    unsigned char vet[] = { 0xBC,0xFF,0x01 };	// Sequenza di bit
        unsigned short int len = 18;				// Lunghezza (in bit) della sequenza

	// Output
	short int minLung;		// Lunghezza minima sotto-sequenza di 0
	short int maxLung;		// Lunghezza massima sotto-sequenza di 0

	__asm
	{
    // Inizializza contatori e flag
    MOV EAX, 0            // EAX = contatore corrente di zeri
    MOV BX, -1            // BX = minLung  (-1 = nessuna sequenza trovata)
    MOV DX, -1            // DX = maxLung  (-1 = nessuna sequenza trovata)

    XOR ESI, ESI          // ESI = indice bit corrente (da 0 a len-1)
    MOV CX, len           // CX = numero totale di bit

scorri_bit:
    // Controlla se abbiamo processato tutti i bit
    CMP ESI, ECX
    JGE fine_loop

    // Calcola byte_index = ESI / 8 e carica vet[byte_index]
    MOV EAX, ESI
    SHR EAX, 3
    MOV BL, [vet + EAX]

    // Calcola offset LSB-first nel byte: offset = ESI mod 8
    MOV EAX, ESI
    AND EAX, 7            // EAX = ESI % 8
    MOV CL, AL            // CL = offset

    // Estrai il bit: (vet[byte] >> offset) & 1
    MOV AL, BL
    SHR AL, CL
    AND AL, 1

    // Se bit = 0, aumenta contatore zeri
    CMP AL, 0
    JNE gestisci_1
    INC EAX               // contatore++
    JMP prossimo_bit

gestisci_1:
    // Se contatore >0, chiude una sequenza di zeri
    CMP EAX, 0
    JE prossimo_bit

    // Se prima sequenza (BX==-1), inizializza min/max
    CMP BX, -1
    JE primo_init
    // Altrimenti aggiorna minimo se EAX < BX
    CMP AX, BX
    JGE salta_min
    MOV BX, AX
salta_min:
    // Aggiorna massimo se EAX > DX
    CMP AX, DX
    JLE salta_max
    MOV DX, AX
salta_max:
    MOV EAX, 0            // reset contatore zeri
    JMP prossimo_bit

primo_init:
    MOV BX, AX            // primo minLung = AX
    MOV DX, AX            // primo maxLung = AX
    MOV EAX, 0            // reset contatore
    JMP prossimo_bit

prossimo_bit:
    INC ESI               // passa al bit successivo
    JMP scorri_bit

fine_loop:
    // Gestisce eventuale sequenza aperta a fine vettore
    CMP EAX, 0
    JE prepara_output
    // stessa logica di chiusura di sopra
    CMP BX, -1
    JE primo_init_end
    CMP AX, BX
    JGE salta_min2
    MOV BX, AX
salta_min2:
    CMP AX, DX
    JLE salta_max2
    MOV DX, AX
salta_max2:

primo_init_end:
    MOV BX, AX            // imposta min/max all'ultima sequenza

prepara_output:
    MOV minLung, BX       // minLung = BX
    MOV maxLung, DX       // maxLung = DX
	}

	// Stampa su video
    printf("Lunghezza minima delle sotto-sequenze di 0: %d\n", minLung);
    printf("Lunghezza massima delle sotto-sequenze di 0: %d\n", maxLung);
    return 0;
}

