#include <iostream>
#include <vector>

using namespace std;

struct Regras
{
  // Lista de transições, cada uma contendo
  // número do estado de partida, número do estado de chegada,
  // símbolo de entrada, símbolo do topo da pilha e string a ser escrita na pilha
  int estPartida, estChegada;
  char simbEntrada, simbPilha;
  string stringPilha;

  Regras(int estPartida, int estChegada,
         char simbEntrada, char simbPilha, string stringPilha)
  {
    this->estPartida = estPartida;
    this->estChegada = estChegada;
    this->simbEntrada = simbEntrada;
    this->simbPilha = simbPilha;
    this->stringPilha = stringPilha;
  };
};

// Árvore de nós com muitos filhos
struct Arvore
{
  int estado;
  string palavra;
  string pilha;
  vector<Arvore *> filhos;

  Arvore(int estado, string palavra, string pilha)
  {
    this->estado = estado;
    this->palavra = palavra;
    this->pilha = pilha;
  };

  void addFilho(Arvore *filho)
  {
    filhos.push_back(filho);
  };
};

// Autômato de pilha
class AP
{
public:
  // Quantidade de transições
  int qntdTransicoes = 12;

  int estPartida, estChegada;
  char simbEntrada, simbPilha;
  string stringPilha;

  // Regras
  vector<Regras> regras;

  AP()
  {
    // for (int i = 0; i < qntdTransicoes; i++)
    // {
    //   cout << "Digite o estado de partida: ";
    //   cin >> estPartida;
    //   cout << "Digite o estado de chegada: ";
    //   cin >> estChegada;
    //   cout << "Digite o símbolo de entrada: ";
    //   cin >> simbEntrada;
    //   cout << "Digite o símbolo da pilha: ";
    //   cin >> simbPilha;
    //   cout << "Digite a string a ser escrita na pilha: ";
    //   cin >> stringPilha;

    //   regras.push_back(Regras(estPartida, estChegada, simbEntrada, simbPilha, stringPilha));
    // }

    regras.push_back(Regras(0, 0, '0', 'Z', "0Z"));
    regras.push_back(Regras(0, 0, '1', 'Z', "1Z"));
    regras.push_back(Regras(0, 0, '0', '0', "00"));
    regras.push_back(Regras(0, 0, '0', '1', "01"));
    regras.push_back(Regras(0, 0, '1', '0', "10"));
    regras.push_back(Regras(0, 0, '1', '1', "11"));
    regras.push_back(Regras(0, 1, 'e', 'Z', "Z"));
    regras.push_back(Regras(0, 1, 'e', '0', "0"));
    regras.push_back(Regras(0, 1, 'e', '1', "1"));
    regras.push_back(Regras(1, 1, '0', '0', "e"));
    regras.push_back(Regras(1, 1, '1', '1', "e"));
    regras.push_back(Regras(1, 2, 'e', 'Z', "Z"));
  };

  // Criar árvore de nós
  Arvore *criarArvore(Arvore *raiz, string palavra, string pilha)
  {
    if (raiz->palavra.length() == 0)
    {
      cout << "(q" << raiz->estado << ", e, " << raiz->pilha << "0; ";
    }
    else
    {
      cout << "(q" << raiz->estado << ", " << raiz->palavra << ", " << raiz->pilha << "0; ";
    }

    for (int i = 0; i < regras.size(); i++)
    {
      // Preciso corrigir a formação da nova palavra.
      string novaStringPilha = pilha;
      if (regras[i].stringPilha[0] != 'e' && regras[i].stringPilha[0] != 'Z')
      {
        novaStringPilha = regras[i].stringPilha[0] + pilha;
      }

      if (regras[i].estPartida == raiz->estado && (regras[i].simbEntrada == palavra[0] || regras[i].simbEntrada == 'e') &&
          regras[i].simbPilha == pilha[0] && palavra.length() > 0)
      {
        Arvore *filho = new Arvore(regras[i].estChegada, palavra.substr(1), novaStringPilha);
        raiz->addFilho(filho);

        criarArvore(filho, palavra.substr(1), novaStringPilha);
      }
    }

    return raiz;
  }
};

int main()
{
  // quantidade de estados - N
  int qntdEstados = 3;

  // Quantidade de símbolos do alfabeto de entrada
  int qntdSimbolos = 2;
  // símbolos do alfabeto de entrada
  char alfabeto[qntdSimbolos] = {'0', '1'};

  // Quantidade de símbolos do alfabeto da pilha
  int qntdSimbolosPilha = 3;
  // símbolos do alfabeto da pilha
  char alfabetoPilha[qntdSimbolosPilha] = {'0', '1', 'Z'};

  // número do estado inicial (entre 0 e N-1)
  int estadoInicial = 0;
  // símbolo inicial da pilha
  char simboloInicialPilha = 'Z';

  // Quantidade de estados finais
  int qntdEstadosFinais = 1;
  // estados finais
  int estadosFinais[qntdEstadosFinais] = {2};

  // uma string
  string palavra = "1111";

  // cria o autômato
  AP automato = AP();
  Arvore *raiz = new Arvore(0, palavra, "Z");
  Arvore *arvore = automato.criarArvore(raiz, raiz->palavra, "Z");

  return 0;
}