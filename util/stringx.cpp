
#include "stringx.h"


namespace util{
    void removeAllOccurrences(std::string& str, const std::string& sub_str){
        auto n = str.find(sub_str);
        while(n != std::string::npos){
            str.replace(n,sub_str.size(),"");
            n = str.find(sub_str);
        }
    }
}