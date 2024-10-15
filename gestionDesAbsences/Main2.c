/*#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)//IGNORER RETOUR SCANF


enum
{
	MAX_CHAR_JUSTIFICATIF = 50,
	MAX_CHAR = 31,
	MAX_ABSENCES = 80,
	MAX_ETUDIANTS = 100
};

typedef enum { TROUVE, NONTROUVE } EtudiantStatus;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSITIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE, DEPASSEMENT_DE_FORMAT
} AbsenceStatus;

typedef enum { AM = 1, PM = 2 } Periode;

typedef struct
{
	char justificatif[MAX_CHAR_JUSTIFICATIF];
	int jourConcernant;
	int demiJourneeConcernant;
} Absence;

typedef struct
{
	char nomEtudiant[MAX_CHAR];
	int numeroGroupe;
	Absence absences[MAX_ABSENCES];
	int nombreAbsences;
	int identifiant;
} Etudiant;

void initializerTableauEtudiants(Etudiant* etudiants);


void faireInscription(Etudiant* tableauEtudiants, int* indice, char nom[MAX_CHAR], int numeroGroupe);

EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
	int nombreEtudiantsInscrits, int numeroGroupe);

void gererCommandeInscription(Etudiant* tableauEtudiants, int* nombreEtudiantsInscrits);

AbsenceStatus verifierAbsence(int etudiantId, int jour, char periode[3], const Etudiant* tableEtudiants);

void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, char* periode);

void gererCommandeAbsence(Etudiant* tableauEtudiants, int* totalAbsences);

int comparer(const void* etudiantA, const void* etudiantB);


void afficherListeEtudiants(const int nombreEtudiantsInscrits, const Etudiant* tableauEtudiants);

int main()
{
	Etudiant tableauEtudiants[MAX_ETUDIANTS];
	int nombreEtudiantsInscrits = 0;
	int totalAbsences = 0;

	initializerTableauEtudiants(tableauEtudiants);

	while (1)
	{
		char entree[MAX_CHAR] = { 0 };
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0)
		{
			return 0;
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

//initializer le tableau des etudiants avec des valeurs egal a 0 pour des entiers et '\0' pour des chaines de char.
void initializerTableauEtudiants(Etudiant* etudiants)
{
	assert(etudiants != NULL);
	for (int i = 0; i < 100; ++i)
	{
		etudiants[i].identifiant = i;
		etudiants[i].nomEtudiant[0] = '\0';
		etudiants[i].nombreAbsences = 0;
		etudiants[i].numeroGroupe = 0;

		for (int j = 0; j < 80; ++j)
		{
			etudiants[i].absences[j].demiJourneeConcernant = 0;
			etudiants[i].absences[j].jourConcernant = 0;
			etudiants[i].absences[j].justificatif[0] = '\0';
		}
	}
}

void faireInscription(Etudiant* tableauEtudiants, int* indice, char nom[MAX_CHAR], int numeroGroupe)
{
	assert(numeroGroupe > 0);
	assert(*indice >= 0 && *indice < MAX_ETUDIANTS);
	assert(tableauEtudiants != NULL);
	if (nom == NULL || strlen(nom) == 0)
	{
		printf("Erreur: Nom de l'etudiant invalide.\n");
		return;
	}
	if (*indice >= MAX_ETUDIANTS)
	{
		printf("Erreur: Nombre maximal d'etudiants atteint.\n");
		return;
	}

	if (verifierInscriptionStatus(tableauEtudiants, nom, *indice, numeroGroupe) == NONTROUVE)
	{
		strcpy(tableauEtudiants[*indice].nomEtudiant, nom);
		tableauEtudiants[*indice].numeroGroupe = numeroGroupe;
		printf("Inscription enregistree (%d)\n", *indice + 1);
		(*indice)++;
	}
	else
	{
		printf("Nom incorrect\n");
	}
}

// verifie si un etudiant est ou n'est pas deja inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
	const int nombreEtudiantsInscrits, const int numeroGroupe)
{
	assert(tableEtudiants != NULL && nomEtudiant != NULL);


	for (int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		//si le nom existe ou si le nom n'est pas vide
		if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0
			&& tableEtudiants[i].nomEtudiant[0] != '\0' && tableEtudiants[i].numeroGroupe == numeroGroupe)
		{
			return TROUVE;
		}
	}
	return NONTROUVE;
}

void gererCommandeInscription(Etudiant* tableauEtudiants, int* nombreEtudiantsInscrits)
{
	char nom[MAX_CHAR];
	int numero;

	if (scanf("%30s %d", nom, &numero) != 2)
	{
		printf("Commande invalide \n");
	}
	else
	{
		faireInscription(tableauEtudiants, nombreEtudiantsInscrits, nom, numero);
	}
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

void gererCommandeAbsence(Etudiant* tableauEtudiants, int* totalAbsences)
{
	int identifiantEtudiant;
	int jour;
	char periode[3];
	scanf("%d %d %2s", &identifiantEtudiant, &jour, periode);
	AbsenceStatus status = verifierAbsence(identifiantEtudiant, jour, periode, tableauEtudiants);


	switch (status)
	{
	case ABSENCE_NON_CONNUE:
		enregistrerAbsence(tableauEtudiants, identifiantEtudiant, jour, periode);
		(*totalAbsences)++;
		printf("Absence enregistree [%d]\n", *totalAbsences);
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

void afficherListeEtudiants(const int nombreEtudiantsInscrits, const Etudiant* tableauEtudiants)
{
	int jourCourant;
	scanf("%d", &jourCourant);
	if (jourCourant < 1)
	{
		printf("Date incorrecte\n");
		return;
	}
	if (nombreEtudiantsInscrits == 0)
	{
		printf("Aucun inscrit\n");
		return;
	}
	const Etudiant* ptrEtudiants[MAX_ETUDIANTS];
	for (int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		ptrEtudiants[i] = &tableauEtudiants[i];
	}
	qsort(ptrEtudiants, nombreEtudiantsInscrits, sizeof(const Etudiant*), comparer);
	for (int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		printf("(%d) %-30s %3d ", ptrEtudiants[i]->identifiant + 1, ptrEtudiants[i]->nomEtudiant,
			ptrEtudiants[i]->numeroGroupe);
		int totalabsence = 0;
		for (int j = 0; j < MAX_ABSENCES; ++j)
		{
			if (ptrEtudiants[i]->absences[j].jourConcernant <= jourCourant && ptrEtudiants[i]->absences[j].
				jourConcernant != 0)
			{
				++totalabsence;
			}
		}
		printf("%3d\n", totalabsence);
	}
}*/