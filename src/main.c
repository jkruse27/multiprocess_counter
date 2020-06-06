/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

// Funcao que verifica se o numero recebido como parametro eh primo
// Se for, retorna 1, caso contrario retorna 0
int is_prime(int num){
	if(num < 2)
		return 0;
	for(int i = 2; i <= num/2; i++)
		if(num % i == 0)
			return 0;
	return 1;
}

int main() {
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	int *count; // numero de valores primos
	int child = 0; // numero de filhos ativos
	
	count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);	

	int c, status, pos = 0;
	pid_t pid = 0;

	do{
		// Recebe 1 inteiro da entrada 
		scanf("%d", &c);

		// Se os 4 filhos estiverem ocupadas, espera um deles acabar (e limpa todos os filhos que acabaram antes)
		if(child == 4){
			for(; waitpid(-1, &status, WNOHANG) > 0; --child);
			for(; child >= 4; --child)
				wait(&status);
		}
		
		//Cria um novo processo para verificar se o numero eh primo
		child++;
		pid = fork();
		if(pid == 0){
			(*count) += is_prime(c);
			exit(0);	
		}	
	}while(getchar() != '\n');
	
	// Espera que todas os processos filhos acabem
	while(wait(&status) != -1);	

	printf("%d\n", *count);
	return 0;
}
