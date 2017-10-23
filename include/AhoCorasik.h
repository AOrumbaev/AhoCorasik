#ifndef _AHO_CORASIK_H_PROTECTED_
#define _AHO_CORASIK_H_PROTECTED_
#include <stdint.h>
#include <vector>
#include <queue>
#include <string>
#include <set>

#define TRIE_ERROR_INDEX -1

class AhoCorasik
{
public:
	static const int max_char_size = 26;
	typedef struct _ac_node_t {
		uint32_t fail : 31;
		uint32_t is_final : 1;
		int32_t parent_index;
		int32_t next[max_char_size];
		_ac_node_t() {
			fail = 0;
			is_final = 0;
			parent_index = 0;
			memset(next, TRIE_ERROR_INDEX, sizeof(next));
		}
	}ac_node_t;

	typedef std::vector<ac_node_t> trie_t;
public:
	AhoCorasik();
	~AhoCorasik();

	void add_string(std::string& word);
	void build_fail_transactions();
	bool retrieve(std::string& str, std::set<std::string>& out);
protected:
	
	void set_fail(int index, int fail_index) {
		if (index < 0 || index >= (int)_trie.size() || fail_index < 0 || fail_index >= (int)_trie.size()) {
			return;
		}
		_trie[index].fail = fail_index;
	}
protected:
	trie_t _trie;
};

AhoCorasik::AhoCorasik()
{
	ac_node_t root;
	_trie.push_back(root);
}

AhoCorasik::~AhoCorasik()
{
}

void AhoCorasik::add_string(std::string& word)
{
	if (word.empty()) {
		return;
	}

	int32_t cur_index = 0;
	for (std::string::size_type i = 0; i < word.size(); ++i) {
		int32_t code = word[i] - 'a';
		if (_trie[cur_index].next[code] == TRIE_ERROR_INDEX) {
			ac_node_t new_node;
			new_node.parent_index = cur_index;
			_trie.push_back(new_node);
			_trie[cur_index].next[code] = _trie.size() - 1;
		}
		cur_index = _trie[cur_index].next[code];
	}
	_trie[cur_index].is_final = 1;
}

void AhoCorasik::build_fail_transactions()
{
	std::queue<int> q;
	q.push(0);
	_trie[0].fail = 0;
	while (!q.empty()) {
		int cur_index = q.front();
		q.pop();
		for (int i = 0; i < max_char_size; ++i) {
			int index = _trie[cur_index].next[i];
			if (index != TRIE_ERROR_INDEX) {
				
				if (cur_index == 0) {
					set_fail(index, 0);
					q.push(index);
					continue;
				}
				
				int tmp_index = _trie[cur_index].fail;
				while (tmp_index != 0 && _trie[tmp_index].next[i] == TRIE_ERROR_INDEX) {
					tmp_index = _trie[tmp_index].fail;
				}
				if (_trie[tmp_index].next[i] != TRIE_ERROR_INDEX) {
					set_fail(index, _trie[tmp_index].next[i]);
				}else {
					set_fail(index, 0);
				}
				q.push(index);
			}
		}
	}
}

bool AhoCorasik::retrieve(std::string& str, std::set<std::string>& out)
{
	if (_trie.empty()) {
		return false;
	}
	std::vector<std::pair<int, int> > states;
	int cur_index = 0;
	ac_node_t* p = &_trie[cur_index];

	int index = 0;
	for (std::string::size_type i = 0; i < str.size();) {
		int32_t code = str[i] - 'a';
		if (_trie[index].next[code] != TRIE_ERROR_INDEX) {
			index = _trie[index].next[code];
			if (_trie[index].is_final) {
				states.push_back(std::pair<int, int>(index, i));
			}
			++i;
		}else {
			if (index == 0) {
				i++;
			}else {
				index = _trie[index].fail;
			}
		}
	}
	for (std::vector<std::pair<int, int> >::size_type i = 0; i < states.size(); ++i) {
		int offset = states[i].second;
		index = states[i].first;
		while (index != 0) {
			offset--;
			index = _trie[index].parent_index;
		}
		out.insert(str.substr(offset + 1, states[i].second - offset));
	}

	return true;
}
#endif
