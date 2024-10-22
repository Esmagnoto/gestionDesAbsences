#include <stdio.h>
#include <assert.h>
#include <string.h>
#pragma warning (disable: 4996 6031)


#define MAXCARACTERES 31
#define MAXABSENCES 80
#define MAXETUDIANTS 100
#define MAXJUSTIFICATIF 51
#define AMPM 3

typedef enum {AUCUN = -1, AM, PM} periode;

typedef enum { TROUVE, NONTROUVE } etudiantStatus;

typedef enum
{
	ATTENTEJUSTIFICATIF, JUSTIFIE, NONJUSTIFIE, ABSENCENONCONNUE,
	IDENTIFIANTINVALIDE, DEMIJOURNEEINVALIDE, ABSENCEDEJACONNUE,
	DATEINVALIDE, DEPASSEMENTDEFORMAT, ENCOURS
} absenceStatus;



typedef struct
{
	char justificatif[MAXJUSTIFICATIF];
	int jourConcernant;
	periode demiJourneeConcernant;
	int identifiantAbsence;
	int identifiatnEtudiant;
	absenceStatus status;
} absence;

typedef struct
{
	char nomEtudiant[MAXCARACTERES];
	int numeroGroupe;
	int nombreAbsences;
	int identifiantEtudiant;
} etudiant;

typedef struct {
	etudiant tableauEtudiants[MAXETUDIANTS];
	absence tableauAbsences[MAXABSENCES];
	int cmpEtudiants;
	int cmpAbsences;
}promo;

// il sert a initializer une promo d'etudiants
void initializerPromo(promo* promo)
{
	assert(promo != NULL);
	promo->cmpEtudiants = 0;
	promo->cmpAbsences = 0;
	for (int i = 0; i < MAXETUDIANTS; ++i)
	{
		promo->tableauEtudiants[i].identifiantEtudiant = -1;
		promo->tableauEtudiants[i].nomEtudiant[0] = '\0';
		promo->tableauEtudiants[i].nombreAbsences = -1;
		promo->tableauEtudiants[i].numeroGroupe = -1;
	}
	for (int j = 0; j < MAXABSENCES; ++j)
	{
		promo->tableauAbsences[j].demiJourneeConcernant = -1;
		promo->tableauAbsences[j].jourConcernant = -1;
		promo->tableauAbsences[j].justificatif[0] = '\0';
		promo->tableauAbsences[j].status = AUCUN;
		promo->tableauAbsences[j].identifiatnEtudiant = -1;
		promo->tableauAbsences[j].identifiantAbsence = -1;
	}
}

// il inscrit un etudiant dans la promo, si le numero de groupe est different de 0 et le nom est nom null
void faireInscription(promo* promo, char nomEtudiant[MAXCARACTERES], int numeroGroupe)
{
	assert(nomEtudiant != NULL);
	assert(numeroGroupe > 0);
	assert(promo != NULL);

	if (promo->cmpEtudiants >= MAXETUDIANTS-1)
	{
		printf("Erreur: Nombre maximal d'etudiants atteint.\n");
		return;
	}

	if (verifierInscriptionStatus(promo, nomEtudiant, numeroGroupe) == NONTROUVE)
	{
		strncpy(promo->tableauEtudiants[promo->cmpEtudiants].nomEtudiant, nomEtudiant, MAXCARACTERES - 1);
		promo->tableauEtudiants[promo->cmpEtudiants].nomEtudiant[MAXCARACTERES] = '\0'; // Sécurité pour la terminaison
		promo->tableauEtudiants[promo->cmpEtudiants].numeroGroupe = numeroGroupe;
		printf("Inscription enregistree (%d)\n", promo->tableauEtudiants[promo->cmpEtudiants].identifiantEtudiant);
		promo->cmpEtudiants++;
	}
	else
	{
		printf("Nom incorrect\n");
	}
}

// return le status predefini de une inscription, complementaire de la fonction faireInscription();
etudiantStatus verifierInscriptionStatus(const promo* promo,char nomEtudiant[MAXCARACTERES], int numeroGroupe)
{
	assert(promo != NULL);
	assert(nomEtudiant != NULL);

	for (unsigned int i = 0; i < promo->cmpEtudiants; i++)
	{
		if (strcmp(promo->tableauEtudiants[i].nomEtudiant, nomEtudiant) == 0 && promo->tableauEtudiants[i].numeroGroupe == numeroGroupe)
		{
			return TROUVE;
		}
	}
	return NONTROUVE;
}

