#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)//IGNORER RETOUR SCANF

enum {MAX_JUSTIFICATIF = 50, MAX_NOM = 31, MAX_ABSENCES = 80, MAX_ETUDIANTS = 100, MAX_ENTREE = 31};
typedef enum { AM = 1, PM = 2 } Periode;
typedef enum { ETUDIANT_NON_TROUVE, ETUDIANT_TROUVE } EtudiantStatus;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSITIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE
} AbsenceStatus;

typedef struct {
	char justificatif[MAX_JUSTIFICATIF];
	int jourConcernant;
	Periode demiJourneeConcernant;
}Absence;

typedef struct {
	char nomEtudiant[MAX_NOM];
	int numeroGroupe;
	Absence absences[MAX_ABSENCES];
	int nombreAbsences;
}Etudiant;

//initialiser un etudiant
void initialiserEtudiant(Etudiant* etudiants);

//inscrire un etudiant
void inscrireEtudiant(Etudiant tableEtudiants[MAX_ETUDIANTS], int nombreEtudiantsInscrits, char nomEtudiant[MAX_NOM],
	unsigned int numeroGroupe);

// verifie si un etudiant est ou n'est pas déjà inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(Etudiant tableEtudiants[MAX_ETUDIANTS], char nomEtudiant[]);

//defini le status d'un etudiant absent
AbsenceStatus verifierAbsence(int etudiantId, int jour, char periode[3],
	Etudiant tableEtudiants[MAX_ETUDIANTS]);

void enregistrerAbsence();

int main() {
	Etudiant etudiants[MAX_ETUDIANTS] = {0};
	int totalAbsences = 0;
	int nombreEtudiantsInscrits = 0;
	char entree[MAX_ENTREE] = { '\0' };

	for (int i = 0; i < MAX_ETUDIANTS; i++) { initialiserEtudiant(&etudiants[i]); }

	while (1)
	{
		printf("ecrire une commande: ");
		scanf("%30s", entree);

		if (strcmp(entree, "exit") == 0)
		{
			return 0;
		}
		else if (strcmp(entree, "inscription") == 0)
		{
			char nomEtudiant[MAX_NOM] = { '\0' };
			unsigned int numeroGroupe = 0;
			scanf("%30s", nomEtudiant);

			if (scanf("%u", &numeroGroupe) == 1 && verifierInscriptionStatus(etudiants, nomEtudiant) ==
				ETUDIANT_NON_TROUVE && numeroGroupe > 0)
			{
				inscrireEtudiant(etudiants, nombreEtudiantsInscrits, nomEtudiant, numeroGroupe);
				printf("Inscription enregistree (%d)\n", nombreEtudiantsInscrits + 1);
				++nombreEtudiantsInscrits;
			}
			else
			{
				printf("Nom Incorrect\n");
			}
		}
		else if (strcmp(entree, "absence") == 0)
		{
			int etudiantId;
			int jour;
			char periode[3];
			scanf("%d %d %2s", &etudiantId, &jour, periode);

			AbsenceStatus status = verifierAbsence(etudiantId, jour, periode, etudiants);
			Etudiant etudiant = etudiants[etudiantId - 1];
			switch (status)
			{
			case ABSENCE_NON_CONNUE:
				enregistrerAbsence();
				printf("Absence enregistree [%u]\n", etudiant.nombreAbsences);
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
			return 1;
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
}

void initialiserEtudiant(Etudiant* etudiant)
{
	assert(etudiant != NULL);
	etudiant->nomEtudiant[0] = '\0'; // initialise avec string null
	etudiant->numeroGroupe = 0;
	etudiant->nombreAbsences = 0;
	

	for (int i = 0; i < MAX_ABSENCES; i++)
	{
		assert(i >= 0 && i < MAX_ABSENCES);
		etudiant->absences[i].justificatif[0] = '\0';
		etudiant->absences[i].jourConcernant = 0; 
		etudiant->absences[i].demiJourneeConcernant = 0; 
	}
}

// verifie si un etudiant est ou n'est pas déjà inscrit dans la table.
EtudiantStatus verifierInscriptionStatus(const Etudiant tableEtudiants[MAX_ETUDIANTS], char nomEtudiant[])
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
void inscrireEtudiant(Etudiant tableEtudiants[MAX_ETUDIANTS], int nombreEtudiantsInscrits, char nomEtudiant[MAX_NOM],
	unsigned int numeroGroupe)
{
	assert(nomEtudiant != NULL && strlen(nomEtudiant) > 0);
	assert(numeroGroupe > 0);
	assert(nombreEtudiantsInscrits >= 0 && nombreEtudiantsInscrits < MAX_ETUDIANTS);
	assert(tableEtudiants != NULL);
	strcpy(tableEtudiants[nombreEtudiantsInscrits].nomEtudiant, nomEtudiant);
	tableEtudiants[nombreEtudiantsInscrits].numeroGroupe = numeroGroupe;
}

//defini le status d'une  absence
AbsenceStatus verifierAbsence(int etudiantId, int jour, char periode[3],
	const Etudiant tableEtudiants[MAX_ETUDIANTS])
{
	Etudiant* etudiant = &tableEtudiants[etudiantId - 1];

	if (etudiantId <= 0 || etudiantId > MAX_ETUDIANTS && verifierInscriptionStatus(tableEtudiants, etudiant->nomEtudiant) != ETUDIANT_TROUVE) { return IDENTIFIANT_INVALIDE; }
	if (jour < 1 || jour > 40){return  DATE_INVALIDE;}
	if (strcmp(periode, "am") != 0 || strcmp(periode, "pm") != 0) { return DEMI_JOURNEE_INVALIDE; }

	for (int i = 0; i < MAX_ABSENCES; i++)
	{
		Absence absence = etudiant->absences[i];
		if (absence.jourConcernant == jour &&( (strcmp(periode, "am") == 0 && absence.demiJourneeConcernant == AM) ||
			(strcmp(periode, "pm") == 0 && absence.demiJourneeConcernant == PM)))
		{
			return ABSENCE_DEJA_CONNUE;
		}

	}
	return ABSENCE_NON_CONNUE;
}

void enregistrerAbsence()
{
	
}
