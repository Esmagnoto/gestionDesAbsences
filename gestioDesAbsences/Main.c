#include <stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)
#include <string.h>

enum { MAX_CARACTERES = 31, MAX_ETUDIANTS = 15 };

typedef struct {
	char nomEtudiant[MAX_CARACTERES];
	int numeroGroupe;
} Etudiant;

//static int verifierEtudiants(Etudiant tableEtudiants[], char entree[]) {}

int main() {
	char entree[MAX_CARACTERES] = "entree";
	Etudiant etudiants[MAX_ETUDIANTS];
	int cmpEtudiants = 0;
	//entree[MAX_CARACTERES - 1] = '\0'; // ajoute explicitement le caractère nul


	while (1) {

		printf("ecrire une chaine: ");
		scanf("%30s", &entree);
		if (strcmp(entree, "exit") == 0)
		{
			return 0;
		}
		else if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_CARACTERES];
			int numeroGroupe;
			scanf("%30s", nomEtudiant);
			if (scanf("%d", &numeroGroupe) == 1) {
				if (strcmp(etudiants[cmpEtudiants].nomEtudiant, nomEtudiant) == 0) {
					printf("Nom  Incorrect\n");
				}
				else
				{
					strcpy(etudiants[cmpEtudiants].nomEtudiant, nomEtudiant);
					etudiants[cmpEtudiants].numeroGroupe = numeroGroupe;
					printf("Inscription realisee (%d)\n", cmpEtudiants);
				}
			}
			else
			{
				printf("NomIncorrect\n");
			}

			printf("print entree %s %d\n", nomEtudiant, numeroGroupe);
		}
		else
		{
			// Attrape les erreurs de commande, nettoie le buffer et continue le programme.
			printf("Comande invalide\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	}

}