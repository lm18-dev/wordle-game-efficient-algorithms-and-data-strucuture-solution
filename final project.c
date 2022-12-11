#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define LUNGHEZZA_PAROLE 64


struct NodoTree{

    char * Stringa;

    struct NodoTree * PuntL;
    struct NodoTree * PuntR;



};


struct NodoLista{


    char * Stringa;
    struct NodoLista * Prox;



};


struct Vincoli{

    int ArrayNonCompare[64];
    int ArrayCompareInPosizineDiversa[64][LUNGHEZZA_PAROLE] ;
    int ArrayComparePoszioneCorretta[64][LUNGHEZZA_PAROLE] ;
    int ArrayCompareAlmenokVolte[64] ;
    int ArrayComparekVolte[64] ;
    int ArrayEsatte[LUNGHEZZA_PAROLE];

};







void InserisciNuoveInFiltrate();

void CostruisciArrayLetterePossibili(int * Array, char * Str, int k);

int FunzioneHashAlfabeto(char c);

void ConfrontaStringhe(char *str1, char *str2, int i, int k, int *ArrLettPoss, int * OutputFrase, int * ArrayLettereContate, struct NodoLista * Nodo);

void StampaOutput(int * Output , int  k);

void CorreggiOutputConfrontaStringhe(char *str1, char *str2, int i, int k, int *ArrLettPoss,int *ArrLettereContate, int *OutputFrase, int n);

int InserisciNuovaParola( char * str, struct NodoTree ** Nodo );

int InserisciNuovaParolaConVincoli( char * str, char * ParolaCorretta, int LunghezzaParole);

void StampaFiltrate(struct NodoLista * Nodo);

void CopiaAmmissibiliInFiltrate(struct NodoTree * Nodo, char * ParolaCorretta, int LunghezzaParole, int * ArrayContaLettere);

struct NodoTree * VerificaParolePossibili(char * str, struct NodoTree * Nodo);

void CancellaNodoFilrate(struct NodoLista ** PuntCurr,struct NodoLista ** PuntPrec );

void StampaFiltrateAlbero(struct NodoTree *Nodo);

void FiltraParole( int LunghezzaParole, char * ParolaCorretta);

void SvuotaFiltrate();

void InserisciNuovaParolaConVincoliInFondo( char * str);

int InserisciNuovaParolaConVincoliCopia( char * str, char * ParolaCorretta, int LunghezzaParola, int * ArraContaLettere);

void SvuotaAlbero();

int verificaVincoli(  char * str, char * ParolaCorretta, int LunghezzaParola,int * arrayContaLettere);






//VARIABILI GLOBALI

int NumFiltrate;
struct Vincoli StrutturaVincoli;
struct NodoTree * Ammissibili;
struct NodoLista * Filtrate;
struct NodoLista * ListaInserimento;
struct NodoLista * UltimaFiltrate;
int ArrayContaLettereCopia[64] = {0};
int LunghezzaParoleGlobale;


//FINE VARIABILI GLOBALI







struct NodoTree *  VerificaParolePossibili(char * str, struct NodoTree * Nodo){

    if(Nodo == NULL   || (  Nodo->Stringa != NULL &&  strcmp(str, Nodo->Stringa) == 0) ){

        return Nodo;
    }

    if( Nodo->Stringa != NULL &&  strcmp(str, Nodo->Stringa) < 0){

        return VerificaParolePossibili( str, Nodo->PuntL);

    }else if( Nodo->Stringa != NULL &&  strcmp(str, Nodo->Stringa) > 0){

        return VerificaParolePossibili( str, Nodo->PuntR);

    }

    return Nodo;
}








void CopiaAmmissibiliInFiltrate(struct NodoTree * Nodo, char * ParolaCorretta, int LunghezzaParole, int * ArrayContaLettere){


    if(Nodo != NULL) {

        CopiaAmmissibiliInFiltrate(Nodo->PuntR, ParolaCorretta, LunghezzaParole, ArrayContaLettere);

        if (Nodo->Stringa != NULL) {

            InserisciNuovaParolaConVincoliCopia(Nodo->Stringa, ParolaCorretta, LunghezzaParole, ArrayContaLettere);

            for(int i = 0; i < LunghezzaParole; i++) {

                int n = FunzioneHashAlfabeto(*((Nodo->Stringa)+i));
                ArrayContaLettere[n]--;

            }

        }

        CopiaAmmissibiliInFiltrate(Nodo->PuntL, ParolaCorretta, LunghezzaParole, ArrayContaLettereCopia);

    }

    return;
}







