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
    inizio:
        XOR EAX, EAX          // Azzera EAX (maschera per controllo bit)
        MOV EBX, 0            // EBX = contatore di zeri consecutivi
        XOR ECX, ECX          // Azzera ECX (indice byte corrente)

        MOV ECX, -1           // Inizia da -1 perché incrementa all'inizio del ciclo
        MOV EAX, 1            // Maschera iniziale per il bit LSB (0000 0001)

        MOV maxLung, 0        // Inizializza lunghezza massima a 0
        MOV DX, len           // Carica lunghezza totale in DX
        MOV minLung, DX       // Inizializza minLung a valore grande

        XOR EDX, EDX          // EDX = contatore bit totali elaborati

    scorri_bits:
        TEST EAX, 1           // Verifica se maschera è 0000 0001 (nuovo byte)
        JZ salta_controllo_byte // Se no, non cambiare byte
        INC ECX               // Passa al byte successivo

    salta_controllo_byte:
        TEST AL, vet[ECX]     // Testa bit corrente con AND logico
        JNZ bit_e_uno         // Se risultato != 0, bit è 1

        INC EBX               // Incrementa contatore zeri consecutivi

    verifica_max:
        CMP BX, maxLung       // Confronta lunghezza corrente con massimo
        JLE verifica_prossimo_bit // Se <= maxLung, non aggiornare
        MOV maxLung, BX       // Aggiorna nuovo massimo

    verifica_prossimo_bit:
        JMP prossimo_bit      // Continua senza azzerare contatore

    bit_e_uno:
        // Trovato bit 1: gestisce fine sequenza di zeri
        CMP BX, minLung       // Confronta con minimo attuale
        JGE azzera_contatore  // Se >= minimo, non aggiornare
        CMP BX, 0             // Verifica se ho trovato almeno uno zero
        JE azzera_contatore   // Se no, non aggiornare minimo

        MOV minLung, BX       // Aggiorna nuovo minimo

    azzera_contatore:
        MOV EBX, 0            // Reset contatore zeri per prossima sequenza

    prossimo_bit:
        INC EDX               // Incrementa contatore bit totali
        CMP DX, len           // Verifica se raggiunta lunghezza richiesta
        JE controllo_finale   // Se sì, termina ciclo

        ROL AL, 1             // Ruota maschera a sinistra per prossimo bit
        JMP scorri_bits       // Continua ciclo principale

    controllo_finale:
        // Gestisce ultima sequenza se termina con zeri
        CMP BX, minLung       // Confronta ultima sequenza con minimo
        JGE verifica_esistenza_sequenze // Se >= minimo, non aggiornare
        CMP BX, 0             // Verifica se c'erano zeri finali
        JE verifica_esistenza_sequenze // Se no, non aggiornare

        MOV minLung, BX       // Aggiorna minimo con ultima sequenza

    verifica_esistenza_sequenze:
        // Se nessuna sequenza trovata, imposta -1
        CMP maxLung, 0        // Verifica se trovato almeno un massimo
        JNE termine           // Se sì, termina normalmente
        MOV maxLung, -1       // Imposta -1 (nessuna sequenza)
        MOV minLung, -1       // Imposta -1 (nessuna sequenza)

    termine :
    }
    // Stampa su video
    printf("Lunghezza minima delle sotto-sequenze di 0: %d\n", minLung);
    printf("Lunghezza massima delle sotto-sequenze di 0: %d\n", maxLung);
}
