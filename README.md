# CPPRTS
The final project of TJ SSE

○开发人员



○启动说明



多人游戏选择开始游戏→新建游戏。其他游戏端选择加入游戏。待连接成功的游戏人数足够后就会自动开始游戏。

单人游戏会允许你在无限资源的情况下随时更换队伍进行游戏功能测试。

设置可以选择关闭音效，并保留在本地。

每个玩家根据自己的编号会以建筑和兵种的颜色进行队伍区分。

提供1V1/2V2两种多人游戏模式。


○游戏玩法说明



基本建筑五个：基地 发电厂 矿厂 兵营 战车工厂

基地是所有建筑的前置建筑。消耗1000黄金

建造一座发电厂可以使你100电力，电力是其他三个建筑的必要资源。消耗1000黄金。

每座矿厂可以每秒提供50黄金。消耗1000黄金、50电力。

兵营可以允许你生产大兵、警犬（独角兽限定皮肤）。消耗1000黄金、50电力。

战车工厂可以允许你生产坦克。消耗2000黄金、100电力。

基本兵种三个：大兵 警犬 坦克

大兵有中等的攻击距离、攻击力和血量。

警犬攻击距离较短，血量较低，但有高额攻击力。

坦克有最强的攻击距离和血量，以及较高的攻击力。

拒绝平衡性吐槽！



○操作与功能说明：



点击按钮可以进行对应建筑的建造。只有条件允许的建筑可以进行建造。

点击已经建造完成的己方兵营/战车工厂，再点击对应按钮可以生产大兵、警犬/坦克，这同样需要条件允许才能生效。建筑会通过红色描边指示是否被选中。

左键点击兵种，或者使用拉选框选中多个兵种选中，然后点击敌方目标或空地可以实现攻击和移动。兵种脚底会有红框指示是否被选中。

右键可以清除一切选中和建造状态。

使用拉选框请先右键清空选中状态避免误点。

右上角会显示您的黄金、电力资源的情况，以及您的阵营与盟友。

左下角的小地图会显示地图各方单位的现状，点击小地图可以跳转屏幕至地图点击区域。

如果游戏模式是单人模式：提供了更换队伍的功能，可以单机测试对战功能。通过点击四色按钮实现换队，右上角队伍显示可能会有1秒左右的延迟。

所有按钮都有对应的键盘交互功能，具体为：三种兵种按照按钮位置对应QWE，五种建筑按照按钮位置对应ASDFG，换队功能为Z键。



○关于游戏的说明：

所有的建筑、兵种都拥有碰撞体积，无论是在行进中还是停止。

建筑被摧毁后会留下废墟，依旧拥有碰撞体积。兵种被击杀则会从地图上消失。

没有实现战争迷雾，十分抱歉。

由于游戏的粗糙，点击己方或敌方兵种、建筑时请尽可能地点击中心靠下的范围，避免误判。

我们没有实现将攻击目标击杀后的自动搜寻目标，十分抱歉。请亲自点击目标进行攻击。

虽然我们提供了拉选框，但是为了使实际效果符合预期，建议您控制每次拉选兵种个数控制在九个以下，并且避免选中多个兵种指向地图边缘或拥挤区域。


碰撞体积与寻路的算法是对Potential Fields进行的魔改。具体参照：
Using Potential Fields in a Real-time Strategy Game Scenario (Tutorial)————Johan Hagelbäck on January 31, 2009

由于只是对这一算法理论的简单应用，该算法并没有很精确地实现我们想要的功能。它可能导致一些行动路线的诡异，但是这些都是基于算法逻辑的正常现象。并且该算法基本实现了我们需要的寻路和碰撞体积。

