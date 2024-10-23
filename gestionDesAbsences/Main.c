#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable: 4996 6031)


#define MAXCARACTERES 31
#define MAXABSENCES 80
#define MAXETUDIANTS 100
#define MAXJUSTIFICATIF 51
#define AMPM 3

typedef enum { AUCUN = -1, AM, PM } periode;

typedef enum { TROUVE, NONTROUVE } etudiantStatus;

typedef enum
{
	NONCONNUE = -1, CONNUE, ATTENTEJUSTIFICATIF, NONJUSTIFIE, ENCOURS, JUSTIFIE, IDENTIFIANTINVALIDE, DATEINVALIDE,
	DEMIJOURNEEINVALIDE
} absenceStatus;


typedef struct
{
	char justificatif[MAXJUSTIFICATIF];
	int jourConcernant;
	periode demiJourneeConcernant;
	int identifiantAbsence;
	int identifiantEtudiant;
	absenceStatus status;
} absence;

typedef struct
{
	char nomEtudiant[MAXCARACTERES];
	int numeroGroupe;
	int nombreAbsences;
	int identifiantEtudiant;
} etudiant;

typedef struct
{
	etudiant tableauEtudiants[MAXETUDIANTS];
	absence tableauAbsences[MAXABSENCES * MAXETUDIANTS];
	int cmpEtudiants;
	int cmpAbsences;
} promotion;

// Fonction pour initializer une promo d'etudiants
void initializerPromo(promotion* promo);

// Fonction pour appeler la fonction correspondant des commandes C0 à C8
void gererCommande(promotion* promo);

//Fonction pour gerer l'entrree utilisateur pour une commande inscription
void gererCommandeInscription(promotion* promo);

// Fonction pour gerer l'entree utilisateur pour une commande absence
void gererCommandeAbsence(promotion* promo);

// appeler les fonctions pour gerencier une justificatif d'absence
void gererCommandeJustificatif(promotion* promo);

// affiche une liste trie d'etudiants
void gererCommandeEtudiants(promotion* promo);

// Fonction pour gerer l'entree utilisateur pour une commande absence
void gererCommandevalidations(promotion* promo);

// je supprime des espaces ou \n à gauche de la string, fonction  temporaire
void supprimerEspacesGauche(char* str);

// returner une absence enregistre
absence* trouverAbsence(promotion* promo, int identificateurAbsence);

// enregistrer une absence pour un etudiant, l'identifiant et jour est un nombre N > 0.
// cette fonction complemente gerer commande absence
void enregistrerAbsence(promotion* promo, int identifiantEtudiant, int jour, char* periode);

//return le status de une absence, et verifie l'identifiant de l'etudiant, ça peuvait etre deux fonctions
absenceStatus verifierAbsence(int identifiantEtudiant, int jour, char periode[AMPM], promotion* promo);

// il inscrit un etudiant dans la promo, si le numero de groupe est different de 0 et le nom est nom null
void faireInscription(promotion* promo, const char* nomEtudiant, int numeroGroupe);

// return le status predefini de une inscription, complementaire de la fonction faireInscription();
etudiantStatus verifierInscriptionStatus(const promotion* promo, const char* nomEtudiant, int numeroGroupe);

// Fonction de comparaison pour qsort, appliquée aux pointeurs
int comparerPointeurs(const void* a, const void* b);

// Fonction de comparaison pour qsort
int comparer(const void* etudiantA, const void* etudiantB);

// Affichage de la liste triée en utilisant les pointeurs
void afficherListeEtudiant(promotion* promo, etudiant** tableauPointeursEtudiants, int jourCourant);

int main()
{
	promotion promo;
	initializerPromo(&promo);
	gererCommande(&promo);
}

// il sert a initializer une promo d'etudiants
void initializerPromo(promotion* promo)
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
	for (int j = 0; j < MAXABSENCES * MAXETUDIANTS; ++j)
	{
		promo->tableauAbsences[j].demiJourneeConcernant = -1;
		promo->tableauAbsences[j].jourConcernant = -1;
		promo->tableauAbsences[j].justificatif[0] = '\0';
		promo->tableauAbsences[j].status = NONCONNUE;
		promo->tableauAbsences[j].identifiantEtudiant = -1;
		promo->tableauAbsences[j].identifiantAbsence = -1;
	}
}

