#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)//IGNORER RETOUR SCANF

enum { MAX_JUSTIFICATIF = 50, MAX_NOM = 31, MAX_ABSENCES = 80, MAX_ETUDIANTS = 100, MAX_ENTREE = 31 };

typedef enum { AM = 1, PM = 2 } Periode;

typedef enum { ETUDIANT_NON_TROUVE, ETUDIANT_TROUVE } EtudiantStatus;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSITIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE, DEPASSEMENT_DE_FORMAT
} AbsenceStatus;

typedef struct
{
	char justificatif[MAX_JUSTIFICATIF];
	int jourConcernant;
	Periode demiJourneeConcernant;
} Absence;

typedef struct
{
	char nomEtudiant[MAX_NOM];
	int numeroGroupe;
	Absence absences[MAX_ABSENCES];
	int nombreAbsences;
	int id;
} Etudiant;

//initialiser un etudiant
void initialiserEtudiant(Etudiant* etudiant);

//inscrire un etudiant
void inscrireEtudiant(Etudiant* tableEtudiants, int nombreEtudiantsInscrits, const char* nomEtudiant,
                      int numeroGroupe);

// verifie si un etudiant est ou n'est pas deja inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
                                         int nombreEtudiantsInscrits, int numeroGroupe);

//defini le status d'un etudiant absent
AbsenceStatus verifierAbsence(int etudiantId, int jour, char periode[3],
                              const Etudiant* tableEtudiants);

void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, char* periode);


int comparer(const void* etudiantA, const void* etudiantB);
int main()
{
	Etudiant* etudiants = (Etudiant*)malloc(MAX_ETUDIANTS * sizeof(Etudiant));
	if (etudiants == NULL)
	{
		printf("Erreur: Impossible d'allouer la memoire pour les etudiants.\n");
		return 0;
	}

	int totalAbsences = 0;
	int nombreEtudiantsInscrits = 0;
	char entree[MAX_ENTREE] = {'\0'};

	for (int i = 0; i < MAX_ETUDIANTS; i++)
	{
		initialiserEtudiant(&etudiants[i]);
		etudiants[i].id = i;
	}

	while (1)
	{
		//printf("ecrire une commande: ");
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0)
		{
			free(etudiants);
			return 0;
		}
		if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_NOM] = {'\0'};
			int numeroGroupe = 0;
			scanf("%30s", nomEtudiant);

			if (nombreEtudiantsInscrits >= MAX_ETUDIANTS)
			{
				printf("Erreur: Nombre maximal d'etudiants atteint.\n");
			}
			else if (scanf("%u", &numeroGroupe) == 1 && verifierInscriptionStatus(
					etudiants, nomEtudiant, nombreEtudiantsInscrits, numeroGroupe) ==
				ETUDIANT_NON_TROUVE && numeroGroupe > 0)
			{
				inscrireEtudiant(etudiants, nombreEtudiantsInscrits, nomEtudiant, numeroGroupe);
				printf("Inscription realisee (%d)\n", nombreEtudiantsInscrits + 1);
				++nombreEtudiantsInscrits;
			}
			else
			{
				printf("Nom incorrect\n");
			}
		}
		else if (strcmp(entree, "absence") == 0)
		{
			int etudiantId;
			int jour;
			char periode[3];
			scanf("%d %d %2s", &etudiantId, &jour, periode);

			AbsenceStatus status = verifierAbsence(etudiantId, jour, periode, etudiants);

			switch (status)
			{
			case ABSENCE_NON_CONNUE:
				enregistrerAbsence(etudiants, etudiantId, jour, periode);
				++totalAbsences;
				printf("Absence enregistree [%d]\n", totalAbsences);
				break;
			case IDENTIFIANT_INVALIDE: printf("Identifiant incorrect\n");
				break;
			case DEMI_JOURNEE_INVALIDE: printf("Demi-journee incorrecte\n");
				break;
			case ABSENCE_DEJA_CONNUE: printf("Absence deja connue\n");
				break;
			case DATE_INVALIDE: printf("Date incorrecte\n");
				break;
			case DEPASSEMENT_DE_FORMAT: printf("L'etudiant a depasse le nombre de absences possible.");
				break;
			default: printf("Erreur inconnue\n");
				break;
			}
		}
		else if (strcmp(entree, "etudiants") == 0)
		{
			int jourCourant;
			scanf("%d", &jourCourant);
			if (jourCourant < 1)
			{
				printf("Date incorrecte\n");
			}
			else if (nombreEtudiantsInscrits == 0)
			{
				printf("Aucun inscrit\n");
			}
			else
			{
				Etudiant* ptrEtudiants[MAX_ETUDIANTS];
				for (int i = 0; i < nombreEtudiantsInscrits; i++)
				{
					ptrEtudiants[i] = &etudiants[i];
				}
				qsort(ptrEtudiants, nombreEtudiantsInscrits, sizeof(Etudiant*), comparer);
				for (int i = 0; i < nombreEtudiantsInscrits; i++)
				{
					printf("(%d) %-30s %3d %3d\n", ptrEtudiants[i]->id+1, ptrEtudiants[i]->nomEtudiant, ptrEtudiants[i]->numeroGroupe,
						ptrEtudiants[i]->nombreAbsences);
				}
			}
		}
		else if (strcmp(entree, "justificatif") == 0)
		{
			return 1;
		}
		else if (strcmp(entree, "validations") == 0)
		{
			return 1;
		}
		else if (strcmp(entree, "validation") == 0)
		{
			return 1;
		}
		else if (strcmp(entree, "etudiant") == 0)
		{
			return 1;
		}
		else if (strcmp(entree, "defaillants") == 0)
		{
			return 1;
		}
		else
		{
			// Attrape les erreurs de commande, nettoie le buffer et continue le programme.
			printf("Comande invalide\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	}

	free(etudiants); // Libera memória alocada antes de sair
	return EXIT_SUCCESS;
}

void initialiserEtudiant(Etudiant* etudiant)
{
	assert(etudiant != NULL);
	etudiant->nomEtudiant[0] = '\0'; // initialise avec string null
	etudiant->numeroGroupe = 0;
	etudiant->nombreAbsences = 0;
	etudiant->id = 0;


	for (int i = 0; i < MAX_ABSENCES; i++)
	{
		assert(i >= 0 && i < MAX_ABSENCES);
		etudiant->absences[i].justificatif[0] = '\0';
		etudiant->absences[i].jourConcernant = 0;
		etudiant->absences[i].demiJourneeConcernant = 0;
	}
}

// verifie si un etudiant est ou n'est pas deja inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
                                         int nombreEtudiantsInscrits, int numeroGroupe)
{
	assert(tableEtudiants != NULL && nomEtudiant != NULL);


	for (int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		//si le nom existe ou si le nom n'est pas vide
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0
			&& tableEtudiants[i].nomEtudiant[0] != '\0' && tableEtudiants[i].numeroGroupe == numeroGroupe)
		{
			return ETUDIANT_TROUVE;
		}
	}
	return ETUDIANT_NON_TROUVE;
}


