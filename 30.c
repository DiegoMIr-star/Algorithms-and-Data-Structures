#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 24
#define DIM 256

//Owner Diego Mirabella
typedef struct{
    char nome[DIM];
}nome_lungo;
typedef struct IR{
    char nome[MAX];
    int hash;
    nome_lungo* sostituto;
    struct IR* succ;
}ingredienti_ricette;
typedef struct ER{
    ingredienti_ricette* ingrediente;
    int peso;
    struct ER* succ;
}elementi_ricettario;
typedef struct RI{
    char nome[MAX];
    nome_lungo* sostituto;
    elementi_ricettario* elemento;
    struct RI* succ;
}ricette;
typedef struct RIF{
    int peso;
    int scadenza;
    struct RIF* succ;
}rifornimenti;
typedef struct IM{
    ingredienti_ricette* ingrediente;
    int peso;
    int scadenza;
    int totale;
    struct IM* succ;
    rifornimenti* rifornimento;
}ingredienti_riforniti;
typedef struct L1{
    int tempo;
    int porzioni;
    int peso;
    char discriminante;
    ricette* ricetta;
    struct L1* succ;
    struct L1* prec;
}liste_preparati;
typedef enum{true,false} bool;


void controlla_scanf(int);
void aggiungi_ricetta(ricette**,ingredienti_ricette**);
void rimuovi_ricetta(ricette**,liste_preparati*);
void rifornimento(ingredienti_riforniti**,ingredienti_ricette**,liste_preparati*,int);
void ordine(ricette**,ingredienti_riforniti**,liste_preparati**,liste_preparati**,int);
void corriere(liste_preparati**,liste_preparati**,int);
void dealloca_ricette(ricette**);
void dealloca_ingredienti_ricette(ingredienti_ricette**);
void dealloca_ingredienti_riforniti(ingredienti_riforniti**);
void dealloca_liste(liste_preparati*);
void ordinamento_peso(liste_preparati**,liste_preparati*);
int funzione_hash(char*);
bool verifica_sufficienza(elementi_ricettario*,ingredienti_riforniti**,int,int);
void svuota_rifornimenti(elementi_ricettario*,ingredienti_riforniti**,int);