//gere l'entrree utilisateur pour une commande inscription
void gererCommandeInscription(promo* promo)
{
	char nom[MAXCARACTERES];
	int numero;

	if (scanf("%30s %d", nom, &numero) != 2)
	{
		printf("Commande invalide\n");
		return;
	}

	faireInscription(&promo, nom, numero);
}

//return le status de une absence, et verifie l'identifiant de l'etudiant, ça peuvait etre deux fonctions
absenceStatus verifierAbsence(int identifiantEtudiant, int jour, char periode[AMPM], promo* promo)
{
	if (identifiantEtudiant < 1 || identifiantEtudiant > MAXETUDIANTS || promo == NULL)
	{
		return IDENTIFIANTINVALIDE;
	}

	if (promo->tableauEtudiants[identifiantEtudiant-1].nomEtudiant[0] == '\0')
	{
		return IDENTIFIANTINVALIDE;
	}

	if (jour < 1 || jour > 40)
	{
		return DATEINVALIDE;
	}

	if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0)
	{
		return DEMIJOURNEEINVALIDE;
	}

	
	if (promo->cmpAbsences < 0 || promo->cmpAbsences >= MAXABSENCES)
	{
		return DEPASSEMENTDEFORMAT;
	}

	for (int i = 0; i < promo->cmpAbsences; i++)
	{
		
		if (promo->tableauAbsences[i].jourConcernant == jour &&
			((strcmp(periode, "am") == 0 && promo->tableauAbsences[i].demiJourneeConcernant == AM) ||
				(strcmp(periode, "pm") == 0 && promo->tableauAbsences[i].demiJourneeConcernant == PM)))
		{
			return ABSENCEDEJACONNUE;
		}
	}

	return ABSENCENONCONNUE;
}

// enregistrer une absence pour un etudiant, l'identifiant et jour est un nombre N > 0.
// cette fonction complemente gerer commande absence
void enregistrerAbsence(promo* promo, int identifiantEtudiant, int jour, char* periode)
{
	assert(identifiantEtudiant > 0);
	assert(jour > 0);
	if (promo->cmpAbsences >= MAXABSENCES*MAXETUDIANTS)
	{
		printf("Nombre maximal d'absences atteint\n");
		return;
	}

	promo->tableauAbsences[promo->cmpAbsences].justificatif[0] = '\0';
	promo->tableauAbsences[promo->cmpAbsences].jourConcernant = jour;
	promo->tableauAbsences[promo->cmpAbsences].demiJourneeConcernant = (strcmp(periode, "am") == 0) ? AM : PM;
	promo->tableauAbsences[promo->cmpAbsences].status = ATTENTEJUSTIFICATIF;
	promo->tableauAbsences[promo->cmpAbsences].identifiantAbsence = promo->cmpAbsences +1;
	promo->tableauAbsences[promo->cmpAbsences].identifiatnEtudiant = identifiantEtudiant;
	promo->cmpAbsences++;
}

void gererCommandeAbsence(promo* promo)
{
	int identifiantEtudiant;
	int jour;
	char periode[AMPM];

	if (scanf("%d %d %2s", &identifiantEtudiant, &jour, periode) != 3)
	{
		printf("Erreur de saisie des donnees.\n");
		return;
	}

	absenceStatus status = verifierAbsence(identifiantEtudiant, jour, periode, promo);

	switch (status)
	{
	case ABSENCENONCONNUE:
		enregistrerAbsence(promo, identifiantEtudiant, jour, periode, totalAbsences);
		printf("Absence enregistree [%d]\n", *totalAbsences);
		break;
	case IDENTIFIANTINVALIDE:
		printf("Identifiant incorrect\n");
		break;
	case DEMIJOURNEEINVALIDE:
		printf("Demi-journee incorrecte\n");
		break;
	case ABSENCEDEJACONNUE:
		printf("Absence deja connue\n");
		break;
	case DATEINVALIDE:
		printf("Date incorrecte\n");
		break;
	case DEPASSEMENTDEFORMAT:
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

void gererCommande() {
	int on = 0;
	while (on == 0)
	{
		char entree[MAXCARACTERES] = { 0 };

		if (scanf("%30s", entree) != 1)
		{
			printf("Commande invalide\n");
			continue;
		}


		if (strcmp(entree, "exit") == 0)
		{
			on = 1;
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
}

int main() {
	promo promo;

	initializerPromo(&promo);
	gererCommande();

}








