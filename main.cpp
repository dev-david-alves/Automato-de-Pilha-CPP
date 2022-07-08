#include <iostream>
#include <vector>
#include <algorithm>

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
  // Regras
  vector<Regras> regras;

  // Estados finais
  vector<int> estadosFinais;

  AP(vector<Regras> regras, vector<int> estadosFinais)
  {
    this->regras = regras;
    this->estadosFinais = estadosFinais;
  };

  // Aceita
  bool aceito = false;

  // Criar árvore de nós
  Arvore *criarArvore(Arvore *raiz)
  {
    for (int i = 0; i < regras.size(); i++)
    {
      if (raiz->estado != regras[i].estPartida)
        continue;

      if (raiz->palavra[0] != regras[i].simbEntrada && regras[i].simbEntrada != 'e')
        continue;

      if (raiz->pilha[0] == regras[i].simbPilha)
      {
        // Monta a nova pilha
        string novaPilha = "";
        if (regras[i].stringPilha[0] == 'Z')
        {
          novaPilha = raiz->pilha;
        }
        else if (regras[i].stringPilha[0] == 'e')
        {
          novaPilha = raiz->pilha.substr(1);
        }
        else
        {
          novaPilha = regras[i].stringPilha.substr(0, regras[i].stringPilha.length() - 1) + raiz->pilha;
        }

        // Cria o novo filho
        if (raiz->palavra.length() > 0 && regras[i].simbEntrada != 'e')
        {
          Arvore *filho = new Arvore(regras[i].estChegada, raiz->palavra.substr(1), novaPilha);
          raiz->addFilho(filho);
          criarArvore(filho);
        }
        else
        {
          Arvore *filho = new Arvore(regras[i].estChegada, raiz->palavra, novaPilha);
          raiz->addFilho(filho);
          criarArvore(filho);
        }

        // Verifica se caiu em alguma das opções de aceitação
        if (raiz->palavra.length() == 0)
        {
          if ((raiz->pilha.size() == 0 || raiz->pilha[0] == 'Z') || find(estadosFinais.begin(), estadosFinais.end(), raiz->estado) != estadosFinais.end())
          {
            aceito = true;
          }
        }
      }
    }

    return raiz;
  }

  // Imprimir árvore
  void imprimirArvore(Arvore *raiz)
  {
    if (raiz->palavra.length() == 0)
    {
      cout << "(" << raiz->estado << ", e, " << raiz->pilha;
    }
    else
    {
      cout << "(" << raiz->estado << ", " << raiz->palavra << ", " << raiz->pilha;
    }

    if (raiz->filhos.size() != 0)
      cout << "; ";

    for (int i = 0; i < raiz->filhos.size(); i++)
    {
      if (raiz->filhos.size() > 1 && i != raiz->filhos.size() - 1)
      {
        imprimirArvore(raiz->filhos[i]);
        cout << "); ";
      }
      else
      {
        imprimirArvore(raiz->filhos[i]);
        cout << ")";
      }
    }
  }
};

int main()
{
  // quantidade de estados - N
  int qntdEstados;
  cin >> qntdEstados;
  // Quantidade de símbolos do alfabeto de entrada
  int qntdSimbolosEntrada;
  cin >> qntdSimbolosEntrada;
  // símbolos do alfabeto de entrada
  char alfabetoEntrada[qntdSimbolosEntrada];
  for (int i = 0; i < qntdSimbolosEntrada; i++)
  {
    cin >> alfabetoEntrada[i];
  }
  // Quantidade de símbolos do alfabeto da pilha
  int qntdSimbolosPilha;
  cin >> qntdSimbolosPilha;
  // símbolos do alfabeto da pilha
  char alfabetoPilha[qntdSimbolosPilha];
  for (int i = 0; i < qntdSimbolosPilha; i++)
  {
    cin >> alfabetoPilha[i];
  }
  // número do estado inicial (entre 0 e N-1)
  int estadoInicial;
  cin >> estadoInicial;
  // símbolo inicial da pilha
  string simboloInicialPilha;
  cin >> simboloInicialPilha;

  // Quantidade de estados finais
  int qntdEstadosFinais;
  cin >> qntdEstadosFinais;
  // estados finais
  vector<int> estadosFinais;
  int aux;
  for (int i = 0; i < qntdEstadosFinais; i++)
  {
    cin >> aux;
    estadosFinais.push_back(aux);
  }

  // Quantidade de regras
  int qntdRegrasTransicoes;
  cin >> qntdRegrasTransicoes;

  // Partes das regras de transições
  int estPartida, estChegada;
  char simbEntrada, simbPilha;
  string stringPilha;

  // Regras de transições
  vector<Regras> regras;
  for (int i = 0; i < qntdRegrasTransicoes; i++)
  {
    cin >> estPartida >> estChegada >> simbEntrada >> simbPilha >> stringPilha;

    regras.push_back(Regras(estPartida, estChegada, simbEntrada, simbPilha, stringPilha));
  }

  // uma string
  string palavra;
  cin >> palavra;

  // cria o autômato
  AP automato = AP(regras, estadosFinais);
  Arvore *raiz = new Arvore(estadoInicial, palavra, simboloInicialPilha);
  Arvore *arvore = automato.criarArvore(raiz);
  automato.imprimirArvore(raiz);

  // Imprime a última chave
  cout << ")\n";

  // Escreve se aceita ou não
  if (automato.aceito)
  {
    cout << "aceita\n";
  }
  else
  {
    cout << "rejeita\n";
  }

  return 0;
}