// faire une inscription table etudiants declare dans main().
void inscrireEtudiant(Etudiant* tableEtudiants, int nombreEtudiantsInscrits, const char* nomEtudiant,
                      int numeroGroupe)
{
	if (nomEtudiant == NULL || strlen(nomEtudiant) == 0)
	{
		printf("Erreur: Nom de l'etudiant invalide.\n");
		return;
	}

	assert(numeroGroupe > 0);
	assert(nombreEtudiantsInscrits >= 0 && nombreEtudiantsInscrits < MAX_ETUDIANTS);
	assert(tableEtudiants != NULL);
	strcpy(tableEtudiants[nombreEtudiantsInscrits].nomEtudiant, nomEtudiant);
	tableEtudiants[nombreEtudiantsInscrits].numeroGroupe = numeroGroupe;
}

//defini le status d'une  absence
AbsenceStatus verifierAbsence(int etudiantId, int jour, char periode[3], const Etudiant* tableEtudiants)
{
	if (etudiantId < 1 || etudiantId > MAX_ETUDIANTS || tableEtudiants == NULL)
	{
		return IDENTIFIANT_INVALIDE;
	}

	if (tableEtudiants[etudiantId - 1].nomEtudiant[0] == '\0')
	{
		return IDENTIFIANT_INVALIDE;
	}

	if (jour < 1 || jour > 40) { return DATE_INVALIDE; }
	if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0) { return DEMI_JOURNEE_INVALIDE; }

	int nombreAbsences = tableEtudiants[etudiantId - 1].nombreAbsences;
	if (nombreAbsences < 0 || nombreAbsences >= MAX_ABSENCES)
	{
		return DEPASSEMENT_DE_FORMAT;
	}

	for (int i = 0; i < nombreAbsences; i++)
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



void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, char* periode)
{
	int absenceNumero = etudiants[etudiantId - 1].nombreAbsences;
	etudiants[etudiantId - 1].absences[absenceNumero].justificatif[0] = '\0';
	etudiants[etudiantId - 1].absences[absenceNumero].jourConcernant = jour;
	etudiants[etudiantId - 1].absences[absenceNumero].demiJourneeConcernant = strcmp(periode, "am") == 0 ? AM : PM;
	++etudiants[etudiantId - 1].nombreAbsences;
}

int comparer(const void* etudiantA, const void* etudiantB)
{
	// conversion
	const Etudiant* etudiant1 = *(const Etudiant**)etudiantA;
	const Etudiant* etudiant2 = *(const Etudiant**)etudiantB;
	// décision (e1 plus petit que e2 ou égal ou plus grand ?)
	if (etudiant1->numeroGroupe < etudiant2->numeroGroupe)
		return -1; // e1 est plus petit  que e2
	if (etudiant1->numeroGroupe > etudiant2->numeroGroupe)
		return 1; // e1 est plus grand  que e2
	// ici on sait que etudiant1->numeroGroupe == etudiant2->numeroGroupe 
	return strcmp(etudiant1->nomEtudiant, etudiant2->nomEtudiant); // on compare les 2 noms
}
