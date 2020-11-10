#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;
vector<string> vec;

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool thereIsPlace (string const &fullString, string const &partString){
    int idx;
    idx = fullString.find(partString);
    if(idx == string::npos) return 0;
    else return 1;
}

int increaseArea (int num){
    int i = 1;
    while( num/i >= 100)
        i *= 10;
    return (num/i+1)*i +50000;
}

//deal with lef file
void getMLayer(string filename, vector<string>& vec_list){
    ifstream lef(filename);
    string str;
    int i;

    while (!lef.eof()) {
        getline(lef, str);
        if (str.find("LAYER") == 0) {
            int pos1 = str.find(" METAL");
            int pos2 = str.find(" M");
            if (pos1 != string::npos) {
                vec_list[i] = str.substr(pos1 + 1, 6);
                i++;
            } else if (pos2 != string::npos) {
                vec_list.push_back(str.substr(pos2 + 1, 2));
            }
        }


    }
    lef.close();
}

bool srchManuf(string filename) {
	ifstream lef(filename);
	string str;

	while (!lef.eof()) {
		getline(lef, str);
		int pos1 = str.find("MANUFACTURINGGRID");
		if (pos1 != string::npos){
			return 1;
		}
	}
	return 0;
}

void write_lef(string filename) {
    ifstream lef(filename);
    ofstream outlef("Add_Lef.lef");
    string str;
    while (!lef.eof()) {
        getline(lef, str);
        if (str.find("END UNITS") == 0) {
            outlef << str << endl;
            outlef << "MANUFACTURINGGRID " << "0.0005 " << ";";
        } else
            outlef << str << '\n';
    }
    lef.close();
    outlef.close();

}

//deal with def file
int AreaNum;

void search_def(string filename) {
    ifstream def(filename);
    string str;
    string a;
    int m = 0;
    while (!def.eof()) {
        getline(def, str);
        if(thereIsPlace(str, "PLACED")) {
            for(int i = 0; i < str.length(); i++){  //21->str.find("PLACED")
                if(str[i]>='0'&&str[i]<='9'){
                    a = a + str[i];
                }
                else if((str[i-1]>='0'&&str[i-1]<='9')&&str[i] == ' '){
                    if(atoi(a.c_str()) > m) {
                        m = atoi(a.c_str());
                    }
                    a = "";
                }

            }
        }
    }
    AreaNum = increaseArea(m);
    def.close();
}

void deletePT(string filename){
    ifstream def(filename);
    ofstream out("out.def");
    string str;
    int idx1, idx2, idx3, idx4, idx5;
    string a;

    while (!def.eof()) {
        getline(def, str);
        idx1 = str.find("+ LAYER METAL");
        idx2 = str.find("+ FIXED");
        idx3 = str.find("USE SIGNAL");
        idx4 = str.find("DIEAREA");
        idx5 = str.find("TRACK");

        if (idx4 != string::npos or idx5 != string::npos){
            out << "";
        }
        else if (idx3 != string::npos){
            for(int i=0; i <= idx3+10; i++){
                a = a + str[i];
            }
            out << a << ";" << endl;
            a = "";
        }
        else if (idx1 == string::npos && idx2 == string::npos){
            out << str <<endl;
        }
    }
}

void write_def(string filename, int AreaNum, vector<string>& vec_list) {
    ifstream def(filename);
    ofstream outdef("Add_Def.def");
    string str;
    int q = AreaNum/500;
    double paramtr[] = {300, 300,  500, 500};
    int pinx, piny;
    piny = AreaNum;
    pinx = 0;

    while (!def.eof()) {
        getline(def, str);
        if (str.find("UNITS") == 0) {
            outdef << str << endl;
            outdef << "DIEAREA " << "( 0 0 ) " << "( " << AreaNum << " " << AreaNum << " ) " << ";" << endl;
            outdef <<endl;
            for (int i = 0; i < vec_list.size(); i++) {
                outdef << "TRACKS" << " X " << paramtr[0] << " DO " << q << " STEP "
                       << paramtr[2]  << " LAYER " << vec_list[i] << " ;" << endl;
                outdef << "TRACKS" << " Y " << paramtr[1] << " DO " << q << " STEP "
                       << paramtr[3]  << " LAYER " << vec_list[i]  << " ;" << endl;
            }
        } else if (hasEnding(str, "SIGNAL") == 1) {
            if(pinx < AreaNum && piny == AreaNum) {
                outdef << str << " + " << "LAYER " << vec_list[vec_list.size()-1] << " (-140 0) (140 280)" << " + "
                       << "FIXED" << " ( " << pinx << " " << piny << " ) " << "S" << endl;
                pinx += 5000;
            }
            else if (piny > 0 && (AreaNum-5000<pinx<AreaNum+5000)) {
                pinx = AreaNum;
                piny -= 5000;
                outdef << str << " + " << "LAYER " << vec_list[vec_list.size()-1] << " (-140 0) (140 280)" << " + "
                       << "FIXED" << " ( " << pinx << " " << piny << " ) " << "S" << endl;
            }
            else if (pinx > 0 && (-5000<piny<5000)){
                piny = 0;
                pinx -= 5000;
                outdef << str << " + " << "LAYER " << vec_list[vec_list.size()-1] << " (-140 0) (140 280)" << " + "
                       << "FIXED" << " ( " << pinx << " " << piny << " ) " << "S" << endl;
            }
            else if (piny < AreaNum) {
                pinx = 0;
                piny += 5000;
                outdef << str << " + " << "LAYER " << vec_list[vec_list.size()-1] << " (-140 0) (140 280)" << " + "
                       << "FIXED" << " ( " << pinx << " " << piny << " ) " << "S" << endl;
            }
            else {
                cout << "Error! Pin is too much!";
                break;
            }
        } else outdef << str << '\n';
    }
    def.close();
    outdef.close();
}



