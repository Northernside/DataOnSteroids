#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
using namespace std;

void help_menu()
{
    cout << "Usage: ./main [options]" << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help \t\t\t\t\t\t   Show this help message" << endl;
    cout << "  -v, --version \t\t\t\t\t   Show version" << endl;
    cout << "  -s, --split <input file> <output directory> \t\t   Split file into chunks" << endl;
    cout << "  -c, --count <input file> \t\t\t\t   Count lines in file" << endl;
    cout << "  -m, --merge <input file 1> <input file 2> <output file>  Merge two files" << endl;
    cout << "  -csv, --csv <input file> <output file> <delimiter> <column number> \t   Convert CSV to TXT" << endl;
}

void version()
{
    cout << "DOS v0.1.0" << endl;
}

void split(char *argv[])
{
    if (argv[2] == NULL)
    {
        cout << "[Split] No input file specified!" << endl;
        return;
    }

    if (argv[3] == NULL)
    {
        cout << "[Split] No output directory specified!" << endl;
        return;
    }

    cout << "[Split] Loading file..." << endl;
    ifstream input(argv[2]);
    string line;

    stringstream buffer;
    buffer << input.rdbuf();
    string fb = buffer.str();

    vector<string> chunks;
    string::size_type pos = 0;
    string::size_type prev = 0;
    const string delimiter = "\n";
    cout << "[Split] Splitting chunks..." << endl;
    while ((pos = fb.find(delimiter, prev)) != string::npos)
    {
        string ticket = fb.substr(prev, pos - prev);
        chunks.push_back(ticket);
        prev = pos + delimiter.length();
    }

    cout << "[Split] Writing chunks..." << endl;
    chunks.push_back(fb.substr(prev));
    for (size_t i = 0; i < chunks.size(); ++i)
    {
        ofstream output(argv[3] + to_string(i) + "_" + argv[2]);
        output << chunks[i];
        output.close();
    }

    cout << "[Split] Done!" << endl;
}

void count(char *argv[])
{
    if (argv[2] == NULL)
    {
        cout << "[Count] No input file specified!" << endl;
        return;
    }

    cout << "[Count] Loading file..." << endl;
    ifstream inFile(argv[2]);
    cout << "[Count] Counted " << count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n') + 1 << " lines!" << endl;
}

void to_lowercase(string &str)
{
    for (char &c : str)
    {
        c = tolower(c);
    }
}

void merge(char *argv[])
{
    if (argv[2] == NULL)
    {
        cout << "[Merge] No 1st input file specified!" << endl;
        return;
    }

    if (argv[3] == NULL)
    {
        cout << "[Merge] No 2nd input file specified!" << endl;
        return;
    }

    if (argv[4] == NULL)
    {
        cout << "[Merge] No output file specified!" << endl;
        return;
    }

    ifstream file1(argv[2]);
    vector<string> content1;
    string line;

    while (getline(file1, line))
    {
        content1.push_back(line);
    }

    file1.close();

    ifstream file2(argv[3]);
    vector<string> content2;
    while (getline(file2, line))
    {
        content2.push_back(line);
    }

    file2.close();

    cout << "Full length: " << content1.size() + content2.size() << endl;
    for (string &line : content1)
    {
        to_lowercase(line);
    }

    for (string &line : content2)
    {
        to_lowercase(line);
    }

    set<string> unique(content1.begin(), content1.end());
    unique.insert(content2.begin(), content2.end());

    cout << "Unique length: " << unique.size() << endl;

    ofstream outputFile(argv[4]);
    for (const string &line : unique)
    {
        outputFile << line << endl;
    }

    outputFile.close();
}

void csv_convert(char *argv[])
{
    if (argv[2] == NULL)
    {
        cout << "[CSV] No input file specified!" << endl;
        return;
    }

    if (argv[3] == NULL)
    {
        cout << "[CSV] No output file specified!" << endl;
        return;
    }

    if (argv[4] == NULL)
    {
        cout << "[CSV] No delimiter specified!" << endl;
        return;
    }

    if (argv[5] == NULL)
    {
        cout << "[CSV] No column number specified!" << endl;
        return;
    }

    ifstream input(argv[2]);
    string line;

    stringstream buffer;
    buffer << input.rdbuf();
    string fb = buffer.str();

    vector<string> chunks;
    string::size_type pos = 0;
    string::size_type prev = 0;
    const string delimiter = "\n";
    while ((pos = fb.find(delimiter, prev)) != string::npos)
    {
        string ticket = fb.substr(prev, pos - prev);
        chunks.push_back(ticket);
        prev = pos + delimiter.length();
    }

    chunks.push_back(fb.substr(prev));

    for (int i = 0; i < chunks.size(); i++)
    {
        string::size_type pos = 0;
        string::size_type prev = 0;
        const string delimiter = argv[4];
        const int column = stoi(argv[5]);
        vector<string> tokens;
        while ((pos = chunks[i].find(delimiter, prev)) != string::npos)
        {
            string token = chunks[i].substr(prev, pos - prev);
            tokens.push_back(token);
            prev = pos + delimiter.length();
        }

        tokens.push_back(chunks[i].substr(prev));

        ofstream output(argv[3], ios::app);
        output << tokens[column] << endl;
        output.close();
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (string(argv[1]) == "-v" || string(argv[1]) == "--version")
        {
            version();
        }
        else if (string(argv[1]) == "-h" || string(argv[1]) == "--help")
        {
            help_menu();
        }
        else if (string(argv[1]) == "-s" || string(argv[1]) == "--split")
        {
            split(argv);
        }
        else if (string(argv[1]) == "-c" || string(argv[1]) == "--count")
        {
            count(argv);
        }
        else if (string(argv[1]) == "-m" || string(argv[1]) == "--merge")
        {
            merge(argv);
        }
        else if (string(argv[1]) == "-csv" || string(argv[1]) == "--csv")
        {
            csv_convert(argv);
        }
        else
        {
            cout << "Invalid options - ";
            help_menu();
        }
    }
    else
    {
        help_menu();
    }

    return 0;
}