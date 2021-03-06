#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <cassert>
#include <algorithm>

using namespace std;

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

struct Term {
	Term(string s) {
		ltrim(s);
		while (s.find(" ") != string::npos) {
			string id = s.substr(0, s.find(' '));
			trim(id);
			ids.push_back(id);
			s.erase(0, s.find(" ") + 1);
		}

		trim(s);
		ids.push_back(s);
	}

    string to_string(int pos = 0) const {
        string s;
        for(; pos < ids.size(); pos++){
            s += ids[pos] + " ";
        }
        s.pop_back();
        assert(s.back() != ' ');
        return s;
    }
	vector<string> ids;
};

struct Production {
public:
	Production(string prod) {
		const string& str = prod;
		auto ind = str.find("->");
		if (ind == string::npos) {
			cout << "Production " << prod << " is Invalid\n";
			exit(0);
		}
		string non_terminal = str.substr(0, ind);
		trim(non_terminal);
		
		lhs = non_terminal;

		string rhs = str.substr(ind + 2);				// -> skip 2 chars for arrow
		vector<string> curr_prod;
		while (rhs.find('|') != string::npos) {
			string production = rhs.substr(0, rhs.find('|'));
			trim(production);

			rhs.erase(0, rhs.find('|') + 1);
			Term term(production);
			terms.push_back(term);
		}
		trim(rhs);
		terms.push_back(Term(rhs));
	}
public:
	string lhs;
	vector<Term> terms;
};

struct Grammar {
public:
	Grammar(const vector<string>& input) {
		for(int i = 0; i < input.size(); i++) {
			Production p(input[i]);
			prods.emplace(p.lhs, p);
			//prods[p.lhs] = p;
			non_terminals.insert(p.lhs);
			if (i == 0)	follows[p.lhs].insert("$");
		}

		for (const auto& prod : prods) {
			for (const auto& term : prod.second.terms) {
				for (const auto& id : term.ids) {
					if (non_terminals.find(id) == non_terminals.end()) {
						terminals.insert(id);
					}
				}
			}
		}

	}

    void remove_left_recursion() {
        vector<Production> new_productions;
        for(const auto& pr : prods)
            new_productions.push_back(pr.second);

        for(int i = 0; i < new_productions.size(); i++){
            for(int j = 0; j < i; j++){
                vector<Term> new_rhs;
                for(int k = 0; k < new_productions[i].terms.size(); k++){
                    if(new_productions[i].terms[k].ids.size()
                        && new_productions[i].terms[k].ids[0] == new_productions[j].lhs){
                            for(int l = 0; l < new_productions[j].terms.size(); l++){
                                Term new_term(new_productions[j].terms[l].to_string() + " " + new_productions[i].terms[k].to_string());
                                new_rhs.push_back(new_term);
                            }
                    }else if(new_productions[i].terms[k].ids.size()) {
                        new_rhs.push_back(new_productions[i].terms[k]);
                    }
                }

                new_productions[i].terms = new_rhs;
            }

            bool has_direct_recursion = false;
            for(int j = 0; j < new_productions[i].terms.size(); j++){
                assert(new_productions[i].terms[j].ids.size());
                if(new_productions[i].terms[j].ids[0] == new_productions[i].lhs){
                    has_direct_recursion = true;
                    break;
                }
            }
            if(!has_direct_recursion)
                continue;
            
            string extra_production = new_productions[i].lhs;
            // extra_production += "'";
            while(find_if(new_productions.begin(), new_productions.end(), [&extra_production](const Production& curr){
                return curr.lhs == extra_production;
            }) != new_productions.end()){
                extra_production += "'";
            }

            vector<Term> rhs_extra_production;
            vector<Term> curr_production_rhs;
            for(int j = 0; j < new_productions[i].terms.size(); j++){
				//cout << "Current Production has " << new_productions[i].terms.size() << " size\n";
                assert(new_productions[i].terms[j].ids.size());
                if(new_productions[i].terms[j].ids[0] == new_productions[i].lhs){
                    string temp = new_productions[i].terms[j].to_string(1);
                    temp += " " + extra_production;
                    rhs_extra_production.emplace_back(temp);
                }else{
                    if(new_productions[i].terms[j].ids[0] == "??"){
                        curr_production_rhs.emplace_back(extra_production);
                    }else{
                        // curr_production_rhs.e
                        curr_production_rhs.push_back(new_productions[i].terms[j]);
                        curr_production_rhs.back().ids.push_back(extra_production);
                    }
                }
            }

            new_productions[i].terms = curr_production_rhs;
            rhs_extra_production.emplace_back("??");
            string temp;
            for(int i = 0; i < rhs_extra_production.size(); i++){
                if(i != 0)
                    temp += " | ";
                temp += rhs_extra_production[i].to_string();
            }
            string line = extra_production + " -> " + temp;
            new_productions.emplace_back(line);
        }

        prods.clear();
        for(auto& prod : new_productions)
            prods.emplace(prod.lhs, prod);
    }

