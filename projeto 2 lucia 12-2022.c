#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

typedef struct noProjeto{
    int cod_proj;
    char gerente[50];
    char cliente[50];
    float receita;
    struct noProjeto *proxProjeto;
}Projeto;

typedef struct noData{
    int cod_proj;
    int dia;
    int mes;
    int ano;
    struct noData *proxData;
}Data;

typedef struct noDespesa{
    int cod_proj;
    float salDev;
    float valHrExtra;
    float valDesloc;
    float outrosGastos;
    struct noDespesa *proxDespesa;
}Despesa;


Projeto *insereProjeto (Projeto *recebida, int codigo, char nomeGerente[50], char nomeCliente[50], float receita){
    Projeto  *novo;
    novo = (Projeto*) malloc(sizeof(Projeto));

    novo->cod_proj = codigo;
    strcpy(novo->gerente, nomeGerente);
    strcpy(novo->cliente, nomeCliente);
    novo->receita = receita;

    novo->proxProjeto = recebida;
}

Data *insereData (Data *recebida, int cod_proj, int dia, int mes, int ano){
    Data *nova;
    nova=(Data*)malloc(sizeof(Data));

    nova->cod_proj = cod_proj;
    nova->dia = dia;
    nova->mes = mes;
    nova->ano = ano;

    nova->proxData = recebida;
}

Despesa *insereDespesa (Despesa *recebida, int cod_proj, float salDev, float valHrExtra, float valDesloc, float outrosGastos){
    Despesa *nova;
    nova=(Despesa*)malloc(sizeof(Despesa));

    nova->cod_proj=cod_proj;
    nova->salDev=salDev;
    nova->valHrExtra=valHrExtra;
    nova->valDesloc=valDesloc;
    nova->outrosGastos=outrosGastos;

    nova->proxDespesa = recebida;
}

void imprime(Despesa* recebida){
    while(recebida!=NULL){
        printf("%f ", recebida->outrosGastos);
        recebida=recebida->proxDespesa;
    }
}

float lucroTotal(Projeto *proj, Despesa *des){
    float lucro, soma=0;
    Projeto *auxProj;
    auxProj=proj;
    Despesa *auxDesp;
    auxDesp=des;

    while(auxProj!=NULL){
        lucro = (auxProj->receita) - (auxDesp->outrosGastos) - (auxDesp->salDev) - (auxDesp->valDesloc) - (auxDesp->valHrExtra);
        soma+=lucro;
        auxProj=auxProj->proxProjeto;
        auxDesp=auxDesp->proxDespesa;
    }
    return soma;
}

int projetosAtrasados(Data *dataRec){
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    int contAtrazado=0;
    Data *aux;
    aux=dataRec;

    while(aux!=NULL){
        if(aux->ano<=tm.tm_year+1900){
            if(aux->mes<=tm.tm_mon+1){
                if(aux->dia<=tm.tm_mday){
                    contAtrazado++;
                }
            }
        }
        aux=aux->proxData;
    }
    return contAtrazado;
}

int projetoMaiorGastoHrExtra(Despesa *despesaRec, Projeto *projetoRec, char gerente[]){
    int cod;
    float maior=0;
    Despesa *auxDesp;
    auxDesp=despesaRec;
    Projeto *auxProj;
    auxProj=projetoRec;


    while(auxDesp!=NULL){
        if(auxDesp->valHrExtra>maior){
            maior=auxDesp->valHrExtra;
            cod=auxProj->cod_proj;
            strcpy(gerente, auxProj->gerente);
        }
        auxDesp=auxDesp->proxDespesa;
        auxProj=auxProj->proxProjeto;
    }
    return cod;
}

int projetoMaiorGastoTotal(Despesa *despesaRec, Projeto *projetoRec, char gerente[]){
    int cod;
    float gastoTotal, maior=0;
    Despesa *auxDesp;
    auxDesp=despesaRec;
    Projeto *auxProj;
    auxProj=projetoRec;


    while(auxDesp!=NULL){
        gastoTotal=(auxDesp->outrosGastos)+(auxDesp->salDev)+(auxDesp->valDesloc)+(auxDesp->valHrExtra);
        if(gastoTotal>maior){
            maior=gastoTotal;
            cod=auxProj->cod_proj;
            strcpy(gerente, auxProj->gerente);
        }
        auxDesp=auxDesp->proxDespesa;
        auxProj=auxProj->proxProjeto;
    }
    return cod;
}

