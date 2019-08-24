# Motor_Control
<table><tr><td bgcolor=orange>
+ `my_printf`函数支持`%f、%c、%x、%d、%s`等格式控制字符，`\r、\n`转义字符，屏幕信息显示<font color=#FF0000>完全</font>使用`my_printf`(图像除外)。
- 角度信息采集使用针对电机控制的<font color=#FF0000>硬件编码器</font>接口，计算量小。
+ 矩阵按键没有采用扫描的方式，而是用<font color=#FF0000>四个中断驱动</font>，避免了扫描对资源的浪费。
- 按键读值采用电平翻转方式，充分利用32的优势，而不是像51那样循环重复地一位一位取。
+ 由于开发板屏幕接口与IIC1冲突，使用软件模拟I2C，<font color=#FF0000>移植了51的程序</font>，全部采用<font color=#FF0000>位带操作</font>。
- 调参时，支持直接输入数值，而不是用上下左右键，屏幕实时反馈，更人性化。</td></tr></table>