	vector<string> find_first(const string& identifier) {
		{
			auto it = firsts.find(identifier);
			if (it != firsts.end()) {
				return it->second;
			}

			if (terminals.find(identifier) != terminals.end())
				return { identifier };
		}

		assert(non_terminals.find(identifier) != non_terminals.end());		// assert we are only finding first of non_terminals
		auto it = prods.find(identifier);
		assert(it != prods.end());
		const Production& curr = it->second;
		vector<string> my_firsts;

		for (const auto& term : curr.terms) {			// A -> a A | b A here there are 2 terms on rhs
			bool has_null_production = true;
			for (const auto& id : term.ids) {			// A -> a A		this term has 2 ids 'a' and 'A'
				if (id == identifier || id == "??")
					continue;
				vector<string> first = find_first(id);
				auto null_pos = find(first.begin(), first.end(), "??");
				my_firsts.insert(my_firsts.end(), first.begin(), first.end());

				has_null_production &= (null_pos != first.end());
				if (!has_null_production)
					break;
			}
			if (has_null_production)
				my_firsts.push_back("??");
		}

		firsts[identifier] = my_firsts;
		return my_firsts;
	}

	set<string> find_follow(string identifier) {
		assert(non_terminals.find(identifier) != non_terminals.end());		// we always call find_follow on non terminals;
		
		set<string> follow;
		for (const auto& prod : prods) {
			for (const auto& term : prod.second.terms) {
				for (int i = 0; i < term.ids.size(); i++) {
					if (term.ids[i] != identifier)
						continue;
					bool has_null_production = true;
					for (int j = i + 1; j < term.ids.size(); j++) {
						const auto& id = term.ids[j];
						const vector<string>& fir = find_first(id);

						bool curr_has_null = false;
						for (const auto& tok : fir) {
							if (tok == "??") {
								curr_has_null = true;
							}
							else {
								follow.insert(tok);
							}
						}
						has_null_production &= curr_has_null;
						if (!has_null_production)
							break;
					}
					if (has_null_production) {		// all id following us had null so follow(identifier) = follow(lhs)
						if (identifier == prod.second.lhs)
							continue;
						const set<string>& fir_lhs = find_follow(prod.second.lhs);
						for (const auto& tok : fir_lhs)
							follow.insert(tok);
					}
				}
			}
		}

		follows[identifier].insert(follow.begin(), follow.end());
		return follows[identifier];
	}

	map<string, Production> prods;
private:
	set<string> terminals, non_terminals;
	map<string, vector<string>> firsts;
	map<string, set<string>> follows;
};

int main() {
	cout << "Enter Symbol ?? for null production copy from this line if necessary\n";
	vector<string> input;
	string temp;

	while (getline(cin, temp)) {
		input.push_back(temp);
	}

	Grammar grammar(input);
    grammar.remove_left_recursion();

	cout << "New Grammar after removing left recursion is as follows\n";
    for(const auto& production : grammar.prods){
        const auto& curr = production.second;
        cout << curr.lhs << " -> ";
        for(int i = 0; i < curr.terms.size(); i++){
            cout << curr.terms[i].to_string();
            if((i + 1) != curr.terms.size())
                cout << " | ";
        }
        cout << endl;
    }
	return 0;
}

/*
E  -> T E'
E' -> + T E' | ??
T  -> F T'
T' -> * F T' | ??
F  -> ( E ) | id
*/
