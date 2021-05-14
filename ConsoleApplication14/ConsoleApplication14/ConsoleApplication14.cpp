#include "pch.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

ofstream wynik;
stringstream ss;
string kodyH[256];

//Drzewo
struct Node
{
	Node * root, *left, *right;
	char character;
	int frequency;
};

//Rekurencyjne przejście drzewa

void inorderTreeTraversal(Node * w, char c[], int lenc)
{
	if (!(w->left))
	{
		ss.str(string()); ss.clear();
		if (w->character == '\n') {
			cout << "\\n" << ": "; wynik << "\\n" << ": ";
		}
		else {

			cout << w->character << ": ";	wynik << w->character << ": ";
		}
		for (int i = 0; i < lenc; i++) {
			cout << c[i];
			wynik << c[i];
			ss << c[i];
		}
		kodyH[w->character] = ss.str();
		cout << endl;
		wynik << endl;
	}
	else
	{
		c[lenc] = '0'; inorderTreeTraversal(w->left, c, lenc + 1);
		c[lenc] = '1'; inorderTreeTraversal(w->right, c, lenc + 1);
	}
}

void findSmallest(std::vector<Node*> &myvector, Node * &w1, Node * &w2, int &i1, int &i2) {
	Node * w;
	w1 = myvector[0]; i1 = 0;
	w2 = myvector[1]; i2 = 1;
	if (w1->frequency > w2->frequency) {
		Node * x;
		int ix;
		x = w1; w1 = w2; w2 = x;
		ix = i1; i1 = i2; i2 = ix;
	}
	for (int i = 2; i < myvector.size(); i++) {
		w = myvector[i];
		if (w->frequency < w2->frequency) {
			w2 = w;
			i2 = i;
			if (w1->frequency > w2->frequency) {
				Node * x;
				int ix;
				x = w1; w1 = w2; w2 = x;
				ix = i1; i1 = i2; i2 = ix;
			}
		}
	}
	if (i1 > i2) {
		int ix;
		ix = i1; i1 = i2; i2 = ix;
	}
}

//Huffman tree decoding made easy :D
void decodeHuffman(vector<Node*>&tab, string zakodowany) {
	Node* root = tab[0];
	for (int i = 0; i < zakodowany.size()+1; i++) {
		if (root->left == nullptr && root->right == nullptr) {
			cout << root->character;
			wynik << root->character;
			root = tab[0];
		}
			if (zakodowany[i] == '0') {
				root = root->left;
			}
			if (zakodowany[i] == '1') {
				root = root->right;
			}
	}
}

int main()
{
	setlocale(LC_ALL, "PL");
	cout << "Program wczytuje tekst z pliku lub z klawiatury, a następnie koduje go za pomocą algorytmu Huffmana." << endl;

	while (1) {
		cout << "Wybierz opcje\n1.Kodowanie Huffmana\n2.Wyjście\nWybór: ";
		int opcja; cin >> opcja;
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
		}
		switch (opcja) {
		case 1: {
			wynik.open("wynik.txt");
			string text, text1;
			int n = 0;
			vector < Node* > tab;
			while (1) {
				system("CLS");
				cout << "1.Wczytaj dane z pliku\n2.Wpisz dane z klawiatury" << endl;
				cin >> n;
				if (n == 1) {
					ifstream dane;
					string nazwa;
					system("CLS");
					cout << "Proszę podać nazwę pliku wraz z rozszerzeniem." << endl;
					cin >> nazwa;
					dane.open(nazwa);

					if (!dane.good()) {
						cout << "Nie znaleziono pliku o podanej nazwie!" << endl;
						continue;
					}

					string wiersz;
					getline(dane, wiersz);
					ss.str(std::string()); ss.clear();
					ss << wiersz;
					while (getline(dane, wiersz)) {
						ss << "\n" << wiersz;
					}
					text = ss.str();
					ss.str(std::string()); ss.clear();
					cout << "Wczytano następujący tekst: " << endl;
					cout << text << endl;
					dane.close();
				}
				if (n == 2) {
					system("CLS");
					cout << "Wpisz tekst do zakodowania: " << endl;
					cin.clear();
					cin.ignore();
					getline(cin, text);
				}
				break;
			}
			system("CLS");
			int waga[256];
			int ileWszystkichZnakow = 0;
			for (int i = 0; i < 256; i++) waga[i] = 0;
			cout << "Tekst posiada " << text.size() << " znaków." << endl;
			wynik << "Tekst posiada " << text.size() << " znaków." << endl;
			for (int i = text.size() - 1; i >= 0; i--) waga[text[i]]++;
			Node * w;
			Node * w1, *w2;
			for (int i = 0; i < 256; i++)
				if (waga[i]) {
					w = new Node;
					w->root = w->left = w->right = NULL;
					w->character = (char)i;
					w->frequency = waga[i];
					ileWszystkichZnakow += waga[i];
					tab.push_back(w);
				}

			cout << endl;
			cout << "Ilość wystąpień znaków w tekście: " << endl;
			for (int i = 0; i < tab.size(); i++) {
				char znaczek = tab[i]->character;
				if (znaczek == '\n') {
					cout << "\\n" << ": " << tab[i]->frequency << endl; wynik << "\\n" << ": " << tab[i]->frequency << endl;
				}
				else {

					cout << tab[i]->character << ": " << tab[i]->frequency << endl;	wynik << tab[i]->character << ": " << tab[i]->frequency << endl;
				}
			}

			int i1 = 0;
			int i2 = 0;
			int j = 0;
			cout << endl;
			cout << "Przydzielone kody dla poszczególnych znaków: " << endl;
			wynik << endl;
			wynik << "Przydzielone kody dla poszczególnych znaków: " << endl;
			while (tab.size() > 1) {
				findSmallest(tab, w1, w2, i1, i2);
				w = new Node;
				w->root = NULL;
				w->left = w1;
				w->right = w2;
				w->left->root = w->right->root = w;
				w->frequency = w1->frequency + w2->frequency;
				tab.push_back(w);
				tab.erase(tab.begin() + i2);
				tab.erase(tab.begin() + i1);
			}
			char tabb[500];
			inorderTreeTraversal(tab[0], tabb, 0);
			cout << endl;
			cout << "Zakodowany tekst:" << endl;
			wynik << endl;
			wynik << "Zakodowany tekst:" << endl;
			stringstream x;
			x.str(std::string());
			string zakodowany;
			for (int i = 0; i < text.size(); i++) {
				x << kodyH[text[i]];
				wynik << kodyH[text[i]];
			}
			zakodowany = x.str();
			cout << zakodowany;
			wynik << zakodowany;
			cout << endl;
			wynik << endl;
			cout << endl;
			wynik << endl;
			cout << "Tekst odkodowany: " << endl;
			wynik << "Tekst odkodowany: " << endl;
			decodeHuffman(tab, zakodowany);
			cout << endl;
			wynik << endl;			
			cout << endl;
			wynik << endl;
			cout << "Dane zostały zapisane do pliku wynik.txt." << endl;
			cout << "Wciśnij dowolny klawisz, by kontynuować..." << endl;
			wynik.close();
			getchar();
			system("CLS");
			break;
		}

		case 2: {
			return 0;
			break;
		}
		}
	}
}

