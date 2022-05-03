# Th10Ai

东方风神录AI，基于深度优先搜索算法。<br />
Mountain of Faith's AI, based on depth first search algorithm.<br />

![](https://github.com/unnamedmemory/Th10Ai/blob/master/1.png)

## 待改进

* 重现子弹的移动轨迹，除了直线移动，还有曲线移动、随机移动，否则后续的碰撞预测和节点搜索都是错的。（缺少子弹ID，导致帧与帧之间的子弹无法关联起来进行轨迹推导，弃坑）<br />
* 5面激光不闪避。（可能是激光的速度过快，需要检测穿越判定）<br />

## 运行要求

東方風神録　～ Mountain of Faith. ver 1.00a<br />
窗口模式<br />
刷新率60FPS<br />
安装[VC++2022可再发行组件包](https://docs.microsoft.com/zh-CN/cpp/windows/latest-supported-vc-redist?view=msvc-170)。<br />
修改Loader.conf里的exe-path为你的东方程序路径，运行Loader.exe。<br />
由于使用了DLL注入，请在杀毒软件放行。<br />

## 编译依赖

[Boost](https://www.boost.org)<br />
[Detours](https://github.com/microsoft/detours)(x86 Native Tools Command Prompt for VS 2022 -> cd Detours-path/src -> nmake)<br />

## 感谢

[TH10AI by Infinideastudio](https://github.com/Infinideastudio/TH10AI)<br />
[TH10_DataReversing](https://github.com/binvec/TH10_DataReversing)<br />
[twinject](https://github.com/Netdex/twinject)<br />