int projetoGerente(Projeto *projRec,Data *dataRec,Despesa *despRec,char cliente[],char gerente[],int *dia,int *mes,int *ano,float *lucro){
    int cod;
    Projeto *auxProj;
    auxProj=projRec;
    Data *auxData;
    auxData=dataRec;
    Despesa *auxDesp;
    auxDesp=despRec;

    while(auxProj!=NULL){
        if(strcmp(auxProj->gerente, gerente)==0){
            cod=auxProj->cod_proj;
            strcpy(cliente, auxProj->cliente);
            *dia=auxData->dia;
            *mes=auxData->mes;
            *ano=auxData->ano;
            *lucro=(auxProj->receita)-(auxDesp->outrosGastos)-(auxDesp->salDev)-(auxDesp->valDesloc)-(auxDesp->valHrExtra);
            return cod;
        }
        auxProj=auxProj->proxProjeto;
        auxData=auxData->proxData;
        auxDesp=auxDesp->proxDespesa;
    }
}

int projetoCliente(Projeto *projRec,Data *dataRec,Despesa *despRec, char cliente[],char gerente[],int *dia,int *mes,int *ano,float *custo){
    int cod;
    Projeto *auxProj;
    auxProj=projRec;
    Data *auxData;
    auxData=dataRec;
    Despesa *auxDesp;
    auxDesp=despRec;

    while(auxProj!=NULL){
        if(strcmp(auxProj->cliente, cliente)==0){
            cod=auxProj->cod_proj;
            strcpy(gerente, auxProj->gerente);
            *dia=auxData->dia;
            *mes=auxData->mes;
            *ano=auxData->ano;
            *custo=(auxDesp->outrosGastos)+(auxDesp->salDev)+(auxDesp->valDesloc)+(auxDesp->valHrExtra);
            return cod;
        }
        auxProj=auxProj->proxProjeto;
        auxData=auxData->proxData;
        auxDesp=auxDesp->proxDespesa;
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");

    //criação das listas//
    Projeto *inicioProjeto;
    inicioProjeto=NULL;
    Data *inicioData;
    inicioData=NULL;
    Despesa *inicioDespesa;
    inicioDespesa=NULL;

    //criação de variaveis//
    int opcao, cod, dia, mes, ano;
    char gerente[50], cliente[50];
    float lucro, custo;

    //inserções//
    inicioProjeto=insereProjeto(inicioProjeto, 12, "eduardo", "geraldo", 10000);
    inicioData=insereData(inicioData, 12, 15, 5, 2003);
    inicioDespesa=insereDespesa(inicioDespesa, 12, 2000, 1800, 70, 50);

    inicioProjeto=insereProjeto(inicioProjeto, 14, "gabriel", "clovis", 20000);
    inicioData=insereData(inicioData, 14, 11, 12, 2022);
    inicioDespesa=insereDespesa(inicioDespesa, 14, 3500, 1200, 100, 600);

    //menu//
    do{
        printf("\n0 - Finalizar\n1 - Lucro total dos projetos em andamento\n2 - Quantos projetos estão atrasados\n3 - Projeto com maior gasto com horas extras");
        printf("\n4 - Projeto com maior gasto total\n5 - Projeto do gerente\n6 - Projeto do cliente\n");
        scanf("%i", &opcao);
        switch(opcao)
        {
        case 0:
            printf("Finalizando...");
            break;
        case 1:
            system("cls");
            printf("Lucro total: %.2f\n", lucroTotal(inicioProjeto, inicioDespesa));
            break;
        case 2:
            system("cls");
            printf("Projetos atrasados: %i\n", projetosAtrasados(inicioData));
            break;
        case 3:
            system("cls");
            cod=projetoMaiorGastoHrExtra(inicioDespesa, inicioProjeto, gerente);
            printf("Gerente do Projeto: %s\nCódigo do Projeto: %i\n", gerente, cod);
            break;
        case 4:
            system("cls");
            cod=projetoMaiorGastoTotal(inicioDespesa, inicioProjeto, gerente);
            printf("Gerente do Projeto: %s\nCódigo do Projeto: %i\n", gerente, cod);
            break;
        case 5:
            system("cls");
            printf("Digite o nome do gerente: ");
            scanf("%s", &gerente);
            cod=projetoGerente(inicioProjeto, inicioData, inicioDespesa, cliente, gerente, &dia, &mes, &ano, &lucro);
            printf("Código Projeto: %i\nCliente: %s\n", cod, cliente);
            printf("Data de entrega: %i/%i/%i\nLucro: %.2f\n", dia, mes, ano, lucro);
            break;
        case 6:
            system("cls");
            printf("Digite o nome do cliente: ");
            scanf("%s", &cliente);
            cod=projetoCliente(inicioProjeto, inicioData, inicioDespesa, cliente, gerente, &dia, &mes, &ano, &custo);
            printf("Código Projeto: %i\nGerente: %s\n", cod, gerente);
            printf("Data de entrega: %i/%i/%i\nCusto: %.2f\n", dia, mes, ano, custo);
            break;
        default:
            printf("opção invalida\n");
        }
    }while(opcao!=0);

    return 0;
}
