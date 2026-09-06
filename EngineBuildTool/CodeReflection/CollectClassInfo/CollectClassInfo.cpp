#include "CollectClassInfo.h"

namespace CollectClassInfo
{
    const char spaceString[] = " ";
    const char colonString[] = ":";
    const char commaString[] = ",";
    const char leftParenthesisString[] = "(";
    const char rightParenthesisString[] = ")";
    const char starString[] = "*";
    const char fetchAddressString[] = "&";
    bool Collection(const string& paths, FClassAnalysis& classAnalysis)
    {
        vector<string> stringArray;
        simple_cpp_helper_file::load_file_to_strings(paths, stringArray);
        
        for (int i = 0; i < stringArray.size(); i++)
        {
            string& row = stringArray[i];
            char* rowPtr = const_cast<char*>(row.c_str());
            
            auto contain = [&](const string& inSubString) -> bool
            {
                return simple_cpp_string_algorithm::string_contain(rowPtr, inSubString.c_str());
            };
            
            if (contain("GENERATED_BODY"))
            {
                classAnalysis.codeLine = i + 1;
            }
            
            //获取类名和继承名
            if (
                (contain("\tclass") ||contain("class"))
                && contain(":")
                && (contain("public") || contain("protected") || contain("private"))
            )
            {
                remove_string_start(rowPtr, "class");
                remove_string_start(rowPtr, "\tclass");

                if (contain("_API"))
                {
                    trim_start_inline(rowPtr);
                    
                    char l[1024] = { 0 };   //XXXXX_API
                    char r[1024] = { 0 };
                    
                    split(rowPtr, spaceString, l, r, false);
                    
                    classAnalysis.APIName = l;
                    
                    row = r;
                }
                
                vector<string> elementStr;
                simple_cpp_string_algorithm::parse_into_vector_array(rowPtr, elementStr, colonString);
                
                //获取类名
                trim_start_and_end_inline(const_cast<char*>(elementStr[0].c_str()));
                classAnalysis.className = elementStr[0];
                
                //多继承
                if (elementStr.size() > 1)
                {
                    vector<string> inheritStr;
                    simple_cpp_string_algorithm::parse_into_vector_array(
                        elementStr[1].c_str(),
                        inheritStr, 
                        commaString
                    );
                    
                    for (auto& inheritStr : inheritStr)
                    {
                        char* tmpPtr = const_cast<char*>(inheritStr.c_str());
                        trim_start_inline(tmpPtr);
                        
                        remove_string_start(tmpPtr, "public");
                        remove_string_start(tmpPtr, "protected");
                        remove_string_start(tmpPtr, "private");
                        
                        trim_start_and_end_inline(tmpPtr);
                        
                        classAnalysis.inheritNames.push_back(tmpPtr);
                    }
                }
                
            }
            
        }
        
        return false;
    }
}