#pragma once

#ifndef JHS_ENGINE_CORE_EXPORTS
#define JHS_ENGINE_CORE_EXPORTS
#endif

#ifdef JHS_ENGINE_CORE_EXPORTS
#define JHS_ENGINE_CORE_API __declspec(dllexport)
#else
#define JHS_ENGINE_CORE_API __declspec(dllimport)
#endif // JHS_ENGINE_CORE_EXPORTS
