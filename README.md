
# C++ Text File Processing

This C++ program searches for text files in a given directory and its subdirectories, and performs various operations on the text files, such as searching for words or tags, replacing words or tags, creating a set of unique tags, and creating a folder.
## Requirements
A C++ compiler (e.g. GCC)
## Usage
1. Clone the repository: git clone https://github.com/frknksp/File-operations.git
2. Navigate to the directory: cd cpp-text-file-processing
3. Compile the program: g++ main.cpp -o main
4. Run the program: ./main
## Functions
- anamenu(): Displays a menu for the user to choose an option from.
- findwordortag(std::vector<std::string> onlytextpaths): Searches for a given word or tag in the text files specified in the onlytextpaths parameter.
- findandreplace(std::vector<std::string> onlytextpaths): Searches for a given word or tag in the text files specified in the onlytextpaths parameter, and replaces it with another word or tag.
- replace_all(std::string& s,std::string const& toReplace,std::string const& replaceWith): Replaces all occurrences of a given string with another string in a given string.
- setbastir(std::set<std::string> printarray): Prints the elements of a given set.
- bastir( std::vector<std::string> printarray): Prints the elements of a given vector.
- recFileList(char *basePath, std::vector<std::string> &allpaths, std::vector<std::string> &filenames): Searches for text files in a given directory and its subdirectories and stores their paths and names in the allpaths and filenames vectors, respectively.
- openfile(std::vector<std::string> allpaths, std::vector<std::string> &contentoftexts): Opens the text files specified in the allpaths parameter and stores their contents in the contentoftexts vector.
- findsquarebrackets(std::vector<std::string> contentoftexts, std::vector<std::string> &tagwords): Searches for tags in the contentoftexts vector and stores them in the tagwords vector. A tag is a string enclosed in square brackets.
- yetimorregulartag(std::vector<std::string> tagwords, std::vector<std::string> filenames): Processes the tags in the tagwords vector and stores them in the nonrepetittags set and the etiket set.
- createfolder(): Creates a folder.


