
#include "task2.h"
void actualizare_medii(secretariat* s) {
    for (int i = 0; i < s->nr_studenti; i++) {
        float sum = 0, count = 0;
        for (int j = 0; j < s->nr_inrolari; j++) {
            if (s->inrolari[j].id_student == i
                && s->inrolari[j].id_materie != -1)
                sum += lil_sum(s, j), count++;
        }
        sum *= round_coeficient;
        sum = roundf(sum);
        sum /= round_coeficient;
        // int copy = (int) (sum * 1000.0001f);
        float x = sum / count;
        x *= round_coeficient;
        x = roundf(x);
        x /= round_coeficient;
        s->studenti[i].medie_generala = x;
    }
}
typedef struct filtrare {
    int camp, operator;
    char valoare[MAX_LENGTH];
} filtrare;
void afisare_simpla(
    secretariat* s, char campuri[NUM_FIELDS][MAX_LENGTH], int nr_campuri,
    char* tabela
) {
    if (strcmp(tabela, "studenti") == 0) {
        for (int i = 0; i < s->nr_studenti; i++)
            if (s->studenti[i].id != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    if (strcmp(campuri[j], "id") == 0)
                        printf("%d", s->studenti[i].id);
                    else if (strcmp(campuri[j], "nume") == 0)
                        printf("%s", s->studenti[i].nume);
                    else if (strcmp(campuri[j], "an_studiu") == 0)
                        printf("%d", s->studenti[i].an_studiu);
                    else if (strcmp(campuri[j], "statut") == 0)
                        printf("%c", s->studenti[i].statut);
                    else if (strcmp(campuri[j], "medie_generala") == 0)
                        printf("%.2f", s->studenti[i].medie_generala);
                    else if (strcmp(campuri[j], "*") == 0)
                        printf(
                            "%d %s %d %c %.2f", s->studenti[i].id,
                            s->studenti[i].nume, s->studenti[i].an_studiu,
                            s->studenti[i].statut, s->studenti[i].medie_generala);
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
    } else if (strcmp(tabela, "materii") == 0) {
        for (int i = 0; i < s->nr_materii; i++)
            if (s->materii[i].id != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    if (strcmp(campuri[j], "id") == 0)
                        printf("%d", s->materii[i].id);
                    else if (strcmp(campuri[j], "nume") == 0)
                        printf("%s", s->materii[i].nume);
                    else if (strcmp(campuri[j], "nume_titular") == 0)
                        printf("%s", s->materii[i].nume_titular);
                    else if (strcmp(campuri[j], "*") == 0)
                        printf(
                            "%d %s %s", s->materii[i].id, s->materii[i].nume,
                            s->materii[i].nume_titular);
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
    } else if (strcmp(tabela, "inrolari") == 0) {
        for (int i = 0; i < s->nr_inrolari; i++)
            if (s->inrolari[i].id_materie != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    if (strcmp(campuri[j], "id_student") == 0) {
                        printf("%d", s->inrolari[i].id_student);
                    } else if (strcmp(campuri[j], "id_materie") == 0) {
                        printf("%d", s->inrolari[i].id_materie);
                    } else if (strcmp(campuri[j], "note") == 0) {
                        for (int k = 0; k < 3; k++) {
                            printf("%.2f", s->inrolari[i].note[k]);
                            if (k != 2)
                                printf(" ");
                        }
                    } else if (strcmp(campuri[j], "*") == 0) {
                        printf(
                            "%d %d %.2f %.2f %.2f", s->inrolari[i].id_student,
                            s->inrolari[i].id_materie, s->inrolari[i].note[0],
                            s->inrolari[i].note[1], s->inrolari[i].note[2]);
                    }
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
    }
}
int codificare_operatori(char* operator) {
    if (strcmp(operator, "<") == 0)
        return less;
    else if (strcmp(operator, "<=") == 0)
        return less_or_eq;
    else if (strcmp(operator, "=") == 0)
        return eq;
    else if (strcmp(operator, "!=") == 0)
        return 4;
    else if (strcmp(operator, ">") == 0)
        return greater;
    else if (strcmp(operator, ">=") == 0)
        return greater_or_eq;
    return 0;
}
int codificare_camp(char camp[MAX_LENGTH], char* tabela) {
    if (strcmp(tabela, "studenti") == 0) {
        if (strcmp(camp, "id") == 0)
            return id_stud;
        else if (strcmp(camp, "nume") == 0)
            return nume_code;
        else if (strcmp(camp, "an_studiu") == 0)
            return an_studiu_code;
        else if (strcmp(camp, "statut") == 0)
            return statut_code;
        else if (strcmp(camp, "medie_generala") == 0)
            return medie;
        else if (strcmp(camp, "*") == 0)
            return st_stud;

    } else if (strcmp(tabela, "materii") == 0) {
        if (strcmp(camp, "id") == 0)
            return id_mat;
        else if (strcmp(camp, "nume") == 0)
            return nume_mat;
        else if (strcmp(camp, "nume_titular") == 0)
            return nume_tit;
        else if (strcmp(camp, "*") == 0)
            return st_mat;
    } else if (strcmp(tabela, "inrolari") == 0) {
        if (strcmp(camp, "id_student") == 0)
            return id_stud_inr;
        else if (strcmp(camp, "id_materie") == 0)
            return id_mat_inr;
        else if (strcmp(camp, "note") == 0)
            return note_inr;
        else if (strcmp(camp, "*") == 0)
            return st_inr;
    }
    return 0;
}
int conditie(secretariat* s, filtrare f, char* tabela, int i) {
    tabela[strlen(tabela)] = 0;
    switch (f.camp) {
    case id_stud:
        switch (f.operator) {
        case less:
            if (s->studenti[i].id < atoi(f.valoare))
                return 1;
            return 0;
            break;
        case less_or_eq:
            if (s->studenti[i].id <= atoi(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            if (s->studenti[i].id == atoi(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            if (s->studenti[i].id != atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            if (s->studenti[i].id > atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            if (s->studenti[i].id >= atoi(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case an_studiu_code:
        switch (f.operator) {
        case less:
            if (s->studenti[i].an_studiu < atoi(f.valoare))
                return 1;
            return 0;
            break;
        case less_or_eq:
            if (s->studenti[i].an_studiu <= atoi(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            if (s->studenti[i].an_studiu == atoi(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            if (s->studenti[i].an_studiu != atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            if (s->studenti[i].an_studiu > atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            if (s->studenti[i].an_studiu >= atoi(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case statut_code:
        switch (f.operator) {
        case less:
            if (s->studenti[i].statut < f.valoare[0])
                return 1;
            return 0;
            break;
        case less_or_eq:
            if (s->studenti[i].statut <= f.valoare[0])
                return 1;
            return 0;
            break;
        case eq:
            if (s->studenti[i].statut == f.valoare[0])
                return 1;
            return 0;
            break;
        case not_eq:
            if (s->studenti[i].statut != f.valoare[0])
                return 1;
            return 0;
            break;
        case greater:
            if (s->studenti[i].statut > f.valoare[0])
                return 1;
            return 0;
            break;
        case greater_or_eq:
            if (s->studenti[i].statut >= f.valoare[0])
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case medie:
        switch (f.operator) {
        case less:
            if (s->studenti[i].medie_generala < atof(f.valoare))
                return 1;
            return 0;
            break;
        case less_or_eq:
            if (s->studenti[i].medie_generala <= atof(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            if (s->studenti[i].medie_generala == atof(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            if (s->studenti[i].medie_generala != atof(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            /* code */
            if (s->studenti[i].medie_generala > atof(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            /* code */
            if (s->studenti[i].medie_generala >= atof(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case id_mat:
        switch (f.operator) {
        case less:
            if (s->materii[i].id < atoi(f.valoare))
                return 1;
            return 0;
            /* code */
            break;
        case less_or_eq:
            /* code */
            if (s->materii[i].id <= atoi(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            /* code */
            if (s->materii[i].id == atoi(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            /* code */
            if (s->materii[i].id != atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            /* code */
            if (s->materii[i].id > atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            /* code */
            if (s->materii[i].id >= atoi(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case id_stud_inr:
        switch (f.operator) {
        case less:
            if (s->inrolari[i].id_student < atoi(f.valoare))
                return 1;
            return 0;
            /* code */
            break;
        case less_or_eq:
            /* code */
            if (s->inrolari[i].id_student <= atoi(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            /* code */
            if (s->inrolari[i].id_student == atoi(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            /* code */
            if (s->inrolari[i].id_student != atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            /* code */
            if (s->inrolari[i].id_student > atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            /* code */
            if (s->inrolari[i].id_student >= atoi(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    case id_mat_inr:
        switch (f.operator) {
        case less:
            if (s->inrolari[i].id_materie < atoi(f.valoare))
                return 1;
            return 0;
            /* code */
            break;
        case less_or_eq:
            /* code */
            if (s->inrolari[i].id_materie <= atoi(f.valoare))
                return 1;
            return 0;
            break;
        case eq:
            /* code */
            if (s->inrolari[i].id_materie == atoi(f.valoare))
                return 1;
            return 0;
            break;
        case not_eq:
            /* code */
            if (s->inrolari[i].id_materie != atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater:
            /* code */
            if (s->inrolari[i].id_materie > atoi(f.valoare))
                return 1;
            return 0;
            break;
        case greater_or_eq:
            /* code */
            if (s->inrolari[i].id_materie >= atoi(f.valoare))
                return 1;
            return 0;
            break;
        default:
            break;
        }
        break;
    default:
        return 1;
        break;
    }
    return 1;
}
// cate un id pt fiecare camp ca sa l folosesc ptr functia conditon
void update(
    secretariat* s, char* camp, char* valoare, filtrare f[2], char* tabel
) {
    int coded_field = codificare_camp(camp, tabel);
    if (strcmp(tabel, "studenti") == 0) {
        for (int i = 0; i < s->nr_studenti; i++)
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i)
                && s->studenti[i].id != -1) {
                switch (coded_field) {
                case id_stud:
                    s->studenti[i].id = atoi(valoare);
                    break;
                case nume_code:
                    valoare[strlen(valoare) - 1] = 0;
                    snprintf(
                        s->studenti[i].nume, strlen(valoare + 1) + 1, "%s",
                        valoare + 1);
                    // strcpy(s->studenti[i].nume, valoare + 1);
                    break;
                case an_studiu_code:
                    s->studenti[i].an_studiu = atoi(valoare);
                    break;
                case statut_code:
                    s->studenti[i].statut = valoare[0];
                    break;
                case medie:
                    s->studenti[i].medie_generala = (float) atof(valoare);
                    break;
                default:
                    break;
                }
            }
    } else if (strcmp(tabel, "materii") == 0) {
        for (int i = 0; i < s->nr_materii; i++)
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i)
                && s->materii[i].id != -1) {
                switch (coded_field) {
                case id_mat:
                    s->materii[i].id = atoi(valoare);
                    break;
                case nume_mat:
                    valoare[strlen(valoare) - 1] = 0;
                    snprintf(
                        s->materii[i].nume, strlen(valoare + 1) + 1, "%s",
                        valoare + 1);
                    // strcpy(s->materii[i].nume, valoare + 1);
                    break;
                case nume_tit:
                    valoare[strlen(valoare) - 1] = 0;
                    snprintf(
                        s->materii[i].nume_titular, strlen(valoare + 1) + 1,
                        "%s", valoare + 1);
                    // strcpy(s->materii[i].nume_titular, valoare + 1);
                    break;
                default:
                    break;
                }
            }
    } else if (strcmp(tabel, "inrolari") == 0) {
        char* p = NULL;
        for (int i = 0; i < s->nr_inrolari; i++) {
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i)
                && s->inrolari[i].id_materie != -1) {
                switch (coded_field) {
                case id_stud_inr:
                    s->inrolari[i].id_student = atoi(valoare);
                    break;
                case id_mat_inr:
                    s->inrolari[i].id_materie = atoi(valoare);
                    break;
                case note_inr:
                    p = strtok(valoare, " ");
                    // printf("%s ", p);
                    for (int j = 0; j < 3; j++) {
                        s->inrolari[i].note[j] = (float) atof(p);
                        p = strtok(NULL, " ");
                    }
                    break;
                default:
                    break;
                }
            }
            // printf("merge update");
        }
        actualizare_medii(s);
        // 11 12 13
    }
}
void afisare_filtrare(
    secretariat* s, int coded_fields[NUM_FIELDS], int nr_campuri, char* tabela,
    filtrare f[2]
) {
    if (strcmp(tabela, "studenti") == 0) {
        for (int i = 0; i < s->nr_studenti; i++) {
            if (conditie(s, f[0], tabela, i) && conditie(s, f[1], tabela, i)
                && s->studenti[i].id != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    switch (coded_fields[j]) {
                    case id_stud:
                        printf("%d", s->studenti[i].id);
                        break;
                    case nume_code:
                        printf("%s", s->studenti[i].nume);
                        break;
                    case an_studiu_code:
                        printf("%d", s->studenti[i].an_studiu);
                        break;
                    case statut_code:
                        printf("%c", s->studenti[i].statut);
                        break;
                    case medie:
                        printf("%.2f", s->studenti[i].medie_generala);
                        break;
                    case st_stud:
                        printf(
                            "%d %s %d %c %.2f", s->studenti[i].id,
                            s->studenti[i].nume, s->studenti[i].an_studiu,
                            s->studenti[i].statut, s->studenti[i].medie_generala);
                        break;
                    default:
                        break;
                    }
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
        }
    } else if (strcmp(tabela, "materii") == 0) {
        for (int i = 0; i < s->nr_materii; i++)
            if (conditie(s, f[0], tabela, i) && conditie(s, f[1], tabela, i)
                && s->materii[i].id != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    switch (coded_fields[j]) {
                    case id_mat:
                        printf("%d", s->materii[i].id);
                        break;
                    case nume_mat:
                        printf("%s", s->materii[i].nume);
                        break;
                    case nume_tit:
                        printf("%s", s->materii[i].nume_titular);
                        break;
                    case st_mat:
                        printf(
                            "%d %s %s", s->materii[i].id, s->materii[i].nume,
                            s->materii[i].nume_titular);
                        break;
                    default:
                        break;
                    }
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
    } else if (strcmp(tabela, "inrolari") == 0) {
        for (int i = 0; i < s->nr_inrolari; i++)
            if (conditie(s, f[0], tabela, i) && conditie(s, f[1], tabela, i)
                && s->inrolari[i].id_materie != -1) {
                for (int j = 0; j < nr_campuri; j++) {
                    switch (coded_fields[j]) {
                    case id_stud_inr:
                        printf("%d", s->inrolari[i].id_student);
                        break;
                    case id_mat_inr:
                        printf("%d", s->inrolari[i].id_materie);
                        break;
                    case note_inr:
                        for (int k = 0; k < 3; k++) {
                            printf("%.2f", s->inrolari[i].note[k]);
                            if (k != 2)
                                printf(" ");
                        }
                        break;
                    case st_inr:
                        printf(
                            "%d %d %.2f %.2f %.2f", s->inrolari[i].id_student,
                            s->inrolari[i].id_materie, s->inrolari[i].note[0],
                            s->inrolari[i].note[1], s->inrolari[i].note[2]);
                        break;
                    default:
                        break;
                    }
                    if (j < nr_campuri - 1)
                        printf(" ");
                }
                printf("\n");
            }
    }
}
void delete(secretariat* s, char* tabel, filtrare f[2]) {
    if (strcmp(tabel, "studenti") == 0) {
        for (int i = 0; i < s->nr_studenti; i++)
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i))
                s->studenti[i].id = -1;
    } else if (strcmp(tabel, "materii") == 0) {
        for (int i = 0; i < s->nr_materii; i++)
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i))
                s->materii[i].id = -1;
    } else if (strcmp(tabel, "inrolari") == 0) {
        for (int i = 0; i < s->nr_inrolari; i++)
            if (conditie(s, f[0], tabel, i) && conditie(s, f[1], tabel, i))
                s->inrolari[i].id_materie = -1;
        actualizare_medii(s);
    }
}
int main(int argc, char* argv[]) {
    // TODO(student): Task 2
    int arguments_count = argc;
    secretariat* s = citeste_secretariat(argv[1]);
    actualizare_medii(s);
    int inter_num = -1;
    char buffer[MAX_LENGTH], campuri[NUM_FIELDS][MAX_LENGTH];
    scanf("%d", &inter_num);
    for (int n = 0; n < inter_num; n++) {
        for (int i = 0; i < NUM_FIELDS; i++)
            campuri[i][0] = 0;
        scanf("%s", buffer);
        if (strcmp(buffer, "SELECT") == 0) {
            char tabel[MAX_LENGTH];
            int nr_campuri = 0;
            scanf("%s", buffer);
            while (strcmp(buffer, "FROM")) {
                if (buffer[strlen(buffer) - 1] == ',')
                    buffer[strlen(buffer) - 1] = 0;
                snprintf(campuri[nr_campuri], strlen(buffer) + 1, "%s", buffer);
                nr_campuri++;
                scanf("%s", buffer);
            }
            scanf("%s", tabel);
            if (tabel[strlen(tabel) - 1] == ';') {
                tabel[strlen(tabel) - 1] = 0;
                afisare_simpla(s, campuri, nr_campuri, tabel);
            } else {
                int i = 1;
                int coded_fields[NUM_FIELDS] = { 0 };
                for (int j = 0; j < nr_campuri; j++)
                    coded_fields[j] = codificare_camp(campuri[j], tabel);
                filtrare filtrare[2];
                for (int i = 0; i < 2; i++)
                    filtrare[i].camp = 0, filtrare[i].valoare[0] = 0,
                    filtrare[i].operator= 0;
                scanf("%s", buffer);
                scanf("%s", buffer);
                filtrare[0].camp = codificare_camp(buffer, tabel);
                scanf("%s", buffer);
                filtrare[0].operator= codificare_operatori(buffer);
                scanf("%s", buffer);
                if (buffer[strlen(buffer) - 1] == ';') {
                    buffer[strlen(buffer) - 1] = 0,
                                            snprintf(
                                                filtrare[0].valoare,
                                                strlen(buffer) + 1, "%s", buffer);
                } else {
                    i++;
                    snprintf(
                        filtrare[0].valoare, strlen(buffer) + 1, "%s", buffer);
                    scanf("%s", buffer);
                    scanf("%s", buffer);
                    filtrare[1].camp = codificare_camp(buffer, tabel);
                    scanf("%s", buffer);
                    filtrare[1].operator= codificare_operatori(buffer);
                    scanf("%s", buffer);
                    buffer[strlen(buffer) - 1] = 0;
                    snprintf(
                        filtrare[1].valoare, strlen(buffer) + 1, "%s", buffer);
                }
                afisare_filtrare(s, coded_fields, nr_campuri, tabel, filtrare);
            }
        } else if (strcmp(buffer, "UPDATE") == 0) {
            char tabel1[MAX_LENGTH];
            scanf("%s", tabel1);
            scanf("%s", buffer);
            char camp[MAX_LENGTH] = "", valoare[MAX_LENGTH] = "";
            scanf("%s", camp);
            scanf("%s", buffer);
            scanf("%s", valoare);
            int size = (int) strlen(valoare);
            if (strcmp(camp, "note") == 0) {
                scanf("%s", buffer);
                char aux[MAX_LENGTH];
                scanf("%s", aux);
                snprintf(
                    valoare + size, strlen(buffer) + strlen(aux) + 2, " %s %s",
                    buffer, aux);
            }
            if (strcmp(camp, "nume") == 0) {
                scanf("%s", buffer);
                snprintf(valoare + size, strlen(buffer) + 2, " %s", buffer);
            }
            scanf("%s", buffer);
            filtrare f1[2];
            for (int j = 0; j < 2; j++)
                f1[j].camp = 0, f1[j].operator= 0, f1[j].valoare[0] = 0;
            scanf("%s", buffer);
            f1[0].camp = codificare_camp(buffer, tabel1);
            scanf("%s", buffer);
            f1[0].operator= codificare_operatori(buffer);
            scanf("%s", buffer);
            if (buffer[strlen(buffer) - 1] == ';') {
                buffer[strlen(buffer) - 1] = 0;
                snprintf(f1[0].valoare, strlen(buffer) + 1, "%s", buffer);
            } else {
                snprintf(f1[0].valoare, strlen(buffer) + 1, "%s", buffer);
                scanf("%s", buffer);
                scanf("%s", buffer);
                f1[1].camp = codificare_camp(buffer, tabel1);
                scanf("%s", buffer);
                f1[1].operator= codificare_operatori(buffer);
                scanf("%s", buffer);
                buffer[strlen(buffer) - 1] = 0;
                snprintf(f1[1].valoare, strlen(buffer) + 1, "%s", buffer);
            }
            update(s, camp, valoare, f1, tabel1);
            // printf("ceva");
        } else if (strcmp(buffer, "DELETE") == 0) {
            scanf("%s", buffer);
            char tabel[MAX_LENGTH] = "";
            scanf("%s", tabel);
            scanf("%s", buffer);
            scanf("%s", buffer);
            // printf("%s ", buffer);
            filtrare f2[2];
            for (int j = 0; j < 2; j++)
                f2[j].camp = 0, f2[j].operator= 0, f2[j].valoare[0] = 0;
            f2[0].camp = codificare_camp(buffer, tabel);
            scanf("%s", buffer);
            f2[0].operator= codificare_operatori(buffer);
            scanf("%s", f2[0].valoare);
            if (f2[0].valoare[strlen(f2[0].valoare) - 1] == ';') {
                f2[0].valoare[strlen(f2[0].valoare) - 1] = 0;
            } else {
                scanf("%s", buffer);
                scanf("%s", buffer);
                // printf("%s ", buffer);
                f2[1].camp = codificare_camp(buffer, tabel);
                scanf("%s", buffer);
                f2[1].operator= codificare_operatori(buffer);
                scanf("%s", f2[1].valoare);
                f2[1].valoare[strlen(f2[1].valoare) - 1] = 0;
            }
            delete (s, tabel, f2);
        }
    }
    elibereaza_secretariat(&s);

    return 0;
}
