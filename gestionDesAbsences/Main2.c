#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)
#pragma warning(disable:6031) // IGNORER RETOUR SCANF

#define MAX_CHAR_JUSTIFICATIF 51
#define MAX_CHAR 31
#define MAX_ABSENCES 80
#define MAX_ETUDIANTS 100

typedef enum { TROUVE, NONTROUVE } EtudiantStatus;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSTIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE,
	IDENTIFIANT_INVALIDE, DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE,
	DATE_INVALIDE, DEPASSEMENT_DE_FORMAT, ENCOURS
} AbsenceStatus;

typedef enum { AM = 1, PM = 2 } Periode;

typedef struct
{
	char justificatif[MAX_CHAR_JUSTIFICATIF];
	unsigned int jourConcernant;
	Periode demiJourneeConcernant;
	unsigned int identifiantAbsence;
	AbsenceStatus status;
} Absence;

typedef struct
{
	char nomEtudiant[MAX_CHAR];
	unsigned int numeroGroupe;
	Absence absences[MAX_ABSENCES];
	unsigned int nombreAbsences;
	unsigned int identifiantEtudiant;
} Etudiant;

void initializerTableauEtudiants(Etudiant* etudiants);
void faireInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits, const char* nom,
                      unsigned int numeroGroupe);
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
                                         unsigned int nombreEtudiantsInscrits, unsigned int numeroGroupe);
void gererCommandeInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits);
AbsenceStatus verifierAbsence(unsigned int etudiantId, unsigned int jour, const char* periode,
                              const Etudiant* tableEtudiants);
void enregistrerAbsence(Etudiant* etudiants, unsigned int etudiantId, unsigned int jour, const char* periode,
                        unsigned int* totalAbsences);
void gererCommandeAbsence(Etudiant* tableauEtudiants, unsigned int* totalAbsences);
int comparer(const void* etudiantA, const void* etudiantB);
void afficherListeEtudiants(unsigned int nombreEtudiantsInscrits, Etudiant* tableauEtudiants);
Absence* trouverAbsence(Etudiant* tableauEtudiants, unsigned int identificateurAbsence);
void gererCommandeJustificatif(Etudiant* tableauEtudiants);
void supprimerEspacesGauche(char* str);

int main()
{
	// Allocate tableauEtudiants on the heap

	Etudiant* tableauEtudiants = (Etudiant*)malloc(MAX_ETUDIANTS * sizeof(Etudiant));

	if (tableauEtudiants == NULL)
	{
		printf("Erreur d'allocation de memoire.\n");

		return 1; // Exit if memory allocation fails
	}


	unsigned int nombreEtudiantsInscrits = 0;

	unsigned int totalAbsences = 0;


	initializerTableauEtudiants(tableauEtudiants);


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
			break; // Exit the loop instead of returning
		}

		if (strcmp(entree, "inscription") == 0)
		{
			gererCommandeInscription(tableauEtudiants, &nombreEtudiantsInscrits);
		}
		else if (strcmp(entree, "absence") == 0)
		{
			gererCommandeAbsence(tableauEtudiants, &totalAbsences);
		}
		else if (strcmp(entree, "etudiants") == 0)
		{
			afficherListeEtudiants(nombreEtudiantsInscrits, tableauEtudiants);
		}
		else if (strcmp(entree, "justificatif") == 0)
		{
			gererCommandeJustificatif(tableauEtudiants);
		}
		else if (strcmp(entree, "validations") == 0)
		{
			int aucuneabsenceAttente = 0;

			for (unsigned int i = 0; i < nombreEtudiantsInscrits; i++)
			{
				for (unsigned int j = 0; j < MAX_ABSENCES; j++)
				{
					if ((tableauEtudiants[i].absences[j].status == ENCOURS || tableauEtudiants[i].absences[j].status ==
						NON_JUSTIFIE) && tableauEtudiants[i].absences[j].justificatif[0] != '\0')
					{
						unsigned int identifiantAffiche = (tableauEtudiants[i].absences[j].identifiantAbsence % 80 == 0)
							                                  ? 80
							                                  : tableauEtudiants[i].absences[j].identifiantAbsence % 80;

						printf("[%d] (%d) %s %d %d/%s (%s)\n", identifiantAffiche,
						       tableauEtudiants[i].identifiantEtudiant, tableauEtudiants[i].nomEtudiant,
						       tableauEtudiants[i].numeroGroupe, tableauEtudiants[i].absences[j].jourConcernant,
						       (tableauEtudiants[i].absences[j].demiJourneeConcernant == AM) ? "am" : "pm",
						       tableauEtudiants[i].absences[j].justificatif);

						aucuneabsenceAttente++;
					}
				}
			}

			if (aucuneabsenceAttente == 0)
			{
				printf("Aucune validation en attente\n");
			}
		}
		else
		{
			printf("Commande invalide\n");

			while (getchar() != '\n'); // Nettoyage du buffer en cas d'erreur
		}
	}


	// Free allocated memory

	free(tableauEtudiants);

	return 0;
}