void SvuotaFiltrate(){



    struct NodoLista * NodoPunt;
    struct NodoLista * NodoPrec;

    NodoPunt = Filtrate;

    NodoPrec = NULL;

    while(NodoPunt != NULL ){

        NodoPrec = NodoPunt;
        NodoPunt = NodoPunt->Prox;
        free(NodoPrec);

    }

    if( NodoPrec == NULL && NodoPunt != NULL){

        free(NodoPunt);

    }
    Filtrate = NULL;
    NumFiltrate = 0;

}









void SvuotaAlbero(){

    struct NodoTree * Nodo;

    Nodo = Ammissibili;

    if (Nodo != NULL) {

        free(Nodo->Stringa);
        free(Nodo);


    }

    Ammissibili = NULL;

    return;

}






void StampaFiltrateAlbero(struct NodoTree *Nodo) {


    if (Nodo != NULL) {

        StampaFiltrateAlbero(Nodo->PuntL);

        if (Nodo->Stringa != NULL) {

            printf("%s\n", Nodo->Stringa);
        }

        StampaFiltrateAlbero(Nodo->PuntR);

    }

    return;

}







void StampaFiltrate(struct NodoLista * Nodo){

    while( Nodo != NULL){

        printf("%s\n", Nodo->Stringa);
        Nodo = Nodo->Prox;

    }

    return;

}






int InserisciNuovaParola( char * str, struct  NodoTree ** Nodo){

    struct NodoTree * pre;
    struct NodoTree * curr;

    pre = NULL;
    curr = *Nodo;

    while(curr != NULL ){

        pre = curr;

        if(strcmp( str, (*curr).Stringa) < 0){

            curr = curr->PuntL;

        }else if(strcmp( str, (*curr).Stringa) == 0){

            return 0;

        }else{

            curr = curr ->PuntR;

        }

    }

    if(pre == NULL){

        *Nodo = malloc(sizeof(struct  NodoTree));
        (*Nodo)->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
        strcpy((*Nodo)->Stringa, str);
        (*Nodo)->PuntL = NULL;
        (*Nodo)->PuntR = NULL;

    }else if( strcmp(str, pre->Stringa) < 0){

        struct NodoTree * NuovoNodo;

        NuovoNodo = malloc(sizeof(struct  NodoTree));
        NuovoNodo->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
        strcpy(NuovoNodo->Stringa, str);

        NuovoNodo->PuntL = NULL;
        NuovoNodo->PuntR = NULL;
        pre->PuntL = NuovoNodo;


    }else{

        struct NodoTree * NuovoNodo;

        NuovoNodo = malloc(sizeof(struct  NodoTree));
        NuovoNodo->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
        strcpy(NuovoNodo->Stringa, str);

        NuovoNodo->PuntL = NULL;
        NuovoNodo->PuntR = NULL;

        pre->PuntR = NuovoNodo;
    }


    return 1;
}







void InserisciNuovaParolaConVincoliInFondo( char * str){


    struct NodoLista * NodoTemp;



    if( Filtrate == NULL){

        Filtrate = malloc(sizeof(struct NodoLista));
        Filtrate->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
        strcpy(Filtrate->Stringa, str);
        Filtrate->Prox = NULL;
        return;

    }

    NodoTemp = malloc(sizeof(struct NodoLista));
    NodoTemp->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
    strcpy(NodoTemp->Stringa, str);
    NodoTemp->Prox = Filtrate;
    Filtrate = NodoTemp;

    return;

}








int verificaVincoli(  char * str, char * ParolaCorretta, int LunghezzaParola,int * arrayContaLettere){

    int w,n;
    for(w = 0; w < LunghezzaParola; w++){

        n = FunzioneHashAlfabeto(*(str+w));

        if (StrutturaVincoli.ArrayNonCompare[n] == 1){

            return 0;

        }


        if(StrutturaVincoli.ArrayCompareInPosizineDiversa[n][w] == 1){

            return 0;

        }

        if(StrutturaVincoli.ArrayEsatte[w] != -1 && StrutturaVincoli.ArrayEsatte[w] != n){

            return 0;

        }

        int k = FunzioneHashAlfabeto(*(ParolaCorretta+w));


        if(StrutturaVincoli.ArrayComparekVolte[k] != 0 && StrutturaVincoli.ArrayComparekVolte[k] != arrayContaLettere[k]){

            return 0;

        }

        if(StrutturaVincoli.ArrayCompareAlmenokVolte[k] > arrayContaLettere[k]){

            return 0;

        }

    }

    return 1;

}









