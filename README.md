# 模拟车床作业报告

## 功能介绍

### 切削

可以选择两种模式进行切削，通过点击第一个按钮即可进行切换（切换后会显示当前的模式）。

![](.\Report\鼠标按钮.JPG) ![](.\Report\曲线按钮.JPG)

第一种模式，鼠标模式。通过鼠标控制刀尖的位置来切削圆柱体材料，鼠标的位置与刀尖的位置**完全相同**，可以做到所想即所得。（注意，刀具无法越过xz平面，即不能到上部去进行切削）

第二种模式，曲线模式。通过在右上角的编辑区中控制四个点的位置来控制Bezier曲线的形状，点击**开始**按钮后。刀具将在10s内自动按照曲线划定的轮廓线进行切割。（在切割过程中无法编辑曲线）

![](.\Report\曲线.JPG) ![](.\Report\曲线切削.JPG) 

### 粒子系统

在切削时，会根据进刀量（即切削掉的部分的大小）产生一定数量的粒子，以模拟切削时产生的碎屑。

粒子的方向/初速度/大小/旋转角度随机，但是大体方向一定是与切削方向相反的。

### 复位

点击**复位**按钮可以使刀具会到初始位置（在鼠标模式下仍然会和鼠标保持一致），同时材料会到最开始的圆柱状态。

### 材质

程序中一共提供了两种材质以供选择，分别为金属材质和木质。可以通过点击**材质**按钮进行切换。在切换后，粒子的颜色和天空盒的背景也会相应发生变化。

#### 金属

通常用于切削的金属材质表面有一层氧化层，漫反射较强，镜面反射较弱；而内部的金属则是漫反射较弱，镜面反射较强。

![](.\Report\金属.JPG)

#### 木质

木质的表面与内部的粗糙程度和颜色都差距较大。因此使用了两种不同的贴图来表示。

![](.\Report\木质.JPG)

### 展示

点击**展示按钮**，可以切换到展示模式，模型将去除底座并进行全方位地展示。再次点击，可以切换回切削模式。在展示模式下，只能调整控制区的点的位置和切换材质，复位和开始都会无效化。

![](.\Report\展示.JPG)

### 键盘操作

KEY_A：镜头左移

KEY_B：镜头下移

KEY_W：镜头上移

KEY_S：镜头下移

KEY_Q：镜头前移

KEY_E：镜头后移

KEY_SPACE：复位操作

注意，镜头无法离开天空盒的范围。

## 具体实现

### 圆柱体与切削

​		将一个圆柱体分为很多个很薄的小圆柱，每一个小圆柱又由多个扇形台体组成。一共扇形台体由四个三角形组成（看不见的地方不需要面片）。使用顶点数组来绘制这些小圆柱。顶点数组中包含的属性有顶点位置（xyz），法向量方向（xyz），是否是表面（float），以及贴图的位置（a,b）。即每个顶点共有9个属性值。

​		计算顶点数组中数据的方法为，首先在radius数组中存储所有小圆柱的半径，每一个小圆柱根据自己及相邻圆柱的半径计算左右表面的半径（取平均值防锯齿）。**每一次切削实际上也是在更新radius数组的值**，然后再根据radius的值重新计算顶点数组中对应扇形台体的顶点的顶点属性。

​		需要注意的是**ifSurface**这个顶点属性，它通过与原始半径比较得出这个小圆柱的侧面是否是表面。若半径等于初始半径，则是表面，在Shader中使用surface的材质；反之则不是表面，需要使用对应的材质（金属或木材）。

### 粒子系统

​		粒子系统首先由一个个小粒子组成。粒子采用较小的正方体，具有随机的初速度/方向/大小/旋转角度。所有的粒子存于一个渲染队列中。

​		在切削的过程中，根据每一个小圆片被切削的量，在小圆片被切削的位置，产生1~8个粒子。进刀量越大，产生粒子越多。将产生的粒子放入渲染队列中。

​		在粒子系统中有重力存在，随着时间增加，其y方向的速度会增大。

​		同时，每个粒子都有自己的寿命（2s），在超过寿命后，粒子将从渲染队列中移除，不再渲染。

### 按钮

​		由于opengl没有按钮，因此大作业中的按钮使用贴上纹理的矩形面片和对点击区域的判断组合起来实现的。

​		因为按钮的位置不能随着镜头移动，所以按钮的顶点渲染器中没有与view，projection等剪裁矩阵相乘，这样它就可以一直显示在画面的右边而不随镜头移动。贴上不同的二维纹理后，就可以表示不同的按钮了。

​		对按钮点击的实现在main.cpp中。首先用glfwGetMouseButton来获取鼠标左键的状态，如果发现它点击了，则修改全局变量Select的值，防止重复点击。根据点击区域的不同，Select的值也不同，从而判断现在需要执行的任务。松开鼠标后，Select的值会被修改为NONE，结束点击事件。

### Bezier曲线

​		三次Bezier曲线的形状由四个点控制，因此，在控制区域中用四个点来控制Bezier曲线的形状。

​		移动控制点的过程为，首先鼠标在区域中点击，将像素坐标换算为对应的标准化设备坐标后，判断这个点击的位置是否在某一个点的一定范围附近（30像素的范围）。如果在范围内，则修改CurveArea类中select属性的值表示选中该点，接下来移动鼠标，只要不松开左键，则之后鼠标所指的像素点的坐标也会被转换为标准化设备坐标用来更新这个点的坐标。在松开左键后，CurveArea类中select属性的值将被修改为-1，表示没有选中任何点，则结束移动。

​		每一次移动控制点的坐标，也会更新真实的轮廓线，因此在切削过程中禁止移动控制点。而控制区域内的曲线则是用100个点连成线段画出的曲线。

### 鼠标与刀尖保持一致

​		要使鼠标和刀尖保持一致，即要把鼠标位置的屏幕坐标转化为刀尖在世界坐标系下的坐标，这个转化在Screen2World函数中实现。

​		首先将屏幕坐标转化为标准化设备坐标，然后将标准化设备坐标与view和projection的逆矩阵相乘得到鼠标点击的位置在世界坐标下的后视面上的位置。最后，根据相似的比例进行换算，得到鼠标坐标在z=0这个面上的对应的点的坐标。

​		（不过这样做只是得到了鼠标在z=0这个面上对应的世界坐标，要想使刀尖与之对齐，还需使刀尖先于世界坐标系的原点对齐，之后再根据上面得到的坐标对刀尖进行位置表换即可）（我让刀尖对齐原点的办法是在用3D builder建模时就对齐……简单但有效）

### 光照

​		采用冯氏光照模型，分为环境光/漫反射/镜面反射（ambient，diffuse，specular）三个部分。根据材质的不同，可以给这三类光线赋予不同的强度系数，以模拟材料自身的性质。完成计算后，相加再与这个像素的颜色值相乘得到它最终应该显示的颜色。

## 参考

天空盒与光照部分的代码参考了Learnopengl
