
2015-11-20:  片内SRAM只能创建45个EDIT，准备将程序移植到原子战舰开发板，
	利用开发板的外部SRAM继续进行UI框架设计，明天开始进行”编程“界面的逻辑开发。

2015-11-21：使用片外SRAM成功，将ISS51216BLL的1M内存全部分配给emWin，
	成功运行点击ADD按钮来添加EDIT控件的功能。

2015-11-24:成功将按钮选择的文本编辑到EDIT中

2015-11-26:编程界面搞成汇博士相同，点击按键然后通过EDIT来编辑文本，确定后将文本内容显示到主程序的EDIT中。
			接下来应该搞一下怎么让单片机知道这段文本的目的是干嘛的
			
2015-12-1: 实现以链表的方式进行存储每个EDIT的属性(包括指令类别标志及文本内容),可以新建指令、更改指令，但是删除指令后再新建指令会出现BUG，
			原因是我没完全按照汇博士的模式来.放弃ADD、DEL按钮，采用固定行数的EDIT，暂时固定为200行。
			
2015-12-2: 把EDIT和TEXT都搞成198行，暂时没想到办法解决EDIT上拉时盖住HEADER的问题。
			采用董学长的思路写了一套解析链表的程序，主要难点在于流程控制语句if or while的实现上：
			大概思路就是if后的条件如果满足的话，则直接在if_branch里取下一个结点来执行，否则跳到p->next->next执行，if语句暂时只支持一条语句
			while语句处理则是先找到代码块的起始地址和终止地址(while语句尾部的前一个结点)，就可以实现循环了，暂时只支持死循环

2015-12-3:将链表的插入、替换、删除、清空的BUG修复，新增"条件结束"语句在流程控制中，将用来支持if后面跟多条语句.
			开发板复位大部分都不成功，不知道是不是供电的原因，有时候会挂在GUI_Init()里？？？好奇怪
			
2015-12-4:复位有时会挂掉的问题解决了，根据网友说的跟我一样的现象，他的原因是LCD_Init在GUI_Init()前面，因为W外部SRAM跟LCD共用一套FSMC数据总线
			这时候如果LCD已经使能了就会导致外部SRAM的初始化出了问题，结果就是GUI_Init()分不到内存。我的LCD_Init在GUI_Init里面，但是Touch_Init在
			前面，按道理是不会影响到FSMC_SRAM_Init的，但是将Touch_Init调到跟LCD_Init一起后，就解决问题了。
			总之，在外部SRAM初始化之前，关于LCD的一切操作最好都别做

2015-12-12:PCB到了，焊好之后发现晶振不起振，3线SWD无法检测到CPU。经核查，CPU上电后默认不使用外部晶振，SWD必须5线制:NRST、VCC必须相接.
			另外PCB的LCD插座的背光控制引脚没跟CPU连接(可以默认接地)，触摸控制芯片的MISO脚被连到了G6.
			
2015-12-14:移植FATFS0.10，发现创建过的文件后面打开就出现错误FR_NO_FILE，后查出来是FLASH的SPI驱动的问题，对于Flash的写操作，必须先将整个扇区(4Kbytes)
			的数据都出来，如果要写的地址里存在数据不是0xFF的，则要先擦除整个扇区再进行写操作。这个例程是借鉴原子的，野火的SPI_Write_Buffer有问题,吗的
		
2015-12-15:早上去公司加上9V电池给板子上电，居然烧了！！！不明白为何会烧了，输出电压是3.3V没错啊
			增加了文件删除功能，测试正常。
			晚上又焊了一块，舵机、电机驱动程序都添加了，初步测试运行程序正常，但是这块CPU的TIM4的3通道即P87引脚好像坏了，不出波，也不输出高低电平
			导致有一个电机接口会一直转。
			
2015-12-16:超声波有跳变，不稳定。LED灯、触碰开关测试ok，模拟传感器引脚悬空时是4095的一半，要解决。
			if else逻辑还有问题，待解决。
		
2015-12-17:if else逻辑解决，超声波好像又没有跳变了，但是不能超过4m。
			数字传感器要做成无信号时输出低电平，有信号时输出高电平，模拟传感器要在没信号时测到的值是0，而不是4096的一半
			找到PB7引脚的问题所在，驱动SRAM时用作FSMC_NADV，虽然实际引脚没用上，但是FSMC控制器确实是用上这个引脚了，只好放弃另选别的定时器
			明天重新画一张原理图，将液晶屏触摸驱动也做上去

2015-12-18:修复手动控制选项里的MULTIPAGE问题。
 	       完成NRF驱动移植
	   
2015-12-24: 完成与遥控器的对接。

2015-12-26: ADC引脚接下拉电阻试试，超声波输入引脚接下拉电阻试试

2015-12-30: ADC、超声波引脚接下拉电阻试验正常，程序编辑窗口的EDIT控件问题修复

