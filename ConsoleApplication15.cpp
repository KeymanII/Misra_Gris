#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using json = nlohmann::json;
using namespace std;

// Класс веера
class Fan {
public:
	vector<pair<int, int>> edges; // Вершина, цвет
	int center;
	Fan(int cv) : center(cv) {}

	void addEdge(int v, int color = 0) {
		edges.push_back(make_pair(v, color));
	}

	bool rotate(int freeColor) {
		int pos = -1;
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].second == freeColor) {
				pos = i;
				break;
			}
		}

		if (pos != -1) {
			int color0 = edges[0].second;
			for (int i = 0; i < pos; i++)
				edges[i].second = edges[i + 1].second;
			edges[pos].second = color0;
			return true;
		}

		return false;
	}

	vector<vector<int>> return_edges_with_colors() {
		vector<vector<int>> edges_with_colors; // edges_with_colors: (вершина 1, вершина 2, цвет)
		for (auto& edge : edges) {
			edges_with_colors.push_back({ center, edge.first, edge.second });
		}

		return edges_with_colors;
	}

	void print() {
		cout << "Веер для вершины " << center << ":" << endl;
		for (auto& edge : edges) cout << "Ребро (" << center << "," << edge.first << "), цвет: " << edge.second << endl;
	}
};

void reverse_way(int goofy_ah_x, vector<vector<int>>& goofied_ah_edges_with_color, int goofy_ah_c, int goofy_ah_d, vector<vector<int>>& absolute_edges_with_colors, vector<bool>& visited) {
	// absolute_edges_with_colors - первый вариант массива, он передается, чтобы не случилось так, что разные рекурсивные шаги инверсировали одно и то же ребро два раза
	// Защита от циклов
	if (visited[goofy_ah_x]) return;
	visited[goofy_ah_x] = true;

	// Рекурсивно идём от вершины х по всем далее идущим, ищем подходящих соседей и инверсируем путь
	for (int j = 0; j < goofied_ah_edges_with_color.size(); j++) {
		// почему смотрю на нулевой (сравниваю х именно с нулевым элементом) элемент в следующей строке кода: предоплагаю, 
		// что в полном массиве точно найдется веер данной вершины, где описаны все соседи (см. return_edges_with_colors)
		if ((goofied_ah_edges_with_color[j][0] == goofy_ah_x) && ((goofied_ah_edges_with_color[j][2] == goofy_ah_c) || (goofied_ah_edges_with_color[j][2] == goofy_ah_d))) {
			if ((goofied_ah_edges_with_color[j][2] == goofy_ah_c) && (absolute_edges_with_colors[j][2] == goofy_ah_c)) {
				goofied_ah_edges_with_color[j][2] = goofy_ah_d;
				// Обновим для goofy_ah_x её цвет в глобальном списке рёбер:
				for (auto& goofy_ah_edge : goofied_ah_edges_with_color) {
					if (goofy_ah_edge[0] == goofy_ah_x && goofy_ah_edge[1] == goofied_ah_edges_with_color[j][1]) {
						goofy_ah_edge[2] = goofy_ah_d;
					}
				}
			}

			else if ((goofied_ah_edges_with_color[j][2] == goofy_ah_d) && (absolute_edges_with_colors[j][2] == goofy_ah_d)) {
				goofied_ah_edges_with_color[j][2] = goofy_ah_c;

				for (auto& goofy_ah_edge : goofied_ah_edges_with_color) {
					if (goofy_ah_edge[0] == goofy_ah_x && goofy_ah_edge[1] == goofied_ah_edges_with_color[j][1]) {
						goofy_ah_edge[2] = goofy_ah_c;
					}
				}
			}
			reverse_way(goofied_ah_edges_with_color[j][1], goofied_ah_edges_with_color, goofy_ah_c, goofy_ah_d, absolute_edges_with_colors, visited);
		}
	}
}

void to_json(vector<pair<int, int>>& edges, vector<int>& colors) {
	json j;

	for (auto& edge : edges) {
		j.push_back({ edge.first, edge.second });
	}
	j.push_back(colors);

	ofstream file("colors.json");
	file << j.dump();
	file.close();
}

