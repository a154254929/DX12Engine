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
    const char codeType[] = "CodeType";
    
    bool GetCodeTypeByFunction(const string& rowString, FFunctionAnalysis& functionAnalysis)
    {
        char* rowPtr = const_cast<char*>(rowString.c_str());
                    
        char l[1024] = { 0 };
        char r[1024] = { 0 };
                    
        split(rowPtr, codeType, r, l, false);
        
        vector<string> elementStr;
        simple_cpp_string_algorithm::parse_into_vector_array(l, elementStr, commaString);
        
        if (elementStr[0].find("Event") != string::npos)
        {
            functionAnalysis.codeType = "Event";
            return true;
        }
        else if (elementStr[0].find("Describe") != string::npos)
        {
            functionAnalysis.codeType = "Describe";
            return true;
        }
        
        return false;
    }
    
    FParamElement CollectionVariableType(
        std::string variable,
        ECollectionParamType collectionParamType = ECollectionParamType::CollectionParamType_Param
    )
    {
        char* ptr = const_cast<char*>(variable.c_str());
        
        FParamElement paramElement;
        paramElement.name = "ReturnValue";
        
        if (simple_cpp_string_algorithm::string_contain(variable, starString))
        {
            remove_char_end(ptr, '*');
            paramElement.bPointer = true;
        }
        
        if (simple_cpp_string_algorithm::string_contain(variable, fetchAddressString))
        {
            remove_char_end(ptr, '&');
            paramElement.bReference = true;
        }
        
        if (simple_cpp_string_algorithm::string_contain(variable, "const"))
        {
            trim_start_and_end_inline(ptr);
            remove_string_start(ptr, "const");
            paramElement.bConst = true;
        }
        
        trim_start_and_end_inline(ptr);
        remove_char_start((const_cast<char*>(variable.c_str())), '\t');
        
        paramElement.type = variable;
        
        return paramElement;
    }
    
    bool GetCodeTypeByPropn(
        const string& rowString,
        FVariableAnalysis& variableAnalysis)
    {
        char l[1024] = { 0 };
        char r[1024] = { 0 };
        
        char* ptr = const_cast<char*>(rowString.c_str());
        split(ptr, codeType, r, l, false);
        
        vector<string> elementStr;
        simple_cpp_string_algorithm::parse_into_vector_array(l, elementStr, commaString);
        
        if (elementStr[0].find("Resource"))
        {
            variableAnalysis.codeType = "Resource";
            return true;
        }
        else if (elementStr[0].find("Describe"))
        {
            variableAnalysis.codeType = "Describe";
            return true;
        }

        return false;
    }
    
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
            
            //获取标记的成员函数
            if (contain("JFUNCTION"))
            {
                FFunctionAnalysis funtionAnalysis;
                if (GetCodeTypeByFunction(row, funtionAnalysis))
                {
                    row = stringArray[i + 1];
                    rowPtr = const_cast<char*>(row.c_str());
                    if (contain("static") || contain("\tstatic"))
                    {
                        funtionAnalysis.bStatic = true;
                        
                        char l[1024] = { 0 };
                        char r[1024] = { 0 };
                        
                        split(rowPtr, spaceString, r, l, false);
                        
                        row = l;
                    }
                    else if (contain("virtual") || contain("\tvirtual"))
                    {
                        funtionAnalysis.bVirtual = true;
                        
                        char l[1024] = { 0 };
                        char r[1024] = { 0 };
                        
                        split(rowPtr, spaceString, r, l, false);
                        
                        row = l;
                    }
                    
                    //确定函数的返回类型
                    char tmp[1024] = { 0 };
                    {
                        char r[1024] = { 0 };
                        trim_start_inline(rowPtr);
                        
                        split(rowPtr, spaceString, r, tmp, false);
                        
                        funtionAnalysis.returnElement = CollectionVariableType(r, ECollectionParamType::CollectionParamType_Return);
                        
                        {
                            remove_char_end(tmp, '}');
                            remove_char_end(tmp, '{');
                            trim_end_inline(tmp);
                            remove_char_end(tmp, ';');
                            remove_char_end(tmp, ')');
                        }
                        
                        char rStr[1024] = { 0 };
                        char lStr[1024] = { 0 };
                        
                        split(tmp, leftParenthesisString, rStr, lStr, false);
                        //函数名
                        funtionAnalysis.functionName = rStr;
                        
                        //解析参数和参数名
                        vector<string> elementStr;
                        simple_cpp_string_algorithm::parse_into_vector_array(lStr, elementStr, commaString);
                        for (auto& element : elementStr)
                        {
                            char* elementPtr = const_cast<char*>(element.c_str());
                            trim_start_and_end_inline(elementPtr);
                            
                            FParamElement paramElement;
                            char r[1024] = { 0 };
                            char l[1024] = { 0 };
                            if (simple_cpp_string_algorithm::string_contain(elementPtr, starString))
                            {
                                paramElement.bPointer = true;
                                split(elementPtr, starString, r, l, false);
                            }
                            else if (simple_cpp_string_algorithm::string_contain(elementPtr, fetchAddressString))
                            {
                                paramElement.bReference = true;
                                split(elementPtr, fetchAddressString, r, l, false);
                            }
                            else
                            {
                                split(elementPtr, spaceString, r, l, false);
                            }
                            if (simple_cpp_string_algorithm::string_contain(r, "const"))
                            {
                                paramElement.bConst = true;
                                remove_string_start(r, "const");
                            }
                            trim_start_and_end_inline(r);
                            trim_start_and_end_inline(l);
                            
                            paramElement.type = r;
                            paramElement.name = l;
                            funtionAnalysis.paramsArray.push_back(paramElement);
                        }
                    }
                }
                classAnalysis.functionsArray.push_back(funtionAnalysis);
            }
            
            //获取标记的成员变量
            if (contain("JPROPERTY"))
            {
                FVariableAnalysis variableAnalysis;
                if (contain("CodeType"))
                {
                    if (GetCodeTypeByPropn(row, variableAnalysis))
                    {
                        char l[1024] = { 0 };
                        char r[1024] = { 0 };
                        
                        row = stringArray[i + 1];
                        
                        remove_char_start(rowPtr, '\t');
                        remove_char_end(rowPtr, ';');
                        
                        split(rowPtr, spaceString, r, l, false);
                        if (contain(starString))
                        {
                            variableAnalysis.bPointer = true;
                            split(rowPtr, starString, r, l, false);
                        }
                        else if (contain(fetchAddressString))
                        {
                            variableAnalysis.bReference = true;
                            split(rowPtr, fetchAddressString, r, l, false);
                        }
                        else
                        {
                            split(rowPtr, spaceString, r, l, false);
                        }
                        
                        if (contain("const"))
                        {
                            variableAnalysis.bConst = true;
                            remove_string_start(r, "const");
                        }
                        trim_start_and_end_inline(r);
                        trim_start_and_end_inline(l);
                        variableAnalysis.type = r;
                        variableAnalysis.name = l;
                    }
                }
                        
                classAnalysis.variablesArray.push_back(variableAnalysis);
            }
        }
        
        return true;
    }
}