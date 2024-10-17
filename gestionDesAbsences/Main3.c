#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)//IGNORER RETOUR SCANF

#define MAX_CHAR_JUSTIFICATIF 51
#define MAX_CHAR 31
#define MAX_ABSENCES 80
#define MAX_ETUDIANTS 100


typedef enum { TROUVE, NONTROUVE } EtudiantStatus;

typedef enum
{
	ATTENTE_JUSTIFICATIF, JUSTIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE, IDENTIFIANT_INVALIDE,
	DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE, DEPASSEMENT_DE_FORMAT
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
	unsigned int identifiant;
} Etudiant;

void initializerTableauEtudiants(Etudiant* etudiants);
void faireInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits, const char* nom,
                      unsigned int numeroGroupe);
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant,
                                         unsigned int nombreEtudiantsInscrits, unsigned int numeroGroupe);
void gererCommandeInscription(Etudiant* tableauEtudiants, unsigned int* nombreEtudiantsInscrits);
AbsenceStatus verifierAbsence(unsigned int etudiantId, unsigned int jour, const char* periode,
                              const Etudiant* tableEtudiants);
void enregistrerAbsence(Etudiant* etudiants, unsigned int etudiantId, unsigned int jour, const char* periode);
void gererCommandeAbsence(Etudiant* tableauEtudiants, unsigned int* totalAbsences);
int comparer(const void* etudiantA, const void* etudiantB);
void afficherListeEtudiants(unsigned int nombreEtudiantsInscrits, Etudiant* tableauEtudiants);
Absence* trouverAbsence(Etudiant* tableauEtudiants, unsigned int identificateurAbsence);

int main()
{
	Etudiant tableauEtudiants[MAX_ETUDIANTS];
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
		else
		{
			printf("Commande invalide\n");
			while (getchar() != '\n'); // Nettoyage du buffer en cas d'erreur
		}
	}
}

//cette fonction peut etre divise en trois fonction, une fonction pour initializer un etudiant, et une pour une absence
// j'ai juge pas nécéssaire dans mon programme car, je n'a pas besoin de initializer des etudiants aprés la
//demarrage du programme, mais si le tableua d'etudiant etais alloque avec memoire dynamique malloc() 
// ça peurrait etre util et dans d'autres cas aussi.
void initializerTableauEtudiants(Etudiant* etudiants)
{
	unsigned int absenceCompteur = 0;
	assert(etudiants != NULL);
	for (int i = 0; i < MAX_ETUDIANTS; ++i)
	{
		etudiants[i].identifiant = i + 1;
		etudiants[i].nomEtudiant[0] = '\0';
		etudiants[i].nombreAbsences = 0;
		etudiants[i].numeroGroupe = 0;

		for (int j = 0; j < MAX_ABSENCES; ++j)
		{
			etudiants[i].absences[j].demiJourneeConcernant = 0;
			etudiants[i].absences[j].jourConcernant = 0;
			etudiants[i].absences[j].justificatif[0] = '\0';
			etudiants[i].absences[j].status = -1;
			etudiants[i].absences[j].identifiantAbsence = absenceCompteur;
			++absenceCompteur;
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
		printf("Inscription enregistree (%d)\n", tableauEtudiants[*nombreEtudiantsInscrits].identifiant);
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

void enregistrerAbsence(Etudiant* etudiants, unsigned int etudiantId, unsigned int jour, const char* periode)
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

	qsort(tableauEtudiants, nombreEtudiantsInscrits, sizeof(Etudiant), comparer);

	for (unsigned int i = 0; i < nombreEtudiantsInscrits; i++)
	{
		printf("(%d) %-30s %3d ", tableauEtudiants[i].identifiant, tableauEtudiants[i].nomEtudiant,
		       tableauEtudiants[i].numeroGroupe);

		int totalAbsence = 0;
		for (unsigned int j = 0; j < tableauEtudiants[i].nombreAbsences; ++j)
		{
			if (tableauEtudiants[i].absences[j].jourConcernant <= jourCourant &&
				tableauEtudiants[i].absences[j].jourConcernant != 0)
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
			// identificateurAbsence est déjà initialisé donc il va etre trouvé mais le jour concernant ne peut pas etre 0, car cela signifie que la absence n'existe pas
			if (tableauEtudiants[i].absences[j].identifiantAbsence == identificateurAbsence && tableauEtudiants[i].
				absences[j].jourConcernant != 0)
			{
				//printf("%d %s %d\n", tableauEtudiants[i].identifiant, tableauEtudiants[i].nomEtudiant, tableauEtudiants[i].absences[j].identifiantAbsence);
				return &tableauEtudiants[i].absences[j];
			}
		}
	}
	return NULL;
}


/*
 * LE OBJECTIF PRINCIPAL DE CETTE FONCTION DEVRAIT ETRE TROUVER UNE ABSENCE ET LA RETOURNER. MAIS A LA FIN ELLE FAIT AUTRE CHOSE? QUI MAS DONNE UNE IDEE POUR AMELIORER LA FONCTION
 * verifierAbsence(); DONC JE LA LAISSE COMENTEE SI JAI LE TEMPS JE LUTILIZERAS
// la fonction verifier absence, verifie si une absence pour un eleve specifique existe, cette fonction verifie si une absence existe pour n'impoirte quel eleve avec un identifiantAbsence precis
// je suis sur que je peux utiliser la meme logique pour trouver un etudiant et trouver une absence, mais la fonction verifier absence etait code au debut du projet et la il me manque 7 jours pour finir
//je changerait la logique des deux fonctions si j'ai le temps de le faire. ça peut etre une seule fonction que fait la meme chose. 
int trouverAbsence(Etudiant* tableauEtudiants, unsigned int identificateurAbsence) {
    for (int i = 0; i < MAX_ETUDIANTS; i++)
    {
        for (int j = 0; j < MAX_ABSENCES; j++)
        {
            // identificateurAbsence est déjà initialisé donc il va etre trouvé mais le jour concernant ne peut pas etre 0, car cela signifie que la absence n'existe pas
            if (tableauEtudiants[i].absences[j].identifiantAbsence == identificateurAbsence && tableauEtudiants[i].absences[j].jourConcernant != 0) 
            {
                //printf("%d %s %d\n", tableauEtudiants[i].identifiant, tableauEtudiants[i].nomEtudiant, tableauEtudiants[i].absences[j].identifiantAbsence);
                return 1;
            }
            
        }
        
    }
    return 0;
}
*/
