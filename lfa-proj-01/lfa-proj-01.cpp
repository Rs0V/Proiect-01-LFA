#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define PRINT_SWITCH 0

#define STR std::string
#define STR_LIST std::vector<STR>
#define DICT std::map<std::pair<STR, STR>, STR_LIST>
#define WORDS std::map<STR, STR_LIST>

std::ifstream fin("input.in");

STR char2str(const char c)
{
	STR str;
	str.push_back(c);
	return str;
}

int str2int(const STR& str)
{
	bool cast = false;
	int number = 0;
	for (char c : str)
	{
		if (c >= '0' && c <= '9')
		{
			cast = true;
			if(c == '0' && number > 0 || c > '0')
				number = (number + (c - '0')) * 10;
		}
	}
	number /= 10;
	return cast ? number : -1;
}

STR_LIST split_string(const STR& str)
{
	STR_LIST list;
	list.push_back("");
	for (char c : str)
		if (c == ' ')
			list.push_back("");
		else
			list.back().push_back(c);
	return list;
}

void automat_aux(const STR_LIST& end_states, const DICT& dict, const STR& input, const int input_index, const STR& curr_state, STR_LIST& state_path, WORDS& words)
{
	bool accepted = true;
	if (input_index < input.size())
	{
		auto state_iter = dict.find({ curr_state, char2str(input[input_index]) });
		if (state_iter != dict.end())
		{
			for (auto& state : state_iter->second)
			{
				state_path.push_back(state);
				automat_aux(end_states, dict, input, input_index + 1, state, state_path, words);
				state_path.pop_back();
			}
		}
		else
		{
			auto word_iter = words.find(input);
			if (word_iter == words.end())
				words.insert({ input, STR_LIST() });
		}
	}
	
	if (accepted)
	{
		bool acc = false;
		for (STR state : end_states)
			if (curr_state == state)
			{
				acc = true;
				break;
			}
		accepted = acc;
	}
	
	STR subinput = input.substr(0, input_index);
	auto word_iter = words.find(subinput);
	if (word_iter == words.end())
		if (accepted)
			words.insert({ subinput, state_path });
		else
			words.insert({ subinput, STR_LIST() });
	else if(word_iter->second.empty())
		if (accepted)
			word_iter->second = state_path;
}

void automat(STR& begin_state, STR_LIST& end_states, DICT& dict)
{
	bool run = true;
	while (run)
	{
		STR input;
		std::cin >> input;
		if (input == "/stop" || input == ".stop")
			run = false;
		else
		{
			bool accepted = true;
			STR curr_state = begin_state;
			STR_LIST state_path;
			state_path.push_back(curr_state);
			WORDS words;

			automat_aux(end_states, dict, input, 0, curr_state, state_path, words);

			// Afiseaza pt toate cuv. (substring-uri din cuv. original) sau doar pentru cuv. original 
			#if PRINT_SWITCH
			for (auto& word : words)
				if (word.first != "")
					if (!word.second.empty())
					{
						std::cout << "cuvantul " << word.first << " este acceptat\ndrumul: ";
						for (STR& state : word.second)
							std::cout << state << ' ';
						std::cout << "\n\n";
					}
					else
						std::cout << "cuvantul " << word.first << " nu este acceptat\n\n";
			#else
			auto word = words.find(input);
			if (word != words.end())
				if (!word->second.empty())
				{
					std::cout << "cuvantul " << word->first << " este acceptat\ndrumul: ";
					for (STR& state : word->second)
						std::cout << state << ' ';
					std::cout << "\n\n";
				}
				else
					std::cout << "cuvantul " << word->first << " nu este acceptat\n\n";
			#endif
		}
	}
}

int main()
{
	STR_LIST input_lines;
	STR begin_state;
	STR_LIST end_states;

	while (!fin.eof())
	{
		STR line;
		std::getline(fin, line);
		input_lines.push_back(line);
	}

	DICT dict;

	for (int i = 0; i < input_lines.size() - 1; ++i)
	{
		STR_LIST vstr = split_string(input_lines[i]);
		
		auto state_iter = dict.find({ vstr[0], vstr[1] });
		if (state_iter != dict.end())
			state_iter->second.push_back(vstr[2]);
		else
			dict.insert({ { vstr[0], vstr[1] }, STR_LIST(1, vstr[2]) });

		int bs_nr = str2int(begin_state);
		int state0_nr = str2int(vstr[0]);
		int state1_nr = str2int(vstr[2]);

		if (bs_nr == -1)
			bs_nr = 999999999;
		if (bs_nr > state0_nr)
			begin_state = vstr[0], bs_nr = state0_nr;
		if (bs_nr > state1_nr)
			begin_state = vstr[2];
	}
	end_states = split_string(input_lines.back());

	automat(begin_state, end_states, dict);
	
	return 0;
}