/*
 * File:   ports-io-7seg.c
 * Author: jmgonet
 *
 * Created on 19. novembre 2012, 08:21
 */

#include <htc.h>
#include <stdio.h>

/**
 * Bits de configuration:
 */
#pragma config FOSC = INTIO67  // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF      // Pas d'osc. au d�marrage.
#pragma config FCMEN = OFF     // Pas de monitorage de l'oscillateur.

// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 est actif comme master reset.
#pragma config WDTEN = OFF     // Watchdog inactif.
#pragma config LVP = OFF       // Single Supply Enable bits off.

/**
 * Table de conversion pour afficheur LED 7 segments.
 */
unsigned char AFFICHAGE_led[] =
{
    0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7c,
    0x39, 0x5e, 0x79, 0x71
};

/**
 * Contenu à afficher sur l'afficheur 7 segments multiple.
 */
unsigned char AFFICHAGE_contenu[7];

/**
 * Prochain digit à activer.
 */
unsigned char AFFICHAGE_digit = 0;

/**
 * À chaque appel, affiche un digit sur l'afficheur
 * multiple. Cette fonction est à appeler successivement
 * pour obtenir l'affichage de tous les digits.
 */
void AFFICHAGE_raffraichir()
{
    char a,c;

    c = 1 << AFFICHAGE_digit;
    a = AFFICHAGE_contenu[AFFICHAGE_digit] - '0';
    a = ~AFFICHAGE_led[a];

    PORTC = 0;
    PORTA = a;
    PORTC = c;

    if(AFFICHAGE_digit++>5)
    {
        AFFICHAGE_digit=0;
    }
}

/**
 * Point d'entrée.
 * Configure les ports A et C comme sorties, puis
 * appelle l'affichage du chiffre "123456".
 */
void main()
{
    int n;

    // Configuration du micro-contrôleur:
    ANSELA = 0x00;      // Désactive les convertisseurs A/D.
    ANSELB = 0x00;      // Désactive les convertisseurs A/D.
    ANSELC = 0x00;      // Désactive les convertisseurs A/D.
    TRISA = 0x00;       // Tous les bits du port A comme sorties.
    TRISC = 0x00;       // Tous les bits du port C comme sorties.

    // Prépare un message à afficher:
    sprintf(AFFICHAGE_contenu,"123456");

    // Boucle principale, appelle le rafraîchissement
    // de l'affichage.
    while(1)
    {
        // Affiche un digit:
        AFFICHAGE_raffraichir();

        // Boucle d'attente:
        for(n=0; n<120; n++);
    }
}
