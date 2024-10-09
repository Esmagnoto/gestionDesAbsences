/*#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)//IGNORER RETOUR SCANF

//
enum { MAX_CARACTERES = 31, MAX_ETUDIANTS = 15, DEMI_JOURNEE = 3, MAX_ABSENCES = 40 };

typedef enum { ETUDIANT_NON_TROUVE, ETUDIANT_TROUVE } EtudiantStatus;

typedef enum { AM = 1, PM = 2 } PeriodeEtudes;


typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSITIFIE, NON_JUSTIFIE, ABSENCE_VALIDE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE
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
	unsigned int nombreAbsences;
} Etudiant;

// initialiser table etudiants
void initEtudiant(Etudiant* etudiant);

// verifie si un etudiant est ou n'est pas déjà inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(Etudiant tableEtudiants[MAX_ETUDIANTS], char nomEtudiant[]);

// faire une inscription table etudiants declaré dans main().
void inscrireEtudiant(Etudiant tableEtudiants[MAX_ETUDIANTS], int cmpEtudiants, char nomEtudiant[MAX_CARACTERES],
                      unsigned int numeroGroupe);

//defini le status d'un etudiant absent
AbsenceStatus verifierAbsent(int etudiantId, int jour, char periode[DEMI_JOURNEE],
                             Etudiant tableEtudiants[MAX_ETUDIANTS]);

int main()
{
	char entree[MAX_CARACTERES] = {'\0'};
	Etudiant etudiants[MAX_ETUDIANTS] = {0};
	int cmpEtudiants = 0;

	for (int i = 0; i < MAX_ETUDIANTS; i++) { initEtudiant(&etudiants[i]); }

	while (1)
	{
		printf("ecrire une commande: ");
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0) { return 0; }
		if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_CARACTERES] = {'\0'};
			unsigned int numeroGroupe = 0;
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

			AbsenceStatus status = verifierAbsent(etudiantId, jour, periode, etudiants);
			switch (status)
			{
			case ABSENCE_VALIDE:
				Etudiant* etudiant = &etudiants[etudiantId - 1];
				Absence* absence = &etudiant->absences[jour - 1];

				if (absence->periode == AM || absence->periode == 0)
				{
					absence->periode += (absence->periode == AM) ? PM : AM;
					absence->status = ATTENTE_JUSTIFICATIF;
					etudiant->nombreAbsences++;
				}
				printf("Absence enregistree [%u]\n", etudiant->nombreAbsences);
				break;

			case IDENTIFIANT_INVALIDE: printf("Identifiant incorrect\n");
				break;
			case DEMI_JOURNEE_INVALIDE: printf("Demi-journee incorrecte\n");
				break;
			case ABSENCE_DEJA_CONNUE: printf("Absence deja conu\n");
				break;
			case DATE_INVALIDE: printf("Date incorrecte\n");
				break;
			default: printf("Erreur inconnue\n");
				break;
			}
		}
		else if (strcmp(entree, "etudiants") == 0)
		{
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


// initialiser table etudiants
void initEtudiant(Etudiant* etudiant)
{
	assert(etudiant != NULL);
	etudiant->nomEtudiant[0] = '\0'; // initialise avec string null
	etudiant->numeroGroupe = 0; // initialise avec 0
	etudiant->nombreAbsences = 0;

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
		//si le nom existe ou si le nom n'est pas vide
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
AbsenceStatus verifierAbsent(int etudiantId, int jour, char periode[DEMI_JOURNEE],
                             Etudiant tableEtudiants[MAX_ETUDIANTS])
{
	if (etudiantId <= 0 || etudiantId > MAX_ETUDIANTS) { return IDENTIFIANT_INVALIDE; }

	Etudiant* etudiant = &tableEtudiants[etudiantId - 1];
	Absence* absence = &etudiant->absences[jour - 1];

	if (jour < 1 || jour > 40 || verifierInscriptionStatus(tableEtudiants, etudiant->nomEtudiant) != ETUDIANT_TROUVE)
	{
		return (jour < 1 || jour > 40) ? DATE_INVALIDE : IDENTIFIANT_INVALIDE;
	}

	if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0) { return DEMI_JOURNEE_INVALIDE; }

	if ((strcmp(periode, "am") == 0 && absence->periode == AM) ||
		(strcmp(periode, "pm") == 0 && absence->periode == PM) ||
		absence->periode == AM + PM)
	{
		return ABSENCE_DEJA_CONNUE;
	}
	absence->periode += (strcmp(periode, "am") == 0) ? AM : PM;
	return ABSENCE_VALIDE;
}
*/