// return le status predefini de une inscription, complementaire de la fonction faireInscription();
etudiantStatus verifierInscriptionStatus(const promotion* promo, const char* nomEtudiant, int numeroGroupe)
{
	assert(promo != NULL);

	for (int i = 0; i < promo->cmpEtudiants; i++)
	{
		if (strcmp(promo->tableauEtudiants[i].nomEtudiant, nomEtudiant) == 0 && promo->tableauEtudiants[i].numeroGroupe
			== numeroGroupe)
		{
			return TROUVE;
		}
	}
	return NONTROUVE;
}

// il inscrit un etudiant dans la promo, si le numero de groupe est different de 0 et le nom est nom null
void faireInscription(promotion* promo, const char* nomEtudiant, int numeroGroupe)
{
	assert(numeroGroupe > 0);
	assert(promo != NULL);

	if (promo->cmpEtudiants >= MAXETUDIANTS)
	{
		printf("Erreur: Nombre maximal d'etudiants atteint.\n");
		return;
	}

	if (verifierInscriptionStatus(promo, nomEtudiant, numeroGroupe) == NONTROUVE)
	{
		strncpy(promo->tableauEtudiants[promo->cmpEtudiants].nomEtudiant, nomEtudiant, MAXCARACTERES - 1);
		promo->tableauEtudiants[promo->cmpEtudiants].nomEtudiant[MAXCARACTERES - 1] = '\0';
		// Sécurité pour la terminaison
		promo->tableauEtudiants[promo->cmpEtudiants].numeroGroupe = numeroGroupe;
		promo->tableauEtudiants[promo->cmpEtudiants].identifiantEtudiant = promo->cmpEtudiants + 1;
		printf("Inscription enregistree (%d)\n", promo->tableauEtudiants[promo->cmpEtudiants].identifiantEtudiant);
		promo->cmpEtudiants++;
	}
	else
	{
		printf("Nom incorrect\n");
	}
}


//gere l'entrree utilisateur pour une commande inscription
void gererCommandeInscription(promotion* promo)
{
	char nom[MAXCARACTERES];
	int numero;

	if (scanf("%30s %d", nom, &numero) != 2)
	{
		printf("Commande invalide\n");
		return;
	}

	faireInscription(promo, nom, numero);
}

//return le status de une absence en verifient l'identifiant de l'etudiant, la date, le periode concernee
absenceStatus verifierAbsence(int identifiantEtudiant, int jour, char periode[AMPM], promotion* promo)
{
	if (jour < 1 || jour > 40)
	{
		return DATEINVALIDE;
	}
	if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0)
	{
		return DEMIJOURNEEINVALIDE;
	}
	if (identifiantEtudiant < 1 || identifiantEtudiant > MAXETUDIANTS || promo->tableauEtudiants[identifiantEtudiant -
		1].identifiantEtudiant != identifiantEtudiant)
	{
		return IDENTIFIANTINVALIDE;
	}
	for (int i = 0; i < promo->cmpAbsences; i++)
	{
		if ((promo->tableauAbsences[i].jourConcernant == jour && promo->tableauAbsences[i].identifiantEtudiant ==
				identifiantEtudiant) &&
			((strcmp(periode, "am") == 0 && promo->tableauAbsences[i].demiJourneeConcernant == AM) ||
				(strcmp(periode, "pm") == 0 && promo->tableauAbsences[i].demiJourneeConcernant == PM)))
		{
			return CONNUE;
		}
	}

	return NONCONNUE;
}

// enregistrer une absence pour un etudiant, l'identifiant et jour est un nombre N > 0.
// cette fonction complemente gerer commande absence
void enregistrerAbsence(promotion* promo, int identifiantEtudiant, int jour, char* periode)
{
	assert(identifiantEtudiant > 0);
	assert(jour > 0);
	if (promo->cmpAbsences >= MAXABSENCES * MAXETUDIANTS)
	{
		printf("Nombre maximal d'absences atteint\n");
		return;
	}

	promo->tableauAbsences[promo->cmpAbsences].justificatif[0] = '\0';
	promo->tableauAbsences[promo->cmpAbsences].jourConcernant = jour;
	promo->tableauAbsences[promo->cmpAbsences].demiJourneeConcernant = (strcmp(periode, "am") == 0) ? AM : PM;
	promo->tableauAbsences[promo->cmpAbsences].status = ATTENTEJUSTIFICATIF;
	promo->tableauAbsences[promo->cmpAbsences].identifiantAbsence = promo->cmpAbsences + 1;
	promo->tableauAbsences[promo->cmpAbsences].identifiantEtudiant = identifiantEtudiant;
	promo->cmpAbsences++;
}

