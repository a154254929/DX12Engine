#include "SpawnIntermediateFile.h"

namespace IntermediateFile
{
	void RemoveEnd(std::vector<std::string>& paramArray, char endChar)
	{
		if (simple_cpp_string_algorithm::index_valid(
			paramArray.size(),
			paramArray.size() - 1))
		{
			remove_char_end(
				const_cast<char*>(paramArray[paramArray.size() - 1].c_str()),
				endChar);
		}
	}

	const string* GetGOrCInheritName(const std::vector<string> &inheritName)
	{
		for (auto &Tmp: inheritName)
		{
			if (!Tmp.empty())
			{
				if (Tmp.c_str()[0] == 'C' || Tmp.c_str()[0] == 'G')
				{
					return &Tmp;
				}
			}
		}

		return NULL;
	}

	std::string GetCombineParmString(
		const FFunctionAnalysis& Function,
		std::vector<std::string>& paramArray)
	{
		if (Function.paramsArray.size() == 0)
		{
			return "";
		}
		else
		{
			std::string ParamString;
			for (auto& Param : Function.paramsArray)
			{
				//int   c, float   b
				std::string NewParam =
					simple_cpp_string_algorithm::printf(
						"%s %s%s %s",
						string((Param.bConst ? ("const") : (""))).c_str(),
						Param.type.c_str(),
						string((Param.bPointer ? ("* ") : (Param.bReference ? ("& ") : (" ")))).c_str(),
						Param.name.c_str());

				//int32 A,
				ParamString += NewParam + (",");

				//int32 A,int32 B.int32 c
				paramArray.push_back(NewParam);
			}

			remove_char_end(const_cast<char*>(ParamString.c_str()), ',');

			return ParamString;
		}	
	}
    
    void GeneratePointH(
        std::vector<std::string>& analysisRaw,
        const FClassAnalysis& classAnalysis,
        std::vector<std::string>& staticRegistration
    )
    {
        //头定义
        {
            analysisRaw.push_back("#pragma once");
            analysisRaw.push_back("");
            analysisRaw.push_back("#include \"ObjectScript.h\"");
            analysisRaw.push_back("");
            
            std::string mClassName = "Z_BT_" + classAnalysis.className;
            analysisRaw.push_back(
                "#define "
                + mClassName
                + (classAnalysis.functionsArray.size() > 0 ? "\\" : "")
            );
            
            //类名
            std::string clearClassName = classAnalysis.className;
            {
                char* clearClassNamePtr = const_cast<char*>(clearClassName.c_str());
                trim_start_and_end_inline(clearClassNamePtr);
                
                //移除开头
                remove_char_start(clearClassNamePtr, 'C');
                remove_char_start(clearClassNamePtr, 'G');
            }
            
            if (classAnalysis.functionsArray.size() > 0)
            {
                for (const auto& function : classAnalysis.functionsArray)
                {
                    if (function.codeType == "Describe")
                    {
                        //std::string vmString = "VMC_" + function.functionName;
                        //analysisRaw.push_back("BT_VM(" + vmString + ") \\");
                    }
                    else if (function.codeType == "Event")
                    {
                        
                    }
                }
                
                if (simple_cpp_string_algorithm::index_valid(analysisRaw.size(), analysisRaw.size() - 1))
                {
                    
                    remove_char_end(
                        const_cast<char*>(analysisRaw[analysisRaw.size() - 1].c_str()),
                        '\\'
                    );
                }
                
                analysisRaw.push_back("");
                
                char buffLine[1024] = { 0 };
                analysisRaw.push_back(
                    "#define"
                    + classAnalysis.className
                    + "_"
                    + _itoa(classAnalysis.codeLine, buffLine, 10)
                    + "_GENERATED_BODY_BT \\"
                );
                analysisRaw.push_back(mClassName);
                analysisRaw.push_back("");
                
                analysisRaw.push_back(
                    "#define "
                    + std::string("CURRENT_FILE_ID_BT")
                    + classAnalysis.className
                );
                
                char buffLine2[1024] = { 0 };
                analysisRaw.push_back(
                    std::string("#define NewLine")
                    + _itoa(classAnalysis.codeLine, buffLine2, 10)
                );
            }
        }
    }
    
