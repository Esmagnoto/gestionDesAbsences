#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)


enum { MAX_CARACTERES = 31, MAX_ETUDIANTS = 15, DEMI_JOURNEE = 2, MAX_ABSENCES = 40 };
typedef enum {ETUDIANT_NON_TROUVE, ETUDIANT_TROUVE} EtudiantStatus;
typedef enum {ABSENT_VALIDE, ABSENT_INVALIDE}AbsentStatus;
typedef enum { AM = 1, PM = 2 } PeriodeEtudes;
typedef enum {JUSITIFIE, NON_JUSTIFIE}AbsenceStatus;

typedef struct {
	PeriodeEtudes periode;
	AbsenceStatus status;
} Absence;

typedef struct {
	char nomEtudiant[MAX_CARACTERES];
	int numeroGroupe;
	Absence absences[MAX_ABSENCES];
	
} Etudiant;

// verifie si un etudiant est ou n'est pas déjà inscrit dans la table.
EtudiantStatus verifierInscription(Etudiant tableEtudiants[MAX_ETUDIANTS], char nomEtudiant[]) {
	assert(tableEtudiants != NULL && nomEtudiant != NULL);

	EtudiantStatus status = ETUDIANT_NON_TROUVE;
	for (int i = 0; i < MAX_ETUDIANTS; i++) {
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0) {
			status =  ETUDIANT_TROUVE;
		}
	}
	return status;
}

// faire une inscription table etudiants declaré dans main().
void inscrireEtudiant(Etudiant tableEtudiants[MAX_ETUDIANTS], int cmpEtudiants,char nomEtudiant[MAX_CARACTERES], int numeroGroupe) {
	assert(nomEtudiant != NULL && strlen(nomEtudiant) > 0);
	assert(numeroGroupe > 0);
	assert(cmpEtudiants >= 0 && cmpEtudiants < MAX_ETUDIANTS);
	assert(tableEtudiants != NULL);

	strcpy(tableEtudiants[cmpEtudiants].nomEtudiant, nomEtudiant);
	tableEtudiants[cmpEtudiants].numeroGroupe = numeroGroupe;
	
}

AbsentStatus verifierAbsent(int etudiantId,int jour, char periode[DEMI_JOURNEE], Etudiant tableEtudiants[MAX_ETUDIANTS]) {
	AbsentStatus status = ABSENT_INVALIDE;
	if (etudiantId < MAX_ETUDIANTS && jour < 41 && etudiantId > 0 && jour > 0)
	{
		if (verifierInscription(tableEtudiants, tableEtudiants[etudiantId].nomEtudiant) == ETUDIANT_TROUVE)
		{
			if (strcmp(periode, "am") == 0 || strcmp(periode, "pm") == 0)
			{
				status = ABSENT_VALIDE;
			}
		}
	}
	return status;
}

void verifierAbsence(etudiantId, jour, periode)

int main() {
	char entree[MAX_CARACTERES] = {0};
	Etudiant etudiants[MAX_ETUDIANTS] = {0} ;
	int cmpEtudiants = 0;
	
	while (1) {

		printf("ecrire une commande: ");
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0){return 0;}
		else if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_CARACTERES];
			unsigned int numeroGroupe;
			scanf("%30s", nomEtudiant);
			if (scanf("%u", &numeroGroupe) == 1 && verifierInscription(etudiants, nomEtudiant) == ETUDIANT_NON_TROUVE && numeroGroupe > 0) 
			{
				inscrireEtudiant(etudiants, cmpEtudiants, nomEtudiant, numeroGroupe);
				printf("Inscription enregistree (%d)\n", cmpEtudiants + 1);
				++cmpEtudiants;
			}
			else
			{
				printf("Nom Incorrect\n");
			}

		}
		else if (strcmp(entree, "absence") == 0) 
		{
			int jour;
			char periode[DEMI_JOURNEE];
			int etudiantId;
			scanf("%d", &etudiantId);
			scanf("%d", &jour);
			scanf("%s", periode);
			if (verifierAbsent(etudiantId, jour, periode, etudiants) == ABSENT_VALIDE)
			{
				verifierAbsence(etudiantId, jour, periode);
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