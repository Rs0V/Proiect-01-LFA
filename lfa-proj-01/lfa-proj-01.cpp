#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define DICT std::map<std::pair<std::string, std::string>, std::string>

std::ifstream fin("input.in");

std::string char2str(char c)
{
	std::string str;
	str.push_back(c);
	return str;
}

int str2int(const std::string& str)
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

std::vector<std::string> split_string(const std::string& str)
{
	std::vector<std::string> list;
	list.push_back("");
	for (char c : str)
		if (c == ' ')
			list.push_back("");
		else
			list.back().push_back(c);
	return list;
}

int main()
{
	std::vector<std::string> input_lines;
	std::string begin_state;
	std::vector<std::string> end_states;

	while (!fin.eof())
	{
		std::string line;
		std::getline(fin, line);
		input_lines.push_back(line);
	}

	DICT dict;

	for (int i = 0; i < input_lines.size() - 1; ++i)
	{
		std::vector<std::string> vstr = split_string(input_lines[i]);
		dict.insert({ { vstr[0], vstr[1] }, vstr[2] });

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

	bool run = true;
	while (run)
	{
		std::string input;
		std::cin >> input;
		if (input == "/stop")
			run = false;
		else
		{
			bool accepted = true;
			std::string curr_state = begin_state;
			std::vector<std::string> state_path;
			state_path.push_back(curr_state);

			for (char c : input)
			{
				auto state_iter = dict.find({ curr_state, char2str(c) });
				if (state_iter != dict.end())
				{
					curr_state = state_iter->second;
					state_path.push_back(curr_state);
				}
				else
				{
					accepted = false;
					break;
				}
			}
			if (accepted)
			{
				bool acc = false;
				for (std::string state : end_states)
					if (curr_state == state)
					{
						acc = true;
						break;
					}
				accepted = acc;
			}
			if (accepted)
			{
				std::cout << "acceptat\ndrumul: ";
				for (std::string state : state_path)
					std::cout << state << ' ';
				std::cout << "\n\n";
			}
			else
				std::cout << "cuvantul nu este acceptat\n\n";
		}
	}

	return 0;
}