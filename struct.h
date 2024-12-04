#ifndef STRUCT_H
#define STRUCT_H

typedef struct position {
    double x;
    double y;
} position;

typedef struct Restaurant {
    char nom[100];
    char adresse[200];
    position Coordonnees;
    char specialite[50];
} Restaurant;

#endif // STRUCT_H

