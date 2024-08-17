# Th10Ai

东方风神录AI，基于深度优先搜索算法。<br />
Mountain of Faith's AI, based on depth first search algorithm.<br />

![](https://github.com/rebuildworld/Th10Ai/blob/master/1.png)

## Building

* Open "x86 Native Tools Command Prompt for VS 2022".<br />
* [Boost](https://www.boost.org)<br />
	```
	cd path/to/boost
	bootstrap.bat
	b2 stage address-model=32 link=static runtime-link=shared threading=multi define=_WIN32_WINNT=0x0A00 define=BOOST_USE_WINAPI_VERSION=0x0A00 compileflags=/utf-8
	```

* [wxWidgets](https://github.com/wxWidgets/wxWidgets)<br />
	```
	git clone https://github.com/wxWidgets/wxWidgets.git wxWidgets-git
	cd wxWidgets-git
	git submodule update --init --recursive
	git checkout v3.2.5
	open build/msw/wx_vc17.sln file, select the appropriate configuration (DLL Debug or DLL Release, Win32) and build the solution. 
	```

* [Detours](https://github.com/microsoft/detours)<br />
	```
	cd path/to/detours/src
	nmake
	```

* Th10Ai<br />
	```
	cd path/to/Th10Ai
	cmake -S . -B build -G "Visual Studio 17 2022" -A Win32 -D BOOST_DIR=path/to/boost -D WXWIDGETS_DIR=path/to/wxwidgets -D DETOURS_DIR=path/to/detours
	cmake --build build --config Release
	```

## Usage

* Install [Visual C++ 2022 Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170).<br />
* Prepare the "東方風神録　～ Mountain of Faith. ver 1.00a", set to windowed mode.<br />
* Monitor refresh rate set to __60FPS__.<br />
* Modify th10Path in Th10Ai.conf to your th10.exe path, and run Th10Ai.exe.<br />
* Keep window focus on the game, select Reimu to start the game, press 'A' to start AI, press 'S' to stop AI.<br />

## Issues

* 严重错误：子弹除了直线移动，还有曲线移动、随机移动等，现在使用单帧偏移量叠加得到的移动轨迹是错误的。缺少子弹ID，导致帧与帧之间的子弹信息无法用于轨迹计算。<br />
* 5、6面激光不闪避：可能是激光的速度过快，需要检测穿越判定。<br />
* 小概率Hook失败。<br />

## Thanks

[TH10AI by Infinideastudio](https://github.com/Infinideastudio/TH10AI)<br />
[TH10_DataReversing](https://github.com/binvec/TH10_DataReversing)<br />
[twinject](https://github.com/Netdex/twinject)<br />
