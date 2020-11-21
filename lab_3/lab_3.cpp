// lab_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;


class Lab3 {
	struct haff
	{
		unsigned s; int i; int p; char c; char k; int j;
		void print() { cout << "c=" << c << " i=" << i << " j=" << j << " p=" << p << " s=" << s << " k=" << k << endl; }
	};
#define Q 0 // 1 - по вероятности, 0 - согласно ссылке на сортировку

public:

	void coder(string nameInFile, string nameOutFile)
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		int N, size, i, j, k, min, jmin1, jmin2, max, imax, s, v;
		int Max = -(signed int)2u / 2;
		int b[256] = { 0 };
		haff* h; short* c; unsigned char* p, g;
		ifstream in(nameInFile, ios::binary);
		if (!in) cerr << "file \"" << nameInFile << "\" not open!\n", exit(1);
		in.seekg(0, ios::end);
		size = in.tellg();
		in.seekg(0, ios::beg);
		p = new unsigned char[size];
		if (!p) cerr << "Error memory!\n", exit(1);
		in.read((char*)p, size); in.close();
		for (i = 0; i < size; i++) b[p[i]]++;
		for (N = i = 0; i < 256; i++) N += !!b[i];
		cout << "N=" << N << endl;
		h = new haff[2 * N + 1];
		if (!h) cerr << "Error memory!\n", exit(1);
		for (j = 0; j < 2 * N - 1; j++) h[j].c = h[j].s = h[j].i = h[j].k = h[j].p = h[j].j = 0;
		for (j = i = 0; i < 256; i++) if (b[i]) h[j].s = b[i], h[j++].c = i;
		vector<vector<char>*> V(N);
		for (j = -N; j < 0; imax >= 0 ? h[imax].i = h[imax].j = j++ : j++) for (imax = -1, max = k = 0; k < N; k++) if (!h[k].i)
			if (h[k].s > max) max = h[k].s, imax = k;
		for (s = j = 0; j < N; s += h[j++].s);
		cout << "s=" << s << endl;
		for (i = 0; i < N - 1; i++)
		{
			for (min = Max, jmin1 = -1, j = 0; j < N + i; j++) if (!h[j].p) if (min >= h[j].s) min = h[j].s, jmin1 = j;
			h[jmin1].p = N + i;
			for (min = Max, jmin2 = -1, j = 0; j < N + i; j++) if (!h[j].p) if (min >= h[j].s) min = h[j].s, jmin2 = j;
			h[jmin2].p = N + i;
			if (Q) h[jmin1].k = 0x30, h[jmin2].k = 0x31; // установка цифрового кода (0 или 1) по вероятности
			else if (h[jmin1].j < h[jmin2].j) h[N + i].j = h[jmin1].j, h[jmin1].k = 0x31, h[jmin2].k = 0x30; else
				h[N + i].j = h[jmin2].j, h[jmin1].k = 0x30, h[jmin2].k = 0x31;
			h[N + i].s = h[jmin1].s + h[jmin2].s;
			h[N + i].i = i + 1;
		}
		for (j = 0; j < N; j++)
		{
			for (i = 0, k = j; h[k].p; k = h[k].p, i++);
			V[j] = new vector<char>(i); if (!V[j]) cerr << "Error memory!\n", exit(1);
			for (i = 0, k = j; h[k].p; k = h[k].p, i++) (*V[j])[V[j]->size() - 1 - i] = h[k].k;
		}
		for (j = 0; j < N; j++, cout << endl)
			for (k = (int)(unsigned char)h[j].c - 32, k < 0 ? cout << "\'\\" << k + 32 << "\':" : cout << "\'" << char(k + 32) << "\': ", i = 0; i < V[j]->size(); i++) cout << (*V[j])[i];
		ofstream ou(nameOutFile, ios::binary); 
		if (!ou) cerr << "Error create the file\"" << nameInFile << "\"\n", exit(1);
		g = N - 1; ou.put(g);
		for (j = 0; j < N; j++)
			for (ou.put(h[j].c), g = V[j]->size(), ou.put(g), i = 0; i < V[j]->size(); i += 8, ou.put(g))
				for (g = k = 0; k < 8; ++k) g |= ((*V[j])[(i + k) % V[j]->size()] & 1) << (7 - k);
		for (i = 0; i < 256; b[i++] = -1); for (i = 0; i < N; b[(unsigned char)h[i].c] = i++);
		for (g = k = j = 0; j < s; j++) for (i = 0; i < V[b[p[j]]]->size(); g |= ((*V[b[p[j]]])[i++] & 1) << (7 -
			k % 8), ++k % 8 ? 1 : (ou.put(g), g = 0));
		if (k % 8) ou.put(g); g = k % 8, ou.put(g);
		cout << "Кодированная часть файла в битах: " << k << endl;