int main(){
    int tempo=0;
    int periodo;
    int capienza;
    char comando[DIM];
    ricette* ricettario[DIM*74]={NULL};
    ingredienti_riforniti* lotto_ingredienti[DIM*74]={NULL};
    ingredienti_ricette* dizionario_ricette[DIM*74]={NULL};
    liste_preparati* realizzati=NULL;
    liste_preparati* coda=NULL;
    controlla_scanf(scanf("%d",&periodo));
    controlla_scanf(scanf("%d",&capienza));
    while(scanf("%s",comando)!=EOF){
        if(comando[0]=='o')
            ordine(ricettario,lotto_ingredienti,&realizzati,&coda,tempo);
        else if(comando[0]=='a')
            aggiungi_ricetta(ricettario,dizionario_ricette);
        else if(comando[2]=='f')
            rifornimento(lotto_ingredienti,dizionario_ricette,coda,tempo);
        else rimuovi_ricetta(ricettario,realizzati);
        tempo++;
        if(tempo%periodo==0)
            corriere(&realizzati,&coda,capienza);
    }
    dealloca_ricette(ricettario);
    dealloca_ingredienti_ricette(dizionario_ricette);
    dealloca_ingredienti_riforniti(lotto_ingredienti);
    dealloca_liste(realizzati);
    return 0;
}
void aggiungi_ricetta(ricette* ricettario[],ingredienti_ricette* dizionario_ricette[]){
    int indiceR=0;
    int indiceI=0;
    char termine='p';
    char nome_ricetta[DIM];
    char nome_ingrediente[DIM];
    char buttare[10000];
    bool ingrediente=false;
    elementi_ricettario* punt_elemento=NULL;
    ingredienti_ricette* scorrimento_ingredienti=NULL;
    ricette* scorrimento_ricette=NULL;
    controlla_scanf(scanf("%s",nome_ricetta));
    indiceR=funzione_hash(nome_ricetta);
    if(ricettario[indiceR]==NULL){
        ricettario[indiceR]=(ricette*) malloc(sizeof(ricette));
        ricettario[indiceR]->succ=NULL;
        ricettario[indiceR]->elemento=NULL;
        if(strlen(nome_ricetta)<MAX){
            strcpy(ricettario[indiceR]->nome,nome_ricetta);
            ricettario[indiceR]->sostituto=NULL;
        }
        else{
            ricettario[indiceR]->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
            strcpy(ricettario[indiceR]->sostituto->nome,nome_ricetta);
        }
        while(termine!='\n'){
            punt_elemento=(elementi_ricettario*) malloc(sizeof(elementi_ricettario));
            punt_elemento->succ=ricettario[indiceR]->elemento;
            ricettario[indiceR]->elemento=punt_elemento;
            controlla_scanf(scanf("%s",nome_ingrediente));
            indiceI=funzione_hash(nome_ingrediente);
            if(dizionario_ricette[indiceI]==NULL){
                dizionario_ricette[indiceI]=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                dizionario_ricette[indiceI]->succ=NULL;
                dizionario_ricette[indiceI]->hash=indiceI;
                if(strlen(nome_ingrediente)<MAX){
                    strcpy(dizionario_ricette[indiceI]->nome,nome_ingrediente);
                    dizionario_ricette[indiceI]->sostituto=NULL;
                }
                else{
                    dizionario_ricette[indiceI]->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                    strcpy(dizionario_ricette[indiceI]->sostituto->nome,nome_ingrediente);
                }
                punt_elemento->ingrediente=dizionario_ricette[indiceI];
                controlla_scanf(scanf("%d",&(punt_elemento->peso)));
            }
            else{
                ingrediente=false;
                scorrimento_ingredienti=dizionario_ricette[indiceI];
                while(scorrimento_ingredienti!=NULL){
                    if(scorrimento_ingredienti->sostituto==NULL){
                        if(scorrimento_ingredienti->nome[strlen(scorrimento_ingredienti->nome)-1]==nome_ingrediente[strlen(nome_ingrediente)-1]){
                            if(strcmp(scorrimento_ingredienti->nome,nome_ingrediente)==0){
                                ingrediente=true;
                                break;
                            }
                        }
                    }
                    else{
                        if(scorrimento_ingredienti->sostituto->nome[strlen(scorrimento_ingredienti->sostituto->nome)-1]==nome_ingrediente[strlen(nome_ingrediente)-1]){
                            if(strcmp(scorrimento_ingredienti->sostituto->nome,nome_ingrediente)==0){
                                ingrediente=true;
                                break;
                            }
                        }
                    }
                    scorrimento_ingredienti=scorrimento_ingredienti->succ;
                }
                if(ingrediente==true)
                    punt_elemento->ingrediente=scorrimento_ingredienti;
                else{
                    scorrimento_ingredienti=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                    scorrimento_ingredienti->succ=dizionario_ricette[indiceI];
                    scorrimento_ingredienti->hash=indiceI;
                    dizionario_ricette[indiceI]=scorrimento_ingredienti;
                    if(strlen(nome_ingrediente)<MAX){
                        strcpy(scorrimento_ingredienti->nome,nome_ingrediente);
                        scorrimento_ingredienti->sostituto=NULL;
                    }
                    else{
                        scorrimento_ingredienti->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                        strcpy(scorrimento_ingredienti->sostituto->nome,nome_ingrediente);
                    }
                    punt_elemento->ingrediente=scorrimento_ingredienti;
                }
                controlla_scanf(scanf("%d",&(punt_elemento->peso)));
            }
            termine=getchar();
        }
    }
    else{
        scorrimento_ricette=ricettario[indiceR];
        while(scorrimento_ricette!=NULL){
            if(scorrimento_ricette->sostituto==NULL){
                if(strcmp(scorrimento_ricette->nome,nome_ricetta)==0){
                    printf("ignorato\n");
                    if(fgets(buttare,sizeof(buttare),stdin)==NULL){
                        printf("Errore lettura\n");
                        exit(1);
                    }
                    return;
                }
            }
            else{
                if(strcmp(scorrimento_ricette->sostituto->nome,nome_ricetta)==0){
                    printf("ignorato\n");
                    if(fgets(buttare,sizeof(buttare),stdin)==NULL){
                        printf("Errore lettura\n");
                        exit(1);
                    }
                    return;
                }
            }
            scorrimento_ricette=scorrimento_ricette->succ;
        }
        scorrimento_ricette=(ricette*) malloc(sizeof(ricette));
        if(strlen(nome_ricetta)<MAX){
            strcpy(scorrimento_ricette->nome,nome_ricetta);
            scorrimento_ricette->sostituto=NULL;
        }
        else{
            scorrimento_ricette->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
            strcpy(scorrimento_ricette->sostituto->nome,nome_ricetta);
        }
        scorrimento_ricette->succ=ricettario[indiceR];
        scorrimento_ricette->elemento=NULL;
        ricettario[indiceR]=scorrimento_ricette;
        while(termine!='\n'){
            punt_elemento=(elementi_ricettario*) malloc(sizeof(elementi_ricettario));
            punt_elemento->succ=ricettario[indiceR]->elemento;
            ricettario[indiceR]->elemento=punt_elemento;
            controlla_scanf(scanf("%s",nome_ingrediente));
            indiceI=funzione_hash(nome_ingrediente);
            if(dizionario_ricette[indiceI]==NULL){
                dizionario_ricette[indiceI]=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                dizionario_ricette[indiceI]->succ=NULL;
                dizionario_ricette[indiceI]->hash=indiceI;
                if(strlen(nome_ingrediente)<MAX){
                    strcpy(dizionario_ricette[indiceI]->nome,nome_ingrediente);
                    dizionario_ricette[indiceI]->sostituto=NULL;
                }
                else{
                    dizionario_ricette[indiceI]->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                    strcpy(dizionario_ricette[indiceI]->sostituto->nome,nome_ingrediente);
                }
                punt_elemento->ingrediente=dizionario_ricette[indiceI];
                controlla_scanf(scanf("%d",&(punt_elemento->peso)));
            }
            else{
                ingrediente=false;
                scorrimento_ingredienti=dizionario_ricette[indiceI];
                while(scorrimento_ingredienti!=NULL){
                    if(scorrimento_ingredienti->sostituto==NULL){
                        if(strcmp(scorrimento_ingredienti->nome,nome_ingrediente)==0){
                            ingrediente=true;
                            break;
                        }
                    }
                    else{
                        if(strcmp(scorrimento_ingredienti->sostituto->nome,nome_ingrediente)==0){
                            ingrediente=true;
                            break;
                        }
                    }
                    scorrimento_ingredienti=scorrimento_ingredienti->succ;
                }
                if(ingrediente==true)
                    punt_elemento->ingrediente=scorrimento_ingredienti;
                else{
                    scorrimento_ingredienti=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                    scorrimento_ingredienti->succ=dizionario_ricette[indiceI];
                    scorrimento_ingredienti->hash=indiceI;
                    dizionario_ricette[indiceI]=scorrimento_ingredienti;
                    if(strlen(nome_ingrediente)<MAX){
                        strcpy(scorrimento_ingredienti->nome,nome_ingrediente);
                        scorrimento_ingredienti->sostituto=NULL;
                    }
                    else{
                        scorrimento_ingredienti->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                        strcpy(scorrimento_ingredienti->sostituto->nome,nome_ingrediente);
                    }
                    punt_elemento->ingrediente=scorrimento_ingredienti;
                }
                controlla_scanf(scanf("%d",&(punt_elemento->peso)));
            }
            termine=getchar();
        }
    }
    printf("aggiunta\n");
    return;
}
void rimuovi_ricetta(ricette* ricettario[],liste_preparati* realizzati){
    int indice=0;
    char ricetta_rimossa[DIM];
    ricette* scorrimento=NULL;
    ricette* prec=NULL;
    elementi_ricettario* scorrimento2=NULL;
    elementi_ricettario* temporaneo=NULL;
    controlla_scanf(scanf("%s",ricetta_rimossa));
    while(realizzati!=NULL){
        if(realizzati->ricetta->sostituto==NULL){
            if(realizzati->ricetta->nome[strlen(realizzati->ricetta->nome)-1]==ricetta_rimossa[strlen(ricetta_rimossa)-1]){
                if(strcmp(realizzati->ricetta->nome,ricetta_rimossa)==0){
                    printf("ordini in sospeso\n");
                    return;
                }
            }
        }
        else{
            if(realizzati->ricetta->sostituto->nome[strlen(realizzati->ricetta->sostituto->nome)-1]==ricetta_rimossa[strlen(ricetta_rimossa)-1]){
                if(strcmp(realizzati->ricetta->sostituto->nome,ricetta_rimossa)==0){
                    printf("ordini in sospeso\n");
                    return;
                }
            }
        }
        realizzati=realizzati->succ;
    }
    indice=funzione_hash(ricetta_rimossa);
    scorrimento=ricettario[indice];
    while(scorrimento!=NULL){
        if(scorrimento->sostituto==NULL){
            if(scorrimento->nome[strlen(scorrimento->nome)-1]==ricetta_rimossa[strlen(ricetta_rimossa)-1]){
                if(strcmp(scorrimento->nome,ricetta_rimossa)==0){
                    if(prec==NULL){
                        scorrimento2=scorrimento->elemento;
                        while(scorrimento2!=NULL){
                            temporaneo=scorrimento2;
                            scorrimento2=scorrimento2->succ;
                            free(temporaneo);
                        }
                        ricettario[indice]=scorrimento->succ;
                        free(scorrimento);
                    }
                    else{
                        scorrimento2=scorrimento->elemento;
                        while(scorrimento2!=NULL){
                            temporaneo=scorrimento2;
                            scorrimento2=scorrimento2->succ;
                            free(temporaneo);
                        }
                        prec->succ=scorrimento->succ;
                        free(scorrimento);
                    }
                    printf("rimossa\n");
                    return;
                }
            }
        }
        else{
            if(scorrimento->sostituto->nome[strlen(scorrimento->sostituto->nome)-1]==ricetta_rimossa[strlen(ricetta_rimossa)-1]){
                if(strcmp(scorrimento->sostituto->nome,ricetta_rimossa)==0){
                    if(prec==NULL){
                        scorrimento2=scorrimento->elemento;
                        while(scorrimento2!=NULL){
                            temporaneo=scorrimento2;
                            scorrimento2=scorrimento2->succ;
                            free(temporaneo);
                        }
                        ricettario[indice]=scorrimento->succ;
                        free(scorrimento);
                    }
                    else{
                        scorrimento2=scorrimento->elemento;
                        while(scorrimento2!=NULL){
                            temporaneo=scorrimento2;
                            scorrimento2=scorrimento2->succ;
                            free(temporaneo);
                        }
                        prec->succ=scorrimento->succ;
                        free(scorrimento);
                    }
                    printf("rimossa\n");
                    return;
                }
            }
        }
        prec=scorrimento;
        scorrimento=scorrimento->succ;
    }
    printf("non presente\n");
    return;
}
void rifornimento(ingredienti_riforniti* lotto_ingredienti[],ingredienti_ricette* dizionario_ricette[],liste_preparati* scorri,int tempo){
    char nome_ingrediente[DIM];
    int indice;
    int peso;
    int scadenza;
    int temp;
    char terminazione='p';
    ingredienti_riforniti* scorrimento=NULL;
    rifornimenti* scorrimento2=NULL;
    ingredienti_ricette* scorri_ancora=NULL;
    rifornimenti* prec=NULL;
    rifornimenti* aggiunto=NULL;
    bool Individuazione;
    bool Trovato;
    bool sufficiente=false;
    while(terminazione!='\n'){
        controlla_scanf(scanf("%s",nome_ingrediente));
        indice=funzione_hash(nome_ingrediente);
        if(lotto_ingredienti[indice]==NULL){
            lotto_ingredienti[indice]=(ingredienti_riforniti*) malloc(sizeof(ingredienti_riforniti));
            if(dizionario_ricette[indice]==NULL){
                dizionario_ricette[indice]=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                if(strlen(nome_ingrediente)<MAX){
                    strcpy(dizionario_ricette[indice]->nome,nome_ingrediente);
                    dizionario_ricette[indice]->sostituto=NULL;
                }
                else{
                    dizionario_ricette[indice]->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                    strcpy(dizionario_ricette[indice]->sostituto->nome,nome_ingrediente);
                }
                dizionario_ricette[indice]->succ=NULL;
                lotto_ingredienti[indice]->ingrediente=dizionario_ricette[indice];
            }
            else{
                scorri_ancora=dizionario_ricette[indice];
                Trovato=false;
                while(scorri_ancora!=NULL){
                    if(scorri_ancora->sostituto==NULL){
                        if(strcmp(scorri_ancora->nome,nome_ingrediente)==0){
                            Trovato=true;
                            lotto_ingredienti[indice]->ingrediente=scorri_ancora;
                            break;
                        }
                    }
                    else{
                        if(strcmp(scorri_ancora->sostituto->nome,nome_ingrediente)==0){
                            Trovato=true;
                            lotto_ingredienti[indice]->ingrediente=scorri_ancora;
                            break;
                        }
                    }
                    scorri_ancora=scorri_ancora->succ;
                }
                if(Trovato==false){
                    scorri_ancora=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                    scorri_ancora->succ=dizionario_ricette[indice];
                    dizionario_ricette[indice]=scorri_ancora;
                    lotto_ingredienti[indice]->ingrediente=scorri_ancora;
                }
            }
            controlla_scanf(scanf("%d",&(lotto_ingredienti[indice]->peso)));
            controlla_scanf(scanf("%d",&(lotto_ingredienti[indice]->scadenza)));
            lotto_ingredienti[indice]->totale=lotto_ingredienti[indice]->peso;
            lotto_ingredienti[indice]->succ=NULL;
            lotto_ingredienti[indice]->rifornimento=NULL;
            terminazione=getchar();
        }
        else{
            Individuazione=false;
            scorrimento=lotto_ingredienti[indice];
            while(scorrimento!=NULL && Individuazione==false){
                if(scorrimento->ingrediente->sostituto==NULL){
                    if(strcmp(scorrimento->ingrediente->nome,nome_ingrediente)==0){
                        Individuazione=true;
                        controlla_scanf(scanf("%d",&peso));
                        controlla_scanf(scanf("%d",&scadenza));
                        scorrimento->totale=(scorrimento->totale)+peso;
                        if(scorrimento->scadenza==scadenza)
                            scorrimento->peso=(scorrimento->peso)+peso;
                        else{
                            if(scadenza<scorrimento->scadenza){
                                temp=scorrimento->scadenza;
                                scorrimento->scadenza=scadenza;
                                scadenza=temp;
                                temp=scorrimento->peso;
                                scorrimento->peso=peso;
                                peso=temp;
                            }
                            if(scorrimento->rifornimento==NULL){
                                scorrimento->rifornimento=(rifornimenti*) malloc(sizeof(rifornimenti));
                                scorrimento->rifornimento->succ=NULL;
                                scorrimento->rifornimento->peso=peso;
                                scorrimento->rifornimento->scadenza=scadenza;
                            }
                            else{
                                prec=NULL;
                                scorrimento2=scorrimento->rifornimento;
                                while(scorrimento2!=NULL && scorrimento2->scadenza<scadenza){
                                    prec=scorrimento2;
                                    scorrimento2=scorrimento2->succ;
                                }
                                if(scorrimento2==NULL){
                                    prec->succ=(rifornimenti*) malloc(sizeof(rifornimenti));
                                    prec->succ->peso=peso;
                                    prec->succ->scadenza=scadenza;
                                    prec->succ->succ=NULL;
                                }
                                else{
                                    if(scorrimento2->scadenza==scadenza)
                                        scorrimento2->peso=(scorrimento2->peso)+peso;
                                    else if(prec==NULL){
                                        prec=(rifornimenti*) malloc(sizeof(rifornimenti));
                                        prec->peso=peso;
                                        prec->scadenza=scadenza;
                                        prec->succ=scorrimento2;
                                        scorrimento->rifornimento=prec;
                                    }
                                    else{
                                        aggiunto=(rifornimenti*) malloc(sizeof(rifornimenti));
                                        aggiunto->peso=peso;
                                        aggiunto->scadenza=scadenza;
                                        aggiunto->succ=scorrimento2;
                                        prec->succ=aggiunto;
                                    }
                                }
                            }
                        }
                    }
                }
                else{
                    if(strcmp(scorrimento->ingrediente->sostituto->nome,nome_ingrediente)==0){
                        Individuazione=true;
                        controlla_scanf(scanf("%d",&peso));
                        controlla_scanf(scanf("%d",&scadenza));
                        scorrimento->totale=(scorrimento->totale)+peso;
                        if(scorrimento->scadenza==scadenza)
                            scorrimento->peso=(scorrimento->peso)+peso;
                        else{
                            if(scadenza<scorrimento->scadenza){
                                temp=scorrimento->scadenza;
                                scorrimento->scadenza=scadenza;
                                scadenza=temp;
                                temp=scorrimento->peso;
                                scorrimento->peso=peso;
                                peso=temp;
                            }
                            if(scorrimento->rifornimento==NULL){
                                scorrimento->rifornimento=(rifornimenti*) malloc(sizeof(rifornimenti));
                                scorrimento->rifornimento->succ=NULL;
                                scorrimento->rifornimento->peso=peso;
                                scorrimento->rifornimento->scadenza=scadenza;
                            }
                            else{
                                prec=NULL;
                                scorrimento2=scorrimento->rifornimento;
                                while(scorrimento2!=NULL && scorrimento2->scadenza<scadenza){
                                    prec=scorrimento2;
                                    scorrimento2=scorrimento2->succ;
                                }
                                if(scorrimento2==NULL){
                                    prec->succ=(rifornimenti*) malloc(sizeof(rifornimenti));
                                    prec->succ->peso=peso;
                                    prec->succ->scadenza=scadenza;
                                    prec->succ->succ=NULL;
                                }
                                else{
                                    if(scorrimento2->scadenza==scadenza)
                                        scorrimento2->peso=(scorrimento2->peso)+peso;
                                    else if(prec==NULL){
                                        prec=(rifornimenti*) malloc(sizeof(rifornimenti));
                                        prec->peso=peso;
                                        prec->scadenza=scadenza;
                                        prec->succ=scorrimento2;
                                        scorrimento->rifornimento=prec;
                                    }
                                    else{
                                        aggiunto=(rifornimenti*) malloc(sizeof(rifornimenti));
                                        aggiunto->peso=peso;
                                        aggiunto->scadenza=scadenza;
                                        aggiunto->succ=scorrimento2;
                                        prec->succ=aggiunto;
                                    }
                                }
                            }
                        }
                    }
                }
                scorrimento=scorrimento->succ;
            }
            if(Individuazione==false){
                scorrimento=(ingredienti_riforniti*) malloc(sizeof(ingredienti_riforniti));
                scorrimento->succ=lotto_ingredienti[indice];
                lotto_ingredienti[indice]=scorrimento;
                scorrimento->rifornimento=NULL;
                if(dizionario_ricette[indice]==NULL){
                    dizionario_ricette[indice]=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                    if(strlen(nome_ingrediente)<MAX){
                        strcpy(dizionario_ricette[indice]->nome,nome_ingrediente);
                        dizionario_ricette[indice]->sostituto=NULL;
                    }
                    else{
                        dizionario_ricette[indice]->sostituto=(nome_lungo*) malloc(sizeof(nome_lungo));
                        strcpy(dizionario_ricette[indice]->sostituto->nome,nome_ingrediente);
                    }
                    dizionario_ricette[indice]->succ=NULL;
                    scorrimento->ingrediente=dizionario_ricette[indice];
                }
                else{
                    scorri_ancora=dizionario_ricette[indice];
                    Trovato=false;
                    while(scorri_ancora!=NULL){
                        if(scorri_ancora->sostituto==NULL){
                            if(strcmp(scorri_ancora->nome,nome_ingrediente)==0){
                                Trovato=true;
                                scorrimento->ingrediente=scorri_ancora;
                                break;
                            }
                        }
                        else{
                            if(strcmp(scorri_ancora->sostituto->nome,nome_ingrediente)==0){
                                Trovato=true;
                                scorrimento->ingrediente=scorri_ancora;
                                break;
                            }
                        }
                        scorri_ancora=scorri_ancora->succ;
                    }
                    if(Trovato==false){
                        scorri_ancora=(ingredienti_ricette*) malloc(sizeof(ingredienti_ricette));
                        scorri_ancora->succ=dizionario_ricette[indice];
                        dizionario_ricette[indice]=scorri_ancora;
                        scorrimento->ingrediente=scorri_ancora;
                    }
                }
                controlla_scanf(scanf("%d",&(scorrimento->peso)));
                controlla_scanf(scanf("%d",&(scorrimento->scadenza)));
                scorrimento->totale=scorrimento->peso;
            }
            terminazione=getchar();
        }
    }
    printf("rifornito\n");
    while(scorri!=NULL){
        if(scorri->discriminante=='a'){
            sufficiente=verifica_sufficienza(scorri->ricetta->elemento,lotto_ingredienti,tempo,scorri->porzioni);
            if(sufficiente==true){
                svuota_rifornimenti(scorri->ricetta->elemento,lotto_ingredienti,scorri->porzioni);
                scorri->discriminante='p';
            }
        }
        scorri=scorri->prec;
    }
    return;
}
void ordine(ricette* ricettario[],ingredienti_riforniti* lotto_ingredienti[],liste_preparati** realizzati,liste_preparati** coda,int tempo){
    char nome_ordine[DIM];
    int indice;
    int porzione;
    int accumulatore=0;
    ricette* scorrimentoR=NULL;
    ricette* salvato=NULL;
    bool Trovato=false;
    bool sufficiente=true;
    elementi_ricettario* scorri_elemento=NULL;
    liste_preparati* nuovo=NULL;
    controlla_scanf(scanf("%s",nome_ordine));
    indice=funzione_hash(nome_ordine);
    scorrimentoR=ricettario[indice];
    while(scorrimentoR!=NULL){
        if(scorrimentoR->sostituto==NULL){
            if(strcmp(scorrimentoR->nome,nome_ordine)==0){
                Trovato=true;
                break;
            }
        }
        else{
            if(strcmp(scorrimentoR->sostituto->nome,nome_ordine)==0){
                Trovato=true;
                break;
            }
        }
        scorrimentoR=scorrimentoR->succ;
    }
    if(Trovato==false){
        controlla_scanf(scanf("%d",&accumulatore));
        printf("rifiutato\n");
        return;
    }
    salvato=scorrimentoR;
    controlla_scanf(scanf("%d",&porzione));
    sufficiente=verifica_sufficienza(scorrimentoR->elemento,lotto_ingredienti,tempo,porzione);
    accumulatore=0;
    scorri_elemento=scorrimentoR->elemento;
    while(scorri_elemento!=NULL){
        accumulatore=accumulatore+(scorri_elemento->peso);
        scorri_elemento=scorri_elemento->succ;
    }
    if(sufficiente==false){
        if(*realizzati==NULL){
            *realizzati=(liste_preparati*) malloc(sizeof(liste_preparati));
            (*realizzati)->prec=NULL;
            (*realizzati)->succ=NULL;
            (*realizzati)->tempo=tempo;
            (*realizzati)->peso=accumulatore;
            (*realizzati)->porzioni=porzione;
            (*realizzati)->discriminante='a';
            (*realizzati)->ricetta=salvato;
            *coda=*realizzati;
        }
        else{
            nuovo=(liste_preparati*) malloc(sizeof(liste_preparati));
            nuovo->succ=*realizzati;
            nuovo->prec=NULL;
            (*realizzati)->prec=nuovo;
            *realizzati=nuovo;
            nuovo->tempo=tempo;
            nuovo->peso=accumulatore;
            nuovo->porzioni=porzione;
            nuovo->discriminante='a';
            nuovo->ricetta=salvato;
        }
    }
    else{
        svuota_rifornimenti(scorrimentoR->elemento,lotto_ingredienti,porzione);
        if(*realizzati==NULL){
            *realizzati=(liste_preparati*) malloc(sizeof(liste_preparati));
            (*realizzati)->prec=NULL;
            (*realizzati)->succ=NULL;
            (*realizzati)->tempo=tempo;
            (*realizzati)->peso=accumulatore;
            (*realizzati)->porzioni=porzione;
            (*realizzati)->discriminante='p';
            (*realizzati)->ricetta=salvato;
            *coda=*realizzati;
        }
        else{
            nuovo=(liste_preparati*) malloc(sizeof(liste_preparati));
            nuovo->succ=*realizzati;
            nuovo->prec=NULL;
            (*realizzati)->prec=nuovo;
            *realizzati=nuovo;
            nuovo->tempo=tempo;
            nuovo->peso=accumulatore;
            nuovo->porzioni=porzione;
            nuovo->discriminante='p';
            nuovo->ricetta=salvato;
        }
    }
    printf("accettato\n");
    return;
}
void corriere(liste_preparati** realizzati,liste_preparati** coda,int capienza){
    int i=0;
    liste_preparati* punt=*coda;
    liste_preparati* temporaneo=NULL;
    liste_preparati* spediti=NULL;
    if(*realizzati==NULL){
        printf("camioncino vuoto\n");
        return;
    }
    while(capienza>0 && punt!=NULL){
        if(punt->discriminante=='a'){
            punt=punt->prec;
            continue;
        }
        i++;
        if(capienza-((punt->peso)*(punt->porzioni))>=0 && punt->discriminante=='p'){
            capienza=capienza-(punt->peso)*(punt->porzioni);
            temporaneo=punt;
            punt=punt->prec;
            if(punt!=NULL)
                punt->succ=temporaneo->succ;
            else
                *realizzati=temporaneo->succ;
            if(temporaneo->succ==NULL)
                *coda=punt;
            else
                temporaneo->succ->prec=punt;
            temporaneo->prec=NULL;
            temporaneo->succ=NULL;
            ordinamento_peso(&spediti,temporaneo);
        }
        else capienza=0;
    }
    if(*coda==NULL)
        *realizzati=NULL;
    while(spediti!=NULL){
        temporaneo=spediti;
        printf("%d %s %d\n",spediti->tempo,spediti->ricetta->nome,spediti->porzioni);
        spediti=spediti->succ;
        free(temporaneo);
    }
    if(i==0)
        printf("camioncino vuoto\n");
    return;
}
void ordinamento_peso(liste_preparati** spediti,liste_preparati* nodo){
    liste_preparati* punt=NULL;
    int a=0;
    int b=0;
    if(nodo==NULL)
        return;
    if(*spediti==NULL)
        *spediti=nodo;
    else{
        b=(nodo->peso)*(nodo->porzioni);
        punt=*spediti;
        while(punt->succ!=NULL && (punt->peso)*(punt->porzioni)>b)
            punt=punt->succ;
        if(punt->succ==NULL){
            a=(punt->peso)*(punt->porzioni);
            b=(nodo->peso)*(nodo->porzioni);
            if(a>b){
                punt->succ=nodo;
                nodo->prec=punt;
            }
            else if(a==b){
                if(nodo->tempo>punt->tempo){
                    punt->succ=nodo;
                    nodo->prec=punt;
                }
                else{
                    if(punt->prec==NULL){
                        *spediti=nodo;
                        nodo->succ=punt;
                        punt->prec=nodo;
                    }
                    else{
                        punt->prec->succ=nodo;
                        nodo->prec=punt->prec;
                        nodo->succ=punt;
                        punt->prec=nodo;
                    }
                }
            }
            else{
                if(punt->prec==NULL){
                    *spediti=nodo;
                    nodo->succ=punt;
                    punt->prec=nodo;
                }
                else{
                    punt->prec->succ=nodo;
                    nodo->prec=punt->prec;
                    nodo->succ=punt;
                    punt->prec=nodo;
                }
            }
        }
        else{
            b=(nodo->peso)*(nodo->porzioni);
            if((punt->peso)*(punt->porzioni)<b){
                if(punt->prec==NULL){
                    *spediti=nodo;
                    nodo->succ=punt;
                    punt->prec=nodo;
                }
                else{
                    punt->prec->succ=nodo;
                    nodo->prec=punt->prec;
                    nodo->succ=punt;
                    punt->prec=nodo;
                }
            }
            else{
                while(punt->succ!=NULL && (punt->peso)*(punt->porzioni)==b && nodo->tempo>=punt->tempo)
                    punt=punt->succ;
                if(punt->succ==NULL){
                    a=(punt->peso)*(punt->porzioni);
                    b=(nodo->peso)*(nodo->porzioni);
                    if(a==b && nodo->tempo>=punt->tempo){
                        punt->succ=nodo;
                        nodo->prec=punt;
                    }
                    else if(a==b){
                            if(punt->prec==NULL){
                                *spediti=nodo;
                                nodo->succ=punt;
                                punt->prec=nodo;
                            }
                            else{
                                punt->prec->succ=nodo;
                                nodo->prec=punt->prec;
                                nodo->succ=punt;
                                punt->prec=nodo;
                            }
                        }
                    else{
                        if(punt->prec==NULL){
                            *spediti=nodo;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                        else{
                            punt->prec->succ=nodo;
                            nodo->prec=punt->prec;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                    }
                }
                else{
                    if((punt->peso)*(punt->porzioni)==(nodo->peso)*(nodo->porzioni)){
                        if(punt->prec==NULL){
                            *spediti=nodo;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                        else{
                            punt->prec->succ=nodo;
                            nodo->prec=punt->prec;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                    }
                    else{
                        if(punt->prec==NULL){
                            *spediti=nodo;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                        else{
                            punt->prec->succ=nodo;
                            nodo->prec=punt->prec;
                            nodo->succ=punt;
                            punt->prec=nodo;
                        }
                    }
                }
            }
        }
    }
    return;
}
int funzione_hash(char* nuova_ricetta){
    int lunghezza=strlen(nuova_ricetta);
    return lunghezza*74+nuova_ricetta[lunghezza-1]-'0';
}
bool verifica_sufficienza(elementi_ricettario* scorri_elemento,ingredienti_riforniti* lotto_ingredienti[],int tempo,int porzione){
    int indice=0;
    bool Trovato=false;
    ingredienti_riforniti* scorri_ingredienti=NULL;
    ingredienti_riforniti* prec=NULL;
    rifornimenti* ingredienti_ripetuti=NULL;
    while(scorri_elemento!=NULL){
        indice=scorri_elemento->ingrediente->hash;
        scorri_ingredienti=lotto_ingredienti[indice];
        prec=NULL;
        Trovato=false;
        while(scorri_ingredienti!=NULL){
            if(scorri_ingredienti->ingrediente->sostituto==NULL){
                if(strcmp(scorri_ingredienti->ingrediente->nome,scorri_elemento->ingrediente->nome)==0){
                    Trovato=true;
                    if(scorri_ingredienti->scadenza<=tempo){
                        if(scorri_ingredienti->rifornimento==NULL){
                            if(prec==NULL)
                                lotto_ingredienti[indice]=scorri_ingredienti->succ;
                            else
                                prec->succ=scorri_ingredienti->succ;
                            free(scorri_ingredienti);
                            return false;
                        }
                        else{
                            ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                            while(ingredienti_ripetuti!=NULL && scorri_ingredienti->scadenza<=tempo){
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-(scorri_ingredienti->peso);
                                scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                free(ingredienti_ripetuti);
                                ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                            }
                            if(ingredienti_ripetuti==NULL){
                                if(scorri_ingredienti->scadenza<=tempo){
                                    if(prec==NULL)
                                        lotto_ingredienti[indice]=scorri_ingredienti->succ;
                                    else
                                        prec->succ=scorri_ingredienti->succ;
                                    free(scorri_ingredienti);
                                    return false;
                                }
                                else{
                                    if((scorri_ingredienti->peso)<porzione*(scorri_elemento->peso))
                                        return false;
                                }
                            }
                            else{
                                if(scorri_ingredienti->totale<porzione*(scorri_elemento->peso))
                                    return false;
                            }
                        }
                    }
                    else{
                        if(scorri_ingredienti->totale<porzione*(scorri_elemento->peso))
                            return false;
                    }
                    break;
                }
            }
            else{
                if(strcmp(scorri_ingredienti->ingrediente->sostituto->nome,scorri_elemento->ingrediente->nome)==0){
                    Trovato=true;
                    if(scorri_ingredienti->scadenza<=tempo){
                        if(scorri_ingredienti->rifornimento==NULL){
                            if(prec==NULL)
                                lotto_ingredienti[indice]=scorri_ingredienti->succ;
                            else
                                prec->succ=scorri_ingredienti->succ;
                            free(scorri_ingredienti);
                            return false;
                        }
                        else{
                            ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                            while(ingredienti_ripetuti!=NULL && scorri_ingredienti->scadenza<=tempo){
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-(scorri_ingredienti->peso);
                                scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                free(ingredienti_ripetuti);
                                ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                            }
                            if(ingredienti_ripetuti==NULL){
                                if(scorri_ingredienti->scadenza<=tempo){
                                    if(prec==NULL)
                                        lotto_ingredienti[indice]=scorri_ingredienti->succ;
                                    else
                                        prec->succ=scorri_ingredienti->succ;
                                    free(scorri_ingredienti);
                                    return false;
                                }
                                else{
                                    if((scorri_ingredienti->peso)<porzione*(scorri_elemento->peso))
                                        return false;
                                }
                            }
                            else{
                                if(scorri_ingredienti->totale<porzione*(scorri_elemento->peso))
                                    return false;
                            }
                        }
                    }
                    else{
                        if(scorri_ingredienti->totale<porzione*(scorri_elemento->peso))
                            return false;
                    }
                    break;
                }
            }
            prec=scorri_ingredienti;
            scorri_ingredienti=scorri_ingredienti->succ;
        }
        if(Trovato==false)
            return false;
        scorri_elemento=scorri_elemento->succ;
    }
    return true;
}
void svuota_rifornimenti(elementi_ricettario* scorri_elemento,ingredienti_riforniti* lotto_ingredienti[],int porzione){
    int indice=0;
    int accumulatore=0;
    int b;
    ingredienti_riforniti* scorri_ingredienti=NULL;
    ingredienti_riforniti* prec=NULL;
    rifornimenti* ingredienti_ripetuti=NULL;
    while(scorri_elemento!=NULL){
        indice=scorri_elemento->ingrediente->hash;
        scorri_ingredienti=lotto_ingredienti[indice];
        prec=NULL;
        while(scorri_ingredienti!=NULL){
            if(scorri_ingredienti->ingrediente->sostituto==NULL){
                if(strcmp(scorri_ingredienti->ingrediente->nome,scorri_elemento->ingrediente->nome)==0){
                    if(scorri_ingredienti->rifornimento==NULL){
                        if((scorri_ingredienti->peso)-porzione*(scorri_elemento->peso)==0){
                            if(prec==NULL)
                                lotto_ingredienti[indice]=scorri_ingredienti->succ;
                            else
                                prec->succ=scorri_ingredienti->succ;
                            free(scorri_ingredienti);
                        }
                        else{
                            b=porzione*(scorri_elemento->peso);
                            scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                            scorri_ingredienti->peso=(scorri_ingredienti->peso)-b;
                        }
                    }
                    else{
                        ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                        b=porzione*(scorri_elemento->peso);
                        if(scorri_ingredienti->peso>b){
                            scorri_ingredienti->peso=(scorri_ingredienti->peso)-b;
                            scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                        }
                        else{
                            if(scorri_ingredienti->peso==b){
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                                scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                free(ingredienti_ripetuti);
                            }
                            else{
                                accumulatore=b;
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                                while(accumulatore>0 && ingredienti_ripetuti!=NULL){
                                    if(accumulatore-(scorri_ingredienti->peso)>=0){
                                        accumulatore=accumulatore-(scorri_ingredienti->peso);
                                        scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                        scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                        scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                        free(ingredienti_ripetuti);
                                        ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                                    }
                                    else{
                                        scorri_ingredienti->peso=(scorri_ingredienti->peso)-accumulatore;
                                        accumulatore=0;
                                    }
                                }
                                if(accumulatore>0){
                                    if(scorri_ingredienti->peso>accumulatore)
                                        scorri_ingredienti->peso=(scorri_ingredienti->peso)-accumulatore;
                                    else{
                                        if(prec==NULL)
                                            lotto_ingredienti[indice]=scorri_ingredienti->succ;
                                        else
                                            prec->succ=scorri_ingredienti->succ;
                                        free(scorri_ingredienti);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
            else{
                if(strcmp(scorri_ingredienti->ingrediente->sostituto->nome,scorri_elemento->ingrediente->nome)==0){
                    if(scorri_ingredienti->rifornimento==NULL){
                        if((scorri_ingredienti->peso)-porzione*(scorri_elemento->peso)==0){
                            if(prec==NULL)
                                lotto_ingredienti[indice]=scorri_ingredienti->succ;
                            else
                                prec->succ=scorri_ingredienti->succ;
                            free(scorri_ingredienti);
                        }
                        else{
                            b=porzione*(scorri_elemento->peso);
                            scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                            scorri_ingredienti->peso=(scorri_ingredienti->peso)-b;
                        }
                    }
                    else{
                        ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                        b=porzione*(scorri_elemento->peso);
                        if(scorri_ingredienti->peso>b){
                            scorri_ingredienti->peso=(scorri_ingredienti->peso)-b;
                            scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                        }
                        else{
                            if(scorri_ingredienti->peso==b){
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                                scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                free(ingredienti_ripetuti);
                            }
                            else{
                                accumulatore=b;
                                scorri_ingredienti->totale=(scorri_ingredienti->totale)-b;
                                while(accumulatore>0 && ingredienti_ripetuti!=NULL){
                                    if(accumulatore-(scorri_ingredienti->peso)>=0){
                                        accumulatore=accumulatore-(scorri_ingredienti->peso);
                                        scorri_ingredienti->peso=ingredienti_ripetuti->peso;
                                        scorri_ingredienti->scadenza=ingredienti_ripetuti->scadenza;
                                        scorri_ingredienti->rifornimento=ingredienti_ripetuti->succ;
                                        free(ingredienti_ripetuti);
                                        ingredienti_ripetuti=scorri_ingredienti->rifornimento;
                                    }
                                    else{
                                        scorri_ingredienti->peso=(scorri_ingredienti->peso)-accumulatore;
                                        accumulatore=0;
                                    }
                                }
                                if(accumulatore>0){
                                    if(scorri_ingredienti->peso>accumulatore)
                                        scorri_ingredienti->peso=(scorri_ingredienti->peso)-accumulatore;
                                    else{
                                        if(prec==NULL)
                                            lotto_ingredienti[indice]=scorri_ingredienti->succ;
                                        else
                                            prec->succ=scorri_ingredienti->succ;
                                        free(scorri_ingredienti);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
            prec=scorri_ingredienti;
            scorri_ingredienti=scorri_ingredienti->succ;    
        }
        scorri_elemento=scorri_elemento->succ;
    }
    return;
}
void controlla_scanf(int errore){
    if(errore==0)
        exit(1);
    return;
}
void dealloca_ricette(ricette* ricettario[]){
    int i;
    ricette* scorrimento=NULL;
    ricette* temporaneo=NULL;
    elementi_ricettario* scorrimento2=NULL;
    elementi_ricettario* temporaneo2=NULL;
    for(i=0;i<DIM*74;i++){
        if(ricettario[i]!=NULL){
            scorrimento=ricettario[i];
            while(scorrimento!=NULL){
                scorrimento2=scorrimento->elemento;
                while(scorrimento2!=NULL){
                    temporaneo2=scorrimento2;
                    scorrimento2=scorrimento2->succ;
                    free(temporaneo2);
                }
                temporaneo=scorrimento;
                scorrimento=scorrimento->succ;
                free(temporaneo->sostituto);
                free(temporaneo);
            }
        }
    }
    return;
}
void dealloca_ingredienti_ricette(ingredienti_ricette* dizionario_ricette[]){
    int i;
    ingredienti_ricette* scorrimento=NULL;
    ingredienti_ricette* temporaneo=NULL;
    for(i=0;i<DIM*74;i++){
        if(dizionario_ricette[i]!=NULL){
            scorrimento=dizionario_ricette[i];
            while(scorrimento!=NULL){
                temporaneo=scorrimento;
                scorrimento=scorrimento->succ;
                free(temporaneo->sostituto);
                free(temporaneo);
            }
        }
    }
    return;
}
void dealloca_ingredienti_riforniti(ingredienti_riforniti* lotto_ingredienti[]){
    int i;
    ingredienti_riforniti* scorrimento=NULL;
    ingredienti_riforniti* temporaneo=NULL;
    rifornimenti* scorrimento2=NULL;
    rifornimenti* temporaneo2=NULL;
    for(i=0;i<DIM*74;i++){
        if(lotto_ingredienti[i]!=NULL){
            scorrimento=lotto_ingredienti[i];
            while(scorrimento!=NULL){
                scorrimento2=scorrimento->rifornimento;
                while(scorrimento2!=NULL){
                    temporaneo2=scorrimento2;
                    scorrimento2=scorrimento2->succ;
                    free(temporaneo2);
                }
                temporaneo=scorrimento;
                scorrimento=scorrimento->succ;
                free(temporaneo);
            }
        }
    }
    return;
}
void dealloca_liste(liste_preparati* testa){
    liste_preparati* temp=NULL;
    while(testa!=NULL){
        temp=testa;
        testa=testa->succ;
        free(temp);
    }
    return;

}
