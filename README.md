# Th10Ai

东方风神录AI，基于深度优先搜索算法。<br />
Mountain of Faith's AI, based on depth first search algorithm.<br />

![](https://github.com/unnamedmemory/Th10Ai/blob/master/1.png)

## 问题

* 重现子弹的移动轨迹，除了直线移动，还有曲线移动、随机移动，否则后续的碰撞预测和节点搜索都是错的。（缺少子弹ID，导致帧与帧之间的子弹无法关联起来进行轨迹推导）<br />
* 5面激光不闪避。（可能是激光的速度过快，需要检测穿越判定）<br />

## 运行

東方風神録　～ Mountain of Faith. ver 1.00a<br />
安装[VC++2022可再发行组件包](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)。<br />
显示器刷新率设置为60FPS，风神录设置为窗口模式。<br />
修改Th10Ai.conf里的Th10Path为你的风神录程序路径，运行Th10Ai.exe。<br />
保持窗口焦点在风神录上，开始游戏，按A启动AI，按S停止AI。<br />

## 依赖

[Boost](https://www.boost.org)<br />
open x86 Native Tools Command Prompt for VS 2022<br />
cd path/to/boost<br />
bootstrap.bat<br />
b2 stage link=static runtime-link=shared threading=multi define=BOOST_USE_WINAPI_VERSION=0x0A00 --without-python<br />

[Detours](https://github.com/microsoft/detours)<br />
open x86 Native Tools Command Prompt for VS 2022<br />
cd path/to/detours/src<br />
nmake<br />

## 感谢

[TH10AI by Infinideastudio](https://github.com/Infinideastudio/TH10AI)<br />
[TH10_DataReversing](https://github.com/binvec/TH10_DataReversing)<br />
[twinject](https://github.com/Netdex/twinject)<br />
