#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

std::string exec(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
        {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}
/*
int main()
{
    string ret = exec("ping -c4 192.168.137.1");

    cout << "---------------------------------------" << endl;
    cout << ret << endl;
    cout << "---------------------------------------" << endl;
    cout << ret.substr(5,14) << endl;

    return 0;

}
*/
