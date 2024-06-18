    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <regex>
    #include <cstdlib>

    const std::regex VALID_CHARACTERS(R"(\b\w+\b|\".+\"|\+|\-|\*|\/{1,2}|%|\^|#|&|~|\||<<|>>|={1,2}|~=|<=|>=|<|>|=|\(|\)|\{|\}|\[|\]|::|;|:|,|\\|\.{1,3})");
    const std::regex KEYWORDS(R"(\b(and|break|do|else|elseif|end|false|for|function|goto|if|in|local|nil|not|or|repeat|return|then|true|until|while)\b)");
    const std::regex OPERATORS(R"([+\-*/%^#&~|()]|<<|>>|//|={1,2}|~=|<=|>=|[<>]=?|[{}[\]]|::|[;:,.]|\.\.|\.{3})");

    void loadFromFile(const std::string& filename, std::string& result) 
    {

        std::ifstream fin(filename);
        std::string line;

        if (!fin.is_open()) 
        {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return;
        }

        while (std::getline(fin, line)) 
        {
            size_t comment_pos = line.find("--");

            if (comment_pos != std::string::npos) 
            {
                // Remove comment
                line = line.substr(0, comment_pos); 
            }
            // Remove spaces from the line
            //line.erase(remove(line.begin(), line.end(), '\n'), line.end());
            result += line + '\n';
        }

        fin.close();
    }

    std::vector<std::string> tokenize(const std::string& line) 
    {
        std::vector<std::string> tokens;

        auto words_begin = std::sregex_iterator(line.begin(), line.end(), VALID_CHARACTERS);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) 
        {
            std::smatch match = *i;
            std::string token = match.str();
            tokens.push_back(token);
        }

        return tokens;
    }

    void processTokens(const std::string& inputFilename, const std::string& outputFilename)
    {
        std::string input;
        std::ofstream outputFile(outputFilename);

        loadFromFile(inputFilename, input);
        std::stringstream inl(input);

        while (std::getline(inl, input)) 
        {
            for (const std::string& token : tokenize(input)) 
            {
                if (std::regex_match(token, KEYWORDS)) 
                {
                    outputFile << '<' << "KW_" << token << '>' << "\n";
                }
                else if (std::regex_match(token, OPERATORS)) 
                {
                    outputFile << "<operator> " << token << "\n";
                }
                else if (std::regex_match(token, std::regex(R"(\b[a-zA-Z]+[0-9]*\b)"))) 
                {
                    outputFile << "<ident> " << token << "\n";
                }
                else if (std::regex_match(token, std::regex(R"(\b\d+\b)"))) 
                {
                    outputFile << "<number> " << token << "\n";
                }
                else if (std::regex_match(token, std::regex(R"(\".+\")"))) 
                {
                    outputFile << "<string> " << token << "\n";
                }
            }
        }
        std::cout << inputFilename << " processed and " << outputFilename << " file created\n";
        outputFile.close();
        inl.clear();
    }

    int main(int argc, char** argv)
    {
        std::string inputFilename = "examples/sample.lua";
        std::string outputFilename = "examples/sample_result.txt";
        if (argc == 2) {
            inputFilename = argv[1];

            outputFilename = "examples/output.txt";
        }
        else if (argc == 3)
        {
            inputFilename = argv[1];
            outputFilename = argv[2];
        }
        //default example files
        processTokens(inputFilename,outputFilename);
        processTokens("examples/all.lua", "examples/all_results.txt");
        return 0;
    }


