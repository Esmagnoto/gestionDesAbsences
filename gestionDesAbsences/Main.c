/*
#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)// IGNORER RETOUR SCANF

// LIMITES NUMERIQUES

#define MAX_CHAR_JUSTIFICATIF 51
#define MAX_CHAR 31
#define MAX_ABSENCES 80
#define MAX_ETUDIANTS 100

// TYPES ENUMEREES

typedef enum { AM, PM } Periode;

typedef enum
{
	ATTENTEJUSTIFICATIF, NONJUSTIFIE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, DATE_INVALIDE, DEPASSEMENT_DE_FORMAT, ABSENCE_DEJA_CONNUE, ABSENCE_NON_CONNUE
} Status;

// STRUCTURES

typedef struct
{
	char justificatif[MAX_CHAR_JUSTIFICATIF];
	unsigned int jourConcernant;
	Periode demiJourneeConcernant;
	unsigned int identifiantAbsence;
	Status absence;
} Absence;

typedef struct
{
	char nomEtudiant[MAX_CHAR];
	unsigned int numeroGroupe;
	Absence tableauAbsence[MAX_ABSENCES];
	unsigned int nombreAbsences;
	unsigned int identifiantEtudiant;
} Etudiant;


// FONCTIONS


void initializerTableauEtudiant(Etudiant* tableauEtudiant)
{
	assert(tableauEtudiant != NULL);
	unsigned int absenceCompteur = 1;
	for (int i = 0; i < MAX_ETUDIANTS; ++i)
	{
		tableauEtudiant[i].identifiantEtudiant = i + 1;
		tableauEtudiant[i].nomEtudiant[0] = '\0';
		tableauEtudiant[i].nombreAbsences = 0;
		tableauEtudiant[i].numeroGroupe = 0;

		for (int j = 0; j < MAX_ABSENCES; ++j)
		{
			tableauEtudiant[i].tableauAbsence[j].demiJourneeConcernant = 0;
			tableauEtudiant[i].tableauAbsence[j].jourConcernant = 0;
			tableauEtudiant[i].tableauAbsence[j].justificatif[0] = '\0';
			tableauEtudiant[i].tableauAbsence[j].absence = -1;
			tableauEtudiant[i].tableauAbsence[j].identifiantAbsence = absenceCompteur;
			++absenceCompteur;
		}
	}

	/*
 *	cette fonction peut etre divise en trois fonction, une fonction pour initializer un etudiant, et une pour une absence
	j'ai juge pas nécéssaire dans mon programme car, je n'a pas besoin de initializer des etudiants aprés la
	demarrage du programme, mais si le tableua d'etudiant etais alloque avec memoire dynamique malloc()
	ça peurrait etre util et dans d'autres cas aussi.
 *
 #1#
}

// si l'etudiant existe return le si non return null
static Etudiant* getEtudiant(Etudiant* tableauEtudiant, char nomEtudiant[MAX_CHAR], unsigned int numeroGroupe)
{
	assert(tableauEtudiant != NULL);
	if (nomEtudiant[0] == '\0')
	{
		printf("Nom Incorrecte\n");
		return NULL;
	}
	for (int i = 0; i < MAX_ETUDIANTS; i++)
	{
		if (strcmp(tableauEtudiant[i].nomEtudiant, nomEtudiant) == 0 && tableauEtudiant[i].nomEtudiant[0] != '\0' &&
			tableauEtudiant[i].numeroGroupe == numeroGroupe)
		{
			return &tableauEtudiant[i];
		}
	}
	return NULL;
}

// si l'absence existe return la si non return null
static Absence* getAbsence(Etudiant* tableauEtudiant, unsigned int identifiantAbsence)
{
	assert(tableauEtudiant != NULL);
	if (identifiantAbsence == 0)
	{
		printf("Identifiant incorrecte\n");
		return NULL;
	}
	for (int i = 0; i < MAX_ETUDIANTS; i++)
	{
		for (int j = 0; j < MAX_ABSENCES; j++)
		{
			if (tableauEtudiant[i].tableauAbsence[j].identifiantAbsence == identifiantAbsence && tableauEtudiant[i].
				tableauAbsence[j].jourConcernant != 0)
			{
				return &tableauEtudiant[i].tableauAbsence[j];
			}
		}
	}
	return NULL;
}

void inscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits)
{
	char entreeNom[MAX_CHAR];
	int entreeGroupe;

	if (scanf("%30s %d", entreeNom, &entreeGroupe) != 2)
	{
		printf("Commande invalide\n");
		return;
	}
	assert(entreeNom != NULL);
	assert(entreeGroupe > 0);
	assert(*nombreEtudiantsInscrits < MAX_ETUDIANTS);
	assert(tableauEtudiants != NULL);
	if (getEtudiant(tableauEtudiants, entreeNom, entreeGroupe) == NULL)
	{
		strncpy(tableauEtudiants[*nombreEtudiantsInscrits].nomEtudiant, entreeNom, MAX_CHAR - 1);
		tableauEtudiants[*nombreEtudiantsInscrits].nomEtudiant[MAX_CHAR - 1] = '\0';
		// Sécurité pour la terminaison
		tableauEtudiants[*nombreEtudiantsInscrits].numeroGroupe = entreeGroupe;
		printf("Inscription enregistree (%d)\n", tableauEtudiants[*nombreEtudiantsInscrits].identifiantEtudiant);
		(*nombreEtudiantsInscrits)++;
	}
	else
	{
		printf("Nom incorrect\n");
	}
}

Status verifierCommandeAbsence(Absence* absence, unsigned int identifiantEtudiant, unsigned int jour, char periode[3])
{
	if (identifiantEtudiant < 1 || identifiantEtudiant > MAX_ETUDIANTS)
	{
		return IDENTIFIANT_INVALIDE;
	}

	if (tableEtudiants[identifiantEtudiant - 1].nomEtudiant[0] == '\0')
	{
		return IDENTIFIANT_INVALIDE;
	}

	if (jour < 1 || jour > 40)
	{
		return DATE_INVALIDE;
	}

	if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0)
	{
		return DEMI_JOURNEE_INVALIDE;
	}

	for (unsigned int i = 0; i < nombreAbsences; i++)
	{
		
		if (absence->jourConcernant == jour &&
			((strcmp(periode, "am") == 0 && absence->demiJourneeConcernant == AM) ||
				(strcmp(periode, "pm") == 0 && absence->demiJourneeConcernant == PM)))
		{
			return ABSENCE_DEJA_CONNUE;
		}
	}

	return ABSENCE_NON_CONNUE;
}


int main()
{
	Etudiant tableauEtudiants[MAX_ETUDIANTS];
	unsigned int nombreEtudiantsInscrits = 0;
	unsigned int totalAbsences = 0;

	initializerTableauEtudiant(tableauEtudiants);

	while (1)
	{
		char entree[MAX_CHAR] = {0};
		if (scanf("%30s", entree) != 1)
		{
			printf("Erreur de lecture.\n");
			continue;
		}
		if (strcmp(entree, "exit") == 0)
		{
			return 0;
		}
		if (strcmp(entree, "inscription") == 0)
		{
			inscription(tableauEtudiants, &nombreEtudiantsInscrits);
		}
		else if (strcmp(entree, "absence") == 0)
		{
			int identifiantEtudiant;
			int jour;
			char periode[3];

			if (scanf("%d %d %2s", &identifiantEtudiant, &jour, periode) != 3)
			{
				printf("Erreur de saisie des donnees.\n");
				return;
			}

			Absence* absence = getAbsence(tableauEtudiants, totalAbsences);

			switch (verifierCommandeAbsence(absence, identifiantEtudiant, jour, periode))
			{
			case ABSENCE_NON_CONNUE:
				enregistrerAbsence(tableauEtudiants, identifiantEtudiant, jour, periode);
				(*totalAbsences)++;
				printf("Absence enregistree [%d]\n", *totalAbsences);
				break;
			case IDENTIFIANT_INVALIDE:
				printf("Identifiant incorrect\n");
				break;
			case DEMI_JOURNEE_INVALIDE:
				printf("Demi-journee incorrecte\n");
				break;
			case ABSENCE_DEJA_CONNUE:
				printf("Absence deja connue\n");
				break;
			case DATE_INVALIDE:
				printf("Date incorrecte\n");
				break;
			case DEPASSEMENT_DE_FORMAT:
				printf("L'etudiant a depasse le nombre d'absences possible.\n");
				break;
			default:
				printf("Erreur inconnue\n");
				break;
			}
		}
		else if (strcmp(entree, "etudiants") == 0)
		{
			afficherListeEtudiants(nombreEtudiantsInscrits, tableauEtudiants);
		}
		else if (strcmp(entree, "justificatif") == 0)
		{
			unsigned int identificateurAbsence;
			unsigned int jour;
			char justificatif[51];
			scanf("%d %d", &identificateurAbsence, &jour);
			fgets(justificatif, 51, stdin);
			Absence* absence = trouverAbsence(tableauEtudiants, identificateurAbsence);
			if (absence == NULL)
			{
				printf("Identifiant incorrect\n");
			}
			else if (absence->justificatif[0] != '\0')
			{
				printf("Justificatif deja connu\n");
			}
			else if (absence->jourConcernant > jour)
			{
				printf("Date incorrecte\n");
			}
			else if (jour >= tableauEtudiants[0].absences[0].identifiantAbsence + 3)
			{
				strncpy(tableauEtudiants[0].absences[0].justificatif, justificatif, MAX_CHAR_JUSTIFICATIF - 1);
				tableauEtudiants[0].absences[0].status = NON_JUSTIFIE;
				printf("Justificatif enregistre\n");
				//printf("Absence non justifiee\n");
			}
			else
			{
				strncpy(tableauEtudiants[0].absences[0].justificatif, justificatif, MAX_CHAR_JUSTIFICATIF - 1);
				tableauEtudiants[0].absences[0].status = ATTENTE_JUSTIFICATIF;
				printf("Justificatif enregistre\n");
			}
		}
		else if (strcmp(entree, "validation") == 0)
		{
		}
		else
		{
			printf("Commande invalide\n");
			while (getchar() != '\n'); // Nettoyage du buffer en cas d'erreur
		}
	}
}
*/
