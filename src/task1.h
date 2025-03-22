#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structuri.h"
#define MAX_LENGTH 200
#define big_db 100
#define round_coeficient 100.0f
float lil_sum(secretariat* s, int i);
secretariat* citeste_secretariat(const char* nume_fisier);

void adauga_student(
    secretariat* s, int id, char* nume, int an_studiu, char statut,
    float medie_generala
);

void elibereaza_secretariat(secretariat** s);