// gre l'entree utilisateur pour une commande absence, verifier depassement de format et entree
void gererCommandeAbsence(promotion* promo)
{
	int identifiantEtudiant;
	int jour;
	char periode[AMPM];

	if (scanf("%d %d %2s", &identifiantEtudiant, &jour, periode) != 3)
	{
		printf("Erreur de saisie des donnees.\n");
		return;
	}

	if (promo->cmpAbsences < 0 || promo->cmpAbsences >= MAXABSENCES * MAXETUDIANTS)
	{
		printf("Depassement de format\n");
		return;
	}
	absenceStatus status = verifierAbsence(identifiantEtudiant, jour, periode, promo);

	switch (status)
	{
	case NONCONNUE:
		enregistrerAbsence(promo, identifiantEtudiant, jour, periode);
		printf("Absence enregistree [%d]\n", promo->cmpAbsences);
		break;
	case IDENTIFIANTINVALIDE:
		printf("Identifiant incorrect\n");
		break;
	case DEMIJOURNEEINVALIDE:
		printf("Demi-journee incorrecte\n");
		break;
	case CONNUE:
		printf("Absence deja connue\n");
		break;
	case DATEINVALIDE:
		printf("Date incorrecte\n");
		break;
	default:
		printf("Erreur inconnue\n");
		break;
	}
}

// Fonction de comparaison pour qsort
int comparer(const void* etudiantA, const void* etudiantB)
{
	const etudiant* etudiant1 = (const etudiant*)etudiantA;
	const etudiant* etudiant2 = (const etudiant*)etudiantB;

	if (etudiant1->numeroGroupe < etudiant2->numeroGroupe)
		return -1;
	if (etudiant1->numeroGroupe > etudiant2->numeroGroupe)
		return 1;
	return strcmp(etudiant1->nomEtudiant, etudiant2->nomEtudiant);
}

// Fonction de comparaison pour qsort, appliquée aux pointeurs
int comparerPointeurs(const void* a, const void* b)
{
	const etudiant* etudiant1 = *(const etudiant**)a;
	const etudiant* etudiant2 = *(const etudiant**)b;

	if (etudiant1->numeroGroupe < etudiant2->numeroGroupe)
		return -1;
	if (etudiant1->numeroGroupe > etudiant2->numeroGroupe)
		return 1;
	return strcmp(etudiant1->nomEtudiant, etudiant2->nomEtudiant);
}

int comparerPointeursAbsences(const void* a, const void* b)
{
	const absence* abs1 = *(const absence**)a;
	const absence* abs2 = *(const absence**)b;

	// Comparaison par identifiant d'étudiant
	if (abs1->identifiantEtudiant < abs2->identifiantEtudiant)
		return -1;
	if (abs1->identifiantEtudiant > abs2->identifiantEtudiant)
		return 1;

	// Si les étudiants sont les mêmes, comparaison par jour
	if (abs1->jourConcernant < abs2->jourConcernant)
		return -1;
	if (abs1->jourConcernant > abs2->jourConcernant)
		return 1;

	// Si les jours sont les mêmes, comparaison par demi-journée
	if (abs1->demiJourneeConcernant < abs2->demiJourneeConcernant)
		return -1;
	if (abs1->demiJourneeConcernant > abs2->demiJourneeConcernant)
		return 1;

	return 0;
}

