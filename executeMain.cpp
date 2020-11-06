#include "preprocessing.cpp"

int main(int argc, char** argv){
    string lef_file_path,def_file_path;
	if(argc < 5){
		std::cout << "Error! insufficient parameter!";
		return 1;
	}
	else{
		argc--;
		argv++;
		while(argc--){
			if(strcmp(*argv, "-l")==0){
				argc--;
				argv++;
				lef_file_path = *argv;
				 std::cout<<"successfully read .lef file "
				 <<*argv<<std::endl;
			}
			else if(strcmp(*argv, "-d")==0){
				argc--;
				argv++;
				def_file_path = *argv;
				std::cout<<"successfully read .def file "
				<<*argv<<std::endl;
			}
			argv++;
		}
	cout << "start...";
    //search_def(def_file_path);

    //get Metal/M layer
    std::cout<<"getMLayer";
    getMLayer(lef_file_path, vec);
    //get maxArea
    std::cout<<"ger maxArea";
    search_def(def_file_path);
    //if need to write_lef
    std::cout<<"write lef file";
    if (!srchManuf(lef_file_path)){
        write_lef(lef_file_path);
    }
    //write track and pinData
    std::cout<<"write def file";
    write_def(def_file_path, AreaNum, vec);

    //if use deletePin_function , the file name will change
    //deletePT(exist_def);
	}
}