int InserisciNuovaParolaConVincoliCopia( char * str, char * ParolaCorretta, int LunghezzaParola,int * arrayContaLettere){


    struct NodoLista * NodoTemp;

    int n;
    int i;

    for(i = 0; i < LunghezzaParola; i++) {

        n = FunzioneHashAlfabeto(*(str+i));
        arrayContaLettere[n]++;

    }


    if( verificaVincoli(str, ParolaCorretta, LunghezzaParola, arrayContaLettere) == 0){

        return 0;

    NumFiltrate++;


    if( Filtrate == NULL){

        Filtrate = malloc(sizeof(struct NodoLista));
        Filtrate->Stringa = str;
        Filtrate->Prox = NULL;
        return 1;

    }

    NodoTemp = malloc(sizeof(struct NodoLista));
    NodoTemp->Stringa = str;
    NodoTemp->Prox = Filtrate;
    Filtrate = NodoTemp;


    return 1;

}








int InserisciNuovaParolaConVincoli( char * str, char * ParolaCorretta, int LunghezzaParola){

    struct NodoLista * pre;
    struct NodoLista * curr;
    struct NodoLista * NodoTemp;

    pre = NULL;

    int arrayContaLettere[64] = {0};

    int n;
    int i;

    for(i = 0; i < LunghezzaParola; i++) {

        n = FunzioneHashAlfabeto(*(str+i));
        arrayContaLettere[n]++;

    }

    int w;
    for(w = 0; w < LunghezzaParola; w++){

        n = FunzioneHashAlfabeto(*(str+w));

        if (StrutturaVincoli.ArrayNonCompare[n] == 1){

            return 0;

        }


        if(StrutturaVincoli.ArrayCompareInPosizineDiversa[n][w] == 1){

            return 0;

        }

        if(StrutturaVincoli.ArrayEsatte[w] != -1 && StrutturaVincoli.ArrayEsatte[w] != n){

            return 0;

        }

        int k = FunzioneHashAlfabeto(*(ParolaCorretta+w));


        if(StrutturaVincoli.ArrayComparekVolte[k] != 0 && StrutturaVincoli.ArrayComparekVolte[k] != arrayContaLettere[k]){

            return 0;

        }

        if(StrutturaVincoli.ArrayCompareAlmenokVolte[k] > arrayContaLettere[k]){

            return 0;

        }

    }
    curr = ListaInserimento;

    if(curr == NULL || strcmp(curr->Stringa, str) > 0 ) {

        NodoTemp = malloc(sizeof(struct NodoLista));
        NodoTemp->Stringa = malloc(sizeof(char)*LunghezzaParola);
        strcpy(NodoTemp->Stringa, str);
        NodoTemp->Prox = curr;
        ListaInserimento = NodoTemp;

        NumFiltrate++;

        return 1;

    }

    while( curr != NULL ){

        if( strcmp(curr->Stringa, str) < 0){

            pre = curr;
            curr = curr->Prox;

        }else if(strcmp(curr->Stringa, str) > 0 ){

            NodoTemp = malloc(sizeof(struct NodoLista));
            NodoTemp->Stringa = malloc(sizeof(char)*LunghezzaParola);
            strcpy(NodoTemp->Stringa, str);
            NodoTemp->Prox = curr;
            pre->Prox = NodoTemp;
            NumFiltrate++;


            return 1;

        }else if(strcmp(curr->Stringa, str) == 0){

            return 0;
        }

    }


    NodoTemp = malloc(sizeof(struct NodoLista));
    NodoTemp->Stringa = malloc(sizeof(char)*LunghezzaParoleGlobale);
    strcpy(NodoTemp->Stringa, str);
    NodoTemp->Prox = curr;
    pre->Prox = NodoTemp;

    NumFiltrate++;

    return 1;


}






void CancellaNodoFilrate(struct NodoLista ** PuntCurr,struct NodoLista ** PuntPrec ){


    if( *PuntPrec != NULL){

        (*PuntPrec)->Prox = (*PuntCurr)->Prox;
        free(*PuntCurr);

    }else{


        Filtrate =  (*PuntCurr)->Prox;
        free(*PuntCurr);

    }

    return;

}









void InserisciNuoveInFiltrate(){



    struct NodoLista * PuntFil;
    struct NodoLista * PuntListaIns;
    PuntListaIns = ListaInserimento;
    PuntFil = Filtrate;

    struct NodoLista * PuntListaMerged;

    if(Filtrate == NULL) {

        Filtrate = ListaInserimento;
        return;

    }


    if( ListaInserimento == NULL ){

        return;

    }


    if(strcmp(PuntListaIns->Stringa, PuntFil->Stringa) < 0){

        PuntListaMerged = PuntListaIns;
        PuntListaIns = PuntListaIns->Prox;
        PuntListaMerged->Prox = NULL;

    }
    else{


        PuntListaMerged = PuntFil;
        PuntFil = PuntFil->Prox;
        PuntListaMerged->Prox = NULL;

    }

    Filtrate  = PuntListaMerged;

    while( PuntListaIns != NULL || PuntFil != NULL){

        if(PuntListaIns == NULL){

            PuntListaMerged->Prox = PuntFil;
            return;

        }


        if(PuntFil == NULL){

            PuntListaMerged->Prox = PuntListaIns;
            return;

        }

        if( strcmp(PuntListaIns->Stringa, PuntFil->Stringa)  < 0 ){


            PuntListaMerged->Prox = PuntListaIns;
            PuntListaIns = PuntListaIns->Prox;
            PuntListaMerged = PuntListaMerged->Prox;
            PuntListaMerged->Prox = NULL;


        }else{


            PuntListaMerged->Prox = PuntFil;
            PuntFil = PuntFil-> Prox;
            PuntListaMerged = PuntListaMerged-> Prox;
            PuntListaMerged->Prox = NULL;

        }

    }

    return;


}







void FiltraParole( int LunghezzaParole, char * ParolaCorretta){

    struct NodoLista * PuntL;
    struct NodoLista * PuntTemp;
    struct NodoLista * PuntPrec;
    int count = 0;
    int n, g;

    PuntPrec = NULL;
    PuntL = Filtrate;

    int arrayContaLettere[64] = {0};

    if( PuntL != NULL ){

        LunghezzaParole = strlen(PuntL->Stringa);

    }

    while( PuntL != NULL ){


        int c = 0;
        for(  g= 0; g< LunghezzaParole ; g++){

            n = FunzioneHashAlfabeto(*((PuntL->Stringa)+g));
            arrayContaLettere[n]++;
            c++;

        }

        if(verificaVincoli(PuntL->Stringa, ParolaCorretta, LunghezzaParole, arrayContaLettere) == 0){

            for(  g= 0; g< LunghezzaParole; g++){

                n = FunzioneHashAlfabeto(*((PuntL->Stringa)+g));
                arrayContaLettere[n] = 0;

            }

            PuntTemp = PuntL->Prox;
            CancellaNodoFilrate( &PuntL, &PuntPrec);
            PuntL = PuntTemp;
            continue;


        }else{

            for(  g= 0; g< LunghezzaParole; g++){

                n = FunzioneHashAlfabeto(*((PuntL->Stringa)+g));
                arrayContaLettere[n] = 0;

            }

            count++;
            PuntPrec = PuntL;
            PuntL = PuntL->Prox;

        }
    }

    NumFiltrate = count;
    return;

}








void StampaOutput(int * Output , int  k){

    int i = 0;

    while( i < k){

        if( Output[i] == 1){

            putchar('+');
        }else if(Output[i] == -1){

            putchar('/');
        }else if(Output[i] == 0){

            putchar('|');
        }

        Output[i] = 0;
        i++;
    }

    if(i>0){

        printf("\n%d\n", NumFiltrate);
    }
}









void CostruisciArrayLetterePossibili(int * Array, char * Str, int k){

    int n;
    int i = 0;

    while( i < k){

        n = FunzioneHashAlfabeto( *(Str+i));
        Array[n]++;

        i++;
    }


    return;
}








int FunzioneHashAlfabeto(char c){

    if ( c >= 65 && c <= 90){

        return c-65;

    }else if( c >= 97 && c <= 122 ){

        return c-97+26;
    }else if( c == 45){

        return 52;
    }else if( c == 95){

        return 53;
    }else if( c >= 48 && c <= 57 ){

        return c-48+54;
    }

    return 1;
}



void CorreggiOutputConfrontaStringhe(char *str1, char *str2, int i, int k, int *ArrLettPoss,int *ArrayLettereContate, int *OutputFrase, int n){

    int j;

    if ( i == 0){

        j = 0;

    }else{

        j = i-1;

    }


    char currlett = *(str1+i);

    while( ArrLettPoss[n] < 0){

        if ( *(str2+j) == currlett && *(str1+j) != currlett && OutputFrase[j] == 0){

            OutputFrase[j] = -1;

            ArrayLettereContate[n]--;
            ArrLettPoss[n]++;

        }

        if( j == 0){

            break;
        }

        j--;

    }

    return;

}











void ConfrontaStringhe(char *str1, char *str2, int i, int k, int *ArrLettPoss, int *OutputFrase, int * ArrayLettereContate, struct NodoLista * Nodo) {

    int n;
    int j = i;

    if ( i > k-1){


        return;
    }

    n = FunzioneHashAlfabeto(str2[i]);
    if (*(str1+i) == *(str2+i)){


        OutputFrase[j] = 1;


        ArrayLettereContate[n]++;



        ArrLettPoss[n]--;
        if ( ArrLettPoss[n] < 0 ){


            CorreggiOutputConfrontaStringhe(str1, str2, i, k, ArrLettPoss, ArrayLettereContate, OutputFrase, n);


            if( ArrayLettereContate[n] > StrutturaVincoli.ArrayComparekVolte[n]){

                StrutturaVincoli.ArrayComparekVolte[n] = ArrayLettereContate[n];

            }


        }
        StrutturaVincoli.ArrayComparePoszioneCorretta[n][i] = 1;
        StrutturaVincoli.ArrayEsatte[i] = n;



    }else{


        if (ArrLettPoss[n] == 0 ) {

            OutputFrase[i] = -1;
            if(ArrayLettereContate[n] == 0){


                StrutturaVincoli.ArrayNonCompare[n] = 1;

            }else{

                StrutturaVincoli.ArrayComparekVolte[n] = ArrayLettereContate[n];
                StrutturaVincoli.ArrayCompareInPosizineDiversa[n][i] = 1;

            }

        }else if (ArrLettPoss[n] > 0  ) {

            OutputFrase[i] = 0;

            ArrayLettereContate[n]++;


            ArrLettPoss[n]--;


            StrutturaVincoli.ArrayCompareInPosizineDiversa[n][i] = 1;



        }
    }
    ConfrontaStringhe(str1, str2, i + 1, k, ArrLettPoss, OutputFrase, ArrayLettereContate, Nodo);

}






















int main() {


    int ArrayLetterePosssibili[64] = {0};
    int ArrayLettereContate[64] = {0};
    int Output[500] = {0};

    int LunghezzaParole, NumeroTentativi;
    int FLAGnuovaPartita, FLAGinseriscIinizio, FLAGprimociclo;
    char *Input, *ParolaCorretta;
    struct NodoTree *PresentePossibili;






    // INIZIO INIZIALIZZAZIONI A ZERO

    Filtrate = NULL;
    ListaInserimento = NULL;
    UltimaFiltrate = NULL;
    Ammissibili = NULL;
    ParolaCorretta = NULL;
    //Input == NULL;



    if(scanf("%d", &LunghezzaParole)){}

    LunghezzaParoleGlobale = LunghezzaParole+1;

    jump:

    FLAGprimociclo = 1;

    SvuotaFiltrate();
    Filtrate = NULL;
    NumFiltrate = 0;


    int g = 0;

    while (g < 64) {

        ArrayLettereContate[g] = 0;
        ArrayLetterePosssibili[g] = 0;

        g++;
    }




    FLAGnuovaPartita = 0;


    g = 0;

    while (g < 64) {

        StrutturaVincoli.ArrayCompareAlmenokVolte[g] = 0;
        StrutturaVincoli.ArrayComparekVolte[g] = 0;
        StrutturaVincoli.ArrayNonCompare[g] = 0;

        if( g< LUNGHEZZA_PAROLE){

            StrutturaVincoli.ArrayEsatte[g] = -1;
        }


        int h = 0;

        while (h < 64) {


            StrutturaVincoli.ArrayCompareInPosizineDiversa[g][h] = 0;
            StrutturaVincoli.ArrayComparePoszioneCorretta[g][h] = 0;

            h++;

        }

        g++;
    }

    // FINE INZIALIZZAZIONI A ZERO






    //INIZIO PREPARAZIONE PARTITA


    while (FLAGnuovaPartita != 1) {


        Input = malloc(LunghezzaParoleGlobale * sizeof(char));
        if(scanf("%s", Input)){}


        if (strcmp(Input, "+nuova_partita") == 0 || strcmp(Input, "") == 0) {


            FLAGnuovaPartita = 1;


        } else if (strcmp(Input, "+inserisci_fine") == 0) {

            continue;


        } else if (strcmp(Input, "+inserisci_inizio") == 0) {


            continue;


        } else if (strcmp(Input, "+stampa_filtrate") == 0) {


            continue;


        } else {

            InserisciNuovaParola(Input, &Ammissibili);
            free(Input);

        }




    }


    ParolaCorretta = malloc(LunghezzaParole * sizeof(char));
    if(scanf("%s", ParolaCorretta)){}



    if(scanf("%d", &NumeroTentativi)){}

    FLAGnuovaPartita = 0;


    //FINE PREPARAZIONE PARTITA



    free(Input);
    Input = malloc(LunghezzaParoleGlobale * sizeof(char));

    while (scanf("%s", Input) > 0) {


        if (FLAGnuovaPartita == 1) {


            ParolaCorretta = malloc(LunghezzaParoleGlobale * sizeof(char));
            if(scanf("%s", ParolaCorretta)){}

            if(scanf("%d", &NumeroTentativi)){}



            FLAGnuovaPartita = 0;

        }



        if (Input[0] != '+' && strcmp(Input, ParolaCorretta) == 0) {


            printf("%s\n", "ok");
            NumeroTentativi = 0;
            free(Input);

            goto jump;


        }


        if (Input[0] == '+' && strcmp(Input, "+inserisci_inizio") == 0) {


            FLAGinseriscIinizio = 1;
            ListaInserimento = NULL;

            while (FLAGinseriscIinizio == 1) {

                free(Input);
                Input = malloc(LunghezzaParoleGlobale * sizeof(char));
                if(scanf("%s", Input)){}
                if (strcmp(Input, "+inserisci_fine") == 0) {

                    break;
                    FLAGinseriscIinizio = 0;
                } else {

                    if( FLAGprimociclo == 0){

                        InserisciNuovaParolaConVincoli(Input, ParolaCorretta, LunghezzaParole);

                    }
                    InserisciNuovaParola(Input, &Ammissibili);

                }

            }

            InserisciNuoveInFiltrate();

        } else if (Input[0] == '+' && strcmp(Input, "+stampa_filtrate") == 0) {

            if( FLAGprimociclo == 1){

                StampaFiltrateAlbero(Ammissibili);
            }else{

                StampaFiltrate(Filtrate);

            }

        } else if (Input[0] == '+' && strcmp(Input, "+nuova_partita") == 0) {


            FLAGnuovaPartita = 1;

        } else {



            PresentePossibili = VerificaParolePossibili(Input, Ammissibili);

            if (PresentePossibili == NULL) {

                printf("%s", "not_exists\n");
                free(Input);
                Input = malloc(LunghezzaParoleGlobale * sizeof(char));
                continue;

            }


            g = 0;

            while (g < 64) {


                ArrayLetterePosssibili[g] = 0;



                g++;
            }


            CostruisciArrayLetterePossibili(ArrayLetterePosssibili, ParolaCorretta, LunghezzaParole);

            ConfrontaStringhe(ParolaCorretta, Input, 0, LunghezzaParole, ArrayLetterePosssibili, Output,
                              ArrayLettereContate, Filtrate);


            g = 0;

            while (g < 64) {


                if (ArrayLettereContate[g] != 0) {

                    if( ArrayLettereContate[g] > StrutturaVincoli.ArrayCompareAlmenokVolte[g] ){
                        StrutturaVincoli.ArrayCompareAlmenokVolte[g] = ArrayLettereContate[g];
                    }

                    ArrayLettereContate[g] = 0;

                }

                g++;
            }



            NumeroTentativi--;


            if( FLAGprimociclo == 1){

                int LunghezzaParoleCopia = LunghezzaParole;

                if( Ammissibili != NULL ){

                    LunghezzaParoleCopia = strlen(Ammissibili->Stringa);

                }

                CopiaAmmissibiliInFiltrate(Ammissibili, ParolaCorretta, LunghezzaParoleCopia, ArrayLettereContate);

            }

            if( FLAGprimociclo != 1){

                FiltraParole( LunghezzaParole, ParolaCorretta);

            }else{


                FLAGprimociclo = 0;
            }

            StampaOutput(Output, LunghezzaParole);


        }


        free(Input);
        Input = malloc(LunghezzaParoleGlobale * sizeof(char));


        if (NumeroTentativi == 0) {

            printf("ko\n");
            free(Input);
            goto jump;
        }

    }


    SvuotaAlbero();
    return 0;

}
