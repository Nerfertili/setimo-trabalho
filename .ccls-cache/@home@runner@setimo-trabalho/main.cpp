#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>
using namespace std;
std::mt19937 generator(123);
std::uniform_int_distribution<int> dis(1, 1000);
std::uniform_int_distribution<int> dis_1(250, 750);

// Data structure to store a max-heap node
struct PriorityQueue {
private:
  // vector to store heap elements
  vector<int> A;

  // return parent of `A[i]`
  // don't call this function if `i` is already a root node
  int PARENT(int i) { return (i - 1) / 2; }

  // return left child of `A[i]`
  int LEFT(int i) { return (2 * i + 1); }

  // return right child of `A[i]`
  int RIGHT(int i) { return (2 * i + 2); }

  // Recursive heapify-down algorithm.
  // The node at index `i` and its two direct children
  // violates the heap property
  void heapify_down(int i) {
    // get left and right child of node at index `i`
    int left = LEFT(i);
    int right = RIGHT(i);

    int largest = i;

    // compare `A[i]` with its left and right child
    // and find the largest value
    if (left < size() && A[left] > A[i]) {
      largest = left;
    }

    if (right < size() && A[right] > A[largest]) {
      largest = right;
    }

    // swap with a child having greater value and
    // call heapify-down on the child
    if (largest != i) {
      swap(A[i], A[largest]);
      heapify_down(largest);
    }
  }

  // Recursive heapify-up algorithm
  void heapify_up(int i) {
    // check if the node at index `i` and its parent violate the heap property
    if (i && A[PARENT(i)] < A[i]) {
      // swap the two if heap property is violated
      swap(A[i], A[PARENT(i)]);

      // call heapify-up on the parent
      heapify_up(PARENT(i));
    }
  }

public:
  // return size of the heap
  unsigned int size() { return A.size(); }

  // Function to check if the heap is empty or not
  bool empty() { return size() == 0; }

  // insert key into the heap
  void push(int key) {
    // insert a new element at the end of the vector
    A.push_back(key);

    // get element index and call heapify-up procedure
    int index = size() - 1;
    heapify_up(index);
  }

  // Function to remove an element with the highest priority (present at the
  // root)
  void pop() {
    try {
      // if the heap has no elements, throw an exception
      if (size() == 0) {
        throw out_of_range("Vector<X>::at() : "
                           "index is out of range(Heap underflow)");
      }
      // replace the root of the heap with the last element
      // of the vector
      A[0] = A.back();
      A.pop_back();

      // call heapify-down on the root node
      heapify_down(0);
    }
    // catch and print the exception
    catch (const out_of_range &oor) {
      cout << endl << oor.what();
    }
  }

  // Function to return an element with the highest priority (present at the
  // root)
  int top() {
    try {
      // if the heap has no elements, throw an exception
      if (size() == 0) {
        throw out_of_range("Vector<X>::at() : "
                           "index is out of range(Heap underflow)");
      }

      // otherwise, return the top (first) element
      return A.at(0); // or return A[0];
    }
    // catch and print the exception
    catch (const out_of_range &oor) {
      cout << endl << oor.what();
    }
  }
};

void insere(PriorityQueue *pq, vector<int> *vq) {
  // aqui eu estou pegando toda a array que eu criei e navegando ela item por
  // item e inserindo dentro do heap
  for (int i = 0; i < vq->size(); i++) {
    pq->push(vq->at(i));
  }
}

int busca(PriorityQueue pq, int vq) {
  // pega os parametros por copia e busca um determinado inteiro dentro do heap
  int founded = 0;
  for (int i = 0; i < pq.size(); i++) {
    if (vq == pq.top()) {
      founded = 1;
      break;
    } else {
      pq.pop();
    }
  }
  return founded;
}

void exclui(PriorityQueue *pq, int vq) {
  // aqui eu extraio os elementos do heap para um backup ate que eu ache o valor
  // que quero deletar, uma vez achado eu o retiro e nao jogo para o backup,
  // apos isso basta inserir tudo denovo
  vector<int> backup;
  for (int i = 0; i < pq->size(); i++) {
    if (pq->top() == vq) {
      pq->pop();
      break;
    } else {
      backup.push_back(pq->top());
      pq->pop();
    }
  }
  insere(pq, &backup);
}

int valida(PriorityQueue pq) {
  // aqui eu jogo tudo pra que tem no heap pra uma lista e verifico a
  // descendencia dessa lista, se algum momento a descendencia nao for
  // verdadeira ieu retorno falso indicando um heap invalido
  vector<int> rs;
  for (int i = 0; i < pq.size(); i++) {
    rs.push_back(pq.top());
    pq.pop();
  }
  int back = rs.back();
  rs.pop_back();
  for (int i = 0; i < rs.size(); i++) {
    if (rs.back() < back) {
      return 0;
    } else {
      back = rs.back();
      rs.pop_back();
    }
  }
  return 1;
}

// Max Heap implementation in C++
int main() {
  PriorityQueue pq;
  PriorityQueue *pt_pq = &pq;

  vector<int> vq(500);
  vector<int> *pt_vq = &vq;

  for (int i = 0; i < vq.size(); i++) {
    vq.at(i) = dis(generator);
  }

  insere(pt_pq, pt_vq);

  if (valida(pq)) {
    cout << "o heap gerado e valido" << endl;
  } else {
    cout << "o heap gerado e nao valido" << endl;
  }

  int founds = 0;
  for (int i = 0; i < 200; i++) {
    if (busca(pq, dis_1(generator))) {
      founds++;
    }
  }
  cout << "fora achados " << founds << " valores dentre os gerados" << endl;
  int init_size = pq.size();
  for (int i = 0; i < 100; i++) {
    exclui(pt_pq, dis_1(generator));
  }
  cout << "fora removidos " << init_size - pq.size() << " valores" << endl;
  if (valida(pq)) {
    cout << "o heap gerado e valido" << endl;
  } else {
    cout << "o heap gerado e nao valido" << endl;
  }
}