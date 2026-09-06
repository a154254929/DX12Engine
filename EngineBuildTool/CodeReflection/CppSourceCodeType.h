#pragma once

#include "../simple_library/public/simple_library.h"

//参数
struct FParamElement
{
    string name;
    bool bConst;        //是否是常量
    bool bPointer;      //是否是指针
    bool bReference;    //是否是引用
    string type;        //类型
    
    FParamElement()
        : bConst(false)
        , bPointer(false)
        , bReference(false)
    {
    }
};

//变量
struct FVariableAnalysis : public FParamElement
{
    string codeType;    //代码类型
    bool bStatic;       //是否是静态变量
    
    FVariableAnalysis()
        : bStatic(false)
    {
    }
};

//函数解析
struct FFunctionAnalysis
{
    std::vector<FParamElement> paramsArray;    //参数列表
    string returnType;                     //返回类型
    
    string functionName;                   //函数名称
    bool bStatic;                          //是否是静态函数
    bool bVirtual;                         //是否是虚函数
    bool bConst;                           //是否是常量函数
    
    string CodeType;                         //代码类型
    
    FFunctionAnalysis()
        : bStatic(false)
        , bVirtual(false)
        , bConst(false)
    {
    }
};

//类解析
struct FClassAnalysis
{
    std::vector<FFunctionAnalysis> functionsArray;
    std::vector<FVariableAnalysis> variablesArray;
    
    string APIName;                         //API名称
    string className;                       //类名称
    std::vector<string> inheritNames;       //继承类名称
    string codeCPPName;                     //cpp类型
    int codeLine;                           //代码行号
};

//集合参数类型
enum ECollectionParamType
{
    CollectionParamType_Return,
    CollectionParamType_Param,
};