// affiche une liste trie d'etudiants
void gererCommandeEtudiants(promotion* promo)
{
	int jourCourant;
	if (scanf("%d", &jourCourant) != 1 || jourCourant < 1)
	{
		printf("Date incorrecte\n");
		return;
	}

	if (promo->cmpEtudiants == 0)
	{
		printf("Aucun inscrit\n");
		return;
	}

	etudiant* tableauPointeursEtudiants[MAXETUDIANTS] = {0};
	for (int i = 0; i < promo->cmpEtudiants; i++)
	{
		tableauPointeursEtudiants[i] = &promo->tableauEtudiants[i];
	}

	// Tri des pointeurs (au lieu de trier les étudiants eux-mêmes)
	qsort(tableauPointeursEtudiants, promo->cmpEtudiants, sizeof(etudiant*), comparerPointeurs);

	// Affichage de la liste triée en utilisant les pointeurs
	afficherListeEtudiant(promo, tableauPointeursEtudiants, jourCourant);


	/*promotion promoCopie;
		memcpy(&promoCopie, promo, sizeof(promotion));

		qsort(promoCopie.tableauEtudiants, promoCopie.cmpEtudiants, sizeof(etudiant), comparer);

		for (int i = 0; i < promoCopie.cmpEtudiants; i++)
		{
			printf("(%d) %-30s %3d ", promoCopie.tableauEtudiants[i].identifiantEtudiant,
				   promoCopie.tableauEtudiants[i].nomEtudiant,
				   promoCopie.tableauEtudiants[i].numeroGroupe);

			int totalAbsence = 0;
			for (int j = 0; j < promoCopie.cmpAbsences; ++j)
			{
				if (promoCopie.tableauAbsences[j].jourConcernant <= jourCourant && promoCopie.tableauAbsences[j].
					jourConcernant
					!= 0)
				{
					totalAbsence++;
				}
			}
			printf("%3d\n", totalAbsence);
		}*/
}

void afficherListeEtudiant(promotion* promo, etudiant** tableauPointeursEtudiants, int jourCourant)
{
	int largeurNom = 0;

	// Trouver la longueur maximale du nom d'étudiant
	for (int i = 0; i < promo->cmpEtudiants; i++)
	{
		int longueurNom = strlen(tableauPointeursEtudiants[i]->nomEtudiant);
		if (longueurNom > largeurNom)
		{
			largeurNom = longueurNom;
		}
	}
	for (int i = 0; i < promo->cmpEtudiants; i++)
	{
		printf("(%d) %-*s %3d ",
		       tableauPointeursEtudiants[i]->identifiantEtudiant,
		       largeurNom, tableauPointeursEtudiants[i]->nomEtudiant,
		       tableauPointeursEtudiants[i]->numeroGroupe);

		// Calcul du total d'absences pour cet étudiant
		int totalAbsence = 0;
		for (int j = 0; j < promo->cmpAbsences; ++j)
		{
			if (promo->tableauAbsences[j].identifiantEtudiant == tableauPointeursEtudiants[i]->identifiantEtudiant &&
				promo->tableauAbsences[j].jourConcernant <= jourCourant &&
				promo->tableauAbsences[j].jourConcernant != 0)
			{
				totalAbsence++;
			}
		}
		printf("%d\n", totalAbsence);
	}
}

// returner une absence enregistre
absence* trouverAbsence(promotion* promo, int identificateurAbsence)
{
	for (int i = 0; i < promo->cmpAbsences; i++)
	{
		if (promo->tableauAbsences[i].identifiantAbsence == identificateurAbsence && promo->tableauAbsences[i].
			jourConcernant != -1)
		{
			return &promo->tableauAbsences[i];
		}
	}
	return NULL;
}

// je supprime des espaces ou \n à gauche de la string, fonction  temporaire
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

// appeler les fonctions pour gerencier une justificatif d'absence
void gererCommandeJustificatif(promotion* promo)
{
	int identificateurAbsence;
	int jour;
	char justificatif[51];
	scanf("%d %d", &identificateurAbsence, &jour);
	while (getchar() == '\n');

	if (fgets(justificatif, sizeof(justificatif), stdin) != NULL)
	{
		//supprimerEspacesGauche(justificatif);
		size_t longueur = strlen(justificatif);
		if (justificatif[longueur - 1] == '\n')
		{
			justificatif[longueur - 1] = '\0';
		}
	}
	absence* absence = trouverAbsence(promo, identificateurAbsence);
	if (absence == NULL)
	{
		printf("Identifiant incorrect\n");
	}
	else if (jour < absence->jourConcernant)
	{
		printf("Date incorrecte\n");
	}
	else if (absence->justificatif[0] != '\0')
	{
		printf("Justificatif deja connu\n");
	}
	else if (jour > absence->jourConcernant + 3)
	{
		strncpy(absence->justificatif, justificatif, MAXJUSTIFICATIF - 1);
		absence->status = NONJUSTIFIE;
		printf("Justificatif enregistre\n");
	}
	else
	{
		strncpy(absence->justificatif, justificatif, MAXJUSTIFICATIF - 1);
		absence->status = ENCOURS;
		printf("Justificatif enregistre\n");
	}
}

