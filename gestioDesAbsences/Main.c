#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)

//
enum { MAX_CARACTERES = 31, MAX_ETUDIANTS = 15, DEMI_JOURNEE = 3, MAX_ABSENCES = 40 };

typedef enum { ETUDIANT_NON_TROUVE, ETUDIANT_TROUVE } EtudiantStatus;

typedef enum
{
	ABSENT_VALIDE, ABSENT_INVALIDE, IDENTIFIANT_INCORRECT, DEMI_JOURNEE_INCORRECTE, ABSENCE_DEJA_CONU, DATE_INCORRECTE
} AbsentStatus;

typedef enum { AM = 1, PM = 2 } PeriodeEtudes;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSITIFIE, NON_JUSTIFIE
} AbsenceStatus;

typedef struct
{
	PeriodeEtudes periode;
	AbsenceStatus status;
} Absence;

typedef struct
{
	char nomEtudiant[MAX_CARACTERES];
	unsigned int numeroGroupe;
	Absence absences[MAX_ABSENCES];
} Etudiant;

// initialiser table etudiants
void initEtudiant(Etudiant* etudiant)
{
	assert(etudiant != NULL);
	etudiant->nomEtudiant[0] = '\0'; // initialise avec string null
	etudiant->numeroGroupe = 0; // initialise avec 0

	for (int i = 0; i < MAX_ABSENCES; i++)
	{
		assert(i >= 0 && i < MAX_ABSENCES);
		etudiant->absences[i].periode = 0; // initialise avec 0
		etudiant->absences[i].status = 0; // initialise avec 0
	}
}

// verifie si un etudiant est ou n'est pas déjà inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(Etudiant tableEtudiants[MAX_ETUDIANTS], char nomEtudiant[])
{
	assert(tableEtudiants != NULL && nomEtudiant != NULL);
	EtudiantStatus status = ETUDIANT_NON_TROUVE;
	for (int i = 0; i < MAX_ETUDIANTS; i++)
	{
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0 && strcmp(tableEtudiants[i].nomEtudiant, "") != 0)
		{
			status = ETUDIANT_TROUVE;
		}
	}
	return status;
}

// faire une inscription table etudiants declaré dans main().
void inscrireEtudiant(Etudiant tableEtudiants[MAX_ETUDIANTS], int cmpEtudiants, char nomEtudiant[MAX_CARACTERES],
                     unsigned int numeroGroupe)
{
	assert(nomEtudiant != NULL && strlen(nomEtudiant) > 0);
	assert(numeroGroupe > 0);
	assert(cmpEtudiants >= 0 && cmpEtudiants < MAX_ETUDIANTS);
	assert(tableEtudiants != NULL);
	strcpy(tableEtudiants[cmpEtudiants].nomEtudiant, nomEtudiant);
	tableEtudiants[cmpEtudiants].numeroGroupe = numeroGroupe;
}

//defini le status d'un etudiant absent
AbsentStatus verifierAbsent(int etudiantId, int jour, char periode[DEMI_JOURNEE],
                            Etudiant tableEtudiants[MAX_ETUDIANTS])
{
	assert(etudiantId <= MAX_ETUDIANTS && etudiantId > 0);
	AbsentStatus status = IDENTIFIANT_INCORRECT;
	if (jour < 41 && jour > 0)
	{
		if (verifierInscriptionStatus(tableEtudiants, tableEtudiants[etudiantId-1].nomEtudiant) == ETUDIANT_TROUVE)
		{
			if (strcmp(periode, "am") == 0 || strcmp(periode, "pm") == 0)
			{
				if (tableEtudiants[etudiantId-1].absences[jour].periode != AM + PM)
				{
					status = ABSENT_VALIDE;
				}
				else { status = ABSENCE_DEJA_CONU; }
			}
			else { status = DEMI_JOURNEE_INCORRECTE; }
		}
	}
	else { status = DATE_INCORRECTE; }
	return status;
}

int main()
{
	char entree[MAX_CARACTERES] = {'\0'};
	Etudiant etudiants[MAX_ETUDIANTS];
	int cmpEtudiants = 0;

	for (int i = 0; i < MAX_ETUDIANTS; i++) { initEtudiant(&etudiants[i]); }

	while (1)
	{
		printf("ecrire une commande: ");
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0) { return 0; }
		else if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_CARACTERES];
			unsigned int numeroGroupe;
			scanf("%30s", nomEtudiant);
			if (scanf("%u", &numeroGroupe) == 1 && verifierInscriptionStatus(etudiants, nomEtudiant) ==
				ETUDIANT_NON_TROUVE && numeroGroupe > 0)
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
			scanf("%2s", periode);

			AbsentStatus status = verifierAbsent(etudiantId, jour, periode, etudiants);
			switch (status)
			{
			case ABSENT_VALIDE:
				if (etudiants[etudiantId].absences[jour].periode == PM || etudiants[etudiantId].absences[jour].periode ==
					0)
				{
					etudiants[etudiantId].absences[jour].periode += AM;
					etudiants[etudiantId].absences[jour].status = ATTENTE_JUSTIFICATIF;

				}
				else if (etudiants[etudiantId].absences[jour].periode == AM || etudiants[etudiantId].absences[jour].
					periode == 0)
				{
					etudiants[etudiantId].absences[jour].periode += PM;
					etudiants[etudiantId].absences[jour].status = ATTENTE_JUSTIFICATIF;
				}
				printf("Absence enregistree");
				break;

			case IDENTIFIANT_INCORRECT: printf("Identifiant incorrect\n");
				break;
			case DEMI_JOURNEE_INCORRECTE: printf("Demi-journee incorrecte\n");
				break;
			case ABSENCE_DEJA_CONU: printf("Absence deja conu\n");
				break;
			case DATE_INCORRECTE: printf("Date incorrecte\n");
				break;
			case ABSENT_INVALIDE: 
				break;
			default: printf("Erreur inconnue\n");
				break;
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
