#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)//IGNORER RETOUR SCANF

#define MAX_CHAR_JUSTIFICATIF 50
#define MAX_CHAR 31
#define MAX_ABSENCES 80
#define MAX_ETUDIANTS 100
#define JOUR_CONCERNE 80

typedef enum { TROUVE, NONTROUVE } EtudiantStatus;

typedef enum
{
    ATTENTE_JUSTIFICATIF, JUSTIFIE, NON_JUSTIFIE, ABSENCE_NON_CONNUE, IDENTIFIANT_INVALIDE,
    DEMI_JOURNEE_INVALIDE, ABSENCE_DEJA_CONNUE, DATE_INVALIDE, DEPASSEMENT_DE_FORMAT
} AbsenceStatus;

typedef enum { AM = 1, PM = 2 } Periode;



typedef struct
{
    char justificatif[JOUR_CONCERNE][MAX_CHAR_JUSTIFICATIF];
    int jourConcernant;
    int demiJourneeConcernant;
    int identifiantAbsence;
    AbsenceStatus status;
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
void faireInscription(Etudiant* tableauEtudiants, int* indice, const char* nom, int numeroGroupe);
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant, int nombreEtudiantsInscrits, int numeroGroupe);
void gererCommandeInscription(Etudiant* tableauEtudiants, int* nombreEtudiantsInscrits);
AbsenceStatus verifierAbsence(int etudiantId, int jour, const char* periode, const Etudiant* tableEtudiants);
void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, const char* periode);
void gererCommandeAbsence(Etudiant* tableauEtudiants, int* totalAbsences);
int comparer(const void* etudiantA, const void* etudiantB);
void afficherListeEtudiants(int nombreEtudiantsInscrits, Etudiant* tableauEtudiants);
int trouverAbsence(Etudiant* tableauEtudiants, int identificateurAbsence);

int main()
{
    Etudiant tableauEtudiants[MAX_ETUDIANTS];
    int nombreEtudiantsInscrits = 0;
    int totalAbsences = 0;

    initializerTableauEtudiants(tableauEtudiants);

    while (1)
    {
        char entree[MAX_CHAR] = { 0 };
        if (scanf("%30s", entree) != 1) {
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
        else if (strcmp(entree,"justificatif") == 0)
        {
            int identificateurAbsence;
            int jour;
            char justificatif[51];
            scanf("%d %d", &identificateurAbsence, &jour);
            fgets(justificatif, 51, stdin);
            int absence = trouverAbsence(tableauEtudiants, identificateurAbsence);
            if (jour >= tableauEtudiants[0].absences[0].identifiantAbsence + 3)
            {

            }

            
        }
        else
        {
            printf("Commande invalide\n");
            while (getchar() != '\n');  // Nettoyage du buffer en cas d'erreur
        }
    }
}

//cette fonction peut etre divise en trois fonction, une fonction pour initializer un etudiant, et une pour une absence
// j'ai juge pas nécéssaire dans mon programme car, je n'a pas besoin de initializer des etdiants aprés la
//demarrage du programme, mais si le tableua d'etudiant etais alloque avec memoire dynamique malloc() 
// ça peurrait etre util et dans d'autres cas aussi.
void initializerTableauEtudiants(Etudiant* etudiants)
{
    int absenceCompteur = 0;
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
            etudiants[i].absences[j].justificatif[0][0] = '\0';
            etudiants[i].absences[j].status = -1;
            etudiants[i].absences[j].identifiantAbsence = absenceCompteur;
            ++absenceCompteur;
        }
    }
}

void faireInscription(Etudiant* tableauEtudiants, int* indice, const char* nom, int numeroGroupe)
{
    assert(nom != NULL);
    assert(numeroGroupe > 0);
    assert(*indice >= 0 && *indice < MAX_ETUDIANTS);
    assert(tableauEtudiants != NULL);

    if (*indice >= MAX_ETUDIANTS)
    {
        printf("Erreur: Nombre maximal d'etudiants atteint.\n");
        return;
    }

    if (verifierInscriptionStatus(tableauEtudiants, nom, *indice, numeroGroupe) == NONTROUVE)
    {
        strncpy(tableauEtudiants[*indice].nomEtudiant, nom, MAX_CHAR - 1);
        tableauEtudiants[*indice].nomEtudiant[MAX_CHAR - 1] = '\0';  // Sécurité pour la terminaison
        tableauEtudiants[*indice].numeroGroupe = numeroGroupe;
        printf("Inscription enregistree (%d)\n", *indice + 1);
        (*indice)++;
    }
    else
    {
        printf("Nom incorrect\n");
    }
}

EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant, int nombreEtudiantsInscrits, int numeroGroupe)
{
    assert(tableEtudiants != NULL);
    assert(nomEtudiant != NULL);

    for (int i = 0; i < nombreEtudiantsInscrits; i++)
    {
        if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0 && tableEtudiants[i].numeroGroupe == numeroGroupe)
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
        printf("Commande invalide\n");
        return;
    }

    faireInscription(tableauEtudiants, nombreEtudiantsInscrits, nom, numero);
}

AbsenceStatus verifierAbsence(int etudiantId, int jour, const char* periode, const Etudiant* tableEtudiants)
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

void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, const char* periode)
{
    int absenceNumero = etudiants[etudiantId - 1].nombreAbsences;
    if (absenceNumero >= MAX_ABSENCES)
    {
        printf("Nombre maximal d'absences atteint pour cet etudiant.\n");
        return;
    }

    etudiants[etudiantId - 1].absences[absenceNumero].justificatif[0][0] = '\0';
    etudiants[etudiantId - 1].absences[absenceNumero].jourConcernant = jour;
    etudiants[etudiantId - 1].absences[absenceNumero].demiJourneeConcernant = (strcmp(periode, "am") == 0) ? AM : PM;
    etudiants[etudiantId - 1].nombreAbsences++;
}

void gererCommandeAbsence(Etudiant* tableauEtudiants, int* totalAbsences)
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

void afficherListeEtudiants(int nombreEtudiantsInscrits, Etudiant* tableauEtudiants)
{
    int jourCourant;
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

    for (int i = 0; i < nombreEtudiantsInscrits; i++)
    {
        printf("(%d) %-30s %3d ", tableauEtudiants[i].identifiant, tableauEtudiants[i].nomEtudiant,
            tableauEtudiants[i].numeroGroupe);

        int totalAbsence = 0;
        for (int j = 0; j < tableauEtudiants[i].nombreAbsences; ++j)
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

int trouverAbsence(Etudiant* tableauEtudiants, int identificateurAbsence) {
    for (int i = 0; i < MAX_ETUDIANTS; i++)
    {
        for (size_t j = 0; j < MAX_ABSENCES; j++)
        {
            if (tableauEtudiants[i].absences[j].identifiantAbsence == identificateurAbsence)
            {
                printf("%d %s %d", tableauEtudiants[i].identifiant, tableauEtudiants[i].nomEtudiant, tableauEtudiants[i].absences[j].identifiantAbsence);
            }
            
        }
        
    }
}
