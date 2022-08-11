#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

int32_t word_to_bitmap(std::string w) {
	int32_t bitmap = 0;
	for (char c : w) {
		bitmap |= 1 << ((int)(c - 'a'));
	}
	return bitmap;
}

struct Word {
	std::string text;
	int32_t bitmap;
	int index;

	Word(std::string _text, int _index): text(_text), index(_index), bitmap(word_to_bitmap(_text)) {}
};

struct Row {
	std::string text;
	std::vector<int> neighbors;

	Row(): text(""), neighbors({}) {}
};

int main() {
	std::vector<Word> words;
	int i=0;
	for (std::string line; std::getline(std::cin, line); i++) {
		if (!line.empty()) words.push_back(Word(line, i));
	}
	std::vector<Row> graph;
	for (const Word& w1 : words) {
		Row r;
		r.text = w1.text;
		for (const Word& w2 : words) {
			if ((w1.bitmap & w2.bitmap) == 0 && w1.index < w2.index) {
				r.neighbors.push_back(w2.index);
			}
		}
		graph.push_back(r);
	}
	for (const Row& row : graph) {
		std::cout << row.text;
		for(const int i : row.neighbors) {
			std::cout <<','<< i;
		}
		std::cout << std::endl;
	}
}
