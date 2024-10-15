/*#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable : 6031)//IGNORER RETOUR SCANF

#define MAX_JUSTIFICATIF 50
#define MAX_NOM 31
#define MAX_ABSENCES 80
#define MAX_ETUDIANTS 100
#define MAX_ENTREE 31

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
} Etudiant;

void initialiserEtudiant(Etudiant* etudiant);
void inscrireEtudiant(Etudiant* tableEtudiants, int nombreEtudiantsInscrits, const char* nomEtudiant, int numeroGroupe);
EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant, int nombreEtudiantsInscrits);
AbsenceStatus verifierAbsence(int etudiantId, int jour, const char* periode, const Etudiant* tableEtudiants);
void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, const char* periode);

int main() {
    Etudiant* etudiants = (Etudiant*)malloc(MAX_ETUDIANTS * sizeof(Etudiant));
    if (etudiants == NULL) {
        fprintf(stderr, "Erreur: Impossible d'allouer la memoire pour les etudiants.\n");
        return EXIT_FAILURE;
    }

    int totalAbsences = 0;
    int nombreEtudiantsInscrits = 0;
    char entree[MAX_ENTREE] = { '\0' };

    // Inicializa os estudantes
    for (int i = 0; i < MAX_ETUDIANTS; i++) {
        initialiserEtudiant(&etudiants[i]);
    }

    while (1) {
        printf("ecrire une commande: ");
        if (scanf("%30s", entree) != 1) {
            fprintf(stderr, "Erreur lors de la lecture de la commande.\n");
            continue;
        }

        if (strcmp(entree, "exit") == 0) {
            break;
        }

        if (strcmp(entree, "inscription") == 0) {
            char nomEtudiant[MAX_NOM] = { '\0' };
            int numeroGroupe = 0;

            if (scanf("%30s %d", nomEtudiant, &numeroGroupe) != 2 || numeroGroupe <= 0) {
                fprintf(stderr, "Nom ou groupe invalide.\n");
                continue;
            }

            if (nombreEtudiantsInscrits >= MAX_ETUDIANTS) {
                printf("Erreur: Nombre maximal d'etudiants atteint.\n");
            }
            else if (verifierInscriptionStatus(etudiants, nomEtudiant, nombreEtudiantsInscrits) == ETUDIANT_NON_TROUVE) {
                inscrireEtudiant(etudiants, nombreEtudiantsInscrits, nomEtudiant, numeroGroupe);
                printf("Inscription enregistree (%d)\n", nombreEtudiantsInscrits + 1);
                nombreEtudiantsInscrits++;
            }
            else {
                printf("Nom deja inscrit.\n");
            }
        }
        else if (strcmp(entree, "absence") == 0) {
            int etudiantId, jour;
            char periode[3];

            if (scanf("%d %d %2s", &etudiantId, &jour, periode) != 3) {
                fprintf(stderr, "Erreur: Entree invalide pour absence.\n");  
                continue;
            }

            AbsenceStatus status = verifierAbsence(etudiantId, jour, periode, etudiants);
            switch (status) {
            case ABSENCE_NON_CONNUE:
                enregistrerAbsence(etudiants, etudiantId, jour, periode);
                totalAbsences++;
                printf("Absence enregistree [%d]\n", totalAbsences);
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
                printf("L'etudiant a depasse le nombre de absences possible.\n");
                break;
            default:
                printf("Erreur inconnue\n");
                break;
            }
        }
        else {
            printf("Commande invalide\n");
            while (getchar() != '\n'); // Limpa o buffer
        }
    }

    free(etudiants); // Libera memória alocada antes de sair
    return EXIT_SUCCESS;
}

void initialiserEtudiant(Etudiant* etudiant) {
    assert(etudiant != NULL);
    memset(etudiant->nomEtudiant, '\0', MAX_NOM); // inicializa string vazia
    etudiant->numeroGroupe = 0;
    etudiant->nombreAbsences = 0;

    for (int i = 0; i < MAX_ABSENCES; i++) {
        etudiant->absences[i].justificatif[0] = '\0';
        etudiant->absences[i].jourConcernant = 0;
        etudiant->absences[i].demiJourneeConcernant = AM; // Default value
    }
}

EtudiantStatus verifierInscriptionStatus(const Etudiant* tableEtudiants, const char* nomEtudiant, int nombreEtudiantsInscrits) {
    assert(tableEtudiants != NULL && nomEtudiant != NULL);

    for (int i = 0; i < nombreEtudiantsInscrits; i++) {
        if (strcmp(tableEtudiants[i].nomEtudiant, nomEtudiant) == 0) {
            return ETUDIANT_TROUVE;
        }
    }
    return ETUDIANT_NON_TROUVE;
}

void inscrireEtudiant(Etudiant* tableEtudiants, int nombreEtudiantsInscrits, const char* nomEtudiant, int numeroGroupe) {
    if (nomEtudiant == NULL || strlen(nomEtudiant) == 0) {
        fprintf(stderr, "Erreur: Nom de l'etudiant invalide.\n");
        return;
    }

    assert(numeroGroupe > 0);
    assert(nombreEtudiantsInscrits >= 0 && nombreEtudiantsInscrits < MAX_ETUDIANTS);
    assert(tableEtudiants != NULL);
    strncpy(tableEtudiants[nombreEtudiantsInscrits].nomEtudiant, nomEtudiant, MAX_NOM - 1);
    tableEtudiants[nombreEtudiantsInscrits].nomEtudiant[MAX_NOM - 1] = '\0'; // Garantir null-terminated string
    tableEtudiants[nombreEtudiantsInscrits].numeroGroupe = numeroGroupe;
}

AbsenceStatus verifierAbsence(int etudiantId, int jour, const char* periode, const Etudiant* tableEtudiants) {
    if (etudiantId < 1 || etudiantId > MAX_ETUDIANTS) {
        return IDENTIFIANT_INVALIDE;
    }
    if (tableEtudiants[etudiantId - 1].nomEtudiant[0] == '\0') {
        return IDENTIFIANT_INVALIDE;
    }
    if (jour < 1 || jour > 40) {
        return DATE_INVALIDE;
    }
    if (strcmp(periode, "am") != 0 && strcmp(periode, "pm") != 0) {
        return DEMI_JOURNEE_INVALIDE;
    }

    int nombreAbsences = tableEtudiants[etudiantId - 1].nombreAbsences;
    if (nombreAbsences >= MAX_ABSENCES) {
        return DEPASSEMENT_DE_FORMAT;
    }

    for (int i = 0; i < nombreAbsences; i++) {
        Absence absence = tableEtudiants[etudiantId - 1].absences[i];
        if (absence.jourConcernant == jour && ((strcmp(periode, "am") == 0 && absence.demiJourneeConcernant == AM) ||
            (strcmp(periode, "pm") == 0 && absence.demiJourneeConcernant == PM))) {
            return ABSENCE_DEJA_CONNUE;
        }
    }
    return ABSENCE_NON_CONNUE;
}

void enregistrerAbsence(Etudiant* etudiants, int etudiantId, int jour, const char* periode) {
    int absenceNumero = etudiants[etudiantId - 1].nombreAbsences;
    if (absenceNumero >= MAX_ABSENCES) {
        fprintf(stderr, "Erreur: Nombre maximum d'absences atteint.\n");
        return;
    }
    etudiants[etudiantId - 1].absences[absenceNumero].jourConcernant = jour;
    etudiants[etudiantId - 1].absences[absenceNumero].demiJourneeConcernant = (strcmp(periode, "am") == 0) ? AM : PM;
    etudiants[etudiantId - 1].nombreAbsences++;
}*/
