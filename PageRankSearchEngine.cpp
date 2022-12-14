#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// for the list of links on a page
struct linkNode {
	int page;
	linkNode *next;
	linkNode(int p, linkNode *n) {page = p; next = n;}
};

// for the list of pages a word is on
struct pageNode {
	int page;
	pageNode *next;
	pageNode(int k, pageNode *n) {page = k; next = n;}
};

struct Page {
	string URL;
	linkNode *links;
	int nLinks;
	double weight;
	double newWeight;
	int scaledWeight;
	Page(string s) {URL = s; links = NULL; nLinks = 0;}
};

struct Word {
	pageNode *pageList;
	Word() {pageList = NULL;}
};

vector <Page*> pages;
vector <Word*> words;
unordered_map <string, int> urlPages;
unordered_map <string, int> wordMap;
int N_pages;

void readPages(string fileName) {
	// build hash table of NEWPAGE links
	ifstream fin;
	fin.open(fileName);
	int i = 0;
	string s;
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			urlPages[s] = i++;
		}
	}
	fin.close();
	N_pages = i;
	pages.reserve(N_pages);

	// build vector of pages
	fin.open(fileName);
	Page* curPage = NULL;
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			curPage = new Page(s);
			pages.push_back(curPage);
		} else {
			if (s.find("http://") != string::npos) {
				if (urlPages.find(s) != urlPages.end()) {
					curPage->nLinks++;
					curPage->links = new linkNode(urlPages[s], curPage->links);
				}
			}
		}
	}
	fin.close();
	cout << "Loaded webpages" << endl;
}

// Give each page initial weight 1 / N
// Repeat 50 times:
//   For each page i, set new_weight[i] = 0.1 / N.
//   For each page i,
//     For each page j (of t total) to which i links,
//       Increase new_weight[j] by 0.9 * weight[i] / t.
//   For each page i, set weight[i] = new_weight[i].
void rankPages() {
	for (int i = 0; i < (int)pages.size(); i++) {
		pages[i]->weight = 1.0/N_pages;
	}
	for (int k = 0; k < 50; k++) {
		for (int i = 0; i < (int)pages.size(); i++) {
			pages[i]->newWeight = 0.1/N_pages;
		}
		for (int i = 0; i < (int)pages.size(); i++) {
			linkNode *curLink = pages[i]->links;
			while (curLink != NULL) {
				Page *linkPage = pages[curLink->page];
				linkPage->newWeight += 0.9*pages[i]->weight/pages[i]->nLinks;
				curLink = curLink->next;
			}
		}
		for (int i = 0; i < (int)pages.size(); i++) {
			pages[i]->weight = pages[i]->newWeight;
		}
	}

	// compute scaled weights
	for (int i = 0; i < (int)pages.size(); i++) {
		pages[i]->scaledWeight = (int)(100.0*pages[i]->weight*N_pages);
	}
	cout << "Ranked webpages" << endl;
}

void indexPages(string fileName) {
	ifstream fin;
	fin.open(fileName);
	int curPage = 0;
	string s;
	int i = 0;
	while (fin >> s) {
		if (s == "NEWPAGE") {
			fin >> s;
			curPage = urlPages[s];
		} else {
			if (s.find("http://") == string::npos) {
				if (wordMap.find(s) == wordMap.end()) {
					wordMap[s] = i++;
					words.push_back(new Word());
				}
				Word *curWord = words[wordMap[s]];
				curWord->pageList = new pageNode(curPage, curWord->pageList);
			}
		}
	}
	fin.close();
	cout << "Loaded word index" << endl;
}

bool pageComp(Page *a, Page *b) {
	return (a->weight > b->weight);
}

string lowerString(string s) {
	std::transform(s.begin(), s.end(), s.begin(), 
		[](unsigned char c){ return std::tolower(c); });
	return s;
}

void getInput() {
	string s;
	cout << "\nInput search word: ";
	while (cin >> s) {
		s = lowerString(s);
		if (wordMap.find(s) != wordMap.end()) {
			Word *curWord = words[wordMap[s]];
			vector <Page*> results;
			pageNode *curPage = curWord->pageList;
			while (curPage != NULL) {
				results.push_back(pages[curPage->page]);
				curPage = curPage->next;
			}
			sort(results.begin(), results.end(), pageComp);
			cout << "Showing " << min(25, (int)results.size());
			cout << " of " << (int)results.size() << " results.\n";
			for (int i = 0; i < min(25, (int)results.size()); i++) {
				cout << (results[i]->scaledWeight) << " ";
				cout << results[i]->URL << endl;
			}
		} else {
			cout << "\nCould not find word" << endl;
		}
		cout << "\nInput search word: ";
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "Invalid argument usage. View README\n";
		return 1;
	}

	readPages(argv[1]);
	rankPages();
	indexPages(argv[1]);
	getInput();
	return 0;
}