2016-1-3:   Window_top界面图标换成美工做的图标，但是16bit现实的颜色还是太少，图片变成了灰色的。
            MPU6050移植进来了，考虑是否支持DMP，因为DMP库代码太多，可能会占用过多的Flash
			
2016-1-4: 链表结点申请首次出现失败的情况，扩大Heap_Size后解决问题。那么问题来了，
		  1、原本只有256byte的堆，如何存放2k多的全局变量的？         全局变量与堆栈区域分开了
		  2、我是申请了十几次后才出现失败的情况，按道理只能申请5次不能再多了，那么在没被回收的前提下他都是怎么申请成功的？
		  3、MSP != RW_DATA + ZI+DATA   因为我的file结构体定义在了外部SRAM
		 
2016-1-6:  问题1：增大堆空间到12K，添加链表结点到第190个fail to malloc ，按道理可以malloc 219次，但那是由于内存碎片造成了资源浪费。
		   最好的办法还是写自己的malloc    
		   移植原子的mymalloc，分内部内存池15K给链表用，外部内存池500K给emWin用，file结构体定义于外部内存池的状态表之后
		   剩余外部SRAM将来用于接收.bin文件
		   问题2：程序编辑界面里，点击滑动条时很容易点到运行按钮，而且_MessageBox没有取消按钮    搞出自己的_MessageBox，解决
		   
		   好像程序里不使用malloc\free等操作时，即使定义了Heap,编译器也会优化它从而不开辟堆空间。
		   __attribute__默认4字节对齐
		   
2016-1-7： Stack也要供中断使用的，CM3线程模式下使用PSP，异常模式下使用MSP。但是没搞明白LR不是指向返回地址的吗，咋变成指向被中断任务的堆栈了
			好奇怪
			
2016-1-8:  1086发烫的厉害，6050读出的数据也不对，不知道是不是因为温飘的原因。超声波反应都不灵敏了，不只懂啊啥原因

2016-1-9:  超声波因为TRIG线的距离变长所以出发的时间要延长才能成功触发，现延时到40us，去掉echo引脚的10K电阻
		   6050的REGOUT引脚电容不对，应为0.1uf。
		   ADC引脚的10K下拉电阻可能还不合理，在不接模拟传感器的情况下仍不为0，大概在6~8浮动
		   
2016-1-10: 程序编辑界面点击Scrollbar仍有问题，将回调代码修改后不会出现点击一下一直向下滚，但仍有问题不知是不是emwin的bug
		   增加触屏校准界面,但是得通过复位按键来返回，也就是说并没有把校准出来的系数写入Flash
		   电机控制要好好考虑考虑方案，暂时不加入平衡车功能，得看看外壳到底能不能拼出来平衡车
		   
2016-1-12： 对接蓝牙协议：发送部分已经优化了，接收部分要跟手机APP商量.
            RJ3\RJ4口的ADC读出的值不为0，可能是板子布局问题？
			解决OR语句的无穷递归导致栈溢出的问题
			
2016-1-13: 遥控器地址排列有问题，但是现在主控板跟遥控器配上了，考虑后面是否重新排列遥控器地址
           连接遥控器总是会出现一次连接错误,将等待NRF_IRQ的次数增加后出现“连接错误”的情况控制在30%内
		   重新焊了块底板，ADC的干扰小了一点，但是6050的Z轴角速度大的离谱，可能是我没焊好
		   
2016-1-14: 画出小车实时方向图标。
           超声波还是有问题，上不上那10K下拉电阻都有问题，还是延时时间的问题？
		   
2016-1-15: 超声波问题解决，还是IO口配置的问题
           ADC3\4证实是单片机引脚问题
           Widow_Running   Window_2界面刷新速率改为0.5s刷新一次，是需求而定
		   验证编程界面节点申请，到140行没问题，下面的还没验证完
           
2016-1-16: 液晶屏花屏问题解决，是初始化代码的问题。
           创建初始化界面，等待上电稳定
		   
2016-1-17: 蜂鸣器无法唱歌，如果想要唱歌，必须独占一个定时器，CPU外设资源不足...
           舵机驱动：50HZ的PWM波驱动效果不如100HZ的.发现模拟舵机的每个舵机能达到转角不太一样.舵机抖动有可能是转角达不到或者电池电流不足
		   需要添加画图指令
		   LED与DS的冲突亟待解决
		   将按钮颜色都换成粉红色?
		   
2016-1-18: 超声波、光敏传感器板子电源和地搞反了，要重做
           解决LED与DS冲突，Port结构体中添加了一个端口方向元素
		   
2016-1-20: 画出几个简单图形，但是暂时没把他们拼到一起
           触摸不准，可能是驱动芯片问题也可能是裸屏的问题
		   