int i_hate_splitting_strings(string str, const char comma = ',') {
	size_t len = str.length();
	for (int i = 0; i < len; i++) {
		if (str[i] == comma) {
			return i;
		}
	}

	return -1;
}
int main()
{
	string raw_pair, raw_ui, raw_vi;
	int n, ui, vi, delta = 0, mx, comma_index;
	int edges_degree_counter[100]; //вспомогательный массив для подсчета максимальной степени вершины
	fill(edges_degree_counter, edges_degree_counter + 99, 0);
	vector<pair<int, int>> edges;
	vector<int> colors;
	vector<Fan> fans(100, Fan(0));
	int mxVert = 0;

	setlocale(LC_ALL, "Russian");
	cout << "Введите количество рёбер в графе: "; cin >> n; cout << endl; // количество ребер в графе

	// На вход подаются ребра в формате: u1,v1 u2,v2 ... ui,vi
	cout << "Введите рёбра в формате u1,v1\\n:" << endl;
	for (int i = 0; i < n; i++) {
		cin >> raw_pair;
		comma_index = i_hate_splitting_strings(raw_pair);

		// Обработка ошибки ввода:
		if (comma_index == -1) {
			cout << "Некорректный ввод ребра" << endl;
			return 1;
		}

		raw_ui = raw_pair.substr(0, comma_index);
		raw_vi = raw_pair.substr(comma_index + 1, raw_pair.length() - comma_index);
		ui = stoi(raw_ui);
		vi = stoi(raw_vi);
		edges_degree_counter[ui]++;

		// Обработка петель
		if (ui != vi)
			edges_degree_counter[vi]++;

		mx = max(edges_degree_counter[ui], edges_degree_counter[vi]);
		edges.push_back(make_pair(ui, vi));
		delta = max(delta, mx);
		mxVert = max(mxVert, max(ui, vi));
	}

	delta++;

	// Заполнение веера:
	for (auto& edge : edges) {
		int u = edge.first;
		int v = edge.second;
		if (fans[u].center == 0) fans[u] = Fan(u);
		if (fans[v].center == 0) fans[v] = Fan(v);
		fans[u].addEdge(v);
		fans[v].addEdge(u);
	}

	colors.resize(n, 0);

	// Алгоритм раскраски рёбер:
	vector<vector<int>> goofied_edges_with_colors;
	for (int i = 0; i < n; i++) {
		int u = edges[i].first;
		int v = edges[i].second;

		// Заполнение использованных цветов соседних рёбер для u и v:
		vector<int> sad_used_colors_u, sad_used_colors_v;
		for (auto& edge : fans[u].edges) {
			for (int k = 0; k < n; k++) {
				if (((edges[k].first == u && edges[k].second == edge.first) || (edges[k].first == edge.first && edges[k].second == u)) && (colors[k] != 0)) {
					sad_used_colors_u.push_back(colors[k]);
				}
			}
		}

		for (auto& edge : fans[v].edges) {
			for (int k = 0; k < n; k++) {
				if (((edges[k].first == v && edges[k].second == edge.first) || (edges[k].first == edge.first && edges[k].second == v)) && (colors[k] != 0)) {
					sad_used_colors_v.push_back(colors[k]);
				}
			}
		}

		//Поиск общего свободного цвета
		int common_color = 0;

		for (int new_common_color = 1; new_common_color <= delta; new_common_color++) {
			if (find(sad_used_colors_u.begin(), sad_used_colors_u.end(), new_common_color) == sad_used_colors_u.end() &&
				find(sad_used_colors_v.begin(), sad_used_colors_v.end(), new_common_color) == sad_used_colors_v.end()) {
				common_color = new_common_color;
				break;
			}
		}

		if (common_color != 0) {
			colors[i] = common_color;
			for (auto& edge : fans[u].edges) {
				if (edge.first == v) edge.second = common_color;
			}

			for (auto& edge : fans[v].edges) {
				if (edge.first == u) edge.second = common_color;
			}
		}

		else {
			// Поиск свободного цвета (для u - c, для v - d)
			int c = 1, d = 1;
			while (find(sad_used_colors_u.begin(), sad_used_colors_u.end(), c) != sad_used_colors_u.end()) c++;
			while ((find(sad_used_colors_v.begin(), sad_used_colors_v.end(), d) != sad_used_colors_v.end()) || d == c) d++;

			if (c <= delta) {
				colors[i] = c;
				for (auto& edge : fans[u].edges) {
					if (edge.first == v) edge.second = c;
				}
				for (auto& edge : fans[v].edges) {
					if (edge.first == u) edge.second = c;
				}
			}

			else {
				// Пробуем повернуть веер
				if (fans[u].rotate(d)) {
					colors[i] = d;
					for (auto& edge : fans[u].edges) {
						if (edge.first == v) edge.second = d;
					}

					for (auto& edge : fans[v].edges) {
						if (edge.first == u) edge.second = d;
					}
				}

				// Инверсия пути
				else {
					goofied_edges_with_colors.clear();
					for (int j = 0; j <= mxVert; j++) {
						if (fans[j].center != 0) {
							vector<vector<int>> fan_edges = fans[j].return_edges_with_colors();
							goofied_edges_with_colors.insert(goofied_edges_with_colors.end(), fan_edges.begin(), fan_edges.end());
						}
					}

					vector<bool> visited(mxVert + 1, false);
					vector<vector<int>> absolute_edges_with_colors = goofied_edges_with_colors;
					reverse_way(u, goofied_edges_with_colors, c, d, absolute_edges_with_colors, visited);

					// Обновление вееров после инверсии
					for (int j = 0; j <= mxVert; j++) {
						if (fans[j].center != 0) {
							for (auto& edge : fans[j].edges) {
								for (auto& goofy_colored_edge : goofied_edges_with_colors) {
									if (goofy_colored_edge[0] == fans[j].center && goofy_colored_edge[1] == edge.first) {
										edge.second = goofy_colored_edge[2];
									}
								}
							}
						}
					}

					colors[i] = c;
					for (auto& edge : fans[u].edges) {
						if (edge.first == v) edge.second = c;
					}
					for (auto& edge : fans[v].edges) {
						if (edge.first == u) edge.second = c;
					}
				}
			}
		}
	}

	// Вывод раскрашенного графа:
	cout << "дельта + 1: " << delta << endl;

	cout << "Цвета рёбер:" << endl;
	for (int i = 0; i < n; i++) {
		cout << "Ребро (" << edges[i].first << ',' << edges[i].second << "); цвет: " << colors[i] << endl;
	}

	to_json(edges, colors);

	return 0;
}
