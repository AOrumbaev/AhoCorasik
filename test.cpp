#include <iostream>
#include "AhoCorasik.h"
using namespace std;

typedef struct _bs 
{
	int a : 31;
	int b : 1;
}bs_t;

int main() 
{

	AhoCorasik trie;
	trie.add_string(std::string("he"));
	trie.add_string(std::string("she"));
	trie.add_string(std::string("his"));
	trie.add_string(std::string("hers"));

	trie.build_fail_transactions();
	std::set<std::string> out;
	trie.retrieve(std::string("ushers"), out);
	for (std::set<std::string>::iterator iter = out.begin(); iter != out.end(); ++iter) {
		cout << *iter << endl;
	}
	return 0;
}
