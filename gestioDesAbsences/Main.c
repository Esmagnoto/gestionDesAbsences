#include <stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)
#include <string.h>

enum { MAX_CARACTERES = 30, MAX_ETUDIANTS = 15, DEMI_JOURNEE = 2, LIMIT_ABSENCES = 5 };

typedef struct {
	int jour;
	char periode[DEMI_JOURNEE];

} Absence;

typedef struct {
	char nomEtudiant[MAX_CARACTERES];
	int numeroGroupe;
	Absence absences[LIMIT_ABSENCES];
	int nombreAbsences;
} Etudiant;

static int verifierEtudiants(Etudiant tableEtudiants[], char nomEtudiant[]) {
	int retour = 0;
	for (int i = 1; i < MAX_ETUDIANTS; i++) {
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0) {
			retour = 1;
		}
	}
	return retour;
}

int main() {
	char entree[MAX_CARACTERES] = "entree";
	Etudiant etudiants[MAX_ETUDIANTS];
	// Initializer un etudian null
	strcpy(etudiants[0].nomEtudiant, "nom");
	etudiants[0].numeroGroupe = 0;
	etudiants[0].absences[0].jour = 0;
	strcpy(etudiants[0].absences[0].periode, "am");
	etudiants[0].nombreAbsences = 0;
	int cmpEtudiants = 1;
	//entree[MAX_CARACTERES - 1] = '\0'; // ajoute explicitement le caract�re nul


	while (1) {

		printf("ecrire une chaine: ");
		scanf("%29s", &entree);
		if (strcmp(entree, "exit") == 0)
		{
			return 0;
		}
		else if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_CARACTERES];
			int numeroGroupe;
			scanf("%29s", nomEtudiant);
			if (scanf("%d", &numeroGroupe) == 1) {
				if(verifierEtudiants(etudiants, nomEtudiant) == 1) {
					printf("Nom  Incorrect\n");
				}else {
					strcpy(etudiants[cmpEtudiants].nomEtudiant, nomEtudiant);
					etudiants[cmpEtudiants].numeroGroupe = numeroGroupe;
					printf("Inscription enregistree (%d)\n", cmpEtudiants);
					++cmpEtudiants;
				}

			}
			else
			{
				printf("Nom Incorrect\n");
			}

			//printf("print entree %s %d\n", nomEtudiant, numeroGroupe);
		}else if (strcmp(entree, "absence") == 0) {
			int etudiantId;
			int jour;
			char periode[DEMI_JOURNEE];
			scanf("%d",&etudiantId);
			scanf("%d",&jour);
			scanf("%s",periode);

			if (verifierEtudiants(etudiants, periode) == 1) {

				if(0 < jour < 41) {
					if(strcmp(periode, "am") == 0 || strcmp(periode, "pm") == 0){
						etudiants[etudiantId].absences[jour].jour = jour;
						strcpy(etudiants[etudiantId].absences[jour].periode, periode);
						etudiants[etudiantId].nombreAbsences += 1;
						printf("Absence enregistree [%d]", etudiants[etudiantId].nombreAbsences);
					}
				}
			}
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