// affiche la liste de justificatifs pour pas encore validées
void gererCommandeValidations(promotion* promo)
{
	int aucuneabsenceAttente = 0;


	absence* tableauPointeursAbsences[MAXABSENCES * MAXETUDIANTS] = {0};

	// Initialisation du tableau de pointeurs
	for (int i = 0; i < promo->cmpAbsences; i++)
	{
		tableauPointeursAbsences[i] = &promo->tableauAbsences[i];
	}

	// Trier le tableau de pointeurs d'absences
	qsort(tableauPointeursAbsences, promo->cmpAbsences, sizeof(absence*), comparerPointeursAbsences);

	// Parcourir le tableau de pointeurs trié et afficher les absences en attente de validation
	for (int j = 0; j < promo->cmpAbsences; j++)
	{
		absence* abs = tableauPointeursAbsences[j];

		// Vérifier si l'absence est en attente de validation
		if (abs->status == ENCOURS && abs->justificatif[0] != '\0')
		{
			// Récupérer les informations de l'étudiant correspondant
			etudiant* etu = NULL;
			for (int i = 0; i < promo->cmpEtudiants; i++)
			{
				if (promo->tableauEtudiants[i].identifiantEtudiant == abs->identifiantEtudiant)
				{
					etu = &promo->tableauEtudiants[i];
					break;
				}
			}

			// Vérification que l'étudiant existe
			if (etu != NULL)
			{
				// Afficher les informations comme demandé
				printf("[%-d] (%-d) %-s %d %d/%s (%-s)\n",
				       abs->identifiantAbsence,
				       abs->identifiantEtudiant,
				       etu->nomEtudiant,
				       etu->numeroGroupe,
				       abs->jourConcernant,
				       abs->demiJourneeConcernant == AM ? "am" : "pm",
				       abs->justificatif);

				aucuneabsenceAttente++;
			}
		}
	}

	// Si aucune absence n'attend de validation
	if (aucuneabsenceAttente == 0)
	{
		printf("Aucune validation en attente\n");
	}
}


// Fonction pour appeler la fonction correspondant des commandes C0 à C8
void gererCommande(promotion* promo)
{
	int on = 0;
	while (on == 0)
	{
		char entree[MAXCARACTERES] = {0};

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
			gererCommandeInscription(promo);
		}
		else if (strcmp(entree, "absence") == 0)
		{
			gererCommandeAbsence(promo);
		}
		else if (strcmp(entree, "etudiants") == 0)
		{
			gererCommandeEtudiants(promo);
		}
		else if (strcmp(entree, "justificatif") == 0)
		{
			gererCommandeJustificatif(promo);
		}
		else if (strcmp(entree, "validations") == 0)
		{
			gererCommandeValidations(promo);
		}
		else if (strcmp(entree, "validation") == 0)
		{
			int identifiantAbsence;
			char validation[3];
			scanf("%d %2s", &identifiantAbsence, validation);
			absence* absence = trouverAbsence(promo, identifiantAbsence);
			if (absence == NULL)
			{
				printf("Identifiant incorrect\n");
			}
			else if (absence->status == JUSTIFIE && absence->justificatif[0] != '\0')
			{
				printf("Validation deja connue\n");
			}
			else if (absence->status != ENCOURS)
			{
				printf("Identifiant incorrect\n");
			}
			else if (strcmp(validation, "ok") != 0 && strcmp(validation, "ko") != 0)
			{
				printf("Code incorrect\n");
			}
			else
			{
				printf("Validation enregistree\n");
				absence->status = strcmp(validation, "ok") == 0 ? JUSTIFIE : NONJUSTIFIE;
			}
		}
		else
		{
			printf("Commande invalide\n");

			while (getchar() != '\n'); // Nettoyage du buffer en cas d'erreur
		}
	}
}