		ou.close();
		for (j = 0; j < N; j++) delete V[j];
		delete[] p;
		delete[] h;
	}
	void decoder(string nameInFile, string nameOutFile)
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		int o = 0, N, size, i, j, k, l = 0, r, e, t, ii;
		unsigned char* p, q; short* c, v; char* a;
		ifstream in(nameInFile, ios::binary);
		if (!in) cerr << "file \"" << nameInFile << "\" not open!\n", exit(1);
		in.seekg(0, ios::end); size = in.tellg(); in.seekg(0, ios::beg);
		p = new unsigned char[size]; if (!p) cerr << "Error memory!\n", exit(1);
		in.read((char*)p, size); in.close();
		N = (unsigned char)p[l++] + 1;
		a = new char[N]; c = new short[2 * N - 2];
		for (i = 0; i < 2 * N - 2; c[i++] = -1);
		cout << "Размер таблицы равен " << N << " символам." << endl;
		for (j = 0; j < N; j++, cout << endl)
		{
			a[j] = p[l++], r = (unsigned char)p[l++];
			k = (unsigned char)a[j] - 32, k < 0 ? cout << "\'\\" << k + 32 << "\': " : cout << "\'" << char(k + 32) << "\': ";
			k = 2 * N - 2; // начинаем с последней ячейки
			for (q = i = 0; i < r; i++, q >>= 1)
			{
				if (!q) t = (unsigned char)p[l++], q = 128;
				v = !!(t & q), cout << v;
				if (c[(k - N) * 2 + v] < 0)
					if (i == r - 1)
						c[(k - N) * 2 + v] = j;
					else
					{
						for (e = k - 1; e >= N; --e) if (c[(e - N) * 2 + v] < 0 && c[(e - N) * 2 + !v] < 0) break;
						if (c[(e - N) * 2 + !v] < 0)
							k = c[(k - N) * 2 + v] = e;
						else
						{
							for (; e >= N; --e) if (c[(e - N) * 2 + !v] < 0 && c[(e - N) * 2 + v] < 0) break;
							k = c[(k - N) * 2 + v] = e;
						}
					}
				else k = c[(k - N) * 2 + v];
			}
		}
		cout << "Заголовок " << l << " байт" << endl;
		size = (size - l - 1 - !!p[size - 1]) * 8 + p[size - 1];
		cout << "Сжатый текст содержал " << size << " бит." << endl;
		ofstream ou(nameOutFile, ios::binary);
		if (!ou) cerr << "Error create the file \"" << nameInFile << "\"\n", exit(1);
		for (q = 128, v = N - 2, j = 0; j < size; j++)
		{
			if (!q) q = 128, l++;
			k = !!(p[l] & q), q >>= 1;
			if (c[2 * v + k] < N) ou.put(a[c[2 * v + k]]), v = N - 2;
			else v = c[2 * v + k] - N;
		}
		ou.close(); delete[] p, delete[] c, delete[] a;
	}
};

int main()
{
	Lab3 lab3;

	cout << "coder: input 1" << "\n";
	cout << "decoder: input 2" << "\n";
	cout << "close program: input not 1 or 2" << "\n";

	while (true) {
		int programNumber;
		cout << "Input program number: ";
		cin >> programNumber;

		if (programNumber == 1)
		{
			string in;
			string out;

			cout << "Input in.txt: ";
			cin >> in;
			cout << "Input out.txt: ";
			cin >> out;

			lab3.coder(in, out);
		}
		else if (programNumber == 2)
		{
			string in;
			string out;

			cout << "Input in.txt: ";
			cin >> in;
			cout << "Input out.txt: ";
			cin >> out;

			lab3.decoder(in, out);
		}
		else
		{
			break;
		}
	}
}