2016-1-21: 移植3.5寸屏失败，emwin暂时不支持ili9486
           添加画bmp图片的指令
		   触摸不准，排除芯片问题，可能是液晶屏问题也可能是没固定的原因.将触摸过滤次数改为10，仍然不见效
		   
2016-1-22: 添加调用子程序功能
           触摸仍然不准，但是野火的裸屏情况较好。
		   EDIT控件点击一下不出现光标问题待解决
		   
2016-1-23: 触摸不准问题解决，开机时屏上有色带问题解决，注释掉/*  VCOM Control 2(C7h)  */
           但MULTIEDIT有时仍然不出现光标
		  
2016-1-26: 进入中断服务函数应该添加OSIntEnter()   OSIntExit()函数

2016-2-15: 传感器板子打孔：半径5mm 中心距8mm
		   修改树形图界面，删除程序仍然可以将叶子项目图标删除
           仍然没有办法将下划线改成汇博士的替代
		   
2016-2-19: 核心板程序跑飞问题解决，仍然是SRAM焊的不牢固问题。。。

2016-2-27: 修改超声波的中断服务函数，UCOS-III中使用ISR有正确地格式!!!

2016-2-28: 光敏传感器电路不对，已修改

2016-3-2:  修改Timer2软件定时器周期为250ms

2016-3-4: 使用说明编辑：模拟传感器输出值在[0,100]
                       电机转速[0,100]
					   舵机转角[0,180]
					   编程规则: 如果、否则语句必须跟上条件结束
					             循环语句必有有头部有尾部
								 程序必须一行一行向下写，不得错误操作
				
		  加上格式化操作的提示
		  SWD寻找不到CPU是因为线接触不良
		  TIM8加了滤波器之后，超声波仍然会出现0的情况
		  
2016-3-7: 改为"LeCoder",完善Motor_Config();
          测试电机、舵机全加上运行效果，五分钟没问题，底板发热30度左右
		  
2016-3-8: 超声波TIRG引脚输出脉冲正常，ECHO引脚高低电平连续跳变是 结算处的高度值为0的原因。真是模块的问题？

2016-3-9: IRSensor电阻换成2.4K，检测距离增加。
          增加USART1接收中断
		  
2016-3-10: 修改GIF图片显示时清屏导致其他一些信息无法显示的问题

2016-3-15: 设置MULTIEDIT闪烁光标不成功

2016-3-16: 遥控器通道连接出现连接不成功的现象基本修复，

2016-3-26：修改小车左转、右转方式为一轮正转一轮反转

2016-6-27: 编程部分的那个"_"问题一定要解决，输入程序名还是会出现不跳出光标的现象
           考虑把6050数据用起来
		   
2016-6-30: 可将第一个"_"替换成光标，但是感觉起来还不如原来删除下划线再填写数值的做法

2016-7-15: 修改手动控制界面的MultiEdge名称
		   修改Window_1的MultiEdit光标闪烁，未验证
		   将下划线所在位置传入EditPad窗口，EditPad根据下划线所在位置来Select区域，用户输入数字直接覆盖下划线，未验证

2016-7-17: 修复下划线问题，如果指令有两条下划线，输入第一个下划线后立即跳到第二个下划线；如果只有一个下划线，则输入第一个数字后可以继续输入

2016-7-21: 声音传感器仍然有问题，插入电话线后会导致LCD白屏，而且传感器拔出电话线再次插入时咪头开始不灵敏,这是为什么

2016-10-25: 修改界面按钮大小，使之方便戳中
			添加判断水平(Y轴)加速度和竖直(Z)加速度大小的指令语句
			
2016-10-26: 完善"删除指令"的操作，删除一行指令的同时将下面所有行文本上移。
			修复删除一条指令后出现的保存出错问题.
			
2016-10-28: 6050水平放置时Z轴加速度<=0.5g,XY轴出现相同现象，单位换算没出错，是板子画错了？

2016-10-29: 重新焊了块底板，6050正常

2016-11-4:  修复MultiEdit点击不出现光标问题。
			”删除程序“按钮添加了提示框
			
2016-11-18: “自平衡”界面做了两个横线随着俯仰角变化而变化，进一步添加自平衡功能

2016-11-19: 暂停本机维护工作。

2017-2-15:  修改GUIDRV_Template.c中_FillRect()函数,对MULTIEDIT不出现光标问题没起到啥作用

2017-2-16: 弃用MULTIEDIT,改为点击"新建"弹出NewFilePad 的模式来编辑程序名
			更改WIDGET_EDITPAD.c, 将数字按键放大了点
			SPEED_STEP  改为 200
			
		    增加FOR循环控制指令，但未测试

2017-22-17: 修复FOR循环BUG
			所有子程序必须是全数字命名，主程序可以带英文
			将所有点击不便的按钮都放大了
			更新所有板子的程序

/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/