void initializerTableauEtudiants(Etudiant* etudiants)
{
	assert(etudiants != NULL);
	for (int i = 0; i < MAX_ETUDIANTS; ++i)
	{
		etudiants[i].identifiantEtudiant = i + 1;
		etudiants[i].nomEtudiant[0] = '\0';
		etudiants[i].nombreAbsences = 0;
		etudiants[i].numeroGroupe = 0;

		for (int j = 0; j < MAX_ABSENCES; ++j)
		{
			etudiants[i].absences[j].demiJourneeConcernant = 0;
			etudiants[i].absences[j].jourConcernant = 0;
			etudiants[i].absences[j].justificatif[0] = '\0';
			etudiants[i].absences[j].status = -1;
			etudiants[i].absences[j].identifiantAbsence = 0;
		}
	}
}

void faireInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits, const char* nom,
                      unsigned int numeroGroupe)
{
	assert(nom != NULL);
	assert(numeroGroupe > 0);
	assert(*nombreEtudiantsInscrits >= 0 && *nombreEtudiantsInscrits < MAX_ETUDIANTS);
	assert(tableauEtudiants != NULL);

	if (*nombreEtudiantsInscrits >= MAX_ETUDIANTS)
	{
		printf("Erreur: Nombre maximal d'etudiants atteint.\n");
		return;
	}

	if (verifierInscriptionStatus(tableauEtudiants, nom, *nombreEtudiantsInscrits, numeroGroupe) == NONTROUVE)
	{
		strncpy(tableauEtudiants[*nombreEtudiantsInscrits].nomEtudiant, nom, MAX_CHAR - 1);
		tableauEtudiants[*nombreEtudiantsInscrits].nomEtudiant[MAX_CHAR - 1] = '\0'; // Sécurité pour la terminaison
		tableauEtudiants[*nombreEtudiantsInscrits].numeroGroupe = numeroGroupe;
		printf("Inscription enregistree (%d)\n", tableauEtudiants[*nombreEtudiantsInscrits].identifiantEtudiant);
		(*nombreEtudiantsInscrits)++;
	}
	else
	{
		printf("Nom incorrect\n");
	}
}

EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
                                         unsigned int nombreEtudiantsInscrits, unsigned int numeroGroupe)
{
	assert(tableEtudiants != NULL);
	assert(nomEtudiant != NULL);

	for (unsigned int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0 && tableEtudiants[i].numeroGroupe == numeroGroupe)
		{
			return TROUVE;
		}
	}
	return NONTROUVE;
}

void gererCommandeInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits)
{
	char nom[MAX_CHAR];
	int numero;

	if (scanf("%30s %d", nom, &numero) != 2)
	{
		printf("Commande invalide\n");
		return;
	}

	faireInscription(tableauEtudiants, nombreEtudiantsInscrits, nom, numero);
}

AbsenceStatus verifierAbsence(unsigned int etudiantId, unsigned int jour, const char* periode,
                              const Etudiant* tableEtudiants)
{
	if (etudiantId < 1 || etudiantId > MAX_ETUDIANTS || tableEtudiants == NULL)
	{
		return IDENTIFIANT_INVALIDE;
	}

	if (tableEtudiants[etudiantId - 1].nomEtudiant[0] == '\0')
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

	unsigned int nombreAbsences = tableEtudiants[etudiantId - 1].nombreAbsences;
	if (nombreAbsences < 0 || nombreAbsences >= MAX_ABSENCES)
	{
		return DEPASSEMENT_DE_FORMAT;
	}

	for (unsigned int i = 0; i < nombreAbsences; i++)
	{
		Absence absence = tableEtudiants[etudiantId - 1].absences[i];
		if (absence.jourConcernant == jour &&
			((strcmp(periode, "am") == 0 && absence.demiJourneeConcernant == AM) ||
				(strcmp(periode, "pm") == 0 && absence.demiJourneeConcernant == PM)))
		{
			return ABSENCE_DEJA_CONNUE;
		}
	}

	return ABSENCE_NON_CONNUE;
}

void enregistrerAbsence(Etudiant* etudiants, unsigned int etudiantId, unsigned int jour, const char* periode,
                        unsigned int* totalAbsences)
{
	unsigned int absenceNumero = etudiants[etudiantId - 1].nombreAbsences;
	if (absenceNumero >= MAX_ABSENCES)
	{
		printf("Nombre maximal d'absences atteint pour cet etudiant.\n");
		return;
	}

	etudiants[etudiantId - 1].absences[absenceNumero].justificatif[0] = '\0';
	etudiants[etudiantId - 1].absences[absenceNumero].jourConcernant = jour;
	etudiants[etudiantId - 1].absences[absenceNumero].demiJourneeConcernant = (strcmp(periode, "am") == 0) ? AM : PM;
	etudiants[etudiantId - 1].absences[absenceNumero].status = ATTENTE_JUSTIFICATIF;
	++(*totalAbsences);
	etudiants[etudiantId - 1].absences[absenceNumero].identifiantAbsence = *totalAbsences;
	etudiants[etudiantId - 1].nombreAbsences++;
}

