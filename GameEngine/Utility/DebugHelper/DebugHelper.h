#pragma once
#include <string>
#include <Windows.h>
#include <functional>
#include <chrono>

namespace DebugHelper
{
	enum TimeType
	{
		Microsecond,
		MilliSecond
	};

	static void Print(const std::wstring& message)
	{
		std::wstring showMessage = message + L"\n";
		OutputDebugStringW(showMessage.c_str());
		MessageBox(nullptr, showMessage.c_str(), L"DebugHelper", 0);
	}

	static void MeasureTime(const std::function<void()>& func, const std::wstring& message, bool isShowBox = false, TimeType type = TimeType::Microsecond)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func();
		auto end = std::chrono::high_resolution_clock::now();

		std::wstring showMessage = L"";

		if (type == TimeType::Microsecond)
		{
			showMessage = message + std::to_wstring(std::chrono::duration<double, std::micro>(end - start).count()) + L"É s" + L"\n";
		}
		else
		{
			showMessage = message + std::to_wstring(std::chrono::duration<double, std::milli>(end - start).count()) + L"ms" + L"\n";
		}

		OutputDebugStringW(showMessage.c_str());

		if (isShowBox)
		{
			MessageBox(nullptr, showMessage.c_str(), L"DebugHelper", 0);
		}
	}
}