    void GeneratePointCpp(
        std::vector<std::string>& analysisRaw,
        const FClassAnalysis& classAnalysis,
        std::vector<std::string>& staticRegistration
    )
    {
        //头定义
        {
            analysisRaw.push_back("");
            analysisRaw.push_back("#ifdef _MSC_VER");
            analysisRaw.push_back("#pragma warning (push)");
            analysisRaw.push_back("#pragma warning (disable : 4883)");
            analysisRaw.push_back("#endif");
            analysisRaw.push_back("PRAGMA_DISABLE_DEPRECATION_WARNINGS");
            
            analysisRaw.push_back("");
            //代码区
            {
			//������
			if (classAnalysis.functionsArray.size() > 0)
			{
				for (const FFunctionAnalysis& function : classAnalysis.functionsArray)
				{
					if (function.codeType == "Event")
					{
						//Name_Hello123
						std::string FunctionName =
							simple_cpp_string_algorithm::printf("Name_%s",
								function.functionName.c_str());

						std::vector<std::string> ParamStr;
						//static std::string Name_Hello123 = std::string(("Hello123"));v
						analysisRaw.push_back(
							simple_cpp_string_algorithm::printf(
								"static std::string %s = std::string((\"%s\"));",
								FunctionName.c_str(),
								function.functionName.c_str()
							));

						//void   ActorObject::Hello123( int   c, float   b)
						analysisRaw.push_back(
							simple_cpp_string_algorithm::printf(
								"%s %s %s::%s(%s)",
								function.returnElement.type.c_str(),
								string((function.returnElement.bPointer ? ("*") : (function.returnElement.bReference ? ("&") : (" ")))).c_str(),
								classAnalysis.className.c_str(),
								function.functionName.c_str(),
								GetCombineParmString(function,ParamStr).c_str()
							));

						analysisRaw.push_back(("{"));
						{
							//Parm_Hello123
							std::string StructName =
								simple_cpp_string_algorithm::printf(
									"Parm_%s",
									function.functionName.c_str());

							if (function.paramsArray.size() > 0)
							{
								//struct FParm_Hello123
								analysisRaw.push_back(
									simple_cpp_string_algorithm::printf("\tstruct F%s",
										StructName.c_str()));
								analysisRaw.push_back(std::string(("\t{")));//{
								{
									for (auto& Param : ParamStr)
									{
										analysisRaw.push_back(
											simple_cpp_string_algorithm::printf("\t\t%s;",
												Param.c_str()));
									}
								}
								analysisRaw.push_back(std::string(("\t};")));//};

								//FParm_Hello123 Parm_Hello123;
								analysisRaw.push_back(
									simple_cpp_string_algorithm::printf(
										"\tF%s %s;",
										StructName.c_str(),
										StructName.c_str()));

								//��ֵ
								{
									//Parm_Hello123.c = c;
									for (auto& Param : function.paramsArray)
									{
										analysisRaw.push_back(
											simple_cpp_string_algorithm::printf(
												"\t%s.%s = %s;",
												StructName.c_str(),
												Param.name.c_str(),
												Param.name.c_str()));
									}
								}
							}

							//ExecutionScript(FindScriptFuntion(Name_Hello123), &Parm_Hello123);
							analysisRaw.push_back(
								simple_cpp_string_algorithm::printf(
									"\tExecutionScript(FindScriptStaticFuntion(%s),%s);",
									FunctionName.c_str(),
									string(function.paramsArray.size() == 0 ? ("NULL") : (("&") + StructName)).c_str()
								));
						}
						analysisRaw.push_back(("} "));
					}
				}
			}

			analysisRaw.push_back((""));

			//void GActorObject::InitReflectionContent()
			analysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
				"void %s::InitReflectionContent()",
					classAnalysis.className.c_str()));
			analysisRaw.push_back("{");
			{
				//Rename("ActorObject");
				analysisRaw.push_back(
					simple_cpp_string_algorithm::printf(
						"\tRename(\"%s\");",
						classAnalysis.codeCPPName.c_str()));
			}

			analysisRaw.push_back("}");

			analysisRaw.push_back((""));

			analysisRaw.push_back(("/* 1xxxx xxxx "));
			analysisRaw.push_back((" 2xxxx xxxx */"));
			//Register_ActorObject
			std::string Register_Func =
				simple_cpp_string_algorithm::printf("Register_%s()",
					classAnalysis.className.c_str());
			//int Register_ActorObject()
			analysisRaw.push_back(
				simple_cpp_string_algorithm::printf("int %s", Register_Func.c_str()));
			analysisRaw.push_back(("{"));
			{
				//�ϲ�����vector
				analysisRaw.insert(
					analysisRaw.end(),
					staticRegistration.begin(),
					staticRegistration.end());

				analysisRaw.push_back((""));
				analysisRaw.push_back(("\treturn 0;"));
			}
			analysisRaw.push_back(("}"));

			//static int ActorObject_Index = Register_ActorObject();
			analysisRaw.push_back(
				simple_cpp_string_algorithm::printf(
					"static int %s_Index = %s;",
					classAnalysis.className.c_str(),
					Register_Func.c_str()));
            }
            
            analysisRaw.push_back("");
            
            analysisRaw.push_back("PRAGMA_ENABLE_DEPRECATION_WARNINGS");
            analysisRaw.push_back("#ifdef _MSC_VER");
            analysisRaw.push_back("#pragma warning (pop)");
            analysisRaw.push_back("#endif");
        }
    }
    
    bool IntermediateFile::Builder(
        const FClassAnalysis& inClassAnalysis,
        std::vector<std::string>& outAnalysisRawH,
        std::vector<std::string>& outAnalysisRawCpp
    )
    {
        vector<string> statucRegistration;
		
		GeneratePointH(outAnalysisRawH, inClassAnalysis, statucRegistration);
		GeneratePointCpp(outAnalysisRawCpp, inClassAnalysis, statucRegistration);
        
        return outAnalysisRawH.size() > 0 && outAnalysisRawCpp.size() > 0;
    }
}
