#include "utility.hh"

static void init() {

    string root = "fw-mobile";
    string path = __FILE__;

    size_t pos = path.find(root);
    if (pos != string::npos) {
        string dir = path.substr(0, pos + root.size());
        chdir(dir);
        closeto.log("work path: %s", dir.c_str());
    } else {
        closeto.log("failed to change the path");
    }
}

static void startup() {
    
    extern bool codecount_launch;
    extern bool cpp_launch;
    extern bool objcpp_launch;
    
    void codecount_main();
    void cpp_main();
    void objcpp_main();
    
    if (codecount_launch) { codecount_main(); }
    if (cpp_launch      ) { cpp_main      (); }
    if (objcpp_launch   ) { objcpp_main   (); }
}

int main() {
    init();
    startup();
    medi(1);
}