void gererCommandeAbsence(Etudiant* tableauEtudiants, unsigned int* totalAbsences)
{
	int identifiantEtudiant;
	int jour;
	char periode[3];

	if (scanf("%d %d %2s", &identifiantEtudiant, &jour, periode) != 3)
	{
		printf("Erreur de saisie des donnees.\n");
		return;
	}

	AbsenceStatus status = verifierAbsence(identifiantEtudiant, jour, periode, tableauEtudiants);

	switch (status)
	{
	case ABSENCE_NON_CONNUE:
		enregistrerAbsence(tableauEtudiants, identifiantEtudiant, jour, periode, totalAbsences);
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

int comparer(const void* etudiantA, const void* etudiantB)
{
	const Etudiant* etudiant1 = (const Etudiant*)etudiantA;
	const Etudiant* etudiant2 = (const Etudiant*)etudiantB;

	if (etudiant1->numeroGroupe < etudiant2->numeroGroupe)
		return -1;
	if (etudiant1->numeroGroupe > etudiant2->numeroGroupe)
		return 1;
	return strcmp(etudiant1->nomEtudiant, etudiant2->nomEtudiant);
}

void afficherListeEtudiants(unsigned int nombreEtudiantsInscrits, Etudiant* tableauEtudiants)
{
	unsigned int jourCourant;
	if (scanf("%d", &jourCourant) != 1 || jourCourant < 1)
	{
		printf("Date incorrecte\n");
		return;
	}

	if (nombreEtudiantsInscrits == 0)
	{
		printf("Aucun inscrit\n");
		return;
	}

	Etudiant tableauCopie[MAX_ETUDIANTS];
	memcpy(tableauCopie, tableauEtudiants, nombreEtudiantsInscrits * sizeof(Etudiant));

	qsort(tableauCopie, nombreEtudiantsInscrits, sizeof(Etudiant), comparer);

	for (unsigned int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		printf("(%d) %-30s %3d ", tableauCopie[i].identifiantEtudiant, tableauCopie[i].nomEtudiant,
		       tableauCopie[i].numeroGroupe);

		int totalAbsence = 0;
		for (unsigned int j = 0; j < tableauCopie[i].nombreAbsences; ++j)
		{
			if (tableauCopie[i].absences[j].jourConcernant <= jourCourant && tableauCopie[i].absences[j].jourConcernant
				!= 0)
			{
				totalAbsence++;
			}
		}
		printf("%3d\n", totalAbsence);
	}
}

Absence* trouverAbsence(Etudiant* tableauEtudiants, unsigned int identificateurAbsence)
{
	for (int i = 0; i < MAX_ETUDIANTS; i++)
	{
		for (int j = 0; j < MAX_ABSENCES; j++)
		{
			if (tableauEtudiants[i].absences[j].identifiantAbsence == identificateurAbsence && tableauEtudiants[i].
				absences[j].jourConcernant != 0)
			{
				return &tableauEtudiants[i].absences[j];
			}
		}
	}
	return NULL;
}

void gererCommandeJustificatif(Etudiant* tableauEtudiants)
{
	unsigned int identificateurAbsence;
	unsigned int jour;
	char justificatif[51];
	scanf("%d %d", &identificateurAbsence, &jour);

	if (fgets(justificatif, sizeof(justificatif), stdin) != NULL)
	{
		supprimerEspacesGauche(justificatif);
		size_t longueur = strlen(justificatif);
		if (justificatif[longueur - 1] == '\n')
		{
			justificatif[longueur - 1] = '\0';
		}
	}
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
	else if (jour >= absence->jourConcernant + 3)
	{
		strncpy(absence->justificatif, justificatif, MAX_CHAR_JUSTIFICATIF - 1);
		absence->status = NON_JUSTIFIE;
		printf("Justificatif enregistre\n");
	}
	else
	{
		strncpy(absence->justificatif, justificatif, MAX_CHAR_JUSTIFICATIF - 1);
		absence->status = ENCOURS;
		printf("Justificatif enregistre\n");
	}
}

void supprimerEspacesGauche(char* str)
{
	int i = 0;

	while (isspace((unsigned char)str[i]))
	{
		i++;
	}

	if (i > 0)
	{
		memmove(str, str + i, strlen(str) - i + 1);
	}
}
