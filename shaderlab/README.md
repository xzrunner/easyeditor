# ShaderLab

Shader Manager

## Shaders

### Shape

几何图形

### Sprite

图片

有四种类型：1. 不上色；2. 有乘色和加色；3. 有颜色转换；4. 有乘色、加色和颜色转换

合并顶点，选择合适的shader。减少因切换shader产生的drawcall，减少不必要的shader开销（比如乘色是0xffffffff，加色是0，这时用2就会很浪费，和直接用1的结果是一样的）

draw时把顶点加到buffer，commit时再决定当前的数据分几批和采用哪种shader来绘制。

绘制顺序|合并策略
:-----:|:----
1111111122222222 | 为了节约shader开销就切换shader提交2次，为了减少drawcall就都用2来绘制
2222222112222222 | 为减少drawcall，用最高的2来绘制

### Blend

类似PS中的图层混合

### Filter

一些滤镜效果
