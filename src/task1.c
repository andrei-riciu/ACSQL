#include "task1.h"

float lil_sum(secretariat* s, int i) {
    return s->inrolari[i].note[0] + s->inrolari[i].note[1]
           + s->inrolari[i].note[2];
}
secretariat* citeste_secretariat(const char* nume_fisier) {
    // TODO(student): 1.1
    FILE* in = fopen(nume_fisier, "rt");

    if (in == NULL) {
        printf("can't open file");
        return NULL;
    }
    secretariat* s = malloc(sizeof(secretariat));
    if (!s)
        return NULL;
    char buffer[MAX_LENGTH];
    fgets(buffer, MAX_LENGTH, in);
    int x = 0;
    s->nr_studenti = -1, s->nr_materii = -1;
    while (strcmp(buffer, "[MATERII]\n")) {
        fgets(buffer, MAX_LENGTH, in);
        s->nr_studenti++;
    }
    s->studenti = malloc(s->nr_studenti * sizeof(student));
    while (strcmp(buffer, "[INROLARI]\n")) {
        fgets(buffer, MAX_LENGTH, in);
        s->nr_materii++;
    }
    s->materii = malloc(s->nr_materii * sizeof(materie));
    while (feof(in) == 0) {
        fgets(buffer, MAX_LENGTH, in);
        x++;
    }
    s->nr_inrolari = x;
    if (s->nr_inrolari > big_db)
        s->nr_inrolari--;
    s->inrolari = malloc(s->nr_inrolari * sizeof(inrolare));
    fclose(in);
    in = fopen(nume_fisier, "rt");
    fgets(buffer, MAX_LENGTH, in);
    for (int i = 0; i < s->nr_studenti; i++) {
        fgets(buffer, MAX_LENGTH, in);
        char* p = strtok(buffer, ",");
        s->studenti[i].id = atoi(p);
        p = strtok(NULL, ",");
        snprintf(s->studenti[i].nume, strlen(p + 1) + 1, "%s", p + 1);
        // strcpy(s->studenti[i].nume, p + 1);
        p = strtok(NULL, ",");
        s->studenti[i].an_studiu = p[1] - '0';
        p = strtok(NULL, ",");
        s->studenti[i].statut = p[1];
    }
    fgets(buffer, MAX_LENGTH, in);
    for (int i = 0; i < s->nr_materii; i++) {
        fgets(buffer, MAX_LENGTH, in);
        char* p = strtok(buffer, ",");
        s->materii[i].id = i;
        p = strtok(NULL, ",");
        s->materii[i].nume = malloc((strlen(p + 1) + 1) * sizeof(char));
        snprintf(s->materii[i].nume, strlen(p + 1) + 1, "%s", p + 1);
        // strcpy(s->materii[i].nume, p + 1);
        p = strtok(NULL, ",");
        p[strlen(p) - 1] = 0;
        s->materii[i].nume_titular = malloc((strlen(p + 1) + 1) * sizeof(char));
        snprintf(s->materii[i].nume_titular, strlen(p + 1) + 1, "%s", p + 1);
        // strcpy(s->materii[i].nume_titular, p + 1);
    }
    fgets(buffer, MAX_LENGTH, in);
    for (int i = 0; i < s->nr_inrolari; i++) {
        fgets(buffer, MAX_LENGTH, in);
        char* p = strtok(buffer, ",");
        s->inrolari[i].id_student = atoi(p);
        p = strtok(NULL, ",");
        s->inrolari[i].id_materie = atoi(p);
        p = strtok(NULL, " ");
        s->inrolari[i].note[0] = (float) atof(p);
        p = strtok(NULL, " ");
        s->inrolari[i].note[1] = (float) atof(p);
        p = strtok(NULL, " ");
        s->inrolari[i].note[2] = (float) atof(p);
    }
    for (int i = 0; i < s->nr_studenti; i++) {
        float sum = 0, count = 0;
        for (int j = 0; j < s->nr_inrolari; j++) {
            if (s->inrolari[j].id_student == i)
                sum += lil_sum(s, j), count++;
        }
        // int copy = (int) (sum * round_coeficient);
        float x = sum / count;
        s->studenti[i].medie_generala = x;
    }
    fclose(in);
    return s;
}

void adauga_student(
    secretariat* s, int id, char* nume, int an_studiu, char statut,
    float medie_generala
) {
    // TODO(student): 1.2
    s->nr_studenti++;
    student* aux = realloc(s->studenti, (s->nr_studenti) * sizeof(student));
    if (aux)
        s->studenti = aux;
    else
        printf("alocare esuata");
    s->studenti[s->nr_studenti - 1].an_studiu = an_studiu;
    s->studenti[s->nr_studenti - 1].id = id;
    s->studenti[s->nr_studenti - 1].statut = statut;
    s->studenti[s->nr_studenti - 1].medie_generala = medie_generala;
    snprintf(
        s->studenti[s->nr_studenti - 1].nume, strlen(nume) + 1, "%s", nume);
    // strcpy(s->studenti[s->nr_studenti - 1].nume, nume);
}

void elibereaza_secretariat(secretariat** s) {
    // TODO(student): 1.3
    for (int i = 0; i < (*s)->nr_materii; i++)
        free((*s)->materii[i].nume), free((*s)->materii[i].nume_titular);
    free((*s)->materii);
    free((*s)->studenti);
    free((*s)->inrolari);
    free(*s);
    *s = NULL;
}
