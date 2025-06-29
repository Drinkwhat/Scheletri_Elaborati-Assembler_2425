/********************************************************************************
*                                                                              *
*                   Architetture dei sistemi di Elaborazione                   *
*                                                                              *
********************************************************************************

Elaborato 2
Descrizione: Dato in input il livello massimo richiesto, restituire il rispettivo
			 triangolo di Tartaglia memorizzando consecutivamente i vari livelli
			 all'interno di un array monodimensionale.

********************************************************************************/

#include <stdio.h>

void main()
{
	// Input
	unsigned int livello = 0;		// livello massimo del triangolo di tartaglia da generare

	// Output
	unsigned int triangolo[1024];	// risultato: il vettore contiene i numeri del 
									// triangolo dal livello 0 al più alto richiesto

	unsigned int i;
	unsigned int k = 0;

	__asm
	{
		// Inizializzazione
		mov eax, 0					// eax = indice corrente nell'array triangolo
		mov ebx, 0					// ebx = livello corrente
		mov ecx, livello			// ecx = livello massimo
		
	livello_loop:
		cmp ebx, ecx				// confronta livello corrente con massimo
		ja fine						// se livello > massimo, termina
		
		// Primo elemento del livello corrente = 1
		mov triangolo[eax*4], 1
		inc eax
		
		// Se siamo al livello 0, passa al livello successivo
		cmp ebx, 0
		je prossimo_livello
		
		// Elementi centrali del livello
		mov edx, 1					// edx = posizione nell'elemento corrente del livello
		
	elemento_loop:
		cmp edx, ebx				// se posizione >= livello corrente
		jae ultimo_elemento			// vai all'ultimo elemento
		
		// Calcola triangolo[pos] = triangolo[pos_sopra_sx] + triangolo[pos_sopra_dx]
		push eax					// salva indice corrente
		push ebx					// salva livello corrente
		push edx					// salva posizione elemento
		
		// Calcola posizione elemento sopra a sinistra
			// Formula: pos_sopra_sx = pos_corrente - livello_corrente + (posizione_elemento - 1)
		mov esi, eax				// esi = posizione corrente
		sub esi, ebx				// esi -= livello corrente (torna all'inizio livello precedente)
		add esi, edx				// esi += posizione elemento
		sub esi, 2					// esi -= 2 (posizione sopra a sinistra)
		
		// Calcola posizione elemento sopra a destra  
		mov edi, esi				// edi = posizione sopra a sinistra
		inc edi						// edi = posizione sopra a destra
		
		// Somma i due elementi
		mov esi, triangolo[esi*4]	// carica elemento sopra a sinistra
		add esi, triangolo[edi*4]	// somma elemento sopra a destra
		mov triangolo[eax*4], esi	// memorizza risultato
		
		pop edx						// ripristina posizione elemento
		pop ebx						// ripristina livello corrente
		pop eax						// ripristina indice corrente
		
		inc eax						// incrementa indice array
		inc edx						// incrementa posizione elemento
		jmp elemento_loop
		
	ultimo_elemento:
		// Ultimo elemento del livello = 1
		mov triangolo[eax*4], 1
		inc eax						// incrementa indice array
		
	prossimo_livello:
		inc ebx						// incrementa livello
		jmp livello_loop
		
	fine:
	}

	// Stampa su video
	printf("Triangolo di Tartaglia fino al livello %d\n", livello);
	for (i = 0; i <= livello; i++)
	{
		unsigned int j;
		for (j = 0; j <= i; j++)
			printf("%d ", triangolo[k++]);
		printf("\n");
	}
}


