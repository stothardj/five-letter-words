#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <thread>
#include <future>

struct Word {
	const std::string text;
	const std::vector<int> neighbors;
	const int index;

	Word(std::string&& _text, std::vector<int>&& _neighbors, int _index):
		text(std::move(_text)), neighbors(std::move(_neighbors)), index(_index) {}
};

using Graph = std::vector<std::vector<int>>;
using CliqueList = std::vector<std::vector<int>>;

Graph load_graph(std::vector<std::string>* words) {
	std::cerr << "Loading graph..." << std::endl;
	Graph graph;
	for (std::string line; std::getline(std::cin, line);) {
		std::stringstream ss(line);
		std::string word;
		std::getline(ss, word, ',');
		std::vector<int> connected;
		for (std::string c; std::getline(ss, c, ',');) {
			connected.push_back(std::stoi(c));
		}
		std::sort(connected.begin(), connected.end());
		words->push_back(word);
		graph.push_back(connected);
	}
	return graph;
}

void find_cliques(
		const Graph& graph,
		std::vector<int>& words,
		int depth,
		const std::vector<int>& connections,
		CliqueList* cliques) {
	if (depth == 4) {
		for (const int c : connections) {
			words[4] = c;
			cliques->push_back(words);
		}
		return;
	}
	for (const int w : connections) {
		const std::vector<int>& v = graph.at(w);
		std::vector<int> common;
		common.reserve(std::min(v.size(), connections.size()));
		std::set_intersection(
				v.begin(), 
				v.end(),
				connections.begin(),
				connections.end(),
				std::back_inserter(common));
		if (common.empty()) {
			continue;
		}
		words[depth] = w;
		find_cliques(graph, words, depth+1, common, cliques);
	}
}

CliqueList find_cliques_thread(
		const Graph& graph,
		int begin_index,
		int end_index) {
	CliqueList cliques;
	for (int i=begin_index; i<end_index; ++i) {
		std::vector<int> words {i,0,0,0,0};
		find_cliques(graph, words, 1, graph[i], &cliques);
	}
	return cliques;
}

CliqueList find_all_cliques(const Graph& graph) {
	std::cerr << "Finding all cliques..." << std::endl;
	CliqueList cliques;

	const auto thread_count = std::thread::hardware_concurrency();
	// const auto thread_count = 1;
	const auto chunk_size = graph.size() / thread_count;
	std::cerr << "Using " << thread_count << " threads..." << std::endl;
	std::vector<std::future<CliqueList>> results;

	for (int i=0; i<thread_count; i++) {
		auto start = i * chunk_size;
		auto end = graph.size();
		if (i < thread_count - 1) {
			end = start + chunk_size;
		}
		results.push_back(std::async(&find_cliques_thread, graph, start, end));
	}
	for (auto& f : results) {
		CliqueList cl = f.get();
		cliques.insert(cliques.end(), cl.begin(), cl.end());
	}
	return cliques;
}

int main() {
	std::vector<std::string> words;
	Graph graph = load_graph(&words);
	CliqueList cliques = find_all_cliques(graph);
	for (const std::vector<int>& clique : cliques) {
		auto it = clique.begin();
		std::cout << words.at(*it);
		for (it = it+1; it != clique.end(); it++) {
			std::cout << ',' << words.at(*it);
		}
		std::cout << std::endl;
	}
	std::cerr << "Finished" << std::endl;
}
