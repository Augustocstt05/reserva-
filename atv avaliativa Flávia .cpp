#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar dados da reserva
typedef struct {
    int idReserva; //numero da reserva               
    char nomeDoCliente[60];    
    char numeroDoQuarto[10];    
    char dataDeEntrada[11];      
    char dataDeSaida[11];       
} Reserva;

// Função para inicializar o arquivo binário
void inicializarArquivo() {
    FILE* arquivo = fopen("reservas.bin", "rb");
    if (arquivo == NULL) { 

        arquivo = fopen("reservas.bin", "wb"); //usei wb criar a pasta sozinho, facilitando para mim 
        if (arquivo != NULL) {
            printf("Arquivo binario 'reservas.bin' criado com sucesso.\n");
            fclose(arquivo);
        } else {
            printf("Erro ao criar o arquivo binario 'reservas.bin'.\n");
            exit(1); 
        }
    } else {
//como ja vai ter uma pasta criada, se tentar dnv, ira aparecer a frase "erro ao criar arquivo binario"
        fclose(arquivo); 
    }
}

// Função para cadastrar uma nova reserva
void cadastrarReserva() {
    FILE* arquivo;
    Reserva novaReserva;

    printf("\n--- Cadastrar Reserva ---\n");

    // Coletar dados da reserva
    printf("ID da reserva: ");
    scanf("%d", &novaReserva.idReserva);
    getchar();

    printf("Nome do cliente: ");
    fgets(novaReserva.nomeDoCliente, sizeof(novaReserva.nomeDoCliente), stdin);
    novaReserva.nomeDoCliente[strcspn(novaReserva.nomeDoCliente, "\n")] = '\0';

    printf("Nmero do quarto: ");
    fgets(novaReserva.numeroDoQuarto, sizeof(novaReserva.numeroDoQuarto), stdin);
    novaReserva.numeroDoQuarto[strcspn(novaReserva.numeroDoQuarto, "\n")] = '\0';

    printf("Data de entrada (dd/mm/aaaa): ");
    scanf("%s", novaReserva.dataDeEntrada);

    printf("Data de saida (dd/mm/aaaa): ");
    scanf("%s", novaReserva.dataDeSaida);

    // Abrir o arquivo para gravar a nova reserva
    arquivo = fopen("reservas.bin", "ab"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    // Escrever a reserva no arquivo
    fwrite(&novaReserva, sizeof(Reserva), 1, arquivo);
    fclose(arquivo);

    printf("Reserva cadastrada com sucesso!\n");
}

// Função para consultar reservas
void consultarReserva() {
    FILE* arquivo;
    Reserva reserva;
    int idBusca, encontrado = 0;

    printf("\n--- Consultar Reserva ---\n");
    printf("Digite o ID da reserva: ");
    scanf("%d", &idBusca);

    arquivo = fopen("reservas.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ou nenhum registro encontrado.\n");
        return;
    }

    // Procurar pela reserva no arquivo
    while (fread(&reserva, sizeof(Reserva), 1, arquivo)) {
        if (reserva.idReserva == idBusca) {
            printf("\nReserva encontrada:\n");
            printf("ID Reserva: %d\n", reserva.idReserva);
            printf("Nome do Cliente: %s\n", reserva.nomeDoCliente);
            printf("Numero do Quarto: %s\n", reserva.numeroDoQuarto);
            printf("Data de Entrada: %s\n", reserva.dataDeEntrada);
            printf("Data de Saida: %s\n", reserva.dataDeSaida);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Reserva nao encontrada com o ID fornecido.\n");
    }

    fclose(arquivo);
}

// Função para excluir uma reserva
void excluirReserva() {
    FILE* arquivo, * temp;
    Reserva reserva;
    int idExcluir, encontrado = 0;

    printf("\n--- Excluir Reserva ---\n");
    printf("Digite o ID da reserva para excluir: ");
    scanf("%d", &idExcluir);

    arquivo = fopen("reservas.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    temp = fopen("temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    // Transferir reservas, excluindo a que possui o ID informado
    while (fread(&reserva, sizeof(Reserva), 1, arquivo)) {
        if (reserva.idReserva != idExcluir) {
            fwrite(&reserva, sizeof(Reserva), 1, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove("reservas.bin");
        rename("temp.bin", "reservas.bin");
        printf("Reserva excluída com sucesso!\n");
    } else {
        printf("Reserva nao encontrada.\n");
        remove("temp.bin"); // Exclui o arquivo temporário
    }
}

// void gerar relatorio 

void gerarRelatorio() {
    FILE* arquivo;
    Reserva reserva;
    int contador = 0;

    printf("\n--- Relatorio de Reservas ---\n");

    // Abrir o arquivo no modo de leitura
    arquivo = fopen("reservas.bin", "rb");
    if (arquivo == NULL) {
        printf("Nenhuma reserva encontrada ou erro ao abrir o arquivo.\n");
        return;
    }

    // Ler e exibir cada reserva no arquivo
    while (fread(&reserva, sizeof(Reserva), 1, arquivo)) {
        printf("\nReserva %d:\n", ++contador);
        printf("ID Reserva: %d\n", reserva.idReserva);
        printf("Nome do Cliente: %s\n", reserva.nomeDoCliente);
        printf("Numero do Quarto: %s\n", reserva.numeroDoQuarto);
        printf("Data de Entrada: %s\n", reserva.dataDeEntrada);
        printf("Data de Saida: %s\n", reserva.dataDeSaida);
    }

    // Se nenhuma reserva for encontrada 
    if (contador == 0) {
        printf("Nenhuma reserva registrada.\n");
    }

    fclose(arquivo);
}
 

int main() {
    int opcao;

    inicializarArquivo(); //chamada do iniciar arquivo para garantir que vai funcionar 

    do {
        // Menu de opções
        printf("\n--- Sistema de Gerenciamento de Reservas ---\n");
        printf("1. Cadastrar reserva\n");
        printf("2. Consultar reserva\n");
        printf("3. Excluir reserva\n");
        printf("4.gerar relatorio\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarReserva();
                break;
            case 2:
                consultarReserva();
                break;
            case 3:
                excluirReserva();
                break;
            case 4:
            	gerarRelatorio();
            	break;
            case 0:
                printf("Fechando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}


